/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: sockets.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: работа с сокетами TCP/IP.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Защита от повторного включения заголовочного файла.
#ifndef SOCKETS_H
#define SOCKETS_H


/*--- Включения ---*/

// Из стандартной библиотеки языка Си.
#include <stdio.h>
#include <inttypes.h>


/*--- Прочее ---*/

// Значения, возвращаемые функцией sockets_init().
#define SOCKETS_INIT_ERR_CREATE     4
#define SOCKETS_INIT_ERR_SETSOCKOPT 3
#define SOCKETS_INIT_ERR_BIND       2
#define SOCKETS_INIT_ERR_LISTEN     1
#define SOCKETS_INIT_OK             0

// Значения, возвращаемые функцией sockets_proceed().
#define SOCKETS_PROCEED_ERR_ACCEPT 3
#define SOCKETS_PROCEED_ERR_SELECT 2
#define SOCKETS_PROCEED_TIMEOUT    1
#define SOCKETS_PROCEED_OK         0


/*************** ПРОТОТИПЫ ФУНКЦИЙ **************/

uint32_t sockets_init(int32_t *sockfd, int32_t port, uint32_t numconn, uint32_t verbosity_level);
uint32_t sockets_proceed(int32_t sockfd, int32_t *connfd, uint32_t timeout_sec, uint32_t verbosity_level);
void sockets_read_message(int32_t connfd, char *buf, size_t buf_size, uint32_t verbosity_level);
void sockets_write_message(int32_t connfd, char *buf, uint32_t verbosity_level);
int32_t sockets_close(int32_t fd, uint32_t pause);


#endif  // Защита от повторного включения заголовочного файла.
