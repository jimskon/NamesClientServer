#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "fifo.h"

using namespace std;
string receive_fifo = "Namereply";
string send_fifo = "Namerequest";

int main() {
  string message, name;
  string reply;
  string type;

  while (1) {
  do {
    cout << "1=female,2=male,3=last:";
    cin >> type;
  } while (!(type == "1" || type == "2" || type == "3"));

  if (type=="1") {
    type="FEMALE";
  } else if (type=="2") {
    type="MALE";
  } else if (type=="3") {
    type="LAST";
  }

	cout << "Enter a " << type <<  " name:";
	cin >>  name;
	transform(name.begin(), name.end(), name.begin(), ::toupper);


	// create the FIFOs for communication
	Fifo recfifo(receive_fifo);
	Fifo sendfifo(send_fifo);

	message = "$"+type+"*" + name;
	//	message = "$FEMALE*" + name;
	cout << "Send:" << message << endl;
	sendfifo.openwrite();
	sendfifo.send(message);

	/* Get a message from a server */
	recfifo.openread();
	reply = recfifo.recv();

	recfifo.fifoclose();
	sendfifo.fifoclose();
	cout << "Server sent: " << reply << endl;
  }


}
