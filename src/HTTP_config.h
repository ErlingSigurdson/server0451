/****************** DESCRIPTION *****************/

/**
 * Filename: HTTP_config.h
 * ---------------------------------------------------------------------------------------------------------------------
 * Purpose: HTTP configs.
 * ---------------------------------------------------------------------------------------------------------------------
 * Notes: 
 */


/************ PREPROCESSOR DIRECTIVES ***********/

// Include guards.

#ifndef HTTP_CONFIG_H
#define HTTP_CONFIG_H


/*--- INCLUDES ---*/

// None.


/*--- MISC ---*/

#define HTTP_RESPONSE_200 "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n"
#define HTTP_RESPONSE_404 "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n"


#endif  // Closing directive of the include guards.
