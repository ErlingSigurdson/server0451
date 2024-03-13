/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: help_page.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: содержимое страницы с подсказками для пользователя.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Защита от повторного включения заголовочного файла.
#ifndef HELP_PAGE_H
#define HELP_PAGE_H

#define PRINT_HELP_PAGE                                                                                    \
puts("Simple Linux TCP server for IoT written in C language.");                                            \
puts("Usage (start server): server0451 -p <port> -f <file_path> [-h][-v][-V]");                            \
puts("Example: server0451 -p 80 -f /home/my_user_name/configs_server0451 -v");                             \
puts("");                                                                                                  \
puts("Options:");                                                                                          \
puts("    -p <port>       specify a port to listen (mandatory option).");                                  \
puts("    -f <file_path>  specify a file path for a command configuration file (mandatory option).");      \
puts("    -v              verbose mode.");                                                                 \
puts("    -V              even more verbose mode.");                                                       \
puts("    -h              this help page.");                                                               \
puts("");                                                                                                  \
puts("Server listens to TCP/IP connections (including ones established for HTTP data transfer) and acts"); \
puts("according to TCP/IP message (including HTTP request) contents.");                                    \
puts("");                                                                                                  \
puts("Server works as a broker that allows for control over remote IoT devices with TCP/IP");              \
puts("messaging capability which control simple ON/OFF loads like LEDSs, relays, etc.");                   \
puts("");                                                                                                  \
puts("Server stores currently prescribed load status (1 or 0, \"ON\" or \"OFF\") in a command");           \
puts("configuration file alongside with substrings which, upon being found in incoming TCP/IP messages");  \
puts("(including HTTP requests), trigger server to either change prescribed load status");                 \
puts("or serve it to a client.");                                                                          \
puts("");                                                                                                  \
puts("TCP/IP client that sends a command in order to change currently prescribed load status is called");  \
puts("a client-poster. TCP/IP client that sends a command in order to read currently prescribed load");    \
puts("status is called a client-subscriber.");								                               \
puts("");                                                                                                  \
puts("Check newly created command configuration file to learn (and modify if necessary)");                 \
puts("what substrings found in TCP/IP messages trigger updating and serving prescribed load status.");     \
puts("");                                                                                                  \
puts("For continuous listening run loop_script_server0451.sh found in /server0451/sh directory");          \
puts("as an executable file. Using nohup is adviced in case you want your server to keep on listening ");  \
puts("after termination of a current shell session.");                                                     \
puts("");																						           \
puts("If you want to control separate groups of IoT devices which are not supposed to update and read");   \
puts("the same prescribed load status (aren't meant to share it), consider starting several instances");   \
puts("of a server with separate command configuration file paths specified. Perhaps it's better to be");   \
puts("done with port numbers also separated for different IoT devices flocks, but it will work within");   \
puts("a single port as well.");																	           \


#endif  // Защита от повторного включения заголовочного файла.
