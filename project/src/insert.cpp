#include "insert.h"

#define DATAPAGESIZE 8192
#define INTBYTESIZE 4

extern string id_attribute[100000];
extern string id_type[100000];
extern int id_count[100000];
extern int id_size;

extern char buffer[DATAPAGESIZE];
extern int buffer_size;

string values[10000];
int ids[10000];
int size = 0;

Insert::Insert() {
  row_count = 0;
}

void Insert::execute(char* filename) {
  if (J2B(filename))
    write_binary_file();
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

    if ((row_count - 2) > 0 && (row_count - 2) % 10 == 0) update_buffer();
  }

  update_buffer();

  infile.close();

  return true;
}

int find_arrt_id(char* key) {
  for (int i = 0; i < id_size; i++) {
    if (id_attribute[i] == string(key)) {
      return i;
    }
  }
  return -1;
}

void Insert::J2B_json_obj(char* json_str) {

  cout << json_str << endl;

  for (int i = 1; i < strlen(json_str); i++) {
    if (json_str[i] == '"') {

      int interval_1 = strchr(json_str + i + 1, '"') - json_str - i - 1;
      char key_tem[100];
      memset(key_tem, '\0' , 100);

      int id;

      if (i + interval_1 < strlen(json_str)) {
        strncpy(key_tem, json_str + i + 1, interval_1);

        id = find_arrt_id(key_tem);

        if (id == -1) {
          id_attribute[id_size] = string(key_tem);
          id_count[id_size]++;
          id_size++;
          ids[size] = id_size - 1;
        } else {
          id_count[id]++;
          ids[size] = id;
        }

        // cout << "key = " << key_tem << " ";

        i += interval_1;
      }

      i += 4;

      if (json_str[i] == '{') {
        int interval_2 = strchr(json_str + i + 1, '}') - json_str - i - 1;
        if (id == -1) id_type[id_size - 1] = "json_obj";
        char tem[500];
        memset(tem, '\0', 500);
        strncpy(tem, json_str + i + 1, interval_2);

        values[size++] = tem;
        i += interval_2;

        i += 3;
      }
      else if (json_str[i] == 'f' || json_str[i] == 't') {
        if (id == -1) id_type[id_size - 1] = "bool";

        if (json_str[i] == 'f') {
          values[size++] = '0';
          i += 5;
        } else {
          values[size++] = '1';
          i += 4;
        }

        i += 1;
      }
      else if (isdigit(json_str[i])) {
        if (id == -1) id_type[id_size - 1] = "int";
        int interval_2 = strchr(json_str + i + 1, ',') - json_str - i;
        char tem[100];
        memset(tem, '\0', 100);
        strncpy(tem, json_str + i, interval_2);
        values[size++] = string(tem);

        i += 2;
      }
      else if (json_str[i] == '[') {
        if (id == -1) id_type[id_size - 1] = "arr";
        int interval_2 = strchr(json_str + i + 1, ']') - json_str - i - 1;
        char tem[500];
        memset(tem, '\0', 500);
        strncpy(tem, json_str + i + 1, interval_2);
        values[size++] = string(tem);

        i += interval_2;

        i += 3;
      } else {
        if (id == -1) id_type[id_size - 1] = "text";
        int interval_2 = strchr(json_str + i + 1, '"') - json_str - i - 1;
        char tem[500];
        memset(tem, '\0', 500);
        strncpy(tem, json_str + i + 1, interval_2);
        values[size++] = string(tem);

        i += interval_2;

        i += 3;
      }
      // cout << "value = " << values[size - 1] << endl;
    }
  }
}

// use quick sort to improve the performance
void Insert::sort_ids_values() {
  for (int i = 0; i < size - 1; i++) {
    for (int j = i + 1; j < size; j++) {
      if (ids[i] > ids[j]) {
        int tem = ids[i];
        ids[i] = ids[j];
        ids[j] = tem;

        string tem_str = values[i];
        values[i] = values[j];
        values[j] = tem_str;
      }
    }
  }
}

void Insert::update_catalog() {
  ofstream outfile("./catalog.data");

  for (int i = 0; i < id_size; i++) {
    outfile << i << " " << id_attribute[i] << " " << id_type[i] << " " << id_count[i] << endl;
  }

  sort_ids_values();
  for (int i = 0; i < size; i++) {
    cout << i << '\t' << ids[i] <<'\t' << values[i] << endl;
  }

  outfile.close();
}

void Insert::update_buffer() {

  if (size == 0) return;

  if (buffer_size + INTBYTESIZE <= DATAPAGESIZE) {
    strncpy(buffer + buffer_size, (char*)&size, INTBYTESIZE);
    buffer_size += INTBYTESIZE;
  } else {
    strncpy(buffer + buffer_size, (char*)&size, DATAPAGESIZE - buffer_size);
    write_binary_file();
    strncpy(buffer + buffer_size, (char*)(&size + DATAPAGESIZE - buffer_size), buffer_size + INTBYTESIZE - DATAPAGESIZE);
  }

  for (int i = 0; i < size; i++) {
    if (buffer_size + INTBYTESIZE <= DATAPAGESIZE) {
      strncpy(buffer + buffer_size, (char*)&ids[i], INTBYTESIZE);
      buffer_size += INTBYTESIZE;
    } else {
      strncpy(buffer + buffer_size, (char*)&ids[i], DATAPAGESIZE - buffer_size);
      write_binary_file();
      strncpy(buffer + buffer_size, (char*)(&ids[i] + DATAPAGESIZE - buffer_size), buffer_size + INTBYTESIZE - DATAPAGESIZE);
    }
  }

  for (int i = 0; i < size; i++) {
    int len = values[i].size();

    if (buffer_size + INTBYTESIZE <= DATAPAGESIZE) {
      strncpy(buffer + buffer_size, (char*)&len, INTBYTESIZE);
      buffer_size += INTBYTESIZE;
    } else {
      strncpy(buffer + buffer_size, (char*)&len, DATAPAGESIZE - buffer_size);
      write_binary_file();
      strncpy(buffer + buffer_size, (char*)(&len + DATAPAGESIZE - buffer_size), buffer_size + INTBYTESIZE - DATAPAGESIZE);
    }

  }

  for (int i = 0; i < size; i++) {
    if (buffer_size + INTBYTESIZE <= DATAPAGESIZE) {
      strncpy(buffer + buffer_size, (char*)values[i].c_str(), values[i].size());
      buffer_size += values[i].size();
    } else {
      strncpy(buffer + buffer_size, (char*)values[i].c_str(), DATAPAGESIZE - buffer_size);
      write_binary_file();
      strncpy(buffer + buffer_size, (char*)(values[i].c_str() + DATAPAGESIZE - buffer_size), buffer_size + INTBYTESIZE - DATAPAGESIZE);
    }
  }

  size = 0;
}

void Insert::write_binary_file() {
  ofstream outfile("./binary_data.data", ios::binary | ios::app);
  outfile.write(buffer, buffer_size);
  buffer_size = 0;
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

  // int cout;

  // int tem = (int&)(*c);
  // cout = cout + 4;

  // if (cout > 8912) {
  //   read_from_file();
  //   cout = 0;
  // }

  // char tem[4];

  
  // strcpy(c + 4, (char*)&b);
  // cout << (int&)((*(c + 4))) << endl;

// void Insert::preprocessing(char* json_str) {
//   char* json_str_tem = new char[1000];
//   memset(json_str_tem,'\0',1000);

//   for (int i = 0; i < strlen(json_str); i++) {
//     if (json_str[i] == '{' || json_str[i] == '"' || json_str[i] == ':' || json_str[i] == ',' || json_str[i] == '}') {
//       if (i == 0) {
//         json_str_tem[i] = json_str[i];
//       }
//       else if ((json_str_tem[strlen(json_str_tem) - 1] != ' ')){
//         json_str_tem[strlen(json_str_tem)] = ' ';
//         json_str_tem[strlen(json_str_tem)] = json_str[i];
//       } else {
//         json_str_tem[strlen(json_str_tem)] = json_str[i];
//       }
//       if (json_str[i + 1] != ' ') json_str_tem[strlen(json_str_tem)] = ' ';
//     } else {
//       json_str_tem[strlen(json_str_tem)] = json_str[i];
//     }
//   }

//   cout << json_str_tem << endl << endl;
//   json_str = json_str_tem;
// }

// void Insert::create_binary_data() {
//   ofstream outfile("./test.data", ios::binary);
//   int num = 9;
//   int aid[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
//   int offset[] = {10, 20, 30, 40, 50, 60, 70, 80, 90};

//   outfile.write((char*)&num, 4);

//   for (int i = 0; i < 9; i++) {
//     outfile.write((char*)&aid[i], 4);
//   }
//   for (int i = 0; i < 9; i++) {
//     outfile.write((char*)&offset[i], 4);
//   }

//   outfile.close();

//   ifstream infile("./test.data", ios::binary);

//   char tem[4];
//   infile.read(tem, 4);
//   cout << "the value of tem is :" << (int&)*tem << endl;
// }


