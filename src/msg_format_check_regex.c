/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: msg_format_regex_check.c
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: проверка формата сообщения с помощью регулярных выражений.
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
#include <regex.h>

// Локальные модули.
#include "msg_format_check_regex.h"


/******************** ФУНКЦИИ *******************/

uint32_t msg_format_check_regex(char *buf, const char *regex_pattern)
{
    regex_t regex_ptr;

    if (regcomp(&regex_ptr, regex_pattern, REG_EXTENDED) != 0) {
        return MSG_FORMAT_REGEX_COMP_FAIL;
    }

    size_t nmatch = 1;
    regmatch_t pmatch[1] = {0};

    if (regexec(&regex_ptr, buf, nmatch, pmatch, 0) != 0) {
        return MSG_FORMAT_NO_MATCH;
    }

    uint32_t msg_len = strlen(buf);
    
    uint32_t offset_end = pmatch[0].rm_eo;
    buf[offset_end] = 0;  // Обрезаем остаток строки.
    uint32_t match_len = strlen(buf);

    if (msg_len == match_len) {
    	return MSG_FORMAT_MATCH;
    } else {
    	return MSG_FORMAT_PARTIAL_MATCH;
    }
}
