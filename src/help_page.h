/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла: help_page.h
 * --------------------------------------------------------------------------------|---------------------------------------|
 * Назначение: содержимое страницы с подсказками для пользователя.
 * --------------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Защита от повторного включения заголовочного файла.
#ifndef HELP_PAGE_H
#define HELP_PAGE_H

#define PRINT_HELP_PAGE  \
puts("Simple Linux TCP server for IoT written in C language.");                                                \
puts("Basic usage (start server): sudo execute_server0451 -p <port> -P <password> [-h][-v][-V]");              \
puts("Example: sudo execute_server0451 -p 451 -P my_pswd -v");                                                 \
puts("It's highly recommended to stick to a provided shell script \"server0451.sh\".");                        \
puts("");                                                                                                      \
puts("Options:");                                                                                              \
puts("    -p <port>      specify a port to listen to (mandatory option).");                                    \
puts("    -P <password>  specify a password to search for in any incoming message (mandatory option).");       \
puts("    -v             verbose mode.");                                                                      \
puts("    -V             even more verbose mode.");                                                            \
puts("    -h             this help page.");                                                                    \
puts("");                                                                                                      \
puts("");                                                                                                      \
puts("/*--- CONCEPT ---*/");                                                                                   \
puts("");                                                                                                      \
puts("Server acts as a broker between clients-posters which push simple ON/OFF commands into specialized");    \
puts("files (\"topics\"), and clients-subscribers which request such commands from respective topics.");       \
puts("");                                                                                                      \
puts("Any device capable of TCP/IP messaging can be a client, but the role of clients-subscribers");           \
puts("is primarily suggested for IoT devices which control simple ON/OFF loads like LEDs, relays, etc.");      \
puts("");                                                                                                      \
puts("Topics are stored in /server0451/.topics directory. Initially they are absent - server creates");        \
puts("a topic upon reception of a first valid message referring to it. There can be up to 100 topics,");       \
puts("indexed 1 through 100.");                                                                                \
puts("");                                                                                                      \
puts("Server is suitable for both WANs and LANs as long as it's provided with a unique IP address.");          \
puts("");                                                                                                      \
puts("");                                                                                                      \
puts("/*--- QUICK START GUIDE ---*/");                                                                         \
puts("");                                                                                                      \
puts("Open /server0451/src directory and run \"make\" utility to build an executable binary file.");           \
puts("");                                                                                                      \
puts("Specify a password in /server0451/.config/config_server0451. Default password is \"admin\".");           \
puts("Password must consist of 5 to 40 alphanumeric characters. If password is not specified, you will");      \
puts("have an opportunity to insert it from the console.");                                                    \
puts("");                                                                                                      \
puts("Specify a port to be used in /server0451/.config/config_server0451. Default port is 451.");              \
puts("");                                                                                                      \
puts("Run /server0451/server0451.sh shell script. Choose between a full-fledged (l)oop mode and");             \
puts("a test (o)neshot mode. Don't worry about closing your console or ssh session after the server");         \
puts("got started - shell script uses nohup utility to run server in a loop.");                                \
puts("");                                                                                                      \
puts("Clients must stick to the following message format, otherwise server will reject any command:");         \
puts("");                                                                                                      \
puts("<password>:topic_<X>:<command_text>");                                                                   \
puts("where");                                                                                                 \
puts("    password     - is a password you've specified before.");                                             \
puts("    X            - is an integer value from 1 to 100.");                                                 \
puts("    command_text - is one of the commands listed in config_general.h: AT+SETLOAD=ON,");                  \
puts("                   AT+SETLOAD=OFF, AT+SETLOAD=TOGGLE, AT+SERVETOPIC.");                                  \
puts("");                                                                                                      \
puts("Server logs are stored in /server0451/.log/log_server0451.");                                            \
puts("");                                                                                                      \
puts("If you want to control separate groups of IoT devices, make them address to separate topics.");


#endif  // Защита от повторного включения заголовочного файла.
