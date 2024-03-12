/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: utilities.c
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: некоторые вспомогательные макросы и функции.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

/*--- Включения ---*/

// Локальные модули.
#include "utilities.h"

// Из стандартной библиотеки языка Си.
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// Из других библиотек.
#include <unistd.h>  // Для readlink().


/******************** ФУНКЦИИ *******************/

/*--- Работа со строками ---*/

bool utilities_remove_CR_and_LF(char *buf)
{
    for (uint32_t i = 0; i < strlen(buf); ++i) {
        if (buf[i] == '\r' || buf[i] == '\n') {
            buf[i] = '\0';

            return 1;
        }
    }

    return 0;
}

bool utilities_append_CR_and_LF(char *buf, size_t buf_size)
{
    if (buf_size - strlen(buf) >= 3) {  // Один байт для CR, второй для LF, третий для нуля.
        buf[strlen(buf)] = '\r';
        buf[strlen(buf)] = '\n';

        return 1;
    }

    return 0;
}

bool utilities_append_LF(char *buf, size_t buf_size)
{
    if (buf_size - strlen(buf) >= 2) {  // Один байт для LF, второй для нуля.
        buf[strlen(buf)] = '\n';

        return 1;
    }

    return 0;
}

bool utilities_append_LF_if_absent(char *buf, size_t buf_size)
{
    if (buf[strlen(buf) - 1] == '\n') {
        return 0;
    } else if (buf_size - strlen(buf) >= 2) {  // Один байт для LF, второй для нуля.
        buf[strlen(buf)] = '\n';

        return 1;
    }

    return 0;
}

bool utilities_force_2xLF(char *buf, size_t buf_size)
{
    for (uint32_t i = 0; i < strlen(buf); ++i) {
        if (buf[i] == '\r' || buf[i] == '\n') {
            buf[i] = '\0';
        }
    }
    
    if (buf_size - strlen(buf) >= 3) {  // Два байта для LF, третий для нуля.
        buf[strlen(buf)] = '\n';
        buf[strlen(buf)] = '\n';

        return 1;
    }

    return 0;
}


/*--- Работа с файлами ---*/

void utilities_write_to_file_single_line(char *str, char *file_path)
{
    FILE *f = fopen(file_path, "w");
    fputs(str, f);
    fclose(f);
}

void utilities_read_from_file_single_line(char *buf, size_t buf_size, char *file_path)
{
    FILE *f = fopen(file_path, "r");
    fgets(buf, buf_size, f);
    fclose(f);
}

void utilities_read_from_file(char *buf, size_t buf_size, char *file_path)
{
    FILE *f = fopen(file_path, "r");

    uint32_t i = 0;
    char c = 0;
    while (c != EOF && i < buf_size - 1) {
    	c = fgetc(f);

    	if (c != EOF) {
    		buf[i] = c;
    	}

    	++i;
    }

    fclose(f);
}

void utilities_file_abs_path_cpy(char *buf, size_t buf_size, char *file_name)
{
    readlink("/proc/self/exe", buf, buf_size);
    char *ptr = strrchr(buf, '/') + 1;

    if (ptr == NULL) {
        return;
    }

    memset(ptr, '\0', strlen(ptr));
    strcat(ptr, file_name);
}
