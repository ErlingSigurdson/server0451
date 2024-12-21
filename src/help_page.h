/******************* ОПИСАНИЕ *******************/

/**
 * Имя файла:  help_page.h
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Назначение: вывод страницы с подсказками для пользователя.
 * ----------------------------------------------------------------------------|---------------------------------------|
 * Примечания:
 */


/************ ДИРЕКТИВЫ ПРЕПРОЦЕССОРА ***********/

// Защита от повторного включения заголовочного файла.
#ifndef HELP_PAGE_H
#define HELP_PAGE_H

#define PRINT_HELP_PAGE \
puts("Simple Linux TCP server for IoT written in C language."); \
puts("Basic usage (start server): sudo exec_bin_server0451 -p <port> -P <password> [-o][-v][-V][-h]"); \
puts("Example: sudo exec_bin_server0451 -p 451 -P my_pswd -v"); \
puts("It's highly recommended to stick to a provided shell script \"run_server0451.sh\"."); \
puts(""); \
puts("Options:"); \
puts("    -p <port>      specify a port to listen to (mandatory option)."); \
puts("    -P <password>  specify a password to search for in any incoming message (mandatory option)."); \
puts("    -o             run server in a oneshot test mode."); \
puts("    -v             verbose mode."); \
puts("    -V             even more verbose mode."); \
puts("    -h             print this help page and terminate."); \
puts(""); \
puts(""); \
puts("/*--- CONCEPT ---*/"); \
puts(""); \
puts("Server acts as a broker between clients-posters which push simple ON/OFF commands into specialized"); \
puts("files (\"topics\") and clients-subscribers which request such commands from respective topics."); \
puts(""); \
puts("Any device capable of TCP/IP messaging can be a client, but the role of clients-subscribers"); \
puts("is primarily suggested for IoT devices which control simple ON/OFF loads like LEDs, relays, etc."); \
puts(""); \
puts("Topics are stored in /server0451/.topics directory. Initially they are absent - server creates"); \
puts("a topic upon reception of a first valid message referring to it. There can be up to 100 topics,"); \
puts("indexed 1 through 100."); \
puts(""); \
puts("Server is suitable for both WANs and LANs as long as it's provided with a unique IP address."); \
puts(""); \
puts(""); \
puts("/*--- QUICK START GUIDE ---*/"); \
puts(""); \
puts("Open /server0451/src directory and run \"make\" utility to build an executable binary file."); \
puts(""); \
puts("Specify configs in /server0451/.config/config_server0451. Read note file in .config directory"); \
puts("for guidance."); \
puts(""); \
puts("Launch /server0451/run_server0451.sh shell script. Don't worry about closing your console or ssh"); \
puts("session after the server has started - in a loop mode the script uses nohup utility to start server."); \
puts(""); \
puts("Clients must stick to the following message format, otherwise server will reject any directive:"); \
puts(""); \
puts("<password>:topic_<X>:<directive_text>"); \
puts("where"); \
puts("    password       - is a password you've specified in a config file."); \
puts("    X              - is an integer value from 1 to 100."); \
puts("    directive_text - is one of the directives listed in config_general.h:"); \
puts("                     AT+LOADDIGITAL=ON, AT+LOADDIGITAL=OFF, AT+LOADDIGITALTOGGLE, AT+SERVETOPIC."); \
puts(""); \
puts("Server logs are stored in /server0451/.log/log_server0451."); \
puts(""); \
puts("If you want to control separate groups of IoT devices, make them address to separate topics.");


#endif  // Защита от повторного включения заголовочного файла.
