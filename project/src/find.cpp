#include "find.h"

#define NUM_OF_KEYS 25
string keys[26]={"","nested_obj","dyn2","dyn1","nested_arr","str2",
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
    
}

int Find::get_key_id(char* key){
    int key_id = -1;
    //string _key = key;
    for (int i = 1; i < NUM_OF_KEYS + 1 ; i++) {
        if (keys[i] == key) {
            key_id = i;
            break;
        }
    }
    return key_id;
}

