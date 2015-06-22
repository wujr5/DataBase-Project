#include "find.h"
#include "insert.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

extern string id_attribute[100000];
extern string id_type[100000];
extern int id_count[100000];
extern int id_size;

#define DATAPAGESIZE 8192

bool isNone = true;


void Find::execute(char* key, char* value) {
    
    isNone = true;
    
    cout << "find command execute!" << endl;
    cout << "key = " << key << endl;
    cout << "value = " << value << endl;
    
    cout << "-------RESULT AS FOLLOWING-------"<<endl;
    
    int key_id = get_key_id(key);
    //cout <<key_id<<endl;
    if(key_id == -1){             //cannot find the key
        cout<<"None"<<endl;
        return;
    }
    
    ifstream infile("binary_data.data",ios::binary);
    char c[DATAPAGESIZE];
    
    
    infile.read(c, DATAPAGESIZE);
    int position = 0;
    int attribute_num=-1;
    
    char temp[4];
    //used for save the number when the space at end of page is not reached 8KB.
    
    while(!infile.eof()){
        //get the number of attributes
        if (position + 4 >= DATAPAGESIZE) {
            strncpy(temp, c + position, DATAPAGESIZE - position);
            infile.read(c, DATAPAGESIZE);
            strncpy(temp + DATAPAGESIZE - position, c , position + 4 - 8192);
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

//        for (int i=0; i<attribute_num; i++) {
//            cout<<"key"<<i<<" = "<<aid[i]<<endl;
//        }
//        cout<<"****"<<endl;
        
        //get all the attributes' offset
        
        for (int i=0; i<=attribute_num; i++) {
            if (position + 4 >= DATAPAGESIZE) {
                strncpy(temp, c + position, DATAPAGESIZE - position);
                infile.read(c, DATAPAGESIZE);
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
                position = (position + offset[attribute_num]) % DATAPAGESIZE;
            }
            else{
                position += offset[attribute_num];
            }
            continue;
        }
        
        
    
        
//        for (int i=0; i<attribute_num; i++) {
//            cout<<"offset"<<i<<" = "<<offset[i]<<endl;
//        }
        
//        cout<<"len = "<<offset[attribute_num]<<endl;  //最后一个，也就是len
        
        int len = offset[attribute_num];
        char data[1000];
        memset(data, 0, sizeof(data));

        for (int i=0; i<len; i++) {
            data[i] = *(c + position);
            if (position + 1 >= DATAPAGESIZE) {
                infile.read(c, DATAPAGESIZE);
                position = (position + 1) % DATAPAGESIZE;
            }
            else{
                position += 1;
            }
            
        }
        
        char result[200];
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
                    char output[200];
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
                    char output[200];
                    memset(output, 0, sizeof(output));
                    strncpy(output, data + offset[i], offset[i+1]-offset[i]);
                    cout<<output;
                    cout<<"\"";
                }
                else if (id_type[aid[i]] == "arr"){
                    cout<<"[";
                    char output[200];
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

    
}

// test case : find str2=GBRDCMBQGAYTCMBQGEYTAMJR

int Find::get_key_id(char* key){
    int key_id = -1;
    //string _key = key;
    for (int i = 0; i < id_size ; i++) {
        if (id_attribute[i] == key) {
            key_id = i;
            break;
        }
    }
    return key_id;
}
