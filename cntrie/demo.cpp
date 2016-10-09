#include "cntrie.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main(int argc, char** argv){
    string tname = "test";
    cntrie tr(tname);
    string in_file = "test.txt";
    ifstream ifs(in_file.c_str(), ios::in|ios::binary);
    if(!ifs.is_open()) return 0;
    string line;
    while(getline(ifs, line)){
        long cn = tr.insert(line, tname);
        //cout << cn  << "insert " << line <<endl;
        //
        //tr.print();
    }
    ifs.close();
    in_file = "test1.txt";
    tname = "new";
    ifstream ifsn(in_file.c_str());
    while(getline(ifsn, line)){
        long cn = tr.insert(line, tname);
        //cout << cn  << "insert " << line <<endl;
        //tr.print();
    }
    ifsn.close();
    vector<pair<string, long> > vec;
    tr.walk( vec, tname);
    for(auto &it:vec){
        cout << it.first << ":" << it.second << endl;
    }
    return 0;
}
