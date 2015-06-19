#include<iostream>
#include<cstring>

#include"insert.h"
#include "check_catalog.h"
#include"find.h"

using namespace std;

void output_commands_info();
int commands_handler(char* command, char* arg1, char* arg2);

int main() {
  // define the three functional object

  Insert insert;
  CheckCatalog chcat;
  Find find;

  output_commands_info();

  while (1) {
    char command[100];

    cout << "DBProject $ ";
    cin.getline(command, 100);
    
    char arg1[100];
    char arg2[100];

    commands_handler(command, arg1, arg2);
    
    insert.execute(arg1);
    chcat.execute();
    find.execute(arg1, arg2);

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

int commands_handler(char * command, char* arg1, char* arg2) {
  if (string(command) == "q") {
    // cout << "Quit DBProject!" << endl;
    return 0;
  }
  else if (string(command) == "check catalog") {
    // cout << "check catalog command!" << endl;
    // return 1;
  }
  else if (strlen(command) > 30) {
    // cout << "your command is to long!" << endl;
    return 2;
  } else {

    if (strcmp("find", command) == 0 || strcmp("find ", command) == 0) {
      // cout << "no arguments" << endl;
      return 3;
    }
    else if (strncmp("find ", command, 5) == 0 && strcmp("find ", command) < 0) {
      if (strchr(command + 5, ' ') == NULL) {
        if (strchr(command + 5, '=') == NULL) {
          // cout << "no = sign!" << endl;
          return 4;
        }
        else if (command[strlen(command) - 1] == '=') {
          // cout << "both side of = should have values" << endl;
          return 5;
        } else {
          // cout << "find command!" << endl;
          // cout << strncmp()
          return 6;
        }
      } else {
        // cout << "extra space!" << endl;
        return 7;
      }
    }

    if (strcmp("insert", command) == 0 || strcmp("insert ", command) == 0) {
      // cout << "no arguments too" << endl;
      return 8;
    }
    else if (strncmp("insert ", command, 7) == 0 && strcmp("insert ", command) < 0) {
      if (strchr(command + 7, ' ') == NULL) {
        // cout << "insert command!" << endl;
        return 9;
      } else {
        // cout << "extra space!" << endl;
        return 10;
      }
    }

    // cout << "No such command!" << endl;
    return 11;
  }
}
