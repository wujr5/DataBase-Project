#include "find.h"
#include "insert.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <ctime>

extern string id_attribute[100000];
extern string id_type[100000];
extern int id_count[100000];
extern int id_size;

#define DATAPAGESIZE 8192

bool isNone = true;



void Find::execute(char* key, char* value) {
    clock_t start_time = clock();

    
    isNone = true;
  
    
    cout << "find command execute!" << endl;
    cout << "key = " << key << endl;
    cout << "value = " << value << endl;
    
    cout << "-------RESULT AS FOLLOWING-------"<<endl;
    
    int key_id = get_key_id(key,value);

    if(key_id == -1){             //cannot find the key
        cout<<"NONE"<<endl;
        return;
    }
    
    ifstream infile("binary_data.data",ios::binary);
    char c[DATAPAGESIZE];
    
    
    
    int position = 0;
    int attribute_num = -1;
    int c_size = 0;
    
    infile.read(c, DATAPAGESIZE);
    c_size = infile.gcount();
    
    char temp[4];
    //used for save the number when the space at end of page is not reached 8KB.
    
    while(!infile.eof() ){
        
        //get the number of attributes
        if (position + 4 >= DATAPAGESIZE) {
            strncpy(temp, c + position, DATAPAGESIZE - position);
            infile.read(c, DATAPAGESIZE);
            c_size = infile.gcount();
        
            strncpy(temp + DATAPAGESIZE - position, c , position + 4 - DATAPAGESIZE);
            attribute_num = (int&)(*temp);
            position = (position + 4) % DATAPAGESIZE;
        }
        else{
            attribute_num = (int&)(*(c + position));
            position += 4;
        }
        
        
        //cout<<attribute_num<<endl;
        
        int aid[100];
        int offset[100];
        int key_position = 0;
        bool exist_key = false;
        
        memset(aid, 0, sizeof(aid));
        
        //get all the attributes' id
        for (int i=0; i<attribute_num; i++) {
            if (position + 4 >= DATAPAGESIZE) {
                strncpy(temp, c + position, DATAPAGESIZE - position);
                infile.read(c, DATAPAGESIZE);
                c_size = infile.gcount();
                
                strncpy(temp + DATAPAGESIZE - position, c , position + 4 - DATAPAGESIZE);
                aid[i] = (int&)(*temp);
                position = (position + 4) % DATAPAGESIZE;
            }
            else{
                aid[i] = (int&)(*( c + position));
                position += 4;
            }
            
            if(aid[i] == key_id){
                exist_key = true;
                key_position = i;
            }
            
            
        }


        
        //get all the attributes' offset
        
        for (int i=0; i<=attribute_num; i++) {
            if (position + 4 >= DATAPAGESIZE) {
                strncpy(temp, c + position, DATAPAGESIZE - position);
                infile.read(c, DATAPAGESIZE);
                c_size = infile.gcount();
                
                strncpy(temp + DATAPAGESIZE - position, c , position + 4 - 8192);
                offset[i] = (int&)(*temp);
                position = (position + 4) % DATAPAGESIZE;
            }
            else{
                offset[i] = (int&)(*(c + position));
                position += 4;
            }
            
        }
        
        if (!exist_key){
            if (position + offset[attribute_num] >= DATAPAGESIZE) {
                infile.read(c, DATAPAGESIZE);
                c_size = infile.gcount();

                position = (position + offset[attribute_num]) % DATAPAGESIZE;
            }
            else{
                position += offset[attribute_num];
            }
            
            memset(aid, 0, sizeof(aid));
            memset(offset, 0, sizeof(offset));
            continue;
        }

        
        int len = offset[attribute_num];
        char data[1000];
        memset(data, 0, sizeof(data));

        for (int i=0; i<len; i++) {
            data[i] = *(c + position);
            if (position + 1 >= DATAPAGESIZE) {
                infile.read(c, DATAPAGESIZE);
                c_size = infile.gcount();
                
                position = (position + 1) % DATAPAGESIZE;
            }
            else{
                position += 1;
            }
            
        }
        
        char result[1000];
        memset(result, 0, sizeof(result));
        strncpy(result, data + offset[key_position], offset[key_position+1] - offset[key_position]);
        
        if (id_type[key_id] == "bool"){
            
            
            if ((*result) == '1') {
                memset(result, 0, sizeof(result));
                strncpy(result, "true", 4);
            }
            else{
                memset(result, 0, sizeof(result));
                strncpy(result, "false", 5);
            }
        }
        
        if (id_type[key_id] == "int"){
            char number[4];
            memset(number, 0, sizeof(number));
            strncpy(number, result, 4);
            int num = (int&)(*number);
            memset(result, 0, sizeof(result));
            sprintf(result,"%d",num);
            
        }
        
        if (id_type[key_id] == "json_obj") {
            char tem[1000];
            memset(tem, 0, sizeof(tem));
            tem[0] = '{';
            strncpy(tem+1, result, strlen(result));
            tem[strlen(result) + 1] = '}';
            memset(result, 0, sizeof(result));
            strncpy(result, tem, strlen(tem));
        }
        
        if (id_type[key_id] == "arr") {
            char tem[1000];
            memset(tem, 0, sizeof(tem));
            tem[0] = '[';
            strncpy(tem+1, result, strlen(result));
            tem[strlen(result) + 1] = ']';
            memset(result, 0, sizeof(result));
            strncpy(result, tem, strlen(tem));
        }

        if (strcmp(value, result) == 0) {
            isNone = false;
            
            bool head_of_json = true;
 
            cout<<"{";
            for (int i=0; i<attribute_num; i++) {
                if (!head_of_json) {
                    cout<<", ";
                }
                head_of_json = false;
                
                cout<<"\""<<id_attribute[aid[i]]<<"\": ";
                
                if (id_type[aid[i]] == "json_obj") {
                    cout<<"{";
                    //上面的方法取到每个value
                    char output[1000];
                    memset(output, 0, sizeof(output));
                    strncpy(output, data + offset[i], offset[i+1]-offset[i]);
                    cout<<output;
                    cout<<"}";
                }
                else if (id_type[aid[i]] == "bool"){
                    char output[1];
                    memset(output, 0, sizeof(output));
                    strncpy(output, data + offset[i], offset[i+1]-offset[i]);
                    cout<<((*output) == '1' ? "true":"false") ;
                    
                }
                else if (id_type[aid[i]] == "int"){
                    char output[4];
                    memset(output, 0, sizeof(output));
                    strncpy(output, data + offset[i], offset[i+1]-offset[i]);
                    cout<<(int&)(*output);
                }
                else if (id_type[aid[i]] == "text"){
                    cout<<"\"";
                    char output[1000];
                    memset(output, 0, sizeof(output));
                    strncpy(output, data + offset[i], offset[i+1]-offset[i]);
                    //memcpy(output, data + offset[i], offset[i+1]-offset[i]);
                    cout<<output;
                    cout<<"\"";
                }
                else if (id_type[aid[i]] == "arr"){
                    cout<<"[";
                    char output[1000];
                    memset(output, 0, sizeof(output));
                    strncpy(output, data + offset[i], offset[i+1]-offset[i]);
                    cout<<output;
                    cout<<"]";
                }
            }
            cout<<"}"<<endl;
           
        }
    }
    
    if (isNone) {
        cout << "NONE" << endl;
    }
    
    clock_t end_time = clock();
    cout << "Running time is: " << static_cast<double>(end_time-start_time) / CLOCKS_PER_SEC*1000 << "ms" << endl;
    
}

// test case :  find str2=GBRDCMBQGAYTCMBQGEYTAMJR
//              find nested_obj={"num": 4507, "str": "GBRDCMBQGAYTCMBQGEYTAMJR"}
//              find nested_arr=["checking", "of", "acquired"]
//              find nested_arr=["a", "an"]
//              find nested_obj={"num": 6924, "str": "GBRDCMJQGEYTAMBQGAYTCMBQ"}
//              find str1=GBRDCMBQGEYDCMBQGEYDAMBRGE======


int Find::get_key_id(char* key,char* value){
    string key_type;
    if (value[0] == '{') {
        key_type = "json_obj";
    }
    else if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0){
        key_type = "bool";
    }
    else if (value[0]>='0' && value[0]<='9'){
        key_type = "int";
    }
    else if (value[0] == '['){
        key_type = "arr";
    }
    else {
        key_type = "text";
    }
    
    int key_id = -1;
    for (int i = 0; i < id_size ; i++) {
        if (id_attribute[i] == key && id_type[i].c_str() == key_type) {
            key_id = i;
            break;
        }
    }

    return key_id;
}
