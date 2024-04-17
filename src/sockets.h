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
//#include <stdbool.h>
//#include <string.h>
//#include <stdlib.h>
//#include <errno.h>


/*--- Прочее ---*/

// Значения, возвращаемые функцией sockets_init().
#define SOCKET_INIT_ERR_CREATE   3
#define SOCKET_INIT_ERR_BIND     2
#define SOCKET_INIT_ERR_LISTEN   1
#define SOCKET_INIT_OK           0

// Значения, возвращаемые функцией sockets_set_connection().
#define SOCKET_SETCON_ERR_ACCEPT 1
#define SOCKET_SETCON_OK         0

// Значения, возвращаемые функцией sockets_peek().
#define SOCKET_PEEK_ERR_SELECT   2
#define SOCKET_PEEK_TIMEOUT      1
#define SOCKET_PEEK_OK           0

// Максимальная длина очереди из ожидающих подключений, второй аргумент для передачи в функцию listen().
#define SOCKET_BACKLOG           1024

// Длительность таймаута для функции select().
#define SELECT_TIMEOUT_SEC       10
#define SELECT_TIMEOUT_USEC      0


/*************** ПРОТОТИПЫ ФУНКЦИЙ **************/

int32_t sockets_init(int32_t *sockfd, int32_t port, uint32_t verbosity_level);
int32_t sockets_set_connection(int32_t sockfd, int32_t *connfd, int32_t port, uint32_t verbosity_level);
int32_t sockets_peek(int32_t connfd, uint32_t verbosity_level);
void sockets_read_message(int32_t connfd, char *buf, size_t buf_size, uint32_t verbosity_level);
void sockets_write_message(int32_t connfd, char *buf, uint32_t verbosity_level);
int32_t sockets_close(int32_t fd);


#endif  // Защита от повторного включения заголовочного файла.
