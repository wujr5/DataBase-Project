#include "find.h"
#include <iostream>
#include <fstream>

#define NUM_OF_KEYS 25
string keys[25]={"nested_obj","dyn2","dyn1","nested_arr","str2",
    "str1","sparse_079","thousandth","sparse_078","num",
    "bool","sparse_072","sparse_073","sparse_070","sparse_071",
    "sparse_076","sparse_077","sparse_074","sparse_075","sparse_681",
    "sparse_683","sparse_682","sparse_685","sparse_689","sparse_686"};

void Find::execute(char* key, char* value) {
    cout << "find command execute!" << endl;
    cout << "key = " << key << endl;
    cout<< "value = " << value << endl;
    
    cout << "-------RESULT AS FOLLOWING-------"<<endl;
    
    int key_id = get_key_id(key);
    cout <<key_id<<endl;
    if(key_id == -1){             //cannot find the key
        cout<<"None"<<endl;
        return;
    }
    
    ifstream infile("binary_data.data",ios::binary);
    char c[8192];
    
    
    infile.read(c, 8192);
    int position = 0;
    int attribute_num=-1;
    
    char temp[4];
    //used for save the number when the space at end of page is not reached 8KB.
    
    while(!infile.eof()){
        //get the number of attributes
        if (position + 4 >= 8192) {
            strncpy(temp, c + position, 8192 - position);
            infile.read(c, 8192);
            strncpy(temp + 8192 - position, c , position + 4 - 8192);
            attribute_num = (int&)(*temp);
            position = (position + 4) % 8192;
        }
        else{
            attribute_num = (int&)(*(c + position));
            position += 4;
        }
        
        
        cout<<attribute_num<<endl;
        
        int aid[100];
        int offset[100];
        
        //get all the attributes' id
        for (int i=0; i<attribute_num; i++) {
            if (position + 4 >= 8192) {
                strncpy(temp, c + position, 8192 - position);
                infile.read(c, 8192);
                strncpy(temp + 8192 - position, c , position + 4 - 8192);
                aid[i] = (int&)(*temp);
                position = (position + 4) % 8192;
            }
            else{
                aid[i] = (int&)(*( c + position));
                position += 4;
            }
            
        }
        
//        for (int i=0; i<attribute_num; i++) {
//            cout<<"key"<<i<<" = "<<aid[i]<<endl;
//        }
//        cout<<"****"<<endl;
        
        //get all the attributes' offset
        
        for (int i=0; i<=attribute_num; i++) {
            if (position + 4 >= 8192) {
                strncpy(temp, c + position, 8192 - position);
                infile.read(c, 8192);
                strncpy(temp + 8192 - position, c , position + 4 - 8192);
                offset[i] = (int&)(*temp);
                position = (position + 4) % 8192;
            }
            else{
                offset[i] = (int&)(*(c + position));
                position += 4;
            }
            
        }
        
//        for (int i=0; i<attribute_num; i++) {
//            cout<<"offset"<<i<<" = "<<offset[i]<<endl;
//        }
        
        cout<<"len = "<<offset[attribute_num]<<endl;  //最后一个，也就是len
        
        int len = offset[attribute_num];
        char data[1000];
        memset(data, 0, sizeof(data));
        //注意！！振文里面的len是距离文件头的！！！！！！！！！
        //注意！！振文里面的len是距离文件头的！！！！！！！！！
        //注意！！振文里面的len是距离文件头的！！！！！！！！！
        //注意！！振文里面的len是距离文件头的！！！！！！！！！
        for (int i=0; i<len-offset[0]; i++) {
            data[i] = *(c + position);
            if (position + 1 >= 8192) {
                infile.read(c, 8192);
                position = (position + 1) % 8192;
            }
            else{
                position += 1;
            }
            
            
        }
        
        cout<<"*********"<<endl;
        //cout<<data<<endl;
        
        char result[200];
        memset(result, 0, sizeof(result));
        strncpy(result, data + offset[key_id] - offset[0], offset[key_id+1] - offset[key_id]);
        cout<<result<<endl;
        
        /*
         
         到这里的功能是：
         
            实现find a=b，找到所有键为a的值，依次打印出来
         
            对于与value不相等的值输出了，因为振文的binary里面有引号
         
         后面的功能：
         
            1. value和result是否相等？ 以及，判断其类型
         
            2. 输出匹配的整条JSON 还要对每个key判断类型以输出其value
         
            3. 目前代码很乱还没有封装之类的
         
         */
        
 
    }

    
}

// test case : find str2=GBRDCMBQGAYTCMBQGEYTAMJR

int Find::get_key_id(char* key){
    int key_id = -1;
    //string _key = key;
    for (int i = 0; i < NUM_OF_KEYS ; i++) {
        if (keys[i] == key) {
            key_id = i;
            break;
        }
    }
    return key_id;
}

void Find::read_data_to_c(ifstream infile, char* c){
    // ???
}