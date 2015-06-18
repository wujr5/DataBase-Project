#include<iostream>

#include"insert.h"
#include "check_catalog.h"
#include"find.h"

using namespace std;

void output_commands_info();
// void commands_handler(string& command);

int main() {
  // define the three functional object
  // Insert insert;
  // Check_catalog chcat;
  // Find find;

  output_commands_info();
  

  while (1) {
    char command[100];

    cout << "DBProject $ ";
    cin.getline(command, 20);
    cout << command << endl;
    // cout << command<< endl;

    // commands_handler(command);
  }
  return 0;
}

void output_commands_info() {
  cout << endl << endl;
  cout << "****************     Welcome to Database Project shell      *******************" << endl << endl;
  cout << "Commands are follows:" << endl << endl;
  cout << "   q               : quit" << endl;
  cout << "   find A=B        : find the key A is equal to B, and if exist, retuan matched" << endl;
  cout << "                     json records, if not, return NONE" << endl;
  cout << "   check catalog   : return the catalog as a kind of format of table" << endl;
  cout << "   insert filename : insrt the file of [filename] which format is json to the " << endl;
  cout << "                   : database" << endl << endl;
  cout << "****************************************************************************" << endl << endl;
}

// void commands_handler(char * command) {
//   if (command == "q" || command == "check catalog") {
//     cout << command << endl;
//     return;
//   } else {

//   }
// }
