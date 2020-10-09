#MakeFile to build and deploy the Sample US CENSUS Name Data using ajax
# For CSC3004 Software Development

# Put your user name below:
user= skon

CC= g++

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -std=c++14

RM= /bin/rm -f

all: nameserver testclient namelookupclient PutCGI PutHTML

testclient.o: testclient.cpp fifo.h
	$(CC) -c $(CFLAGS) testclient.cpp

NameEntry.o: NameEntry.cpp NameEntry.h
	$(CC) $(CFLAGS) NameEntry.cpp -c

NameMap.o: NameMap.cpp NameMap.h NameEntry.h
	$(CC) $(CFLAGS) NameMap.cpp -c

nameserver.o: nameserver.cpp fifo.h NameMap.h NameEntry.h
	$(CC) -c $(CFLAGS) nameserver.cpp

namelookupclient.o: namelookupclient.cpp fifo.h
	$(CC) -c $(CFLAGS) namelookupclient.cpp

testclient: testclient.o fifo.o
	$(CC) testclient.o fifo.o -o testclient

nameserver: nameserver.o fifo.o NameEntry.o NameMap.o
	$(CC) nameserver.o NameEntry.o NameMap.o fifo.o -o nameserver

fifo.o:	fifo.cpp fifo.h
	g++ -c fifo.cpp

namelookupclient: namelookupclient.o  fifo.h
	$(CC) namelookupclient.o  fifo.o -o namelookupclient -L/usr/local/lib -lcgicc

PutCGI: namelookupclient
	chmod 757 namelookupclient
	cp namelookupclient /usr/lib/cgi-bin/$(user)_namelookupclient.cgi

	echo "Current contents of your cgi-bin directory: "
	ls -l /usr/lib/cgi-bin/

PutHTML:
	cp namelookupCS.html /var/www/html/class/softdev/$(user)/Nameserver/
	cp namelookupCS.css /var/www/html/class/softdev/$(user)/Nameserver/
	cp namelookupCS.js /var/www/html/class/softdev/$(user)/Nameserver/

	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/class/softdev/$(user)/Nameserver/

clean:
	rm -f *.o namelookupclient nameserver testclient
