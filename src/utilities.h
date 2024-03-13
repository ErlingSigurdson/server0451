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

bool utilities_nullify_first_CR_or_LF_in_string(char *buf);
bool utilities_nullify_all_CR_and_LF_in_char_array(char *buf, size_t buf_size);
bool utilities_nullify_all_trailing_CR_and_LF_in_string(char *buf);
bool utilities_substitute_all_CR_and_LF_in_char_array(char *buf, size_t buf_size, char character);
bool utilities_append_LF_to_string(char *buf, size_t buf_size);
bool utilities_append_LF_if_absent_to_string(char *buf, size_t buf_size);
bool utilities_append_CR_to_string(char *buf, size_t buf_size);


/*--- Работа с файлами ---*/

void utilities_write_to_file_single_line(char *str, char *file_path);
void utilities_read_from_file_single_line(char *buf, size_t buf_size, char *file_path);
void utilities_read_from_file(char *buf, size_t buf_size, char *file_path);


#endif  // Защита от повторного включения заголовочного файла.
