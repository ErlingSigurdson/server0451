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
puts("Usage (start server): server0451 -p <port> -P <password> [-h][-v][-V]");                               \
puts("Example: server0451 -p 451 -P my_new_pswd -v");                                                        \
puts("");                                                                                                    \
puts("Options:");                                                                                            \
puts("    -p <port>       specify a port to listen to (mandatory option).");                                 \
puts("    -P <password>   specify a password to search for in incoming messages (mandatory option).");       \
puts("    -v              verbose mode.");                                                                   \
puts("    -V              even more verbose mode.");                                                         \
puts("    -h              this help page.");                                                                 \
puts("");                                                                                                    \
puts("");                                                                                                    \
puts("/*--- CONCEPT ---*/");                                                                                 \
puts("");                                                                                                    \
puts("Server listens to TCP/IP connections and acts according to message contents.");                        \
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
puts("2) If you want to control separate groups of IoT devices, make them address to separate topics.");     \


#endif  // Защита от повторного включения заголовочного файла.
