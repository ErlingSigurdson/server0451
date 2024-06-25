/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла:  sockets.c
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: работа с сокетами TCP/IP.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

/*--- Включения ---*/

// Из стандартной библиотеки языка Си.
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

// Из библиотек POSIX.
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>

/* Эти заголовочные файлы упоминаются в примерах серверов, написанных
 * на языке Си, но код компилируется и работает и без них. Оставляю на память.
 */
//#include <netdb.h>
//#include <sys/socket.h>
//#include <sys/types.h>

// Настройки.
#include "config_general.h"

// Локальные модули.
#include "sockets.h"
#include "utilities.h"
#include "timestamp.h"

/******************** ФУНКЦИИ *******************/

uint32_t sockets_init(int32_t *sockfd, int32_t port, uint32_t numconn, uint32_t verbosity_level)
{
    /*--- Создание сокета ---*/

    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd < 0) {
        return SOCKETS_INIT_ERR_CREATE;
    } else if (verbosity_level > 0) {
        printf("\n...socket successfully created.\n");
    }


    /*--- Установка опций сокетов ---*/

    /* Условной компиляцией можно управлять с помощью соответствующих директив,
     * приведённых в файле config_general.h.
     */

    #ifdef SOCKOPT_SO_REUSEPORT
        /* Разрешение заново использовать сочетание порта и адреса
         * немедленно после закрытия предыдущего соединения.
         * Если это делается, то обязательно до вызова bind().
         */
        int32_t so_reuseport = 1;
        int32_t so_reuseport_result = setsockopt(*sockfd,
                                                 SOL_SOCKET,
                                                 SO_REUSEPORT,
                                                 &so_reuseport,
                                                 (socklen_t)sizeof(so_reuseport));

        if (so_reuseport_result != 0) {
            return SOCKETS_INIT_ERR_SETSOCKOPT;
        } else if (verbosity_level > 1) {
            printf("...setting SO_REUSEPORT socket option. Success.\n");
        }
    #endif

    #ifdef SOCKOPT_SO_REUSEADDR
        /* Разрешение заново использовать адрес
         * немедленно после закрытия предыдущего соединения.
         * Если это делается, то обязательно до вызова bind().
         */
        int32_t so_reuseaddr = 1;
        int32_t so_reuseaddr_result = setsockopt(*sockfd,
                                                 SOL_SOCKET,
                                                 SO_REUSEADDR,
                                                 &so_reuseaddr,
                                                 (socklen_t)sizeof(so_reuseaddr));

        if (so_reuseaddr_result != 0) {
            return SOCKETS_INIT_ERR_SETSOCKOPT;
        } else if (verbosity_level > 1) {
            printf("...setting SO_REUSEADDR socket option. Success.\n");
        }
    #endif

    #ifdef SOCKOPT_SO_LINGER
        /* Установка времени ожидания полной передачи данных.
         * Если это делается, то обязательно до вызова bind().
         */
        struct linger so_linger;
        so_linger.l_onoff = 1;
        so_linger.l_linger = L_LINGER;
        int32_t so_linger_result = setsockopt(*sockfd,
                                              SOL_SOCKET,
                                              SO_LINGER,
                                              &so_linger,
                                              (socklen_t)sizeof(so_linger));

        if (so_linger_result != 0) {
            return SOCKETS_INIT_ERR_SETSOCKOPT;
        } else if (verbosity_level > 1) {
            printf("...setting SO_LINGER socket option, specified linger time is %d sec. Success.\n",
                   so_linger.l_linger);
        }
    #endif


    /*--- Связка вновь созданного сокета и адреса ---*/

    struct sockaddr_in serveraddr;

    // Заполнение структуры нулями.
    memset(&serveraddr, '\0', sizeof(serveraddr));  /* Иногда используют функцию bzero(),
                                                     * но она считается устаревшей.
                                                     */

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);

    if (bind(*sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) != 0) {
        return SOCKETS_INIT_ERR_BIND;
    } else if (verbosity_level > 0) {
        printf("...socket successfully bound.\n");
    }


    /*--- Связка вновь созданного сокета и адреса ---*/

    if (listen(*sockfd, numconn) != 0) {
        return SOCKETS_INIT_ERR_LISTEN;
    } else if (verbosity_level > 0) {
        printf("...server is listening.\n");
    }

    return SOCKETS_INIT_OK;
}

uint32_t sockets_proceed(int32_t sockfd, int32_t *connfd, uint32_t timeout_sec, uint32_t verbosity_level)
{
    /*--- Установка связи с клиентом ---*/
    
    int32_t socklen = 0;
    struct sockaddr_in clientaddr;

    socklen = sizeof(clientaddr);
    *connfd = accept(sockfd, (struct sockaddr*)&clientaddr, (socklen_t*)&socklen);
    if (*connfd < 0) {
        return SOCKETS_PROCEED_ERR_ACCEPT;
    } else if (verbosity_level > 0) {
        printf("\nServer accepted a client, waiting for incoming data. ");
        timestamp_print();
        printf(".\n");
        fflush(stdout);
    }


    /*--- Проверка наличия данных в сокете ---*/
    
    struct timeval tv;
    tv.tv_sec = timeout_sec;
    tv.tv_usec = 0;
                   
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(*connfd, &readfds);

    int32_t result = select(*connfd + 1, &readfds, NULL, NULL, &tv);
    int32_t retval = -1;

    if (result < 0) {
        retval = SOCKETS_PROCEED_ERR_SELECT;
    } else if (result == 0) {
        retval = SOCKETS_PROCEED_TIMEOUT;
        if (verbosity_level > 0) {
            printf("\nConnection closed due to timeout.\n");
        }
    } else if (result > 0) {
        retval = SOCKETS_PROCEED_OK;
    }

    return retval;
}

void sockets_read_message(int32_t connfd, char *buf, size_t buf_size, uint32_t verbosity_level)
{
    // Чтение сообщения от клиента и его запись в буфер.
    read(connfd, buf, buf_size);
    utilities_nullify_all_trailing_CR_and_LF_in_string(buf);

    // Вывод содержимого буфера.
    if (verbosity_level > 0) {
        printf("\nMessage received from the client: %s\n", buf);
    }
}

void sockets_write_message(int32_t connfd, char *buf, uint32_t verbosity_level)
{
    utilities_nullify_all_trailing_CR_and_LF_in_string(buf);

    strcat(buf, "\n");
    write(connfd, buf, strlen(buf));

    utilities_nullify_all_trailing_CR_and_LF_in_string(buf);

    if (verbosity_level > 0) {
        printf("\nMessage sent to the client: %s\n", buf);
    }
}

int32_t sockets_close(int32_t fd, uint32_t pause)
{
    //shutdown(fd, SHUT_RDWR);  // Вроде не обязательно, хотя иногда рекомендуют.
    usleep(pause);
    int32_t retval = close(fd);
    usleep(pause);

    return retval;
}

void sockets_report_broken_pipe(int32_t sig)
{
    if (sig == SIGPIPE) {
        fprintf(stderr, "\n"
                        "Error: a client had disconnected before any response was sent by the server (broken pipe).\n");
    }
}

void sockets_sig_ign()
{
    signal(SIGPIPE, sockets_report_broken_pipe);
}
