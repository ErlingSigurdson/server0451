/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: sockets.c
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: базовая работа с сокетами в Linux.
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
//#include <stdlib.h>
#include <errno.h>

// Из библиотек POSIX.
#include <unistd.h>
#include <netinet/in.h>

// Локальные модули.
#include "utilities.h"
#include "sockets.h"


/******************** ФУНКЦИИ *******************/

uint32_t sockets_init(int32_t *sockfd, int32_t port, uint32_t verbosity_level)
{
    /*--- Создание сокета ---*/
    
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd < 0) {
        return SOCKET_ERR_CREATE;
    } else if (verbosity_level > 0) {
        printf("\n...socket successfully created at port %d.\n", port);
    }


    /*--- Установка опций сокетов (опционально) ---*/

    /* Разрешение заново использовать IP-адрес
     * немедленно после закрытия предыдущего соединения.
     * Если это делается, то обязательно до вызова bind().
     */
    const int32_t so_reuseaddr = 1;
    int32_t socket_option_a = setsockopt(*sockfd,
                           SOL_SOCKET,
                           SO_REUSEADDR,
                           &so_reuseaddr,
                           /* То ли мне кажется, то ли в каких-то случаях
                            * увеличение этого значения в два раза исправляло
                            * какие-то проблемы. Бред, наверное, но на всякий
                            * случай оставляю этот комментарий.
                            */
                           (socklen_t)sizeof(so_reuseaddr));

    if (verbosity_level > 1) {
        printf("...setting SO_REUSEADDR socket option, returned value: %d. ", socket_option_a);
        printf("Status or error description: %s\n", strerror(errno));
    }

    /* Разрешение закрывать сокет, не дожидаясь завершения передачи данных.
     * Если это делается, то обязательно до вызова bind().
     */
    const int32_t so_linger = 0;
    int32_t socket_option_b = setsockopt(*sockfd,
                           SOL_SOCKET,
                           SO_LINGER,
                           &so_linger,
                           /* То ли мне кажется, то ли в каких-то случаях
                            * увеличение этого значения в два раза исправляло
                            * какие-то проблемы. Бред, наверное, но на всякий
                            * случай оставляю этот комментарий.
                            */
                           (socklen_t)sizeof(so_linger));

    if (verbosity_level > 1) {
        printf("...clearing SO_LINGER socket option, returned value: %d. ", socket_option_b);
        printf("Status or error description: %s\n", strerror(errno));
    }


    /*--- Привязка вновь созданного сокета к IP-адресу ---*/

    struct sockaddr_in serveraddr;

    // Заполнение структуры нулями.
    memset(&serveraddr, '\0', sizeof(serveraddr));  /* Иногда используют функцию bzero(),
                                                     * но она считается устаревшей.
                                                     */

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);

    if (bind(*sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) != 0) {
        return SOCKET_ERR_BIND;
    } else if (verbosity_level > 0) {
        printf("...socket successfully bound at port %d.\n", port);
    }

    return SOCKET_OK;
}

uint32_t sockets_set_connection(int32_t sockfd, int32_t *connfd, int32_t port, uint32_t verbosity_level)
{
    int32_t socklen = 0;
    struct sockaddr_in clientaddr;

    // Сервер начинает слушать.
    if (listen(sockfd, SOCKET_BACKLOG) != 0) {
        return SOCKET_ERR_LISTEN;
    } else if (verbosity_level > 0) {
        printf("...server is listening at port %d.\n", port);
    }

    // Установка связи с клиентом.
    socklen = sizeof(clientaddr);
    *connfd = accept(sockfd, (struct sockaddr*)&clientaddr, (socklen_t*)&socklen);
    if (*connfd < 0) {
        return SOCKET_ERR_ACCEPT;
    } else if (verbosity_level > 0) {
        printf("\nServer accepted a client at port %d. Starting communication.\n", port);
    }

    return SOCKET_OK;
}

void sockets_read_message(int32_t connfd, char *buf, size_t buf_size, uint32_t verbosity_level)
{
    // Чтение сообщения от клиента и его запись в буфер.
    read(connfd, buf, buf_size);
    utilities_nullify_all_trailing_CR_and_LF_in_string(buf);
    
    // Вывод содержимого буфера.
    if (verbosity_level > 0) {
        printf("\nMessage received from the client:\n%s\n", buf);
    }
}

void sockets_write_message(int32_t connfd, char *buf, uint32_t verbosity_level)
{  
    utilities_nullify_all_trailing_CR_and_LF_in_string(buf);

    strcat(buf, "\n"); 
    write(connfd, buf, strlen(buf));

    utilities_nullify_all_trailing_CR_and_LF_in_string(buf);
    
    if (verbosity_level > 0) {
        printf("\nMessage sent to the client:\n%s\n", buf);    
    }
}

void sockets_close(int32_t sockfd)
{
    // Закрытие сокета.
    shutdown(sockfd, SHUT_RDWR);  // Вроде бы не обязательно, но иногда рекомендуется.
    close(sockfd);
}
