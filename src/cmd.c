/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: cmd.c
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: обработка команд.
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

// Из библиотек POSIX.
#include <unistd.h>

// Настройки проекта.
#include "config_general.h"

// Локальные модули.
#include "cmd.h"
#include "utilities.h"
#include "sockets.h"


/******************** ФУНКЦИИ *******************/

void cmd_handle(int32_t connfd, char *buf, uint32_t verbosity_level)
{
    /* --- Извлечение имени топика и команды из сообщения ---*/

    char buf_topic[STR_MAX_LEN + 1] = {0};
    char buf_cmd[STR_MAX_LEN + 1] = {0};
    cmd_extract(buf, buf_topic, buf_cmd, DELIM_CHAR);
    utilities_to_lowercase_string(buf_topic);


    /*--- Определение пути к файлу топика ---*/

    char topic_file_path[STR_MAX_LEN * 2 + 1] = {0};
    readlink("/proc/self/exe", topic_file_path, sizeof(topic_file_path) / 2);
    char *ptr = strrchr(topic_file_path, '/') + 1;
    strcpy(ptr, "../.topics/");
    strcat(topic_file_path, buf_topic);


    /*--- Выполнение команды ---*/

    bool current_cmd_load_toggle = !strcmp(buf_cmd, CMD_LOAD_TOGGLE);
    if (current_cmd_load_toggle) {
        utilities_read_from_file_single_line(buf_cmd, sizeof(buf_cmd), topic_file_path);

        if (!strcmp(buf_cmd, CMD_LOAD_ON)) {
            strcpy(buf_cmd, CMD_LOAD_OFF);
        } else if (!strcmp(buf_cmd, CMD_LOAD_OFF)) {
            strcpy(buf_cmd, CMD_LOAD_ON);
        } else {
            printf("\nError: can't toggle current load state (invalid data in the topic).\n");
            strcpy(buf, "Error: can't toggle current load state (invalid data in the topic).");
            sockets_write_message(connfd, buf, 0);  // verbosity_level overridden.

            return;
        }
    }

    bool current_cmd_load_on =        !strcmp(buf_cmd, CMD_LOAD_ON);
    bool current_cmd_load_off =       !strcmp(buf_cmd, CMD_LOAD_OFF);
    bool current_cmd_topic_request =  !strcmp(buf_cmd, CMD_TOPIC_REQUEST);

    if (current_cmd_load_on || current_cmd_load_off) {
        utilities_write_to_file_single_line(buf_cmd, topic_file_path);

        printf("\nNew command posted:\n");
        printf("%s\n", buf_cmd);

        strcpy(buf, "New command posted: ");
        strcat(buf, buf_cmd);
        sockets_write_message(connfd, buf, 0);  // verbosity_level overridden.

        return;
    }

    if (current_cmd_topic_request) {
        utilities_read_from_file_single_line(buf_cmd, sizeof(buf_cmd), topic_file_path);

        printf("\nCurrent topic contents requested.\n");

        sockets_write_message(connfd, buf_cmd, verbosity_level);

        return;
    }

    /* Программа доходит до этой точки только в случае, если в сообщении
     * от клиента не было найдено ни одной валидной команды.
     */
    if (verbosity_level > 0) {
        printf("\nNo valid command received.\n");
    }

    strcpy(buf, "No valid command received.");
    sockets_write_message(connfd, buf, 0);  // verbosity_level overridden.
}

void cmd_extract(char *buf, char *buf_topic, char *buf_cmd, char delim)
{
    char *cmd_ptr = strrchr(buf, delim);
    *cmd_ptr = '\0';
    strcpy(buf_cmd, cmd_ptr + 1);

    char *topic_ptr = strchr(buf, delim);
    strcpy(buf_topic, topic_ptr + 1);
}
