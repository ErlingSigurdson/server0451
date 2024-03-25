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
#include <stdlib.h>      // exit().

// Из библиотек POSIX.
#include <time.h>

// Настройки проекта.
#include "config_general.h"

// Локальные модули.
#include "utilities.h"
#include "sockets.h"
#include "cmd.h"
#include "help_page.h"


/*************** ПРОТОТИПЫ ФУНКЦИЙ **************/

// Обработка опций командной строки и их аргументов.
void opt_handle(int32_t argc, char *argv[], int32_t *port, uint32_t *verbosity_level);

// Вывод в консоль текущего времени в человекочитаемом формате.
void print_timestamp(uint32_t port);

// Обработка входящих команд.
//void cmd_handler(int32_t connfd, char *cmd_config_file_path, uint32_t verbosity_level);


/******************** ФУНКЦИИ *******************/

int32_t main(int32_t argc, char *argv[])
{
    /*--- Обработка опций командной строки и их аргументов ---*/

    // Переменные для хранения значений, переданных из командной строки.
    int32_t port = -1;             // По умолчанию задано невалидное значение.
    uint32_t verbosity_level = 0;

    // Поиск и чтение опций командной строки и их аргументов.
    opt_handle(argc, argv, &port, &verbosity_level);

    if (port <= 0) {
        fprintf(stderr, "Error: invalid port\n");
        fprintf(stderr, "Please restart the program and insert a valid port number as a -p option argument.\n");
        exit(1);
    }

    if (verbosity_level > 0) {
        printf("\n\n* * * * * * * * * * * * * * * * * *\n");
        printf("Starting TCP server at port %d\n", port);
        print_timestamp(port);
        printf("* * * * * * * * * * * * * * * * * *\n");
    }


    /*--- Работа с сокетами ---*/

    int32_t sockfd = 0;
    // В случае ошибки при создании сокета выполнение программы будет прервано.
	sockets_init(&sockfd, port, verbosity_level);

	int32_t connfd = 0;
	// В случае ошибки при использовании сокета выполнение программы будет прервано.
	sockets_set_connection(sockfd, &connfd, port, verbosity_level);

    char buf[STR_MAX_LEN + 1] = {0};
    sockets_read_message(connfd, buf, sizeof(buf), verbosity_level);

    cmd_handle(connfd, buf, verbosity_level);

	sockets_close(sockfd);

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
    #define TIMESTAMP_STR_MAX_LEN 50

	time_t posix_time;
	char buf[TIMESTAMP_STR_MAX_LEN] = {0};
	struct tm *time_fields;

	posix_time = time(NULL);
	time_fields = localtime(&posix_time);

	strftime(buf, TIMESTAMP_STR_MAX_LEN, "date: %d.%m.%Y, time (UTC): %H:%M:%S", time_fields);
	printf("Port %u, %s\n", port, buf);
}
