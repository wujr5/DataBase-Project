#include <iostream>
#include <fstream>
#include <cstring>
#include <ctype.h>
#include <iomanip>
#include <strstream>
#include <time.h>

using namespace std;

class Insert {
public:
  Insert();
  void execute(char* filename);
  bool J2B(char* filename);
  void J2B_json_obj(char* json_str);
  void sort_ids_values();
  void preprocessing(char* json_str);
  void update_buffer();
  void write_binary_file();
  void update_catalog();
  void create_binary_data();
  // void B2J();

private:
  int row_count;
};
