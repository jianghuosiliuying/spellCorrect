#include <iostream>
#include <string>
#include <set>
#include <algorithm>

using namespace std;

int main()
{
    string word="agddlgasgakdjjsddsg";
    cout<<word<<endl;
    //word.erase(unique(word.begin(),word.end()),word.end());
    set<char> tmp(word.begin(),word.end());
    string w;
    for(auto & ch:tmp)
    {
        w+=ch;
    }
    cout<<word<<endl;
    cout<<w<<endl;
    return 0;
}

