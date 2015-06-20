#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

#define DATAPAGESIZE 8192
#define INTBYTESIZE 4

class Insert {
public:
  Insert();
  void execute(char* filename);
  bool J2B(char* filename);
  char* J2B_json_obj(char* json_str);
  char* J2B_text(char* text_str);
  char* J2B_int(char* int_str);
  char* J2B_arr(char* arr_str);
  char* J2B_bool(char* bool_str);
  void preprocessing(char* json_str);
  void update_catalog();
  void create_binary_data();

private:
  char buffer[DATAPAGESIZE];
  int buffer_size;
  int row_count;
};
