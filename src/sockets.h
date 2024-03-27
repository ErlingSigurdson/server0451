/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: sockets.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: базовая работа с сокетами в Linux.
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

// Коды значений, возвращаемых функцией sockets_init().
#define SOCKET_ERR_CREATE 2
#define SOCKET_ERR_BIND   1

// Коды значений, возвращаемых функцией sockets_set_connection().
#define SOCKET_ERR_LISTEN 2
#define SOCKET_ERR_ACCEPT 1

// Код значения, возвращаемого при успешной отработке функции.
#define SOCKET_OK         0

// Второй аргумент для передачи в функцию listen().
#define SOCKET_BACKLOG    5


/*************** ПРОТОТИПЫ ФУНКЦИЙ **************/

uint32_t sockets_init(int32_t *sockfd, int32_t port, uint32_t verbosity_level);
uint32_t sockets_set_connection(int32_t sockfd, int32_t *connfd, int32_t port, uint32_t verbosity_level);
void sockets_read_message(int32_t connfd, char *buf, size_t buf_size, uint32_t verbosity_level);
void sockets_write_message(int32_t connfd, char *buf, uint32_t verbosity_level);
void sockets_close(int32_t sockfd);


#endif  // Защита от повторного включения заголовочного файла.