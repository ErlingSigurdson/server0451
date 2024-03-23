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

#define PRINT_HELP_PAGE                                                                                      \
puts("Simple Linux TCP server for IoT written in C language.");                                              \
puts("Usage (start server): server0451 -p <port> -f <cmd_config_file_name> [-h][-v][-V]");                   \
puts("Example: server0451 -p 80 -f cmd_config_server0451 -v");                                               \
puts("");                                                                                                    \
puts("Options:");                                                                                            \
puts("    -p <port>       specify a port to listen to (mandatory option).");                                 \
puts("    -f <file_path>  specify a name for a command configuration file (mandatory option).");             \
puts("    -v              verbose mode.");                                                                   \
puts("    -V              even more verbose mode.");                                                         \
puts("    -h              this help page.");                                                                 \
puts("");                                                                                                    \
puts("");                                                                                                    \
puts("/*--- CONCEPT ---*/");                                                                                 \
puts("");                                                                                                    \
puts("Server listens to TCP/IP connections (including ones established for HTTP requests and responses)");   \
puts("and acts according to TCP/IP message (including HTTP request) contents.");                             \
puts("");                                                                                                    \
puts("Server works as a broker that allows for control over remote IoT devices with TCP/IP");                \
puts("messaging capability which control simple ON/OFF loads like LEDSs, relays, etc.");                     \
puts("");                                                                                                    \
puts("Server stores currently prescribed load status (1 or 0, \"ON\" or \"OFF\") in a command");             \
puts("configuration file alongside with substrings which, upon being found in incoming TCP/IP messages");    \
puts("(including HTTP requests), trigger server to either change prescribed load status");                   \
puts("or serve it to a client.");                                                                            \
puts("");                                                                                                    \
puts("TCP/IP client that sends a command in order to change currently prescribed load status is called");    \
puts("a client-poster. TCP/IP client that sends a command in order to get currently prescribed load");       \
puts("status is called a client-subscriber.");                                                               \
puts("");                                                                                                    \
puts("Server runs simultaneously at two ports specified in /server0451/.config/port_config_server0451.");    \
puts("First one is for client-posters, second one is for client-subscribers.");                              \
puts("");                                                                                                    \
puts("Check newly created command configuration file to learn (and modify if necessary)");                   \
puts("what substrings found in TCP/IP messages trigger updating and serving prescribed load status.");       \
puts("");                                                                                                    \
puts("");                                                                                                    \
puts("/*--- HOW TO START ---*/");                                                                            \
puts("");                                                                                                    \
puts("Go to /server0451/src directory and run make utility to build a program executable file.");            \
puts("");                                                                                                    \
puts("In order to start server0451, run server0451.sh (an executable shell script) and choose between)");    \
puts("loop mode (for continuous listening) and oneshot mode (for a test run). In both cases program is");    \
puts("run using nohup utility, and its stdout and stderr are redirected to a log file at");                  \
puts("/server0451/.log/log_server0451.");                                                                    \
puts("");                                                                                                    \
puts("");                                                                                                    \
puts("/*--- MISC ---*/");                                                                                    \
puts("");                                                                                                    \
puts("1) Server logs are stored at /server0451/.log/log_server0451.");                                       \
puts("");                                                                                                    \
puts("2) If you want to control separate groups of IoT devices which are not supposed to update and read");  \
puts("the same prescribed load status (aren't meant to share it), consider creating two or more separate");  \
puts("directories containing all server0451 files and provide different PORT_1 and PORT_2 values in");       \
puts("/.config/port_config_server0451 for every separate device flock. It's sloppy, I know.");


#endif  // Защита от повторного включения заголовочного файла.
