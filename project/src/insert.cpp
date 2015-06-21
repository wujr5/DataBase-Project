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
  if (J2B(filename)) {
    write_binary_file();
    update_catalog();
    cout << "Insert file done!" << endl;
  } else {
    cout << "Insert file failed!" << endl;
  }
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

    update_buffer();
  }

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

  // cout << json_str << endl;

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
    int interval = DATAPAGESIZE - buffer_size;
    strncpy(buffer + buffer_size, (char*)&size, interval);
    buffer_size += interval;
    write_binary_file();
    strncpy(buffer + buffer_size, (char*)(&size + interval),INTBYTESIZE - interval);
    buffer_size += INTBYTESIZE - interval;
  }

  for (int i = 0; i < size; i++) {
    if (buffer_size + INTBYTESIZE <= DATAPAGESIZE) {
      strncpy(buffer + buffer_size, (char*)&ids[i], INTBYTESIZE);
      buffer_size += INTBYTESIZE;
    } else {
      int interval = DATAPAGESIZE - buffer_size;
      strncpy(buffer + buffer_size, (char*)&ids[i], interval);
      buffer_size += interval;
      write_binary_file();
      strncpy(buffer + buffer_size, (char*)(&ids[i] + interval),INTBYTESIZE - interval);
      buffer_size += INTBYTESIZE - interval;
    }
  }

  int len;
  for (int i = 0; i <= size; i++) {\

    if (i == 0) len = 0;
    else len += values[i - 1].size();

    if (buffer_size + INTBYTESIZE <= DATAPAGESIZE) {
      strncpy(buffer + buffer_size, (char*)&len, INTBYTESIZE);
      buffer_size += INTBYTESIZE;
    } else {
      int interval = DATAPAGESIZE - buffer_size;
      strncpy(buffer + buffer_size, (char*)&len, interval);
      buffer_size += interval;
      write_binary_file();
      strncpy(buffer + buffer_size, (char*)(&len + interval),INTBYTESIZE - interval);
      buffer_size += INTBYTESIZE - interval;
    }

  }

  for (int i = 0; i < size; i++) {
    if (buffer_size + values[i].size() <= DATAPAGESIZE) {
      strncpy(buffer + buffer_size, (char*)values[i].c_str(), values[i].size());
      buffer_size += values[i].size();
    } else {
      int interval = DATAPAGESIZE - buffer_size;
      strncpy(buffer + buffer_size, (char*)&values[i], interval);
      buffer_size += interval;
      write_binary_file();
      strncpy(buffer + buffer_size, (char*)(&values[i] + interval),values[i].size() - interval);
      buffer_size += values[i].size() - interval;
    }
  }

  size = 0;
}

void Insert::write_binary_file() {
  ofstream outfile("./binary_data.data", ios::binary | ios::app);
  outfile.write(buffer, buffer_size);
  outfile.close();
  buffer_size = 0;
  memset(buffer, '\0', DATAPAGESIZE);
}
