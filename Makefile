#!/bin/bash
#HEADER_PATH = -I/usr/local/include/
#CCFLAG="-Wall -fexceptions -g -std=c++11 -lwiringPi -lpthread -DNDEBUG"
AutoStart_Script=AutoControlFan
all: autocontrolfan showtemperature

autocontrolfan : main.o
	g++ -o autocontrolfan.out main.o -lwiringPi -lpthread $(HEADER_PATH)
main.o : main.cpp
	g++ -Wall -fexceptions -O2 -std=c++11 -lwiringPi -lpthread -DNDEBUG -c main.cpp  $(HEADER_PATH)
showtemperature : showtemperature.o 
	g++ -o showtemperature.out showtemperature.o
showtemperature.o : showtemperature.cpp
	g++ -Wall -fexceptions -O2 -std=c++11 -lpthread -DNDEBUG -c showtemperature.cpp


clean :
	rm -f *.o
	rm *.out
install :
	service ${AutoStart_Script} stop
	rsync autocontrolfan.out /usr/bin/autocontrolfan 
	rsync showtemperature.out /usr/bin/showptp
	rsync AutoControlFan /etc/init.d/
	chmod +x /etc/init.d/${AutoStart_Script}
	update-rc.d ${AutoStart_Script} defaults
	service ${AutoStart_Script} start
uninstall :
	service ${AutoStart_Script} stop
	rm /usr/bin/autocontrolfan
	update-rc.d $(AutoStart_Script) remove
	rm -f /etc/init.d/$(AutoStart_Script)
