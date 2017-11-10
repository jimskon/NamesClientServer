#MakeFile to build and deploy the Sample US CENSUS Name Data using ajax
# For CSC3004 Software Development

# Put your user name below:
USER= skon

CC= g++

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -g

RM= /bin/rm -f

all: nameserver testclient namelookupclient PutCGI PutHTML

testclient.o: testclient.cpp fifo.h
	$(CC) -c $(CFLAGS) testclient.cpp

nameserver.o: nameserver.cpp fifo.h
	$(CC) -c $(CFLAGS) nameserver.cpp

namelookupclient.o: namelookupclient.cpp fifo.h
	$(CC) -c $(CFLAGS) namelookupclient.cpp

testclient: testclient.o fifo.o
	$(CC) testclient.o fifo.o -o testclient

nameserver: nameserver.o fifo.o
	$(CC) nameserver.o  fifo.o -o nameserver

fifo.o:	fifo.cpp fifo.h
	g++ -c fifo.cpp

namelookupclient: namelookupclient.o  fifo.h
	$(CC) namelookupclient.o  fifo.o -o namelookupclient -L/usr/local/lib -lcgicc

PutCGI: namelookupclient
	chmod 757 namelookupclient
	cp namelookupclient /usr/lib/cgi-bin/$(USER)_namelookupclient.cgi 

	echo "Current contents of your cgi-bin directory: "
	ls -l /usr/lib/cgi-bin/

PutHTML:
	cp namelookupCS.html /var/www/html/class/softdev/$(USER)/Names/
	cp namelookupCS.css /var/www/html/class/softdev/$(USER)/Names/
	cp namelookupCS.js /var/www/html/class/softdev/$(USER)/Names/

	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/class/softdev/$(USER)/Names/

clean:
	rm -f *.o namelookupclient nameserver testclient
