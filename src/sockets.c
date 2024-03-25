/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: sockets.c
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: работа с сокетами.
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
#include <netinet/in.h>

/* Эти заголовочные файлы упоминаются в примерах серверов, написанных на языке Си,
 * но код компилируется и работает и без них. Оставляю на память.
 */
//#include <netdb.h>
//#include <sys/socket.h>
//#include <sys/types.h>

// Локальные модули.
#include "utilities.h"
#include "sockets.h"


/******************** ФУНКЦИИ *******************/

uint32_t sockets_init(int32_t *sockfd, int32_t port, uint32_t verbosity_level)
{
    // Создание сокета.
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd < 0) {
        return 2;
    } else if (verbosity_level > 0) {
    	printf("\n...socket successfully created at port %d.\n", port);
    }

    struct sockaddr_in serveraddr;

    // Заполнение структуры нулями.
    memset(&serveraddr, '\0', sizeof(serveraddr));  // Иногда используют функцию bzero(), но она считается устаревшей.

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);

    /* Позволяем заново использовать IP-адрес немедленно после закрытия
     * предыдущего соединения. Обязательно сделать это до вызова bind().
     */
    uint32_t so_reuseaddr = 1;
    int32_t a = setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, (socklen_t)sizeof(so_reuseaddr) * 2);

	if (verbosity_level > 1) {
        printf("...setting SO_REUSEADDR return value: %d. ", a);
        printf("Status or error description: %s\n", strerror(errno));
	}

    /* Позволяем закрывать сокет, не дожидаясь конца передачи данных.
     * Обязательно сделать это до вызова bind().
     */
    uint32_t so_linger = 0;
    int32_t b = setsockopt(*sockfd, SOL_SOCKET, SO_LINGER, &so_linger, (socklen_t)sizeof(so_linger) * 2);

    if (verbosity_level > 1) {
        printf("...clearing SO_LINGER return value: %d. ", b);
        printf("Status or error description: %s\n", strerror(errno));
    }

    // Привязать вновь соданный сокет к IP.
    if ((bind(*sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr))) != 0) {
        return 1;
    } else if (verbosity_level > 0) {
        printf("...socket successfully bound at port %d.\n", port);
    }

    return 0;
}

uint32_t sockets_set_connection(int32_t sockfd, int32_t *connfd, int32_t port, uint32_t verbosity_level)
{
    int32_t socklen = 0;
    struct sockaddr_in clientaddr;

    // Сервер начинает слушать.
    if ((listen(sockfd, 5)) != 0) {
        return 2;
    } else if (verbosity_level > 0) {
        printf("...server is listening at port %d.\n", port);
    }

    // Устанавливает связь с клиентом..
    socklen = sizeof(clientaddr);
    *connfd = accept(sockfd, (struct sockaddr*)&clientaddr, (socklen_t*)&socklen);
    if (*connfd < 0) {
        return 1;
    } else if (verbosity_level > 0) {
        printf("\nServer accepted a client at port %d. Starting communication.\n", port);
    }

    return 0;
}

void sockets_read_message(int32_t connfd, char *buf, size_t buf_size, uint32_t verbosity_level)
{
    // Читаем сообщение от клиента и сохраняем его в буфере.
    read(connfd, buf, buf_size);
    utilities_nullify_all_trailing_CR_and_LF_in_string(buf);
    
    // Выводим содержимое буфера.
    if (verbosity_level > 0) {
        utilities_nullify_all_trailing_CR_and_LF_in_string(buf);
        printf("\nMessage received from the client:\n%s\n\n", buf);
    }
}

void sockets_write_message(int32_t connfd, char *buf, uint32_t verbosity_level)
{
    if (verbosity_level > 0) {
        printf("Message sent to the client: %s\n", buf);	
    }
    
    write(connfd, buf, strlen(buf));
}

void sockets_close(int32_t sockfd)
{
    // Закрытие сокета.
    //shutdown(sockfd, SHUT_RDWR);  // Вроде бы не нужно, но иногда упоминается.
    close(sockfd);
}
