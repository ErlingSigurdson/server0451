/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: utilities.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: некоторые вспомогательные макросы и функции.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Защита от повторного включения заголовочного файла.
#ifndef UTILITIES_H
#define UTILITIES_H


/*--- Включения ---*/

// Из стандартной библиотеки языка Си.
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>


/*--- Прочее ---*/

// Пара макросов для превращения аргумента в строку.
#define STRINGIFY(x) INTERMEDIATE_STRINGIFY(x)
#define INTERMEDIATE_STRINGIFY(x) #x


/*************** ПРОТОТИПЫ ФУНКЦИЙ **************/

/*--- Работа со строками ---*/

bool utilities_remove_CR_and_LF(char *buf);
bool utilities_append_CR_and_LF(char *buf, size_t buf_size);
bool utilities_append_LF(char *buf, size_t buf_size);
bool utilities_append_LF_if_absent(char *buf, size_t buf_size);
bool utilities_force_2xLF(char *buf, size_t buf_size);


/*--- Работа с файлами ---*/

void utilities_write_to_file_single_line(char *str, char *file_path);
void utilities_read_from_file_single_line(char *buf, size_t buf_size, char *file_path);
void utilities_read_from_file(char *buf, size_t buf_size, char *file_path);
void utilities_file_abs_path_cpy(char *buf, size_t buf_size, char *file_name);


#endif  // Защита от повторного включения заголовочного файла.
