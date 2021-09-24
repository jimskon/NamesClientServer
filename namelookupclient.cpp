/***************************************************************************                   
 * namelookupCS.cpp  -  Program to display name statistics use a server                                      
 *                                                                                             
 * copyright : (C) 2021 by Jim Skon                                                            
 *                                                                            
 * This program create an index US Census name                                       
 * Data on the frequency of names in response to requestes.                           
 * It then allows you to look up any name, giving the 10 closest matches               
 *                                                            
 *     
 *
 ***************************************************************************/

// Stuff for AJAX
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

//Stuff for pipes
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include "fifo.h"

// For Transform
#include <algorithm>

using namespace std;
using namespace cgicc; // Needed for AJAX functions.

// Possible values returned by results_select
#define LAST  "Last"
#define MALE "Male"
#define FEMALE "Female"

// fifo for communication
string receive_fifo = "Namereply";
string send_fifo = "Namerequest";

int main() {
  Cgicc cgi;    // Ajax object
  char *cstr;
  // Create AJAX objects to recieve information from web page.
  form_iterator ts = cgi.getElement("type_select");
  form_iterator name = cgi.getElement("name");

  // Output header for CGI
  cout << "Content-Type: text/plain\n\n";

  // create the FIFOs for communication
  Fifo recfifo(receive_fifo);
  Fifo sendfifo(send_fifo);
  bool success; 
  string type = "$LAST*";
  if (**ts == LAST) {
	type = "$LAST*";
  } else if (**ts == MALE) {
	type = "$MALE*";
  } else if (**ts == FEMALE) {
	type = "$FEMALE*";
  }

  // Call server to get results
  string stname = **name;

  /* Convert name to upper case*/
  transform(stname.begin(), stname.end(), stname.begin(), ::toupper);  

  string message =  type+stname;

  success = sendfifo.openwrite();
  if (!success) {
    cout << "Fail to open send fifo" << endl;
  }

  sendfifo.send(message);
  
  /* Get a message from a server */
  success = recfifo.openread();
  if (!success) {
    cout << "Fail to open receive fifo" << endl;
  }
  string results = recfifo.recv();
  recfifo.fifoclose();
  sendfifo.fifoclose();
  cout << results << endl;
  
return 0;
}
