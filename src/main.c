/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: main.c
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: основной файл с исходным кодом простого TCP-сервера IoT
 * для Linux, написанным на языке Си.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

/*--- Включения ---*/

// Из стандартной библиотеки языка Си.
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// Из библиотек POSIX.
#include <unistd.h>

// Настройки.
#include "config_general.h"

// Локальные модули.
#include "sockets.h"
#include "cmd.h"
#include "msg_format_check.h"
#include "timestamp.h"
#include "help_page.h"


/*************** ПРОТОТИПЫ ФУНКЦИЙ **************/

// Чтение опций командной строки и их аргументов.
void opt_handle(int32_t argc, char *argv[],
                int32_t *port,
                char *password, size_t password_buf_size,
                bool *oneshot_mode, 
                uint32_t *verbosity_level,
                bool *print_help_page);

// Завершение связи с клиентом.
void finish_communication(int32_t fd, uint32_t attempts, uint32_t pause, uint32_t verbosity_level);


/******************** ФУНКЦИИ *******************/

int32_t main(int32_t argc, char *argv[])
{
    /*--- Чтение и обработка опций командной строки и их аргументов ---*/

    // Переменные для хранения значений, переданных из командной строки.
    int32_t port = -1;      // По умолчанию задано невалидное значение.
    char password[STR_MAX_LEN + 1] = {0};
    bool oneshot_mode = 0;  // Флаг запуска в тестовом режиме (oneshot).
    uint32_t verbosity_level = 0;
    bool print_help_page = 0;

    // Чтение опций командной строки и их аргументов.
    opt_handle(argc, argv, &port, password, sizeof(password), &oneshot_mode, &verbosity_level, &print_help_page);

    if (print_help_page) {
        PRINT_HELP_PAGE;
        exit(0);
    }

    // Проверка валидности значений, переданных из командной строки.
    if (port <= 0) {
        fprintf(stderr, "Error: invalid port specified. Program terminated.\n"
                        "Please restart the program and insert a valid port number as a -p option argument.\n");
        exit(1);
    }
    
    if (strlen(password) < PASSWORD_MIN_LEN || strlen(password) > PASSWORD_MAX_LEN) {
        fprintf(stderr, "Error: invalid password specified. Program terminated.\n"
                        "Please restart the program and insert a valid password as a -P option argument.\n"
                        "Password must consist of 5 to 40 ASCII alphanumerics.\n");
        exit(1);
    }

    if (verbosity_level > 0) {
        printf("\n\n"
               "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n"
               "                        Starting TCP IoT server\n"
               "Server started at port %u. ", port);
        timestamp_print();
        printf("\n"
               "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
    }


    /*--- Работа с сокетами ---*/

    // Создание и подготовка "слушающего" сокета.
    int32_t sockfd = -1;  // По умолчанию задано невалидное значение.
    uint32_t sockets_init_retval = sockets_init(&sockfd, port, SOCKET_BACKLOG, verbosity_level);
    switch (sockets_init_retval) {
        case SOCKETS_INIT_ERR_CREATE:
            fprintf(stderr, "\n"
                            "Error: socket creation failed. Program terminated.\n"
                            "Error description: %s\n",
                            strerror(errno));
            exit(1);
            break;        // По сути не нужно после вызова exit(), но здесь и далее присутствует для единообразия.
        case SOCKETS_INIT_ERR_SETSOCKOPT:
            fprintf(stderr, "\n"
                            "Error: setting socket option failed. Program terminated.\n"
                            "Error description: %s\n",
                            strerror(errno));
            exit(1);
            break;
        case SOCKETS_INIT_ERR_BIND:
            fprintf(stderr, "\n"
                            "Error: socket binding failed. Program terminated.\n"
                            "Error description: %s\n",
                            strerror(errno));
            exit(1);
            break;
        case SOCKETS_INIT_ERR_LISTEN:
            fprintf(stderr, "\n"
                            "Error: entering a listen mode failed. Program terminated.\n"
                            "Error description: %s\n",
                            strerror(errno));
            exit(1);
            break;
        case SOCKETS_INIT_OK:
            // Успешная инициализация "слушающего" сокета.
            break;
        default:
            // Ничего не делаем, отдаём дань MISRA.
            break;
    }

    // Сокет текущего соединения.
    int32_t connfd = -1;  // По умолчанию задано невалидное значение.

    /* Установка соединения и обмен данными.
     * В тестовом режиме (oneshot) следующий блок кода выполнится единожды,
     * в основном режиме (loop) он будет выполняться циклически.
     */
    do {
        /*--- Вывод буфера ---*/

        /* Если в программе есть бесконечный цикл, то по умолчанию выводимые в цикле данные
         * будут копиться в буфере, пока не будет дана команда на вывод из буфера
         * или он не заполнится до определённого значения.
         */
        fflush(stdout);
        fflush(stderr);
    
        uint32_t sockets_proceed_retval = sockets_proceed(sockfd, &connfd, SELECT_TIMEOUT_SEC, verbosity_level);
        switch (sockets_proceed_retval) {
            case SOCKETS_PROCEED_ERR_ACCEPT:
                fprintf(stderr, "\n"
                                "Error: server failed to accept a client. "
                                "Keeps listening for a next connection.\n"
                                "Error description: %s\n",
                                strerror(errno));
                continue;
                break;
            case SOCKETS_PROCEED_ERR_SELECT:
                fprintf(stderr, "\n"
                                "Error: server failed to find a socket available for reading. "
                                "Keeps listening for a next connection.\n"
                                "Error description: %s\n",
                                strerror(errno));
                continue;
                break;
            case SOCKETS_PROCEED_TIMEOUT:
                finish_communication(connfd, SOCKET_GRACEFUL_CLOSE_ATTEMPTS, SOCKET_CLOSE_PAUSE, verbosity_level);
                continue;
                break;
            case SOCKETS_PROCEED_OK:
                // Связь с клиентом успешно установлена, клиент направил данные через сокет.
                break;
            default:
                // Ничего не делаем, отдаём дань MISRA.
                break;
        }

        /* Исполнение не дойдёт досюда, пока не будет установлена связь
         * с клиентом и тот не направит данные через сокет.
         */
        char buf[STR_MAX_LEN + 1] = {0};
        sockets_read_message(connfd, buf, sizeof(buf), verbosity_level);

       
        /*--- Проверка формата сообщения от клиента ---*/

        char resulting_pattern[STR_MAX_LEN * 2 + 1] = {0};
        strcpy(resulting_pattern, password);
        strcat(resulting_pattern, MSG_FORMAT_REGEX_PATTERN);

        uint32_t msg_format_check_retval = msg_format_check(buf, resulting_pattern);
        switch (msg_format_check_retval) {
            case MSG_FORMAT_CHECK_REGEX_COMP_FAIL:
                fprintf(stderr, "\nError: message format check failed - error compiling regex.\n");
                strcpy(buf, "Error: message format check failed - error compiling regex.");
                sockets_write_message(connfd, buf, 0);
                break;
            case MSG_FORMAT_CHECK_NO_MATCH:
                fprintf(stderr, "\nError: message format check failed - no match found.\n");
                strcpy(buf, "Error: message format check failed - no match found.");
                sockets_write_message(connfd, buf, 0);
                break;
            case MSG_FORMAT_CHECK_PARTIAL_MATCH:
                fprintf(stderr, "\nError: message format check failed - partial match.\n");
                strcpy(buf, "Error: message format check failed - partial match.");
                sockets_write_message(connfd, buf, 0);
                break;
            case MSG_FORMAT_CHECK_MATCH:
                // Проверка формата сообщения пройдена успешно.
                break;
            default:
                // Ничего не делаем, отдаём дань MISRA.
                break;
        }


        /*--- Обработка поступившей команды ---*/

        uint32_t cmd_handle_retval = cmd_handle(connfd, buf, verbosity_level);
        switch (cmd_handle_retval) {
            case CMD_ERR_EXTRACT:
                /* По идее после предыдущей проверки такого быть не должно,
                 * но на всякий случай здесь вшита собственная проверка.
                 */
                fprintf(stderr, "\nError: invalid message format.\n");
                strcpy(buf, "Error: invalid message format.");
                sockets_write_message(connfd, buf, 0);
                break;
            case CMD_ERR_TOGGLE:
                fprintf(stderr, "\nError: can't toggle current load state (invalid data in the topic).\n");
                strcpy(buf, "Error: can't toggle current load state (invalid data in the topic).");
                sockets_write_message(connfd, buf, 0);
                break;
            case CMD_ERR_NO_VALID_COMMAND:
                fprintf(stderr, "\nError: no valid command received.\n");
                strcpy(buf, "Error: no valid command received.");
                sockets_write_message(connfd, buf, 0);
                break;
            case CMD_OK:
                // Поступившая команда обработана успешно.
                break;
            default:
                // Ничего не делаем, отдаём дань MISRA.
                break;
        }


        /*--- Завершение коммуникации с очередным клиентом ---*/

        finish_communication(connfd, SOCKET_GRACEFUL_CLOSE_ATTEMPTS, SOCKET_CLOSE_PAUSE, verbosity_level);
    }
    while (!oneshot_mode);

    /* Закрытие слушающего сокета.
     * Исполнение программы доходит досюда только в режиме одиночного прогона.
     */
    finish_communication(sockfd, SOCKET_GRACEFUL_CLOSE_ATTEMPTS, SOCKET_CLOSE_PAUSE, 0);

    return 0;
}

void opt_handle(int32_t argc, char *argv[],
                int32_t *port,
                char *password, size_t password_buf_size,
                bool *oneshot_mode,
                uint32_t *verbosity_level,
                bool *print_help_page)
{
    int32_t opt = 0;
    while ((opt = getopt(argc, argv, "p:P:ovVh")) >= 0) {
        switch (opt) {
            // Обязательная опция, принимает в качестве аргумента номер порта.
            case 'p':
                *port = strtol(optarg, NULL, 10);
                break;

            // Обязательная опция, принимает в качестве аргумента строку с паролем.
            case 'P':
                if (strlen(optarg) < password_buf_size) {
                    strcpy(password, optarg);
                }
                break;

            case 'o':
                *oneshot_mode = 1;
                break;

            case 'v':
                *verbosity_level = 1;
                break;

            case 'V':
                *verbosity_level = 2;
                break;

            case 'h':
                *print_help_page = 1;
                break;

            default:
                // Ничего не делаем, отдаём дань MISRA.
                break;
        }
    }
}

void finish_communication(int32_t fd, uint32_t attempts, uint32_t pause, uint32_t verbosity_level)
{
    if (verbosity_level == 0) {
        if (sockets_close(fd, pause) != 0) {
            fprintf(stderr, "\n"
                    "Communication closed ungracefully.\n"
                    "----------------------------------\n");         
        }
        return;
    }

    if (verbosity_level == 1) {
        if (sockets_close(fd, pause) == 0) {
            printf("\n"
                   "Communication closed gracefully.\n"
                   "--------------------------------\n");   
        } else {
            fprintf(stderr, "\n"
                    "Communication closed ungracefully.\n"
                    "----------------------------------\n");         
        }
        return;
    }

    for (uint32_t i = 1; i <= attempts; ++i) {
        if (sockets_close(fd, pause) != 0) {
            fprintf(stderr, "\n"
                            "Closing socket failed on attempt %d of %d, status: %s\n",
                            i, attempts, strerror(errno));
        } else {
            printf("\n"
                   "Communication closed gracefully on attempt %d.\n"
                   "-----------------------------------------------\n",
                   i);
            
            return;
        }
    }
    fprintf(stderr, "\n"
                    "Communication closed ungracefully.\n"
                    "----------------------------------\n");
}
