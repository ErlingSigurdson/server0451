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
#include <errno.h>

// Из библиотек POSIX.
#include <unistd.h>      // getopt(), read(), write(), close().
#include <netinet/in.h>
#include <time.h>

/* Эти заголовочные файлы упоминаются в примерах серверов, написанных на языке Си,
 * но код компилируется и работает и без них.
 */
//#include <netdb.h>
//#include <sys/socket.h>
//#include <sys/types.h>

// Настройки проекта.
#include "config_general.h"
#include "config_HTTP.h"

// Локальные модули.
#include "utilities.h"
#include "cmd.h"
#include "help_page.h"


/*************** ПРОТОТИПЫ ФУНКЦИЙ **************/

// Обработчик опций командной строки и их аргументов.
void opt_handler(int32_t argc, char *argv[], int32_t *port, char *cmd_config_file_name, uint32_t *verbosity_level);

// Вывод в консоль текущего времени в человекочитаемом формате.
void print_timestamp(uint32_t port);

// Проверка упоминания HTTP.
bool HTTP_is_mentioned(char *buf);

// Обработчик входящих команд.
void cmd_handler(int32_t connfd, char *cmd_config_file_path, uint32_t verbosity_level);


/******************** ФУНКЦИИ *******************/

int main(int32_t argc, char *argv[])
{
    /*--- Обработка опций командной строки и их аргументов ---*/

    // Переменные для хранения значений, переданных из командной строки.
    int32_t port = -1;  // По умолчанию задано невалидное значение.
    char cmd_config_file_name[STR_MAX_LEN + 1] = {0};
    uint32_t verbosity_level = 0;

    // Поиск и чтение опций командной строки и их аргументов.
    opt_handler(argc, argv, &port, cmd_config_file_name, &verbosity_level);


    /*--- Работа с сокетами ---*/

	if (verbosity_level > 0) {
	    printf("\n\n* * * * * * * * * * * * * * * * * *\n");
	    printf("Starting TCP server at port %d\n", port);
	    print_timestamp(port);
	    printf("* * * * * * * * * * * * * * * * * *\n");
	}

    // Переменные, связанные с сокетами.
    int32_t sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // Создание сокета.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "\nSocket creation at port %d failed. Terminating the program.\n", port);
        fprintf(stderr, "Error description: %s\n", strerror(errno));
        exit(1);
    } else if (verbosity_level > 0) {
    	printf("\n...socket successfully created at port %d.\n", port);
    }

    // Заполнение нулями.
    memset(&servaddr, '\0', sizeof(servaddr));  /* Иногда используют функцию bzero(),
												 * но она считается устаревшей.
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
        printf("...setting SO_REUSEADDR return value: %d. ", a);
        printf("Status or error description: %s\n", strerror(errno));
	}

    /* Позволяем закрывать сокет, не дожидаясь конца передачи данных.
     * Обязательно сделать это до вызова bind().
     */
    uint32_t so_linger = 0;
    int32_t b = setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &so_linger, (socklen_t)sizeof(so_linger) * 2);

    if (verbosity_level > 1) {
        printf("...clearing SO_LINGER return value: %d. ", b);
        printf("Status or error description: %s\n", strerror(errno));
    }

    // Привязать вновь соданный сокет к IP.
    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        fprintf(stderr, "\nSocket binding at port %d failed. Terminating the program.\n", port);
        fprintf(stderr, "Error description: %s\n", strerror(errno));
        exit(1);
    } else if (verbosity_level > 0) {
        printf("...socket successfully bound at port %d.\n", port);
    }

    // Сервер начинает слушать.
    if ((listen(sockfd, 5)) != 0) {
        fprintf(stderr, "\nEntering a listen mode at port %d failed. Terminating the program.\n", port);
        fprintf(stderr, "Error description: %s\n", strerror(errno));
        exit(1);
    } else if (verbosity_level > 0) {
        printf("...server is listening at port %d.\n", port);
    }

    // Принимаем и проверяем сообщение от клиента.
    len = sizeof(cli);
    connfd = accept(sockfd, (struct sockaddr*)&cli, (socklen_t*)&len);
    if (connfd < 0) {
        fprintf(stderr, "\nServer failed to accept the client at port %d. Terminating the program.\n", port);
        fprintf(stderr, "Error description: %s\n", strerror(errno));
        exit(1);
    } else if (verbosity_level > 0) {
        printf("\nServer accepted a client at port %d. Starting communication.\n", port);
    }

    // Вызываем функцию-обработчик поступивших команд.
    cmd_handler(connfd, cmd_config_file_name, verbosity_level);

    // Закрытие сокетов.
    while (close(connfd) != 0);
    while (close(sockfd) != 0);

    return 0;
}

void opt_handler(int32_t argc, char *argv[], int32_t *port, char *cmd_config_file_name, uint32_t *verbosity_level)
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
                sscanf(optarg, "%s", cmd_config_file_name);
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
            	break;
        }
    }

    if (*port < 0) {
        fprintf(stderr, "Please restart the program and insert a valid port number as a -p option argument.\n");
        exit(1);
    }

    if (strchr(cmd_config_file_name, '/') != NULL || strlen(cmd_config_file_name) == 0) {
        fprintf(stderr, "Please restart the program and insert a valid name for a command configuration file\n");
        fprintf(stderr, "as an -f option argument. If such file doesn't exist, the program will create it\n");
        fprintf(stderr, "at server0451/bin directory automatically. The file will contain default values.\n");
        exit(1);
    }
}

// Вывести в консоль текущее время в человекочитаемом формате.
void print_timestamp(uint32_t port)
{
    #define BUF_LEN 50

	time_t posix_time;
	char buf[BUF_LEN] = {0};
	struct tm *time_fields;

	posix_time = time(NULL);
	time_fields = localtime(&posix_time);

	strftime(buf, BUF_LEN, "date: %d.%m.%Y, time (UTC): %H:%M:%S", time_fields);
	printf("Port %u, %s\n", port, buf);
}

bool HTTP_is_mentioned(char *buf)
{
    if (strstr(buf, "HTTP")) {
        return 1;
    } else {
        return 0;
    }
}

void cmd_handler(int32_t connfd, char *cmd_config_file_name, uint32_t verbosity_level)
{
    /*--- Нахождение полного пути к настроечному файлу ---*/
        
    char cmd_config_file_path[CMD_CONFIG_FILE_NAME_LEN] = {0};
    readlink("/proc/self/exe", cmd_config_file_path, sizeof(cmd_config_file_path));
    char *ptr = strrchr(cmd_config_file_path, '/') + 1;
    strcpy(ptr, "../.config/");
    strcat(cmd_config_file_path, cmd_config_file_name);
    

    /*--- Чтение настроечного файла ---*/

    // Считывание значений.
    char cmd_config_file_contents[CMD_CONFIG_FILE_LIST_LEN][STR_MAX_LEN] = {0};
    cmd_config_file_read_else_write_defaults(cmd_config_file_contents, cmd_config_file_path);


    /*--- Чтение направленных клиентом данных ---*/

    char buf[STR_MAX_LEN + 1] = {0};

    // Читаем сообщение от клиента и сохраняем его в буфер.
    read(connfd, buf, sizeof(buf));

    // Выводим содержимое буфера.
    if (verbosity_level > 0) {
        utilities_nullify_all_trailing_CR_and_LF_in_string(buf);
        utilities_append_LF_to_string(buf, sizeof(buf));
        utilities_append_LF_to_string(buf, sizeof(buf));
        printf("\nMessage received from the client:\n%s", buf);
    }


    /*--- Поиск и обработка поступившей команды ---*/

	// Поиск команды в поступившем от клиента сообщении.
    bool is_setloadon = strstr(buf, cmd_config_file_contents[CMD_CONFIG_FILE_ASCII_CMD_ON]) ||         \
                        strstr(buf, cmd_config_file_contents[CMD_CONFIG_FILE_URI_CMD_ON])   ||         \
                        strstr(buf, cmd_config_file_contents[CMD_CONFIG_FILE_VAL_CMD_ON]) ;

    bool is_setloadoff = strstr(buf, cmd_config_file_contents[CMD_CONFIG_FILE_ASCII_CMD_OFF]) ||       \
                         strstr(buf, cmd_config_file_contents[CMD_CONFIG_FILE_URI_CMD_OFF])   ||       \
                         strstr(buf, cmd_config_file_contents[CMD_CONFIG_FILE_VAL_CMD_OFF]) ;

    bool is_setloadtoggle = strstr(buf, cmd_config_file_contents[CMD_CONFIG_FILE_ASCII_CMD_TOGGLE]) || \
                            strstr(buf, cmd_config_file_contents[CMD_CONFIG_FILE_URI_CMD_TOGGLE])   || \
                            strstr(buf, cmd_config_file_contents[CMD_CONFIG_FILE_VAL_CMD_TOGGLE]) ;

    bool is_request = strstr(buf, cmd_config_file_contents[CMD_CONFIG_FILE_REQUEST_CMD]);

    bool is_HTTP = HTTP_is_mentioned(buf);


	/*--- Запись в файл нового предписываемого состояния нагрузки и направление ответа клиенту ---*/

	// Проверка текущего предписываемого состояния нагрузки.
	bool current_load_status_cmd = cmd_config_file_contents[CMD_CONFIG_FILE_CURRENT_CMD][0] - '0';
	
    if (is_setloadtoggle) {
        if (current_load_status_cmd) {
            is_setloadon = 0;
        	is_setloadoff = 1;
        } else {
        	is_setloadon = 1;
        	is_setloadoff = 0;
        }
    }

    if (is_setloadon) {
        cmd_config_file_update_current_load_status_cmd(cmd_config_file_path, 1);

        memset(buf, '\0', sizeof(buf));
        char *ptr = buf;

        if (is_HTTP) {
            strcat(buf, HTTP_RESPONSE_200);
            ptr += strlen(HTTP_RESPONSE_200);
        }

        strcat(buf, "New command posted: load ON.\n");

        if (verbosity_level > 0) {
            printf("%s", ptr);
            printf("Communication closed.\n");
        }

        write(connfd, buf, strlen(buf));
        return;
    }

    if (is_setloadoff) {
        cmd_config_file_update_current_load_status_cmd(cmd_config_file_path, 0);
        
        memset(buf, '\0', sizeof(buf));
        char *ptr = buf;
    
        if (is_HTTP) {
            strcat(buf, HTTP_RESPONSE_200);
            ptr += strlen(HTTP_RESPONSE_200);
        }
    
        strcat(buf, "New command posted: load OFF.\n");
    
        if (verbosity_level > 0) {
            printf("%s", ptr);
            printf("Communication closed.\n");
        }

        write(connfd, buf, strlen(buf));
        return;
    }
        
    if (is_request) {
        if (current_load_status_cmd) {
        	strcpy(buf, cmd_config_file_contents[CMD_CONFIG_FILE_ASCII_CMD_ON]);	
        } else {
        	strcpy(buf, cmd_config_file_contents[CMD_CONFIG_FILE_ASCII_CMD_OFF]);
        }

		// Позволяет клиенту быстрее считать ответ, реагируя на символ конца строки.
        strcat(buf, "\n");

        if (verbosity_level > 0) {
            printf("Current command requested.\n");
            printf("Current command sent: %s", buf);
            printf("Communication closed.\n");
        }

        write(connfd, buf, strlen(buf));
        return;
    }

	/* Программа доходит до этой точки только в случае, если в сообщении
	 * от клиента не было найдено ни одной валидной команды.
	 */
    if (verbosity_level > 0) {
        printf("No valid command received.\n");
        printf("Communication closed.\n");
    }

    memset(buf, '\0', sizeof(buf));

    if (is_HTTP) {
        strcat(buf, HTTP_RESPONSE_404);
    }
    
    strcat(buf, "No valid command received\n");

    write(connfd, buf, strlen(buf));
}
