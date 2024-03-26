/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: config_general.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: общие настройки проекта.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Защита от повторного включения заголовочного файла.
#ifndef CONFIG_GENERAL_H
#define CONFIG_GENERAL_H


/*--- Включения ---*/

// Локальные модули.
#include "utilities.h"


/*--- Прочее ---*/

// Предельная длина строки.
#define STR_MAX_LEN 1000

// Формат, которому должны следовать сообщения от клиентов.
#define DELIM_STRING ":"
#define DELIM_CHAR   ':'
#define DELIM_RAW     :
#define MSG_FORMAT_REGEX_PATTERN "[tT][oO][pP][iI][cC]_([1-9][0-9]{0,1}|100)" STRINGIFY(DELIM_RAW) "[aA][tT]\\+[a-zA-Z]+=*[a-zA-Z]*"

// Команды.
#define CMD_LOAD_ON        "AT+SETLOAD=ON"
#define CMD_LOAD_OFF       "AT+SETLOAD=OFF"
#define CMD_LOAD_TOGGLE    "AT+SETLOAD=TOGGLE"
#define CMD_TOPIC_REQUEST  "AT+SERVETOPIC"


#endif  // Защита от повторного включения заголовочного файла.
