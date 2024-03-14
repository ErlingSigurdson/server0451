/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: cmd.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: функции и макросы для обработки команд.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Защита от повторного включения заголовочного файла.
#ifndef CMD_H
#define CMD_H


/*--- Включения ---*/

// Из стандартной библиотеки языка Си.
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

// Настройки проекта.
#include "config_general.h"


/*--- Прочее ---*/

// Предельная длина имени настроечного файла.
#define CMD_FILE_NAME_LEN 1000

// Длина списка команд.
#define CMD_FILE_LIST_LEN 11

// Нумерация команд в списке.
#define CMD_FILE_CURRENT_CMD      0
#define CMD_FILE_ASCII_CMD_ON     1
#define CMD_FILE_URI_CMD_ON       2
#define CMD_FILE_VAL_CMD_ON       3
#define CMD_FILE_ASCII_CMD_OFF    4
#define CMD_FILE_URI_CMD_OFF      5
#define CMD_FILE_VAL_CMD_OFF      6
#define CMD_FILE_ASCII_CMD_TOGGLE 7
#define CMD_FILE_URI_CMD_TOGGLE   8
#define CMD_FILE_VAL_CMD_TOGGLE   9
#define CMD_FILE_REQUEST_CMD      10

// Устанавливаемое по умолчанию содержимое файла с настройками команд.
#define DEFAULT_CMD_FILE_COMMENT_1   	  "// Текущее предписываемое положение нагрузки для данного экземпляра программы.\n"
#define DEFAULT_CMD_FILE_CURRENT_CMD  	  "CURRENT_CMD=0\n"

#define DEFAULT_CMD_FILE_COMMENT_2        "// Команда для установки предписываемого положения нагрузки \"ВКЛ\".\n"
#define DEFAULT_CMD_FILE_ASCII_CMD_ON     "ASCII_CMD_ON=AT+SETLOAD=ON\n"
#define DEFAULT_CMD_FILE_COMMENT_3        "// Команда для установки предписываемого положения нагрузки \"ВКЛ\", помещаемая в URI GET-запроса.\n"
#define DEFAULT_CMD_FILE_URI_CMD_ON       "URI_CMD_ON=/setloadon\n"
#define DEFAULT_CMD_FILE_COMMENT_4        "// Команда для установки предписываемого положения нагрузки \"ВКЛ\", помещаемая в тело POST-запроса (в пару параметр+значение).\n"
#define DEFAULT_CMD_FILE_VAL_CMD_ON       "VAL_CMD_ON=cmd=setloadon\n"

#define DEFAULT_CMD_FILE_COMMENT_5        "// Команда для установки предписываемого положения нагрузки \"ВЫКЛ\".\n"
#define DEFAULT_CMD_FILE_ASCII_CMD_OFF    "ASCII_CMD_OFF=AT+SETLOAD=OFF\n"
#define DEFAULT_CMD_FILE_COMMENT_6        "// Команда для установки предписываемого положения нагрузки \"ВЫКЛ\", помещаемая в URI GET-запроса.\n"
#define DEFAULT_CMD_FILE_URI_CMD_OFF      "URI_CMD_OFF=/setloadoff\n"
#define DEFAULT_CMD_FILE_COMMENT_7        "// Команда для установки предписываемого положения нагрузки \"ВЫКЛ\", помещаемая в тело POST-запроса (в пару параметр+значение).\n"
#define DEFAULT_CMD_FILE_VAL_CMD_OFF      "VAL_CMD_OFF=cmd=setloadoff\n"

#define DEFAULT_CMD_FILE_COMMENT_8        "// Команда для смены предписываемого положения нагрузки на противоположное.\n"
#define DEFAULT_CMD_FILE_ASCII_CMD_TOGGLE "ASCII_CMD_TOGGLE=AT+SETLOAD=TOGGLE\n"
#define DEFAULT_CMD_FILE_COMMENT_9        "// Команда для смены предписываемого положения нагрузки на противоположное, помещаемая в URI GET-запроса.\n"
#define DEFAULT_CMD_FILE_URI_CMD_TOGGLE   "URI_CMD_TOGGLE=/setloadtoggle\n"
#define DEFAULT_CMD_FILE_COMMENT_10       "// Команда для смены предписываемого положения нагрузки на противоположное, помещаемая в тело POST-запроса (в пару параметр+значение).\n"
#define DEFAULT_CMD_FILE_VAL_CMD_TOGGLE   "VAL_CMD_TOGGLE=cmd=setloadtoggle\n"

#define DEFAULT_CMD_FILE_COMMENT_11       "// Запрос текущего предписываемого положения нагрузки.\n"
#define DEFAULT_CMD_FILE_REQUEST_CMD      "REQUEST_CMD=UPD_REQ\n"


/*************** ПРОТОТИПЫ ФУНКЦИЙ **************/

void cmd_file_read_else_write_defaults(char cmd_file_contents[CMD_FILE_LIST_LEN][STR_MAX_LEN], char *cmd_file_path);
void cmd_file_update_current_load_status_cmd(char *cmd_file_path, bool load_status_cmd_to_post);


#endif  // Защита от повторного включения заголовочного файла.
