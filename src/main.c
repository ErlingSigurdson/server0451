/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: main.c
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: основной файл с исходным кодом простого TCP-сервера для Linux,
 * написанным на языке Си. Сервер рассчитан на применение в IoT.
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
void opt_handle(int32_t argc, char *argv[], int32_t *port, uint32_t *verbosity_level);

// Вывод в консоль текущего времени (UTC) в человекочитаемом формате.
void print_timestamp(uint32_t port);


/******************** ФУНКЦИИ *******************/

int32_t main(int32_t argc, char *argv[])
{
    /*--- Чтение и обработка опций командной строки и их аргументов ---*/

    // Переменные для хранения значений, переданных из командной строки.
    int32_t port = -1;             // По умолчанию задано невалидное значение.
    uint32_t verbosity_level = 0;

    // Основная функция, имеющая дело с опциями командной строки и их аргументами.
    // В случае введения ключа вывода страницы с руководством выполнение программы будет прервано.
    opt_handle(argc, argv, &port, &verbosity_level);

    if (port <= 0) {
        fprintf(stderr, "Error: invalid port\n");
        fprintf(stderr, "Please restart the program and insert a valid port number as a -p option argument.\n");
        exit(1);
    }

    if (verbosity_level > 0) {
        printf("\n\n* * * * * * * * * * * * * * * * * * * * * * * *\n");
        printf("        Starting TCP server at port %d\n", port);
        print_timestamp(port);
        printf("* * * * * * * * * * * * * * * * * * * * * * * *\n");
    }


    /*--- Работа с сокетами ---*/
    
    int32_t sockfd = 0;
    uint32_t init_result = sockets_init(&sockfd, port, verbosity_level);
    switch (init_result) {
        case 2:
            fprintf(stderr, "\nError: socket creation at port %d failed. Terminating the program.\n", port);
            fprintf(stderr, "Error description: %s\n", strerror(errno));
            exit(1);
            break;
        case 1:
            fprintf(stderr, "\nSocket binding at port %d failed. Terminating the program.\n", port);
            fprintf(stderr, "Error description: %s\n", strerror(errno));
            exit(1);
        case 0:
            // Успешная инициализация сокета..
            break;
        default:
            // Ничего не делаем, отдаём дань MISRA.
            break;
    }

	int32_t connfd = 0;
	uint32_t set_connection_result = sockets_set_connection(sockfd, &connfd, port, verbosity_level);
    switch (set_connection_result) {
        case 2:
            fprintf(stderr, "\nEntering a listen mode at port %d failed. Terminating the program.\n", port);
            fprintf(stderr, "Error description: %s\n", strerror(errno));
            exit(1);
        case 1:
            fprintf(stderr, "\nServer failed to accept the client at port %d. Terminating the program.\n", port);
            fprintf(stderr, "Error description: %s\n", strerror(errno));
            exit(1);
        case 0:
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
    
    uint32_t msg_format_check_result = msg_format_check_regex(buf, MSG_FORMAT_REGEX_PATTERN);
    switch (msg_format_check_result) {
        case 3:
            printf("Message format check failed: error compiling regex.");
            strcpy(buf, "Message format check failed: error compiling regex.");
            sockets_write_message(connfd, buf, verbosity_level);
            exit(1);
        case 2:
            printf("Message format check failed: no match found.");
            strcpy(buf, "Message format check failed: no match found.");
            sockets_write_message(connfd, buf, verbosity_level);
            exit(1);
        case 1:
            printf("Message format check failed: partial match.");
            strcpy(buf, "Message format check failed: partial match.");
            sockets_write_message(connfd, buf, verbosity_level);
            exit(1);
        case 0:
            // Проверка формата пройдена.
            break;
        default:
            // Ничего не делаем, отдаём дань MISRA.
            break;
    }


    /*--- Обработка поступившей команды ---*/

    // В случае ошибки в формате сообщения, полученного от клиента, выполнение программы будет прервано.
    cmd_handle(connfd, buf, verbosity_level);


    /*--- Завершение коммуникации с клиентом ---*/

	sockets_close(sockfd);
    if (verbosity_level > 0) {
        printf("Communication closed.\n");
    }
	
    return 0;
}

void opt_handle(int32_t argc, char *argv[], int32_t *port, uint32_t *verbosity_level)
{
    int32_t opt = 0;
    while ((opt = getopt(argc, argv, "p:vVh")) >= 0) {
        switch (opt) {
            // Обязательная опция.
            case 'p':
                *port = strtol(optarg, NULL, 10);
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

// Вывести в консоль текущее время в человекочитаемом формате.
void print_timestamp(uint32_t port)
{
	time_t posix_time;
	char buf[STR_MAX_LEN + 1] = {0};
	struct tm *time_fields;

	posix_time = time(NULL);
	time_fields = localtime(&posix_time);

	strftime(buf, sizeof(buf), "date: %d.%m.%Y, time (UTC): %H:%M:%S", time_fields);
	printf("Port %u, %s\n", port, buf);
}
