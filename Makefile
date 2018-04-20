#!/bin/bash
#HEADER_PATH = -I/usr/local/include/
#CCFLAG="-Wall -fexceptions -g -std=c++11 -lwiringPi -lpthread -DNDEBUG"
AutoStart_Script=AutoControlFan
#showtempera: showtempera.o main.o
#	g++ $(CCFLAG) -oshowtempera showtempera.o
autocontrolfan : main.o
	g++ -o autocontrolfan main.o -lwiringPi -lpthread $(HEADER_PATH)
main.o : main.cpp
	g++ -Wall -fexceptions -g -std=c++11 -lwiringPi -lpthread -DNDEBUG -c main.cpp  $(HEADER_PATH)
clean :
	rm -f *.o
	rm autocontrolfan
install :
	service ${AutoStart_Script} stop
	cp autocontrolfan /usr/bin/
	cp AutoControlFan /etc/init.d/
	chmod +x /etc/init.d/${AutoStart_Script}
	update-rc.d ${AutoStart_Script} defaults
	service ${AutoStart_Script} start
uninstall :
	service ${AutoStart_Script} stop
	rm /usr/bin/autocontrolfan
	update-rc.d $(AutoStart_Script) remove
	rm -f /etc/init.d/$(AutoStart_Script)
