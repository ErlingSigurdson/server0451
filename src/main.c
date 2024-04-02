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
//#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// Из библиотек POSIX.
#include <unistd.h>
#include <time.h>

// Настройки проекта.
#include "config_general.h"

// Локальные модули.
#include "sockets.h"
#include "cmd.h"
#include "msg_format_check_regex.h"
#include "help_page.h"


/*************** ПРОТОТИПЫ ФУНКЦИЙ **************/

// Чтение опций командной строки и их аргументов.
void opt_handle(int32_t argc, char *argv[],
                int32_t *port,
                char *password, size_t password_buf_size,
                uint32_t *verbosity_level);

// Вывод в консоль текущей даты и времени (UTC+0) в человекочитаемом формате.
void timestamp_print();

// Завершение связи с клиентом.
void finish_communication(int32_t sockfd, uint32_t verbosity_level);


/******************** ФУНКЦИИ *******************/

int32_t main(int32_t argc, char *argv[])
{
    /*--- Чтение и обработка опций командной строки и их аргументов ---*/

    // Переменные для хранения значений, переданных из командной строки.
    int32_t port = -1;  // По умолчанию задано невалидное значение.
    char password[STR_MAX_LEN + 1 ] = {0};
    uint32_t verbosity_level = 0;

    // Чтение опций командной строки и их аргументов.
    opt_handle(argc, argv, &port, password, sizeof(password), &verbosity_level);

    if (port <= 0) {
        fprintf(stderr, "Error: invalid port.\n");
        fprintf(stderr, "Please restart the program and insert a valid port number as a -p option argument.\n");
        exit(1);
    }

    if (strlen(password) <= 0) {
        fprintf(stderr, "Error: invalid password.\n");
        fprintf(stderr, "Please restart the program and insert a valid password as a -P option argument.\n");
        exit(1);
    }

    if (verbosity_level > 0) {
        printf("\n\n* * * * * * * * * * * * * * * * * * * * * * * * * *\n");
        printf("              Starting TCP IoT server\n");
        printf("Port %u, ", port);
        timestamp_print(port);
        printf("\n* * * * * * * * * * * * * * * * * * * * * * * * * *\n");
    }


    /*--- Работа с сокетами ---*/

    int32_t sockfd = 0;
    uint32_t init_result = sockets_init(&sockfd, port, verbosity_level);
    switch (init_result) {
        case SOCKET_ERR_CREATE:
            fprintf(stderr, "\nError: socket creation at port %d failed.\n", port);
            fprintf(stderr, "Error description: %s\n", strerror(errno));
            exit(1);
            break;    // Не особо нужно после вызова exit(), но здесь и далее присутствует для единообразия.
        case SOCKET_ERR_BIND:
            fprintf(stderr, "\nError: socket binding at port %d failed.\n", port);
            fprintf(stderr, "Error description: %s\n", strerror(errno));
            exit(1);
            break;
        case SOCKET_ERR_LISTEN:
            fprintf(stderr, "\nError: entering a listen mode at port %d failed.\n", port);
            fprintf(stderr, "Error description: %s\n", strerror(errno));
            exit(1);
            break;
        case SOCKET_OK:
            // Успешная инициализация сокета.
            break;
        default:
            // Ничего не делаем, отдаём дань MISRA.
            break;
    }

    while (1) {
        int32_t connfd = 0;
        uint32_t set_connection_result = sockets_set_connection(sockfd, &connfd, port, verbosity_level);
        switch (set_connection_result) {
            case SOCKET_ERR_ACCEPT:
                fprintf(stderr, "\nError: server failed to accept a client at port %d.\n", port);
                fprintf(stderr, "Error description: %s\n", strerror(errno));
                continue;
                break;
            case SOCKET_OK:
                // Установка связи с клиентом прошла успешно.
                break;
            default:
                // Ничего не делаем, отдаём дань MISRA.
                break;
        }

        // Получение сообщения от клиента.
        char buf[STR_MAX_LEN + 1] = {0};
        sockets_read_message(connfd, buf, sizeof(buf), verbosity_level);


        /*--- Проверка формата сообщения от клиента ---*/

        char resulting_pattern[STR_MAX_LEN * 2 + 1] = {0};
        strcpy(resulting_pattern, password);
        strcat(resulting_pattern, MSG_FORMAT_REGEX_PATTERN);

        uint32_t msg_format_check_result = msg_format_check_regex(buf, resulting_pattern);
        switch (msg_format_check_result) {
            case MSG_FORMAT_REGEX_COMP_FAIL:
                printf("\nMessage format check failed: error compiling regex.\n");
                strcpy(buf, "Message format check failed: error compiling regex.");
                sockets_write_message(connfd, buf, 0);  // verbosity_level overridden.
                break;
            case MSG_FORMAT_NO_MATCH:
                printf("\nMessage format check failed: no match found.\n");
                strcpy(buf, "Message format check failed: no match found.");
                sockets_write_message(connfd, buf, 0);  // verbosity_level overridden.
                break;
            case MSG_FORMAT_PARTIAL_MATCH:
                printf("\nMessage format check failed: partial match.\n");
                strcpy(buf, "Message format check failed: partial match.");
                sockets_write_message(connfd, buf, 0);  // verbosity_level overridden.
                break;
            case MSG_FORMAT_MATCH:
                // Проверка формата сообщения пройдена.
                break;
            default:
                // Ничего не делаем, отдаём дань MISRA.
                break;
        }


        /*--- Обработка поступившей команды ---*/

        cmd_handle(connfd, buf, verbosity_level);


        /*--- Завершение коммуникации с очередным клиентом ---*/

        finish_communication(connfd, verbosity_level);
    }

    /* В норме исполнение программы не должно доходить досюда,
     * но предполагаемое закрытие "слушающего" сокета иллюстрирует
     * логику работы сокетов.
     */
    finish_communication(sockfd, verbosity_level);

    return 0;
}

void opt_handle(int32_t argc, char *argv[],
                int32_t *port,
                char *password, size_t password_buf_size,
                uint32_t *verbosity_level)
{
    int32_t opt = 0;
    while ((opt = getopt(argc, argv, "p:P:vVh")) >= 0) {
        switch (opt) {
            // Обязательная опция, принимает номер порта в качестве аргумента.
            case 'p':
                *port = strtol(optarg, NULL, 10);
                break;

            case 'P':
                if (strlen(optarg) < password_buf_size) {
                    sscanf(optarg, "%s", password);
                }
                break;

            case 'v':
                *verbosity_level = 1;
                break;

            case 'V':
                *verbosity_level = 2;
                break;

            case 'h':
                PRINT_HELP_PAGE;
                exit(0);
                break;

            default:
                // Ничего не делаем, отдаём дань MISRA.
                break;
        }
    }
}

void timestamp_print()
{
    char buf[STR_MAX_LEN + 1] = {0};
    time_t posix_time;
    struct tm *time_fields;

    posix_time = time(NULL);
    time_fields = localtime(&posix_time);

    strftime(buf, sizeof(buf), "date: %d.%m.%Y, time (UTC+0): %H:%M:%S", time_fields);
    printf("%s", buf);
}

void finish_communication(int32_t fd, uint32_t verbosity_level)
{
    sockets_close(fd);
    if (verbosity_level > 0) {
        printf("\nCommunication closed.\n------------------------------------------------------\n");
    }
}
