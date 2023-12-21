/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: config_HTTP.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: настройки проекта, связанные с HTTP.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Защита от повторного включения заголовочного файла.
#ifndef CONFIG_HTTP_H
#define CONFIG_HTTP_H

// Содержание ответов на HTTP-запросы.
#define HTTP_RESPONSE_200 "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n"
#define HTTP_RESPONSE_404 "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n"


#endif  // Завершающая директива защиты от повторного включения заголовочного файла.