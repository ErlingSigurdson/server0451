#include "http.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool starts_with_http_method(const char *s) {
    static const char *methods[] = {
        "GET ", "POST ", "PUT ", "DELETE ", "HEAD ",
        "OPTIONS ", "PATCH ", "TRACE ", "CONNECT "
    };

    size_t i;
    for (i = 0; i < sizeof(methods) / sizeof(methods[0]); ++i) {
        if (strncmp(s, methods[i], strlen(methods[i])) == 0) {
            return true;
        }
    }
    return false;
}

bool looks_like_http_request(const char *s) {
    const char *line_end;
    const char *http_ver;

    if (s == NULL) {
        return false;
    }

    if (!starts_with_http_method(s)) {
        return false;
    }

    line_end = strstr(s, "\r\n");
    if (line_end == NULL) {
        return false;
    }

    http_ver = strstr(s, " HTTP/1.");
    if (http_ver == NULL || http_ver > line_end) {
        return false;
    }

    return true;
}

const char *find_http_payload(const char *s) {
    const char *sep;

    if (s == NULL) {
        return NULL;
    }

    sep = strstr(s, "\r\n\r\n");
    if (sep == NULL) {
        return NULL;
    }

    return sep + 4;
}

#include <stdlib.h>

size_t extract_http_payload(const char *request,
                                  char *out_buf,
                                  size_t out_buf_size)
{
    const char *payload;
    size_t payload_len;

    if (request == NULL || out_buf == NULL || out_buf_size == 0) {
        return 0;
    }

    payload = find_http_payload(request);
    if (payload == NULL) {
        return 0;
    }

    payload_len = strlen(payload);

    if (payload_len >= out_buf_size) {
        payload_len = out_buf_size - 1;
    }

    memcpy(out_buf, payload, payload_len);
    out_buf[payload_len] = '\0';

    return payload_len;
}
