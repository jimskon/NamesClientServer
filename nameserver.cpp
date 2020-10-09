/***************************************************************************
 * nameservercpp  -  Program to serve of last name statistics
 *
 * copyright : (C) 2017 by Jim Skon
 *
 * This program runs as a background server to a CGI program, providinging US Census
 * Data on the frequency of names in response to requestes.  The rational for this
 * is to demonstrait how a background process can serve up information to a web program,
 * saving the web program from the work of intiallizing large data structures for every
 * call.
 *
 *
 *
 ***************************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <map>
#include "NameMap.h"
#include "NameEntry.h"
#include "fifo.h"

using namespace std;

/* Fifo names */
string receive_fifo = "Namerequest";
string send_fifo = "Namereply";

/* Name of name data files */
const string path = "/home/skon/NamesClientServer/namedata/";
const string lastFile = path+"dist.all.last";
const string maleFile = path+"dist.male.first";
const string femaleFile = path+"dist.female.first";

/* Server main line,create name MAP, wait for and serve requests */
int main() {

  string inMessage, outMessage,name,percent,rank,type;
  int pos;

  // Build the name maps
  NameMap lastMap(lastFile);
  NameMap femaleMap(femaleFile);
  NameMap maleMap(maleFile);

  vector<NameEntry> matchList; // place to stare the matches

  // create the FIFOs for communication
  Fifo recfifo(receive_fifo);
  Fifo sendfifo(send_fifo);

  while (1) {

    /* Get a message from a client */
    recfifo.openread();
    inMessage = recfifo.recv();
    /* Parse the incoming message */
    /* Form:  $type*name  */
    pos=inMessage.find_first_of("*");
    if (pos!=string::npos) {
      type = inMessage.substr(0,pos);
      pos++;
    } else {
      type = "$LAST";
      pos = 0;
    }
    name = inMessage.substr(pos,2000);
    cout << "Message: " << type << " : " << name << endl;

   if (type=="$LAST") {
      matchList = lastMap.getMatches(name);
    } else if (type=="$FEMALE"){
      matchList = femaleMap.getMatches(name);
    } else if (type=="$MALE"){
      matchList = maleMap.getMatches(name);
    }
    outMessage = "";
    for (int i = 0; i < matchList.size(); i++) {
      outMessage += matchList.at(i).name + "," +  matchList.at(i).percent + "," +  matchList.at(i).rank;
      if (i<matchList.size()-1)
	outMessage += ","; // Only put out this comma if not last entry.
    }

    cout << " Results: " << outMessage << endl;

    sendfifo.openwrite();
    sendfifo.send(outMessage);
    sendfifo.fifoclose();
    recfifo.fifoclose();

  }
}
