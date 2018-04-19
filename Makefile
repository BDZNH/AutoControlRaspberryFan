#!/bin/bash
#HEADER_PATH = -I/usr/local/include/
AutoStart_Script=AutoControlFan
autocontrolfan : main.o
	g++ -o autocontrolfan main.o -lwiringPi -lpthread $(HEADER_PATH)
main.o : main.cpp
	g++ -Wall -fexceptions -g -std=c++11 -lwiringPi -lpthread -DNDEBUG -c main.cpp  $(HEADER_PATH)
clean :
	rm -f *.o
	rm autocontrolfan
install :
	cp autocontrolfan /usr/bin/
	cp AutoControlFan /etc/init.d/
	chmod +x /etc/init.d/${AutoStart_Script}
	update-rc.d ${AutoStart_Script} defaults
uninstall :
	sudo rm /usr/bin/autocontrolfan
	sudo update-rc.d $(AutoStart_Script) remove
	sudo rm -f /etc/init.d/$(AutoStart_Script)
