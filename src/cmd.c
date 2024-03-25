/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: cmd.c
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: функции и макросы для обработки команд.
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
#include <stdlib.h>

// Локальные модули.
#include "utilities.h"
#include "cmd.h"
#include "sockets.h"


/******************** ФУНКЦИИ *******************/

void cmd_extract(char *buf, char *buf_topic, char *buf_cmd, char delim)
{
    char *cmd_ptr = NULL;
    for (uint32_t i = 0; i < strlen(buf); ++i) {
        if (buf[i] == delim) {
            buf[i] = '\0';
            cmd_ptr = &buf[i + 1];
            break;
        }
    }

    strcpy(buf_topic, buf);
    strcpy(buf_cmd, cmd_ptr);
}

void cmd_handle(int32_t connfd, char *buf, uint32_t verbosity_level)
{
    /* --- Извлечение имени топика и команды из сообщения ---*/
    
    char buf_topic[STR_MAX_LEN + 1] = {0};
    char buf_cmd[STR_MAX_LEN + 1] = {0};
    cmd_extract(buf, buf_topic, buf_cmd, DELIM);


    /*--- Определение пути к файлу топика ---*/
        
    char topic_file_path[STR_MAX_LEN * 2 + 1] = {0};
    readlink("/proc/self/exe", topic_file_path, sizeof(topic_file_path));
    char *ptr = strrchr(topic_file_path, '/') + 1;
    strcpy(ptr, "../.topics/");
    strcat(topic_file_path, buf_topic);


    /*--- Выполнение команды ---*/

    bool current_cmd_on =            !strcmp(buf_cmd, CMD_LOAD_ON);
    bool current_cmd_off =           !strcmp(buf_cmd, CMD_LOAD_OFF);
    bool current_cmd_toggle =        !strcmp(buf_cmd, CMD_LOAD_TOGGLE);
    bool current_cmd_request_topic = !strcmp(buf_cmd, CMD_REQUEST_TOPIC);
    
    if (current_cmd_toggle) {
        utilities_read_from_file_single_line(buf_cmd, sizeof(buf_cmd), topic_file_path);
    
        if (!strcmp(buf_cmd, CMD_LOAD_ON)) {
            current_cmd_off = 1;
        } else if (!strcmp(buf_cmd, CMD_LOAD_OFF)) {
            current_cmd_on = 1;
        } else {
            printf("DEBUG. Posted cmd: %s\n", buf_cmd);
            printf("DEBUG. Posted cmd len: %lu\n", strlen(buf_cmd));
            printf("DEBUG. Hardcoded cmd len: %lu\n", strlen(CMD_LOAD_ON));
            
            printf("Error: couldn't toggle current load state (invalid data in the topic).");
            strcpy(buf, "Error: couldn't toggle current load state (invalid data in the topic).");
            sockets_write_message(connfd, buf, verbosity_level);
            return;
        }
    }
    
    if (current_cmd_on) {
        utilities_write_to_file_single_line(CMD_LOAD_ON, topic_file_path);
        strcpy(buf, "New command posted: " CMD_LOAD_ON);
         
        printf("%s\n", buf);
        sockets_write_message(connfd, buf, verbosity_level);
        return;
    }
        
    if (current_cmd_off) {
        utilities_write_to_file_single_line(CMD_LOAD_OFF, topic_file_path);
        strcpy(buf, "New command posted: " CMD_LOAD_OFF);
         
        printf("%s\n", buf);
        sockets_write_message(connfd, buf, verbosity_level);
        return;
    }
    
    if (current_cmd_request_topic) {
        strcpy(buf, buf_cmd);
        strcat(buf, "\n");  // Позволяет клиенту быстрее считать ответ, реагируя на символ конца строки.

        printf("Current   contents requested:");
        printf("Current topic contents sent to the client: %s", buf);
        sockets_write_message(connfd, buf, verbosity_level);
        return;
    }
        
    /* Программа доходит до этой точки только в случае, если в сообщении
     * от клиента не было найдено ни одной валидной команды.
     */
    if (verbosity_level > 0) {
        printf("No valid command received.\n");
        printf("Communication closed.\n");
    }
    
    strcpy(buf, "No valid command received\n");
    sockets_write_message(connfd, buf, 0);  // verbosity_level overridden.
}
