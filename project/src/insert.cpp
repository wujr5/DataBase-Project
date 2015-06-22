#include "insert.h"

#define DATAPAGESIZE 8192
#define INTBYTESIZE 4

extern string id_attribute[100000];
extern string id_type[100000];
extern int id_count[100000];
extern int id_size;

extern char buffer[DATAPAGESIZE];
extern int buffer_size;

int select = 0;

string values[10000];
unsigned int ids[10000];
unsigned int size = 0;

Insert::Insert() {
  row_count = 0;
}

void Insert::execute(char* filename) {
  // unsigned int d = 256;
  // cout << hex << d << endl;
  // char tem[4];

  // memcpy(tem, (char*)&d, 4);

  // cout << (unsigned int &)*tem << endl;

  if (select == 1) B2J();
  else if (select == 0) {
    if (J2B(filename)) {
      write_binary_file();
      update_catalog();
      cout << "Insert file done!" << endl;
    } else {
      cout << "Insert file failed!" << endl;
    }
  }
}

void Insert::B2J() {
  ifstream infile("./binary_data.data", ios::binary);

  for (int k = 0; k < 10000; k++) {
    

    int num;

    char tem[1000];
    memset(tem, '\0', 1000);

    infile.read((char*)&num, 4);
    cout << "attrbutes_num: " << num << endl;

    unsigned int all_ids[1000];

    for (int i = 0; i < num; i++) {
      unsigned int id;
      infile.read((char*)&id, 4);
      all_ids[i] = id;
      cout << "id: " << id << endl;
    }

    unsigned int offsets[1000];
    offsets[0] = 0;

    unsigned int last_offset = 0;
    for (int i = 0; i <= num; i++) {
      unsigned int offset;
      infile.read((char*)&offset, 4);
      if (i != 0) {
        offsets[i - 1] = offset - last_offset;
        cout << "offset[" << (i - 1) << "] = " << offsets[i - 1] << endl;
      }

      cout << "offset: " << offset << endl;
      last_offset = offset;
    }

    for (int i = 0; i < num; i++) {
      unsigned int int_tem;
      char str_tem[1000];
      memset(str_tem, '\0', 1000);

      if (id_type[all_ids[i]] == "text") {
        infile.read(str_tem, offsets[i]);
        cout << id_attribute[all_ids[i]] << " = " << str_tem << endl;
      }
      else if (id_type[all_ids[i]] == "json_obj") {
        infile.read(str_tem, offsets[i]);
        cout << id_attribute[all_ids[i]] << " = " << "{" << str_tem << "}" << endl;
      }
      else if (id_type[all_ids[i]] == "arr") {
        infile.read(str_tem, offsets[i]);
        cout << id_attribute[all_ids[i]] << " = [" << str_tem << "]" << endl;
      }
      else if (id_type[all_ids[i]] == "int") {
        infile.read((char*)&int_tem, 4);
        cout << id_attribute[all_ids[i]] << " = " << int_tem << endl;
      }
      else if (id_type[all_ids[i]] == "bool") {
        infile.read(str_tem, 1);
        if (str_tem[0] == '0')
          cout << id_attribute[all_ids[i]] << " = " << "false" << endl;
        else 
          cout << id_attribute[all_ids[i]] << " = " << "true" << endl;
      }
    }
    cout << "k = " << k << endl;
  }

  infile.close();
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
    // cout << "row_count = " << row_count << endl;
    // if (row_count == 33) break;

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

int find_arrt_id(char* key, string type) {
  for (int i = 0; i < id_size; i++) {
    if (id_attribute[i] == string(key) && id_type[i] == type) {
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
      string type;

      if (i + interval_1 < strlen(json_str)) {
        memcpy(key_tem, json_str + i + 1, interval_1);

        // cout << "key = " << key_tem << " ";

        i += interval_1 + 4;

        if (json_str[i] == '{') {
          type = "json_obj";
        }
        else if (json_str[i] == 'f' || json_str[i] == 't') {
          type = "bool";
        }
        else if (isdigit(json_str[i])) {
          type = "int";
        }
        else if (json_str[i] == '[') {
          type = "arr";
        } else {
          type = "text";
        }

        id = find_arrt_id(key_tem, type);

        if (id == -1) {
          id_attribute[id_size] = string(key_tem);
          id_count[id_size]++;
          id_size++;
          ids[size] = id_size - 1;
        } else {
          id_count[id]++;
          ids[size] = id;
        }

        if (json_str[i] == '{') {
          int interval_2 = strchr(json_str + i + 1, '}') - json_str - i - 1;
          if (id == -1) id_type[id_size - 1] = "json_obj";
          char tem[500];
          memset(tem, '\0', 500);
          memcpy(tem, json_str + i + 1, interval_2);

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
          memcpy(tem, json_str + i, interval_2);
          // cout << tem << endl;
          values[size++] = string(tem);

          i += 2;
        }
        else if (json_str[i] == '[') {
          if (id == -1) id_type[id_size - 1] = "arr";
          int interval_2 = strchr(json_str + i + 1, ']') - json_str - i - 1;
          char tem[500];
          memset(tem, '\0', 500);
          memcpy(tem, json_str + i + 1, interval_2);
          values[size++] = string(tem);

          i += interval_2;

          i += 3;
        } else {
          if (id == -1) id_type[id_size - 1] = "text";
          int interval_2 = strchr(json_str + i + 1, '"') - json_str - i - 1;
          char tem[500];
          memset(tem, '\0', 500);
          memcpy(tem, json_str + i + 1, interval_2);
          values[size++] = string(tem);

          i += interval_2;

          i += 3;
        }
      }
      // cout << "value = " << values[size - 1] << endl;
    }
  }

  // int k = 0;
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

  outfile.close();
}

void Insert::update_buffer() {

  if (size == 0) return;

  sort_ids_values();

  // for (int i = 0; i < size; i++) {
  //   cout << i << "\t" << ids[i] << "\t" << id_attribute[ids[i]] << "\t" << values[i] << "\t" << id_type[ids[i]] << endl;
  // }

  // store attrbutes_num
  if (buffer_size == DATAPAGESIZE) {
    write_binary_file();
  }

  if (buffer_size + INTBYTESIZE <= DATAPAGESIZE) {
    memcpy(buffer + buffer_size, (char*)&size, INTBYTESIZE);
    buffer_size += INTBYTESIZE;
  } else {
    char size_tem[4];
    memcpy(size_tem, (char*)&size, INTBYTESIZE);

    int interval = DATAPAGESIZE - buffer_size;
    memcpy(buffer + buffer_size, size_tem, interval);
    buffer_size += interval;
    write_binary_file();
    memcpy(buffer + buffer_size, size_tem + interval, INTBYTESIZE - interval);
    buffer_size += (INTBYTESIZE - interval);
  }

  // store aids
  for (int i = 0; i < size; i++) {
    if (buffer_size == DATAPAGESIZE) {
      write_binary_file();
    }

    if (buffer_size + INTBYTESIZE <= DATAPAGESIZE) {
      memcpy(buffer + buffer_size, (char*)&ids[i], INTBYTESIZE);
      buffer_size += INTBYTESIZE;
    } else {
      unsigned int id = ids[i];
      char id_tem[4];
      memcpy(id_tem, (char*)&ids[i], INTBYTESIZE);

      int interval = DATAPAGESIZE - buffer_size;
      memcpy(buffer + buffer_size, id_tem, interval);
      buffer_size += interval;
      write_binary_file();
      memcpy(buffer + buffer_size, id_tem + interval, INTBYTESIZE - interval);
      buffer_size += INTBYTESIZE - interval;
    }
  }

  // store offset
  unsigned int len = 0;
  // for (int i = 0; i < size; i++) {
  //   cout << i << "\t" << ids[i] << "\t" << id_attribute[ids[i]] << "\t" << values[i] << "\t" << id_type[ids[i]] << endl;
  // }

  for (int i = 0; i <= size; i++) {

    if (i == 0) len = 0;
    else if (id_type[ids[i - 1]] == "int") {
      len += 4;
    }
    else if (id_type[ids[i - 1]] == "bool") {
      len += 1;
    } else {
      len += values[i - 1].size();
    }

    // cout << "len = " << len << endl;

    if (buffer_size == DATAPAGESIZE) {
      write_binary_file();
    } 
    
    if (buffer_size + INTBYTESIZE <= DATAPAGESIZE) {
      memcpy(buffer + buffer_size, (char*)&len, INTBYTESIZE);
      buffer_size += INTBYTESIZE;

    } else {
      char len_tem[4];
      memcpy(len_tem, (char*)&len, INTBYTESIZE);

      int interval = DATAPAGESIZE - buffer_size;
      memcpy(buffer + buffer_size, len_tem, interval);
      buffer_size += interval;

      write_binary_file();

      memcpy(buffer + buffer_size, len_tem + interval, INTBYTESIZE - interval);
      buffer_size += INTBYTESIZE - interval;
    }
  }

  // store data 
  for (int i = 0; i < size; i++) {
    if (id_type[ids[i]] == "arr" || id_type[ids[i]] == "text" || id_type[ids[i]] == "json_obj" || id_type[ids[i]] == "bool") {
      if (buffer_size == DATAPAGESIZE) {
        write_binary_file();
      }

      if (buffer_size + values[i].size() <= DATAPAGESIZE) {
        memcpy(buffer + buffer_size, (char*)values[i].c_str(), values[i].size());
        buffer_size += values[i].size();
      } else {
        int interval = DATAPAGESIZE - buffer_size;
        char* str_tem = (char*)values[i].c_str();

        memcpy(buffer + buffer_size, (char*)&str_tem, interval);
        buffer_size += interval;
        write_binary_file();
        memcpy(buffer + buffer_size, (char*)(&str_tem + interval), values[i].size() - interval);
        buffer_size += values[i].size() - interval;
      }
    } else {
      unsigned int int_tem = 0;
      char* str_tem = (char*)values[i].c_str();

      istrstream istr(str_tem);
      istr >> int_tem;

      if (buffer_size == DATAPAGESIZE) {
        write_binary_file();
      }

      if (buffer_size + INTBYTESIZE <= DATAPAGESIZE) {
        memcpy(buffer + buffer_size, (char*)&int_tem, INTBYTESIZE);
        buffer_size += INTBYTESIZE;
      } else {
        char data_tem[4];
        memcpy(data_tem, (char*)&int_tem, INTBYTESIZE);

        int interval = DATAPAGESIZE - buffer_size;
        memcpy(buffer + buffer_size, (char*)&data_tem, interval);
        buffer_size += interval;

        write_binary_file();

        memcpy(buffer + buffer_size, (char*)(&data_tem + interval),INTBYTESIZE - interval);
        buffer_size += INTBYTESIZE - interval;
      }
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
