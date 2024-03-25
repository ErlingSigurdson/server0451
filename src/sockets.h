/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: sockets.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: работа с сокетами.
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


/*************** ПРОТОТИПЫ ФУНКЦИЙ **************/

void sockets_init(int32_t *sockfd, int32_t port, uint32_t verbosity_level);
void sockets_set_connection(int32_t sockfd, int32_t *connfd, int32_t port, uint32_t verbosity_level);
void sockets_read_message(int32_t connfd, char *buf, size_t buf_size, uint32_t verbosity_level);
void sockets_write_message(int32_t connfd, char *buf, uint32_t verbosity_level);
void sockets_close(int32_t sockfd);


#endif  // Защита от повторного включения заголовочного файла.
