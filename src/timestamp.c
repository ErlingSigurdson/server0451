sss/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: timestamp.c
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: вывод текущей даты и времени (UTC+0) в человекочитаемом формате.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

/*--- Включения ---*/

// Из стандартной библиотеки языка Си.
#include <stdio.h>
//#include <inttypes.h>
//#include <stdbool.h>
//#include <string.h>
//#include <stdlib.h>
//#include <errno.h>

// Из библиотек POSIX.
#include <time.h>

// Локальные модули.
#include "timestamp.h"
#include "config_general.h"


/******************** ФУНКЦИИ *******************/

void timestamp_print()
{
    char buf[STR_MAX_LEN + 1] = {0};
    time_t posix_time;
    struct tm *time_fields;

    posix_time = time(NULL);
    time_fields = localtime(&posix_time);

    strftime(buf, sizeof(buf), "date: %d.%m.%Y, time (UTC+0): %H:%M:%S", time_fields);
    printf("%s", buf);
}
