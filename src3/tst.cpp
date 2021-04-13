#include <iostream>
#include <unordered_map>
#include <cstdint>
#include "enums.h"

using namespace std;

struct data {
    ptypes type;
    union  {
        std::uint8_t a;
        std::uint32_t b;
        char c[32];
    };
};


int main() {
    // Declaring umap to be of <string, int> type
    // key will be of string type and mapped value will
    // be of int type
    unordered_map<string, data> umap;

    data fred={ INVALID };
    fred.type = BOOL;

    //    fred.bValue = true;
    //    fred.value = "Hello";

    // inserting values by using [] operator
    umap["GeeksforGeeks"] = fred ;
    fred.type = BYTE;
    umap["JOE"] = fred ;

    std::cout << to_string(fred.type) << "\n" ;
//    std::cout << to_string((umap["GeeksforGeeks"]).type) << "\n" ;
    std::cout << (umap["GeeksforGeeks"]).type << "\n" ;

    umap["GeeksforGeeks"].type = STRING;
    std::cout << to_string((umap["GeeksforGeeks"]).type) << "\n" ;

    std::cout << "JOE " + to_string((umap["JOE"]).type) << "\n" ;

    //
    /*
       umap["Practice"] = 20;
       umap["Contribute"] = 30;

    // Traversing an unordered map
    for (auto x : umap)
    cout << x.first << " " << x.second << endl;
    */
}
