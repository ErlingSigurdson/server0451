/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: config_general.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: общие настройки сервера.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Защита от повторного включения заголовочного файла.
#ifndef CONFIG_GENERAL_H
#define CONFIG_GENERAL_H


/*--- Прочее ---*/

// Предельная длина строки.
#define STR_MAX_LEN 1000

// Минимальная длина пароля.
#define PASSWORD_MIN_LEN 5

// Формат, которому должны следовать сообщения от клиентов.
#define DELIM_STR  ":"
#define DELIM_CHAR ':'
#define MSG_FORMAT_REGEX_PATTERN DELIM_STR "[tT][oO][pP][iI][cC]_([1-9][0-9]{0,1}|100)" DELIM_STR "[aA][tT]\\+[a-zA-Z]+=*[a-zA-Z]*"

// Команды.
#define CMD_LOAD_ON        "AT+SETLOAD=ON"
#define CMD_LOAD_OFF       "AT+SETLOAD=OFF"
#define CMD_LOAD_TOGGLE    "AT+SETLOAD=TOGGLE"
#define CMD_TOPIC_REQUEST  "AT+SERVETOPIC"

// Директивы управления условной компиляцией кода, устанавливающего опции сокетов.
#define SOCKOPT_SO_REUSEPORT
#define SOCKOPT_SO_REUSEADDR
#define SOCKOPT_SO_LINGER

// Значения для установки опций сокетов.
#define L_LINGER 2


#endif  // Защита от повторного включения заголовочного файла.
