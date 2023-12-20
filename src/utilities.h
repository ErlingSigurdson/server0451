/****************** DESCRIPTION *****************/

/**
 * Filename: utilities.h
 * ---------------------------------------------------------------------------------------------------------------------
 * Purpose: some general purpose macros and function.
 * ---------------------------------------------------------------------------------------------------------------------
 * Notes:
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.
#ifndef UTILITIES_H
#define UTILITIES_H


/*--- INCLUDES ---*/

// None.


/*--- MISC ---*/

// Macros for turning argument into a string.
#define STRINGIFY(x) INTERMEDIATE_STRINGIFY(x)
#define INTERMEDIATE_STRINGIFY(x) #x


/************** FUNCTION PROTOTYPES *************/

/*--- Strings and stuff ---*/

bool utilities_remove_CR_and_LF(char *buf);
bool utilities_append_CR_and_LF(char *buf, size_t bufsize);
bool utilities_append_LF_if_absent(char *buf, size_t bufsize);
bool utilities_force_2xLF(char *buf, size_t bufsize);


/*--- Files and stuff ---*/

void utilities_write_to_file_single_line(char *str, char *file_path);
// No size safety! Note buffer size!
void utilities_read_from_file_single_line(char *buf, size_t bufsize, char *file_path);
void utilities_read_from_file(char *buf, size_t bufsize, char *file_path);
void utilities_file_abs_path_cpy(char *buf, size_t bufsize, char *filename);


#endif  // Closing directive of the include guards.
