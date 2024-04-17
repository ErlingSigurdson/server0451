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

// Из стандартной библиотеки языка Си.
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
//#include <stdlib.h>
//#include <errno.h>

// Локальные модули.
#include "utilities.h"


/******************** ФУНКЦИИ *******************/

/*--- Работа со строками ---*/

bool utilities_nullify_first_CR_or_LF_in_string(char *buf)
{
    for (uint32_t i = 0; i < strlen(buf); ++i) {
        if (buf[i] == '\r' || buf[i] == '\n') {
            buf[i] = '\0';

            return 1;
        }
    }

    return 0;
}

bool utilities_nullify_all_CR_and_LF_in_char_array(char *buf, size_t buf_size)
{
    uint32_t i = 0;
    for (uint32_t j = 0; j < buf_size - 1; ++j) {
        if (buf[j] == '\r' || buf[i] == '\n') {
            buf[j] = '\0';
            ++i;
        }
    }

    return i;
}

bool utilities_nullify_all_trailing_CR_and_LF_in_string(char *buf)
{
    uint32_t i = 0;
    while (buf[strlen(buf) - 1] == '\r' || buf[strlen(buf) - 1] == '\n') {
        buf[strlen(buf) - 1] = '\0';
        ++i;
    }

    return i;
}

bool utilities_substitute_all_CR_and_LF_in_char_array(char *buf, size_t buf_size, char character)
{
    uint32_t i = 0;
    for (uint32_t j = 0; j < buf_size - 1; ++j) {
        if (buf[j] == '\r' || buf[i] == '\n') {
            buf[j] = character;
            ++i;
        }
    }

    return i;
}

bool utilities_append_LF_to_string(char *buf, size_t buf_size)
{
    if (buf_size - strlen(buf) >= 2) {  // Один байт для LF, второй для нуля.
        buf[strlen(buf)] = '\n';

        return 1;
    }

    return 0;
}

bool utilities_append_LF_if_absent_to_string(char *buf, size_t buf_size)
{
    if (buf[strlen(buf) - 1] == '\n') {
        return 0;
    } else if (buf_size - strlen(buf) >= 2) {  // Один байт для LF, второй для нуля.
        buf[strlen(buf)] = '\n';

        return 1;
    }

    return 0;
}

bool utilities_append_CR_to_string(char *buf, size_t buf_size)
{
    if (buf_size - strlen(buf) >= 2) {  // Один байт для CR, второй для нуля.
        buf[strlen(buf)] = '\r';

        return 1;
    }

    return 0;
}

void utilities_to_lowercase_string(char *buf)
{
    for (uint32_t i = 0; i < strlen(buf); ++i) {
        if (buf[i] >= 'A' && buf[i] <= 'Z') {
            buf[i] += 32;  // Разница между ASCII-кодом заглавной и строчной буквы.
        }
    }
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

    if (f == NULL) {
        f = fopen(file_path, "w");
        freopen(NULL, "r", f);
    }

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
