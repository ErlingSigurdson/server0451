/****************** DESCRIPTION *****************/

/**
 * Filename: cmd.h
 * ---------------------------------------------------------------------------------------------------------------------
 * Purpose: command handling macros and functions.
 * ---------------------------------------------------------------------------------------------------------------------
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef CMD_H
#define CMD_H


/*--- INCLUDES ---*/

// None.


/*--- MISC ---*/

#define STR_MAX_LEN 2000

#define CMD_FILE_LIST_LEN 11

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

#define DEFAULT_CMD_FILE_COMMENT_1    "// Current (last posted) load status command for this instance of server0451.\n"
#define DEFAULT_CMD_FILE_CURRENT_CMD  "CURRENT_CMD=0\n"

#define DEFAULT_CMD_FILE_COMMENT_2        "// Plain ASCII command for turning load ON.\n"
#define DEFAULT_CMD_FILE_ASCII_CMD_ON     "ASCII_CMD_ON=AT+SETLOAD=ON\n"
#define DEFAULT_CMD_FILE_COMMENT_3        "// Command for turning load ON found in GET request URI.\n"
#define DEFAULT_CMD_FILE_URI_CMD_ON       "URI_CMD_ON=/setloadon\n"
#define DEFAULT_CMD_FILE_COMMENT_4        "// Command for turning load ON found in POST request key+value pair.\n"
#define DEFAULT_CMD_FILE_VAL_CMD_ON       "VAL_CMD_ON=cmd=setloadon\n"

#define DEFAULT_CMD_FILE_COMMENT_5        "// Plain ASCII command for turning load OFF.\n"
#define DEFAULT_CMD_FILE_ASCII_CMD_OFF    "ASCII_CMD_OFF=AT+SETLOAD=OFF\n"
#define DEFAULT_CMD_FILE_COMMENT_6        "// Command for turning load OFF found in GET request URI.\n"
#define DEFAULT_CMD_FILE_URI_CMD_OFF      "URI_CMD_OFF=/setloadoff\n"
#define DEFAULT_CMD_FILE_COMMENT_7        "// Command for turning load OFF found in POST request key+value pair.\n"
#define DEFAULT_CMD_FILE_VAL_CMD_OFF      "VAL_CMD_OFF=cmd=setloadoff\n"

#define DEFAULT_CMD_FILE_COMMENT_8        "// Plain ASCII command for toggling load.\n"
#define DEFAULT_CMD_FILE_ASCII_CMD_TOGGLE "ASCII_CMD_TOGGLE=AT+SETLOAD=TOGGLE\n"
#define DEFAULT_CMD_FILE_COMMENT_9        "// Command for toggling load found in GET request URI.\n"
#define DEFAULT_CMD_FILE_URI_CMD_TOGGLE   "URI_CMD_TOGGLE=/setloadtoggle\n"
#define DEFAULT_CMD_FILE_COMMENT_10       "// Command for toggling load found in POST request key+value pair.\n"
#define DEFAULT_CMD_FILE_VAL_CMD_TOGGLE   "VAL_CMD_TOGGLE=cmd=setloadtoggle\n"

#define DEFAULT_CMD_FILE_COMMENT_11       "// Plain ASCII request for a current (last posted) load status command.\n"
#define DEFAULT_CMD_FILE_REQUEST_CMD      "REQUEST_CMD=GETLOAD\n"


/************** FUNCTION PROTOTYPES *************/

void cmd_file_read_else_write_defaults(char cmd_file_contents[CMD_FILE_LIST_LEN][STR_MAX_LEN], char *cmd_filepath);
void cmd_file_update_current_load_status_cmd(char *cmd_file_path, bool load_status_cmd_to_post);
bool cmd_file_get_current_load_status_cmd(char *cmd_file_path);  // Virtually unnecessary, but kept just in case.


#endif  // Closing directive of the include guards.
