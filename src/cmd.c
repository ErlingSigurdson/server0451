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

// Локальные модули.
#include "cmd.h"
#include "utilities.h"


/******************** ФУНКЦИИ *******************/

void cmd_file_read_else_write_defaults(char cmd_file_contents[CMD_FILE_LIST_LEN][STR_MAX_LEN], char *cmd_file_path)
{
    FILE *f = fopen(cmd_file_path, "r");

    if (f == NULL) {
        f = fopen("./configs_server0451", "w");
        fputs(DEFAULT_CMD_FILE_COMMENT_1, f);
        fputs(DEFAULT_CMD_FILE_CURRENT_CMD, f);
        fputs(DEFAULT_CMD_FILE_COMMENT_2, f);
        fputs(DEFAULT_CMD_FILE_ASCII_CMD_ON, f);
        fputs(DEFAULT_CMD_FILE_COMMENT_3, f);
        fputs(DEFAULT_CMD_FILE_URI_CMD_ON, f);
        fputs(DEFAULT_CMD_FILE_COMMENT_4, f);
        fputs(DEFAULT_CMD_FILE_VAL_CMD_ON, f);
        fputs(DEFAULT_CMD_FILE_COMMENT_5, f);
        fputs(DEFAULT_CMD_FILE_ASCII_CMD_OFF, f);
        fputs(DEFAULT_CMD_FILE_COMMENT_6, f);
        fputs(DEFAULT_CMD_FILE_URI_CMD_OFF, f);
        fputs(DEFAULT_CMD_FILE_COMMENT_7, f);
        fputs(DEFAULT_CMD_FILE_VAL_CMD_OFF, f);
        fputs(DEFAULT_CMD_FILE_COMMENT_8, f);
        fputs(DEFAULT_CMD_FILE_ASCII_CMD_TOGGLE, f);
        fputs(DEFAULT_CMD_FILE_COMMENT_9, f);
        fputs(DEFAULT_CMD_FILE_URI_CMD_TOGGLE, f);
        fputs(DEFAULT_CMD_FILE_COMMENT_10, f);
        fputs(DEFAULT_CMD_FILE_VAL_CMD_TOGGLE, f);
        fputs(DEFAULT_CMD_FILE_COMMENT_11, f);
        fputs(DEFAULT_CMD_FILE_REQUEST_CMD, f);

        freopen(NULL, "r", f);
    }

    char buf[STR_MAX_LEN + 1] = {0};
    uint32_t i = 0;
    bool end_reached = 0;
    while (i < CMD_FILE_LIST_LEN && end_reached == (bool)NULL) {
        end_reached = !(bool)fgets(buf, sizeof(buf), f);

        if (strstr(buf, "CMD") && strchr(buf, '=')) {
            strcpy(cmd_file_contents[i], buf);
            ++i;
        }

        memset(buf, '\0', sizeof(buf));
    }
    
    if (i != CMD_FILE_LIST_LEN) {
        printf("Error reading command configuration file contents.\n");
        printf("For a reference insert \"/home/my_user_name/default_dummy.txt\"\n");
        printf("as an -f option argument and follow new file's format and pattern.");
        exit(0);
    }

    for (uint32_t i = 0; i < CMD_FILE_LIST_LEN; ++i) {
        strcpy(cmd_file_contents[i], strchr(cmd_file_contents[i], '=') + 1);
        utilities_remove_CR_and_LF(cmd_file_contents[i]);
    }
}

void cmd_file_update_current_load_status_cmd(char *cmd_file_path, bool load_status_cmd_to_post)
{
	FILE *f = fopen(cmd_file_path, "r+");
	char c = 0;
	while (c != '=' && c != EOF) {
		c = fgetc(f); 
	}

	if (load_status_cmd_to_post) {
		fputc('1', f);
	} else {
		fputc('0', f);
	}

	fclose(f);
}
