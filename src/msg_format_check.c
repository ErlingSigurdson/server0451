/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: msg_format_check.c
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: проверка формата сообщения с помощью регулярных выражений.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

/*--- Включения ---*/

// Из стандартной библиотеки языка Си.
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

// Из библиотек POSIX.
#include <regex.h>

// Локальные модули.
#include "msg_format_check.h"


/******************** ФУНКЦИИ *******************/

uint32_t msg_format_check(char *buf, const char *regex_pattern)
{
    /*--- Компиляция регулярного выражения ---*/
    
    regex_t regex_ptr;

    if (regcomp(&regex_ptr, regex_pattern, REG_EXTENDED) != 0) {
        return MSG_FORMAT_CHECK_REGEX_COMP_FAIL;
    }
    

    /*--- Применение регулярного выражения ---*/
    
    size_t nmatch = 1;
    regmatch_t pmatch[1] = {0};

    bool match_result = regexec(&regex_ptr, buf, nmatch, pmatch, 0);
    regfree(&regex_ptr);  /* Очищаем выделенную под регулярное выражение память,
                           * иначе случится утечка памяти.
                           */

    if (match_result != 0) {
        return MSG_FORMAT_CHECK_NO_MATCH;
    }


    /*--- Проверка полноты совпадения ---*/
    
    uint32_t msg_len = strlen(buf);
    uint32_t offset_end = pmatch[0].rm_eo;
    buf[offset_end] = '\0';  // Обрезаем остаток строки.
    uint32_t match_len = strlen(buf);

    if (msg_len == match_len) {
        return MSG_FORMAT_CHECK_MATCH;
    } else {
        return MSG_FORMAT_CHECK_PARTIAL_MATCH;
    }
}
