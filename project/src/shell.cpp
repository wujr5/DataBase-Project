#include<iostream>
#include<cstring>
#include <stdlib.h>

#include"insert.h"
#include "check_catalog.h"
#include"find.h"

using namespace std;

void output_commands_info();
int commands_handler(char* command, char* arg1, char* arg2);
void execute_command(int command_id, char* arg1, char* arg2);

int main() {
  // define the three functional object

  output_commands_info();

  while (1) {
    char command[100];

    cout << "DBProject $ ";
    cin.getline(command, 100);
    
    char arg1[100];
    char arg2[100];

    int command_id = commands_handler(command, arg1, arg2);
    execute_command(command_id, arg1, arg2);
  }

  return 0;
}

void execute_command(int command_id, char* arg1, char* arg2) {

  Insert insert;
  CheckCatalog chcat;
  Find find;

  switch (command_id) {
    case 0:
      cout << "Quit DBProject! GoodBye!" << endl;
      exit(1);
      break;
    case 1:
      cout << "Executing check catalog command..." << endl << endl;
      chcat.execute();
      break;
    case 2:
      cout << "Your command is to long. Please input again." << endl << endl;
      break;
    case 3:
      cout << "No arguments in your find command. Please input again." << endl << endl;
      break;
    case 4:
      cout << "No \"=\" sign in your find command. Please input again." << endl << endl;
      break;
    case 5:
      cout << "Both side of = should have values. Please input again." << endl << endl;
      break;
    case 6:
      cout << "Executing find command..." << endl << endl;
      find.execute(arg1, arg2);
      break;
    case 7:
      cout << "Your find command have extra space. Please input again." << endl << endl;
      break;
    case 8:
      cout << "Your insert command have no argument. Please input again." << endl << endl;
      break;
    case 9:
      cout << "Executing insert command..." << endl << endl;
      insert.execute(arg1);
      break;
    case 10:
      cout << "Your insert command have extra space. Please input again." << endl << endl;
      break;
    case 11:
      cout << "No such command. Please input again." << endl << endl;
      break;
  }
}

void output_commands_info() {
  cout << endl << endl;
  cout << "****************     Welcome to Database Project shell      *******************" << endl << endl;
  cout << "Commands are as follows:" << endl << endl;
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
    return 1;
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
          int offset;

          offset = strchr(command, '=') - strchr(command, ' ');
          strncpy(arg1, command + 5, offset - 1);
          arg1[offset - 1] = '\0';

          // cout << arg1 << endl;

          offset = strchr(command, '=') - command + 1;
          strncpy(arg2, command + offset, strlen(command) - offset);
          arg2[strlen(command) - offset] = '\0';

          // cout << arg2 << endl;

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
        strncpy(arg1, command + 7, strlen(command) - 7);
        arg1[strlen(command) - 7] = '\0';
        
        // cout << arg1 << endl;
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
