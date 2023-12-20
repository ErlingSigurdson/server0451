/****************** DESCRIPTION *****************/

/**
 * Filename: utilities.c
 * ---------------------------------------------------------------------------------------------------------------------
 * Purpose: some general purpose macros and function..
 * ---------------------------------------------------------------------------------------------------------------------
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

/*--- INCLUDES ---*/

// Standard.
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Special.
#include <unistd.h>  // readlink()

// Local modules.
#include "utilities.h"

// Local configs.
// None.


/*--- MISC ---*/

// None.


/******************* FUNCTIONS ******************/

/*--- Strings and stuff ---*/

bool utilities_remove_CR_and_LF(char *buf)
{
    for (uint32_t i = 0; i < strlen(buf); ++i) {
        if (buf[i] == '\r' || buf[i] == '\n') {
            buf[i] = '\0';

            return 1;
        }
    }

    return 0;
}

bool utilities_append_CR_and_LF(char *buf, size_t bufsize)
{
    if (bufsize - strlen(buf) >= 3) {  // One byte for CR, another byte for LF and third byte for a null terminator.
        buf[strlen(buf)] = '\r';
        buf[strlen(buf)] = '\n';

        return 1;
    }

    return 0;
}

bool utilities_append_LF_if_absent(char *buf, size_t bufsize)
{
    if (strrchr(buf, '\n') == &buf[strlen(buf) - 1]) {
        return 0;
    } else if (bufsize - strlen(buf) >= 2) {
        buf[strlen(buf)] = '\n';

        return 1;
    }

    return 0;
}

bool utilities_force_2xLF(char *buf, size_t bufsize)
{
    char *ptr = &buf[strlen(buf) - 4];  // Check last four bytes.

	uint32_t LF_erased = 0;
    for (uint32_t i = 0; i < 4; ++i) {
        if (ptr[i] == '\n') {
            ptr[i] = '\0';
            ++LF_erased;
        }

        if (ptr[i] == '\r') {
        	ptr[i] = '\0';
        }
    }

    if (bufsize - strlen(buf) >= 3) {  // Two bytes for two LFs and third byte for a null terminator.
        buf[strlen(buf)] = '\n';
        buf[strlen(buf)] = '\n';

        return 1;
    } else {
    	for (uint32_t i = 0; i < LF_erased; ++i) {
    		buf[strlen(buf)] = '\n';
    	}
    }

    return 0;
}


/*--- Files and stuff ---*/

void utilities_write_to_file_single_line(char *str, char *file_path)
{
    FILE *f = fopen(file_path, "w");
    fputs(str, f);
    fclose(f);
}

// No size safety! Note buffer size!
void utilities_read_from_file_single_line(char *buf, size_t bufsize, char *file_path)
{
    FILE *f = fopen(file_path, "r");
    fgets(buf, bufsize, f);
    fclose(f);
}

void utilities_read_from_file(char *buf, size_t bufsize, char *file_path)
{
    FILE *f = fopen(file_path, "r");

    uint32_t i = 0;
    char c = 0;
    while (c != EOF && i < bufsize - 1) {
    	c = fgetc(f);

    	if (c != EOF) {
    		buf[i] = c;
    	}

    	++i;
    }

    fclose(f);
}

void utilities_file_abs_path_cpy(char *buf, size_t bufsize, char *filename)
{
    readlink("/proc/self/exe", buf, bufsize);
    char *ptr = strrchr(buf, '/') + 1;

    if (ptr == NULL) {
        return;
    }

    memset(ptr, '\0', strlen(ptr));
    strcat(ptr, filename);
}
