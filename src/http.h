#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include <stdbool.h>

bool starts_with_http_method(const char *s);
bool looks_like_http_request(const char *s);
const char *find_http_payload(const char *s);
size_t extract_http_payload(const char *request, char *out_buf, size_t out_buf_size);


#endif  // Защита от повторного включения заголовочного файла.
