void Insert::B2J() {
  bool flag = true;

  ifstream infile("./binary_data.data", ios::binary);

  memset(buffer, '\0', DATAPAGESIZE);
  infile.read(buffer, DATAPAGESIZE);
  buffer_size = infile.gcount();
  if (buffer_size != DATAPAGESIZE) flag = false;
  buffer_position = 0;

  while (1) {
    cout << "{";
    int num;

    char tem[1000];
    memset(tem, '\0', 1000);

    if (buffer_position == buffer_size) {
      memset(buffer, '\0', DATAPAGESIZE);
      infile.read(buffer, DATAPAGESIZE);
      buffer_size = infile.gcount();
      if (buffer_size != DATAPAGESIZE) flag = false;
      buffer_position = 0;

    }

    if (buffer_position + 4 <= buffer_size) {
      memcpy((char*)&num, buffer + buffer_position, 4);
      buffer_position += 4;
    } else {
      char num_tem[4];
      int interval = buffer_size - buffer_position;
      memcpy(num_tem, buffer + buffer_position, interval);

      memset(buffer, '\0', DATAPAGESIZE);
      infile.read(buffer, DATAPAGESIZE);
      buffer_size = infile.gcount();
      if (buffer_size != DATAPAGESIZE) flag = false;
      buffer_position = 0;

      memcpy(num_tem + interval, buffer + buffer_position, 4 - interval);
      buffer_position = buffer_position + 4 - interval;

      istrstream istr(num_tem);

      istr >> num;
    }

    // cout << "attrbutes_num: " << num << endl;

    unsigned int all_ids[1000];

    for (int i = 0; i < num; i++) {
      unsigned int id;

      if (buffer_position == buffer_size) {
        memset(buffer, '\0', DATAPAGESIZE);
        infile.read(buffer, DATAPAGESIZE);
        buffer_size = infile.gcount();
        if (buffer_size != DATAPAGESIZE) flag = false;
        buffer_position = 0;
      }

      if (buffer_position + 4 <= buffer_size) {
        memcpy((char*)&id, buffer + buffer_position, 4);
        buffer_position += 4;
      } else {
        char num_tem[4];
        int interval = buffer_size - buffer_position;
        memcpy(num_tem, buffer + buffer_position, interval);

        memset(buffer, '\0', DATAPAGESIZE);
        infile.read(buffer, DATAPAGESIZE);
        buffer_size = infile.gcount();
        if (buffer_size != DATAPAGESIZE) flag = false;
        buffer_position = 0;

        memcpy(num_tem + interval, buffer + buffer_position, 4 - interval);
        buffer_position = buffer_position + 4 - interval;

        istrstream istr(num_tem);

        istr >> id;
      }

      all_ids[i] = id;
      // cout << "id: " << id << endl;
    }

    unsigned int offsets[1000];
    offsets[0] = 0;

    unsigned int last_offset = 0;

    for (int i = 0; i <= num; i++) {
      unsigned int offset;

      if (buffer_position == buffer_size) {
        memset(buffer, '\0', DATAPAGESIZE);
        infile.read(buffer, DATAPAGESIZE);
        buffer_size = infile.gcount();
        if (buffer_size != DATAPAGESIZE) flag = false;
        buffer_position = 0;
      }

      if (buffer_position + 4 <= buffer_size) {
        memcpy((char*)&offset, buffer + buffer_position, 4);
        buffer_position += 4;
      } else {
        char num_tem[4];
        int interval = buffer_size - buffer_position;
        memcpy(num_tem, buffer + buffer_position, interval);

        memset(buffer, '\0', DATAPAGESIZE);
        infile.read(buffer, DATAPAGESIZE);
        buffer_size = infile.gcount();
        if (buffer_size != DATAPAGESIZE) flag = false;
        buffer_position = 0;

        memcpy(num_tem + interval, buffer + buffer_position, 4 - interval);
        buffer_position = buffer_position + 4 - interval;
        
        istrstream istr(num_tem);

        istr >> offset;
      }

      if (i != 0) {
        offsets[i - 1] = offset - last_offset;
        // cout << "offset[" << (i - 1) << "] = " << offsets[i - 1] << endl;
      }
      // cout << "offset: " << offset << endl;
      last_offset = offset;
    }
    
    for (int i = 0; i < num; i++) {
      unsigned int int_tem;
      char str_tem[1000];
      memset(str_tem, '\0', 1000);

      if (id_type[all_ids[i]] == "text" || id_type[all_ids[i]] == "json_obj" || id_type[all_ids[i]] == "arr") {
        memcpy(str_tem, buffer + buffer_position, offsets[i]);
        buffer_position += offsets[i];

        if (buffer_position == buffer_size) {
          memset(buffer, '\0', DATAPAGESIZE);
          infile.read(buffer, DATAPAGESIZE);
          buffer_size = infile.gcount();
          if (buffer_size != DATAPAGESIZE) flag = false;
          buffer_position = 0;
        }

        if (buffer_position + offsets[i] <= buffer_size) {
          memcpy((char*)&str_tem, buffer + buffer_position, offsets[i]);
          buffer_position += offsets[i];
        } else {
          int interval = buffer_size - buffer_position;
          memcpy(str_tem, buffer + buffer_position, interval);

          memset(buffer, '\0', DATAPAGESIZE);
          infile.read(buffer, DATAPAGESIZE);
          buffer_size = infile.gcount();
          if (buffer_size != DATAPAGESIZE) flag = false;
          buffer_position = 0;

          memcpy(str_tem + interval, buffer + buffer_position, offsets[i] - interval);
          buffer_position = buffer_position + offsets[i] - interval;
        }
      }

      if (id_type[all_ids[i]] == "text") {

        if (i == num - 1) cout << "\"" << id_attribute[all_ids[i]] << "\": " << "\"" << str_tem << "\"";
        else cout << "\"" << id_attribute[all_ids[i]] << "\": " << "\"" << str_tem << "\", ";
      }
      else if (id_type[all_ids[i]] == "json_obj") {

        if (i == num - 1) cout << "\"" << id_attribute[all_ids[i]] << "\": " << "{" << str_tem << "}";
        else cout << "\"" << id_attribute[all_ids[i]] << "\": " << "{" << str_tem << "}, ";
      }
      else if (id_type[all_ids[i]] == "arr") {
        
        if (i == num - 1) cout << "\"" << id_attribute[all_ids[i]] << "\": " << "[" << str_tem << "]";
        else cout << "\"" << id_attribute[all_ids[i]] << "\": " << "[" << str_tem << "], ";
      }
      else if (id_type[all_ids[i]] == "int") {
        if (buffer_position == buffer_size) {
          memset(buffer, '\0', DATAPAGESIZE);
          infile.read(buffer, DATAPAGESIZE);
          buffer_size = infile.gcount();
          if (buffer_size != DATAPAGESIZE) flag = false;
          buffer_position = 0;
          
        }

        if (buffer_position + 4 <= buffer_size) {
          memcpy((char*)&int_tem, buffer + buffer_position, 4);
          buffer_position += 4;
        } else {
          char num_tem[4];
          int interval = buffer_size - buffer_position;
          memcpy(num_tem, buffer + buffer_position, interval);

          memset(buffer, '\0', DATAPAGESIZE);
          infile.read(buffer, DATAPAGESIZE);
          buffer_size = infile.gcount();
          if (buffer_size != DATAPAGESIZE) flag = false;
          buffer_position = 0;

          memcpy(num_tem + interval, buffer + buffer_position, 4 - interval);
          buffer_position = buffer_position + 4 - interval;

          istrstream istr(num_tem);

          istr >> int_tem;
        }
        if (i == num - 1) cout << "\"" << id_attribute[all_ids[i]] << "\": " << int_tem;
        else cout << "\"" << id_attribute[all_ids[i]] << "\": " << int_tem << ", ";
      }
      else if (id_type[all_ids[i]] == "bool") {

        if (buffer_position == buffer_size) {
          memset(buffer, '\0', DATAPAGESIZE);
          infile.read(buffer, DATAPAGESIZE);
          buffer_size = infile.gcount();
          if (buffer_size != DATAPAGESIZE) flag = false;
          buffer_position = 0;
        }

        memcpy(str_tem, buffer + buffer_position, 1);
        buffer_position += 1;

        if (str_tem[0] == '0') {
          if (i == num - 1) cout << "\"" << id_attribute[all_ids[i]] << "\": " << "false";
          else cout << "\"" << id_attribute[all_ids[i]] << "\": " << "false, ";
          
        } else {
          if (i == num - 1) cout << "\"" << id_attribute[all_ids[i]] << "\": " << "true, ";
          else cout << "\"" << id_attribute[all_ids[i]] << "\": " << "true, ";
        }
      }
    }
    cout << "}" << endl << endl;

    if (flag == false || buffer_size == buffer_position) break;
  }

  // for (int k = 0; k < 100; k++) {
  //   int num;

  //   char tem[1000];
  //   memset(tem, '\0', 1000);

  //   infile.read((char*)&num, 4);
  //   cout << "attrbutes_num: " << num << endl;

  //   unsigned int all_ids[1000];

  //   for (int i = 0; i < num; i++) {
  //     unsigned int id;
  //     infile.read((char*)&id, 4);
  //     all_ids[i] = id;
  //     cout << "id: " << id << endl;
  //   }

  //   unsigned int offsets[1000];
  //   offsets[0] = 0;

  //   unsigned int last_offset = 0;
  //   for (int i = 0; i <= num; i++) {
  //     unsigned int offset;
  //     infile.read((char*)&offset, 4);
  //     if (i != 0) {
  //       offsets[i - 1] = offset - last_offset;
  //       cout << "offset[" << (i - 1) << "] = " << offsets[i - 1] << endl;
  //     }

  //     cout << "offset: " << offset << endl;
  //     last_offset = offset;
  //   }

  //   for (int i = 0; i < num; i++) {
  //     unsigned int int_tem;
  //     char str_tem[1000];
  //     memset(str_tem, '\0', 1000);

  //     if (id_type[all_ids[i]] == "text") {
  //       infile.read(str_tem, offsets[i]);
  //       cout << id_attribute[all_ids[i]] << " = " << str_tem << endl;
  //     }
  //     else if (id_type[all_ids[i]] == "json_obj") {
  //       infile.read(str_tem, offsets[i]);
  //       cout << id_attribute[all_ids[i]] << " = " << "{" << str_tem << "}" << endl;
  //     }
  //     else if (id_type[all_ids[i]] == "arr") {
  //       infile.read(str_tem, offsets[i]);
  //       cout << id_attribute[all_ids[i]] << " = [" << str_tem << "]" << endl;
  //     }
  //     else if (id_type[all_ids[i]] == "int") {
  //       infile.read((char*)&int_tem, 4);
  //       cout << id_attribute[all_ids[i]] << " = " << int_tem << endl;
  //     }
  //     else if (id_type[all_ids[i]] == "bool") {
  //       infile.read(str_tem, 1);
  //       if (str_tem[0] == '0')
  //         cout << id_attribute[all_ids[i]] << " = " << "false" << endl;
  //       else 
  //         cout << id_attribute[all_ids[i]] << " = " << "true" << endl;
  //     }
  //   }
  //   cout << "k = " << k << endl;
  // }

  infile.close();
}