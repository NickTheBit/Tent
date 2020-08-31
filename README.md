# Tenta controller firmware

The following code constitutes the entirety of the web controller of the shades. Including the webserver, the website and the hardware management.

## Features:
* Webserver
* up to 4 tents (2 relays each)
* Spi screen for status report
* temperature and hydro meter
* mozzilla web thing

## Warning
* The display `doesn't work` unless the TFT library is modified
* Filesystem updates need to be executed seperatly command is: `platformio run -t uploadfs`

