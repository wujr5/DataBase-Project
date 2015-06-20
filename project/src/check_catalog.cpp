#include "check_catalog.h"
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

void CheckCatalog::execute() {
    cout << "check catalog execute!" << endl;
    ifstream catalog("catalog.data");
    string temp;
    
    cout<<"_id "<<"Key_name "<<"Key_Type "<<"count"<<endl;
    while (getline(catalog,temp)){
        cout<<temp<<endl;
    }
    
    
}