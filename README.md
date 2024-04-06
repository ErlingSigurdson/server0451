# Концепция
Простой TCP-сервер (программа) для Linux, который выступает в качестве брокера
для клиентов-постеров, которые размещают команды на включение или выключение нагрузки в специальных файлах ("топиках"),
и для клиентов-подписчиков, которые запрашивают размещённые в топиках команды.

По получении сообщения (пакета TCP), формат и содержание которого соответствуют
требованиям и настройкам сервера, сервер:
- по запросу клиента-подписчика направляет ему заготовленную в топике команду, либо
- по указанию клиента-постера меняет заготовленную в топике команду (меняет текущее предписываемое состояние нагрузки).

Клиентами могут быть любые устройства, способные к коммуникации по TCP/IP, но роль клиентов-подписчиков предполагается
в первую очередь для устройств IoT, например, модулей ESP8266 или ESP32 с прошивкой 
"[Релешка](https://github.com/ErlingSigurdson/Billy_the_Relay)
[Билли](https://gitflic.ru/project/efimov-d-v/billy_the_relay)".

Топики находятся в директории `/server0451/.topics`. Изначально их не существует, они создаются сервером при первом
обращении клиента к топику с соответствующим именем (номером). Топиков может быть до 100 штук, пронумерованных
от 1 до 100, и разные клиенты-подписчики (их группы) могут обращаться к разным топикам, благодаря чему может
осуществляться раздельное управление устройствами (их группами).

Сервер может работать как в локальной, так и в глобальной сети - лишь бы был индивидуальный IP-адрес, по которому
к нему можно обратиться.


# Инструкция
### Компиляция
Зайдите в директорию `/server0451/src` и запустите команду `make`. Исполняемый бинарный файл программы появится
в директории `/server0451/bin`.

### Настройка
В файле `/server0451/password_config_server0451` задайте пароль, с которого должны будут начинаться все направляемые
серверу сообщения. Пароль по умолчанию - "admin". Пароль может состоять из цифр и из строчных и заглавных английских
букв. Длина пароля должна составлять от 5 до 40 символов. Если файл с паролем будет удалён или пароль будет представлять
собой пустую строку, пароль можно будет задать из командной строки при запуске скрипта оболочки
`server0451.sh` (см. ниже).

В файле `/server0451/ports_config_server0451` задайте номер порта, который будет использоваться сервером.
Значение по умолчанию: 451.

### Быстрый старт
Запустите исполняемый скрипт оболочки `/server0451/server0451.sh` в основной директории сервера и выберите между
полноценным циклическим запуском (loop) и одиночным тестовым прогоном (oneshot).

### Детали запуска
Если разбираться более детально, то общий вид команды запуска сервера следующий:
```
sudo execute_server0451 -p <номер_порта> -P <пароль> [-h][-v][-V] 
```
Опции `-p` и `-P` являются обязательными. Необязательные опции модифицируют поведение программы: 
- `-h` - выводит в `stdout` англоязычную страницу с подсказками по использованию программы
и прекращает выполнение программы.
- `-v` - включает вывод в `stdout` информации о текущих действиях сервера, о принятых и переданных сообщениях.
- `-V` - аналогично предыдущему, но дополнительно выводит информацию об установке параметров сокетов.

### Протокол
Сервер принимает от клиента-постера простые последовательности байт, то есть сырые (raw) данные.
Направляемые серверу сообщения должны следовать формату:

```
<пароль>:topic_<X>:<текст_команды> 
```
где:
- пароль - заданный для сервера пароль;
- X - номер топика от 1 до 100;
- текст команды - одна из команд, заданных в файле `config_general.h`: `AT+SETLOAD=ON`, `AT+SETLOAD=OFF`,
`AT+SETLOAD=TOGGLE`, `AT+SERVETOPIC`

Двоеточия означают буквально двоеточия.

### Логи
Логи сервера хранятся в `/server0451/.log/log_server0451`.


# Справедливые вопросы
### Почему "0451"?
[Отсылка](https://deusex.fandom.com/wiki/0451) к серии видеоигр "Deus Ex".

### Почему не MQTT?
Не так интересно. Используя такую простую программу как server0451, можно в деталях пощупать, что происходит
на сервере и как программа взаимодействует с файлами.

### Будет ли программа работать после закрытия терминала?
Циклический запуск программы осуществляется скриптом `server0451.sh` с помощью утилиты `nohup`, дабы программа
гарантированно продолжала работать после закрытия вашей сессии работы с оболочкой операционной системы.

### Что с безопасностью?
Это не то программное обеспечение, которое предназначено для выполнения ответственных задач, но наличие заданного
серверу пароля, который сервер будет искать в каждом сообщении и не найдя который он не будет принимать команды,
по крайней мере, задаёт индивидуальный синтаксис обращения к серверу.


# Note for English speakers
Please note that help page is written in English. Run binary executable with `-h` option to read it or refer to
[help_page.h](https://github.com/ErlingSigurdson/server0451/blob/main/src/help_page.h).
