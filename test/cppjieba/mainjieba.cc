#include "WordCnSegmentation.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace mm;

int main()
{
    string str="上善若水，厚德载物。";
    WordCnSegmentation wordSeg;
    vector<string> result=wordSeg(str);
    cout<<"分词结果："<<endl;
    for(auto & re:result)
    {
        cout<<re.size()<<" "<<re<<" "<<endl;
    }
    cout<<endl;
    return 0;
}

