/****************** DESCRIPTION *****************/

/**
 * Filename: main.c
 * ---------------------------------------------------------------------------------------------------------------------
 * Purpose: main file of a simple TCP server project for Internet of things ("IoT")
 * implemented in C language.
 * ---------------------------------------------------------------------------------------------------------------------
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- INCLUDES ---*/

// Standard.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

// Special.
#include <unistd.h>      // getopt(), read(), write(), close().
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

// Local modules.
#include "utilities.h"
#include "cmd.h"

// Local configs.
#include "HTTP_config.h"
#include "help_page.h"


/*--- MISC ---*/

// None.


/************** FUNCTION PROTOTYPES *************/

// Handle command line options and arguments.
void opt_handler(int32_t argc, char *argv[], int32_t *port, char *cmd_file_path, uint32_t *verbosity_level);

// Check for a mention of HTTP.
bool HTTP_is_mentioned(char *buf);

// Handle incoming commands.
void cmd_handler(int32_t connfd, char *cmd_file_path, uint32_t verbosity_level);


/******************* FUNCTIONS ******************/

int main(int32_t argc, char *argv[])
{
    /*--- Handling command line options and arguments ---*/

    // Values based on command line arguments.
    int32_t port = -1;
    char cmd_file_path[STR_MAX_LEN + 1] = {0};  // STR_MAX_LEN is defined in cmd.h.
    uint32_t verbosity_level = 0;

    // Parse and process options and arguments.
    opt_handler(argc, argv, &port, cmd_file_path, &verbosity_level);


    /*--- Sockets and stuff ---*/

	if (verbosity_level > 0) {
	    printf("\n\n* * * * * * * * * * * * * * * * *\n");
	    printf("Starting TCP server at port %d", port);
	    printf("\n* * * * * * * * * * * * * * * * *\n");
	}

    // Socket-related variables.
    int32_t sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // Create a socket.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("\nSocket creation failed. Terminating the program.\n");
        printf("Error description: %s\n", strerror(errno));
        exit(0);
    } else if (verbosity_level > 0) {
    	printf("\n...socket successfully created.\n");
    }

    // Fill with zeros.
    memset(&servaddr, '\0', sizeof(servaddr));  // Some folks use bzero(), but it seems to be deprecated.

    // Assign IP and port.
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    // Allow a certain IP address to be reused immediately after previous connection was closed.
    // If implemented, should be done before calling bind().
    uint32_t so_reuseaddr = 1;
    int32_t a = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, (socklen_t)sizeof(so_reuseaddr) * 2);

	if (verbosity_level > 1) {
        printf("...setting SO_REUSEADDR return value: %d\n", a);
        printf("Status or error description: %s\n", strerror(errno));
	}

    // Allow closing a socket immediately, without waiting for data to be transfered.
    // If implemented, should be done before calling bind().
    uint32_t so_linger = 0;
    int32_t b = setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &so_linger, (socklen_t)sizeof(so_linger) * 2);

    if (verbosity_level > 1) {
        printf("...clearing SO_LINGER return value: %d\n", b);
        printf("Status or error description: %s\n", strerror(errno));
    }

    // Bind a newly created socket to a given IP.
    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("\nSocket bind failed. Terminating the program.\n");
        printf("Error description: %s\n", strerror(errno));
        exit(0);
    } else if (verbosity_level > 0) {
        printf("...socket successfully bound.\n");
    }

    // Server starts listening.
    if ((listen(sockfd, 5)) != 0) {
        printf("\nEntering listening mode failed. Terminating the program.\n");
        printf("Error description: %s\n", strerror(errno));
        exit(0);
    } else if (verbosity_level > 0) {
        printf("\nServer is listening.\n");
    }

    // Accept and verify a data packet from a client.
    len = sizeof(cli);
    connfd = accept(sockfd, (struct sockaddr*)&cli, (socklen_t*)&len);
    if (connfd < 0) {
        printf("\nServer failed to accept the client. Terminating the program.\n");
        printf("Error description: %s\n", strerror(errno));
        exit(0);
    } else if (verbosity_level > 0) {
        printf("Server accepted the client. Starting communication.\n");
    }

    // Call the incoming command handler function.
    cmd_handler(connfd, cmd_file_path, verbosity_level);

    // Close the socket.
    //shutdown(sockfd, SHUT_RDWR);  // Seems to be unnecessary, but I bothered to mention it just in case.
    close(sockfd);

    return 0;
}

void opt_handler(int32_t argc, char *argv[], int32_t *port, char *cmd_file_path, uint32_t *verbosity_level)
{
    int32_t opt = 0;
    while ((opt = getopt(argc, argv, "p:f:vVh")) >= 0) {
        switch (opt) {
            // Mandatory option.
            case 'p':
                *port = strtol(optarg, NULL, 10);
                break;

            // Mandatory option.
            case 'f':
                sscanf(optarg, "%s", cmd_file_path);
                break;

            case 'v':
                *verbosity_level = 1;
                break;

            case 'V':
                *verbosity_level = 2;
                break;

            case 'h':
                PRINT_HELP_PAGE;
                exit(0);

            default:
            	// Do nothing and hail MISRA.
        }
    }

    if (*port < 0) {
        printf("Please restart the program and insert a valid port number as a -p option argument.\n");
        exit(0);
    }

    if (cmd_file_path[0] != '/') {
        printf("Please restart the program and insert a valid path\n");
        printf("to a command configuration file as an -f option argument.\n");
        printf("If such file doesn't exist, the program will create it\n");
        printf("at specified path automatically. File will contain\n");
        printf("default values.\n");
        exit(0);
    }
}

bool HTTP_is_mentioned(char *buf)
{
    if (strstr(buf, "HTTP")) {
        return 1;
    } else {
        return 0;
    }
}

void cmd_handler(int32_t connfd, char *cmd_file_path, uint32_t verbosity_level)
{
    /*--- Reading from a command configuration file ---*/

    // Retrieve values from a command configuration file.
    char cmd_file_contents[CMD_FILE_LIST_LEN][STR_MAX_LEN] = {0};  // CMD_FILE_LIST_LEN is defined in cmd.h.
    cmd_file_read_else_write_defaults(cmd_file_contents, cmd_file_path);


    /*--- Reading a message from a client ---*/

    char buf[STR_MAX_LEN + 1] = {0};

    // Read a message from a client and copy it to the buffer.
    read(connfd, buf, sizeof(buf));

    // Print from the buffer.
    if (verbosity_level > 0) {
        utilities_force_2xLF(buf, sizeof(buf));
        printf("\nMessage received from the client:\n%s", buf);
    }


    /*--- Parsing and processing an incoming command ---*/

	// Parse an incoming command from a client.
	// CMD_FILE_ macros are all defined in cmd.h.
    bool is_setloadon = strstr(buf, cmd_file_contents[CMD_FILE_ASCII_CMD_ON]) ||          \
                        strstr(buf, cmd_file_contents[CMD_FILE_URI_CMD_ON]  ) ||          \
                        strstr(buf, cmd_file_contents[CMD_FILE_VAL_CMD_ON]  ) ;

    bool is_setloadoff = strstr(buf, cmd_file_contents[CMD_FILE_ASCII_CMD_OFF]) ||        \
                         strstr(buf, cmd_file_contents[CMD_FILE_URI_CMD_OFF]  ) ||        \
                         strstr(buf, cmd_file_contents[CMD_FILE_VAL_CMD_OFF]  ) ;

    bool is_setloadtoggle = strstr(buf, cmd_file_contents[CMD_FILE_ASCII_CMD_TOGGLE]) ||  \
                            strstr(buf, cmd_file_contents[CMD_FILE_URI_CMD_TOGGLE]  ) ||  \
                            strstr(buf, cmd_file_contents[CMD_FILE_VAL_CMD_TOGGLE]  ) ;

    bool is_request = strstr(buf, cmd_file_contents[CMD_FILE_REQUEST_CMD]);

    bool is_HTTP = HTTP_is_mentioned(buf);


	/*--- Posting updated load status and sending a response ---*/

	// Check currently prescribed load status.
	bool current_load_status_cmd = cmd_file_contents[CMD_FILE_CURRENT_CMD][0] - '0';
	
    if (is_setloadtoggle) {
        if (current_load_status_cmd) {
        	is_setloadoff = 1;
        } else {
        	is_setloadon = 1;
        }
    }

    if (is_setloadon) {
        cmd_file_update_current_load_status_cmd(cmd_file_path, 1);

        memset(buf, '\0', sizeof(buf));
        char *ptr = buf;

        if (is_HTTP) {
            strcat(buf, HTTP_RESPONSE_200);
            ptr += strlen(HTTP_RESPONSE_200);
        }

        strcat(buf, "New command posted: load ON.\n");

        if (verbosity_level > 0) {
            printf("%s", ptr);
        }

        write(connfd, buf, strlen(buf));
        return;
    }

    if (is_setloadoff) {
        cmd_file_update_current_load_status_cmd(cmd_file_path, 0);
        
        memset(buf, '\0', sizeof(buf));
        char *ptr = buf;
    
        if (is_HTTP) {
            strcat(buf, HTTP_RESPONSE_200);
            ptr += strlen(HTTP_RESPONSE_200);
        }
    
        strcat(buf, "New command posted: load OFF.\n");
    
        if (verbosity_level > 0) {
            printf("%s", ptr);
        }

        write(connfd, buf, strlen(buf));
        return;
    }
        
    if (is_request) {
        if (current_load_status_cmd) {
        	strcpy(buf, cmd_file_contents[CMD_FILE_ASCII_CMD_ON]);	
        } else {
        	strcpy(buf, cmd_file_contents[CMD_FILE_ASCII_CMD_OFF]);
        }

		// Allows a client-subscriber to read response quicker due to a presence of a terminating character.
        strcat(buf, "\n");

        if (verbosity_level > 0) {
            printf("Current command requested.\n");
        }

        write(connfd, buf, strlen(buf));
        return;
    }

	// This point is reached only if no valid command was found in a client's message.
    if (verbosity_level > 0) {
        printf("No valid command received.\n");
    }

    memset(buf, '\0', sizeof(buf));

    if (is_HTTP) {
        strcat(buf, HTTP_RESPONSE_404);
    }
    
    strcat(buf, "No valid command received\n");

    write(connfd, buf, strlen(buf));
}
