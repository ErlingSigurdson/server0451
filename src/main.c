/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: main.c
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: основной файл с исходным кодом простого TCP-сервера для Linux,
 * написанным на языке C.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

/*--- Включения ---*/

// Настройки проекта.
#include "config_general.h"
#include "config_HTTP.h"

// Локальные модули.
#include "utilities.h"
#include "cmd.h"
#include "help_page.h"

// Стандартная библиотека языка C.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

// Другие библиотеки.
#include <unistd.h>      // для getopt(), read(), write(), close().
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>


/*************** ПРОТОТИПЫ ФУНКЦИЙ **************/

// Обработчик опций командной строки и их аргументов.
void opt_handler(int32_t argc, char *argv[], int32_t *port, char *cmd_file_path, uint32_t *verbosity_level);

// Проверка упоминания HTTP.
bool HTTP_is_mentioned(char *buf);

// Обработка входящих команд.
void cmd_handler(int32_t connfd, char *cmd_file_path, uint32_t verbosity_level);


/******************** ФУНКЦИИ *******************/

int main(int32_t argc, char *argv[])
{
    /*--- Обработка опций командной строки и их аргументов ---*/

    // Переменные для хранения значений, переданных из командной строки.
    int32_t port = -1;
    char cmd_file_path[STR_MAX_LEN + 1] = {0};
    uint32_t verbosity_level = 0;

    // Поиск и обработка опций и аргументов.
    opt_handler(argc, argv, &port, cmd_file_path, &verbosity_level);


    /*--- Работа с сокетами ---*/

	if (verbosity_level > 0) {
	    printf("\n\n* * * * * * * * * * * * * * * * *\n");
	    printf("Starting TCP server at port %d", port);
	    printf("\n* * * * * * * * * * * * * * * * *\n");
	}

    // Переменные, связанные с сокетами.
    int32_t sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // Создание сокета.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("\nSocket creation failed. Terminating the program.\n");
        printf("Error description: %s\n", strerror(errno));
        exit(0);
    } else if (verbosity_level > 0) {
    	printf("\n...socket successfully created.\n");
    }

    // Заполнение нулями.
    memset(&servaddr, '\0', sizeof(servaddr));  /* Иногда используют функцию bzero(),
												 * но это не благословляется.
												 */

    // Назначение IP-адреса и порта.
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    /* Позволяем заново использовать IP-адрес немедленно после закрытия
     * предыдущего соединения. Обязательно сделать это до вызова bind().
     */
    uint32_t so_reuseaddr = 1;
    int32_t a = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, (socklen_t)sizeof(so_reuseaddr) * 2);

	if (verbosity_level > 1) {
        printf("...setting SO_REUSEADDR return value: %d\n", a);
        printf("Status or error description: %s\n", strerror(errno));
	}

    /* Позволяем закрывать сокет, не дожидаясь конца передачи данных.
     * Обязательно сделать это до вызова bind().
     */
    uint32_t so_linger = 0;
    int32_t b = setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &so_linger, (socklen_t)sizeof(so_linger) * 2);

    if (verbosity_level > 1) {
        printf("...clearing SO_LINGER return value: %d\n", b);
        printf("Status or error description: %s\n", strerror(errno));
    }

    // Привязать вновь соданный сокет к IP.
    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("\nSocket bind failed. Terminating the program.\n");
        printf("Error description: %s\n", strerror(errno));
        exit(0);
    } else if (verbosity_level > 0) {
        printf("...socket successfully bound.\n");
    }

    // Сервер начинает слушать.
    if ((listen(sockfd, 5)) != 0) {
        printf("\nEntering listening mode failed. Terminating the program.\n");
        printf("Error description: %s\n", strerror(errno));
        exit(0);
    } else if (verbosity_level > 0) {
        printf("\nServer is listening.\n");
    }

    // Принимаем и проверяем сообщение от клиента.
    len = sizeof(cli);
    connfd = accept(sockfd, (struct sockaddr*)&cli, (socklen_t*)&len);
    if (connfd < 0) {
        printf("\nServer failed to accept the client. Terminating the program.\n");
        printf("Error description: %s\n", strerror(errno));
        exit(0);
    } else if (verbosity_level > 0) {
        printf("Server accepted the client. Starting communication.\n");
    }

    // Вызываем функцию-обработчик поступивших команд.
    cmd_handler(connfd, cmd_file_path, verbosity_level);

    // Закрытие сокета.
    //shutdown(sockfd, SHUT_RDWR);  // Вроде бы не нужно, но иногда упоминается.
    close(sockfd);

    return 0;
}

void opt_handler(int32_t argc, char *argv[], int32_t *port, char *cmd_file_path, uint32_t *verbosity_level)
{
    int32_t opt = 0;
    while ((opt = getopt(argc, argv, "p:f:vVh")) >= 0) {
        switch (opt) {
            // Обязательная опция.
            case 'p':
                *port = strtol(optarg, NULL, 10);
                break;

            // Обязательная опция.
            case 'f':
                sscanf(optarg, "%s", cmd_file_path);
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

            default:
            	// Ничего не делаем, отдаём дань MISRA.
        }
    }

    if (*port < 0) {
        printf("Please restart the program and insert a valid port number as a -p option argument.\n");
        exit(0);
    }

    if (cmd_file_path[0] != '/' && cmd_file_path[0] != '.') {
        printf("Please restart the program and insert a valid path\n");
        printf("to a command configuration file as an -f option argument.\n");
        printf("If such file doesn't exist, the program will create it\n");
        printf("at specified path automatically. File will contain\n");
        printf("default values.\n");
        exit(0);
    }
}

bool HTTP_is_mentioned(char *buf)
{
    if (strstr(buf, "HTTP")) {
        return 1;
    } else {
        return 0;
    }
}

void cmd_handler(int32_t connfd, char *cmd_file_path, uint32_t verbosity_level)
{
    /*--- Чтение файла конфигурации команд ---*/

    // Считывание значений.
    char cmd_file_contents[CMD_FILE_LIST_LEN][STR_MAX_LEN] = {0};
    cmd_file_read_else_write_defaults(cmd_file_contents, cmd_file_path);


    /*--- Чтение ---*/

    char buf[STR_MAX_LEN + 1] = {0};

    // Читаем сообщение от клиента и сохраняем его в буфер.
    read(connfd, buf, sizeof(buf));

    // Выводим содержимое буфера.
    if (verbosity_level > 0) {
        utilities_force_2xLF(buf, sizeof(buf));
        printf("\nMessage received from the client:\n%s", buf);
    }


    /*--- Поиск и обработка поступившей команды ---*/

	// Поиск команды в поступившем от клиента сообщении.
    bool is_setloadon = strstr(buf, cmd_file_contents[CMD_FILE_ASCII_CMD_ON]) ||         \
                        strstr(buf, cmd_file_contents[CMD_FILE_URI_CMD_ON])   ||         \
                        strstr(buf, cmd_file_contents[CMD_FILE_VAL_CMD_ON]) ;

    bool is_setloadoff = strstr(buf, cmd_file_contents[CMD_FILE_ASCII_CMD_OFF]) ||       \
                         strstr(buf, cmd_file_contents[CMD_FILE_URI_CMD_OFF])   ||       \
                         strstr(buf, cmd_file_contents[CMD_FILE_VAL_CMD_OFF]) ;

    bool is_setloadtoggle = strstr(buf, cmd_file_contents[CMD_FILE_ASCII_CMD_TOGGLE]) || \
                            strstr(buf, cmd_file_contents[CMD_FILE_URI_CMD_TOGGLE])   || \
                            strstr(buf, cmd_file_contents[CMD_FILE_VAL_CMD_TOGGLE]) ;

    bool is_request = strstr(buf, cmd_file_contents[CMD_FILE_REQUEST_CMD]);

    bool is_HTTP = HTTP_is_mentioned(buf);


	/*--- Запись в файл нового предписываемого положения нагрузки и направление ответа клиенту ---*/

	// Проверка текущего предписанного состояния нагрузки.
	bool current_load_status_cmd = cmd_file_contents[CMD_FILE_CURRENT_CMD][0] - '0';
	
    if (is_setloadtoggle) {
        if (current_load_status_cmd) {
        	is_setloadoff = 1;
        } else {
        	is_setloadon = 1;
        }
    }

    if (is_setloadon) {
        cmd_file_update_current_load_status_cmd(cmd_file_path, 1);

        memset(buf, '\0', sizeof(buf));
        char *ptr = buf;

        if (is_HTTP) {
            strcat(buf, HTTP_RESPONSE_200);
            ptr += strlen(HTTP_RESPONSE_200);
        }

        strcat(buf, "New command posted: load ON.\n");

        if (verbosity_level > 0) {
            printf("%s", ptr);
        }

        write(connfd, buf, strlen(buf));
        return;
    }

    if (is_setloadoff) {
        cmd_file_update_current_load_status_cmd(cmd_file_path, 0);
        
        memset(buf, '\0', sizeof(buf));
        char *ptr = buf;
    
        if (is_HTTP) {
            strcat(buf, HTTP_RESPONSE_200);
            ptr += strlen(HTTP_RESPONSE_200);
        }
    
        strcat(buf, "New command posted: load OFF.\n");
    
        if (verbosity_level > 0) {
            printf("%s", ptr);
        }

        write(connfd, buf, strlen(buf));
        return;
    }
        
    if (is_request) {
        if (current_load_status_cmd) {
        	strcpy(buf, cmd_file_contents[CMD_FILE_ASCII_CMD_ON]);	
        } else {
        	strcpy(buf, cmd_file_contents[CMD_FILE_ASCII_CMD_OFF]);
        }

		// Позволяет клиенту быстрее считать ответ, реагируя на символ конца строки.
        strcat(buf, "\n");

        if (verbosity_level > 0) {
            printf("Current command requested.\n");
        }

        write(connfd, buf, strlen(buf));
        return;
    }

	/* Программа доходит до этой точки только в случае, если в сообщении
	 * от клиента не было найдено ни одной корректной команды.
	 */
    if (verbosity_level > 0) {
        printf("No valid command received.\n");
    }

    memset(buf, '\0', sizeof(buf));

    if (is_HTTP) {
        strcat(buf, HTTP_RESPONSE_404);
    }
    
    strcat(buf, "No valid command received\n");

    write(connfd, buf, strlen(buf));
}
