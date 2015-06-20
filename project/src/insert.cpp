#include "insert.h"

extern string id_attribute[100000];
extern string id_type[100000];
extern int id_count[100000];
extern int id_size;

Insert::Insert() {
  buffer_size = 0;
  row_count = 0;
}

void Insert::execute(char* filename) {
  if (J2B(filename))
    update_catalog();
}

bool Insert::J2B(char* filename) {
  ifstream infile(filename);
  if (!infile) {
    cout << "The file with the path \"" << filename << "\" is not exit. Please input again." << endl;
    return false;
  }

  while (!infile.eof()) {
    char json_obj[1000];
    infile.getline(json_obj, 1000);

    row_count++;

    if (row_count == 1 && json_obj[0] != '[' || row_count == 2 && json_obj[0] != '{') {
      cout << "This file isn't a json format file. Please insert correct file." << endl;
      return false;
    }
    else if (json_obj[0] != '[' && json_obj[0] != ']') {
      J2B_json_obj(json_obj);
    }
  }

  infile.close();
  return true;
}

char* Insert::J2B_json_obj(char* json_str) {
  preprocessing(json_str);
  cout << json_str << endl;

}

void Insert::preprocessing(char* json_str) {
  char* json_str_tem = new char[1000];
  memset(json_str_tem,'\0',1000);

  for (int i = 0; i < strlen(json_str); i++) {
    if (json_str[i] == [])
  }

  json_str = json_str_tem;
}

void Insert::update_catalog() {
  ofstream outfile("./catalog.txt");

  for (int i = 0; i < id_size; i++) {
    outfile << (i + 1) << " " << id_attribute[i] << " " << id_type[i] << " " << id_count[i] << endl;
  }

  outfile.close();
}


  // int a = 50000;
  // ofstream outfile("./a.dat", ios::binary);
  // outfile.write((char*)&a, 4);
  // outfile.close();

  // int b;
  // cout << "sizeof b is :" << sizeof(b) << endl;
  // infile.open("./a.dat", ios::binary);
  // infile.read((char*)&b, 4);
  // cout << "this is b:" << b << endl;
  // infile.close();

  // outfile.open("./b.dat", ios::binary);

  // a = 500;
  // b = 1000;

  // char c[8192];
  // // infile.read(c, 8192);

  // strncpy(c, (char*)&a, 4);
  // strncpy(c + 4, (char*)&b, 4);

  // cout << "cout lala :" << (int&)(*c) << endl;
  // cout << "cout lala :" << (int&)(*(c + 4)) << endl;

  // // int cout;

  // // int tem = (int&)(*c);
  // // cout = cout + 4;

  // // if (cout > 8912) {
  // //   read_from_file();
  // //   cout = 0;
  // // }

  // // char tem[4];

  
  // strcpy(c + 4, (char*)&b);
  // cout << (int&)((*(c + 4))) << endl;


void Insert::create_binary_data() {
  ofstream outfile("./test.data", ios::binary);
  int num = 9;
  int aid[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  int offset[] = {10, 20, 30, 40, 50, 60, 70, 80, 90};

  outfile.write((char*)&num, 4);

  for (int i = 0; i < 9; i++) {
    outfile.write((char*)&aid[i], 4);
  }
  for (int i = 0; i < 9; i++) {
    outfile.write((char*)&offset[i], 4);
  }

  outfile.close();

  ifstream infile("./test.data", ios::binary);

  char tem[4];
  infile.read(tem, 4);
  cout << "the value of tem is :" << (int&)*tem << endl;
}