/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: utilities.cpp
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: некоторые вспомогательные макросы и функции.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

/*--- Включения ---*/

// Локальные модули.
#include "utilities.h"


/******************** ФУНКЦИИ *******************/

/*--- Работа со строками ---*/

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

bool utilities_append_CR_and_LF(char *buf, size_t buf_size)
{
    if (buf_size - strlen(buf) >= 3) {  // Один байт для CR, второй для LF, третий для нуля.
        buf[strlen(buf)] = '\r';
        buf[strlen(buf)] = '\n';
        buf[strlen(buf)] = '\0';

        return 1;
    }

    return 0;
}

bool utilities_append_LF(char *buf, size_t buf_size)
{
    if (buf_size - strlen(buf) >= 2) {  // Один байт для LF, второй для нуля.
        buf[strlen(buf)] = '\n';
        buf[strlen(buf)] = '\0';

        return 1;
    }

    return 0;
}

bool utilities_append_LF_if_absent(char *buf, size_t buf_size)
{
    if (strrchr(buf, '\n') == &buf[strlen(buf) - 1]) {
        return 0;
    } else if (buf_size - strlen(buf) >= 2) {  // Один байт для LF, второй для нуля.
        utilities_append_LF(buf, buf_size);

        return 1;
    }

    return 0;
}

bool utilities_force_2xLF(char *buf, size_t buf_size)
{
    utilities_remove_CR_and_LF(buf);
    if (buf_size - strlen(buf) >= 3) {  // Два байта для LF, третий для нуля.
        buf[strlen(buf)] = '\n';
        buf[strlen(buf)] = '\n';
        buf[strlen(buf)] = '\0';

        return 1;
    }

    return 0;
}


/*--- Работа с файлами ---*/

void utilities_write_to_file_single_line(char *str, char *file_path)
{
    FILE *f = fopen(file_path, "w");
    fputs(str, f);
    fclose(f);
}

// Без защиты от переполнения буфера!
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
