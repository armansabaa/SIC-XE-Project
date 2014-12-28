//myAssembler.cpp
//11102054
//AbdelRahman ElSabaa
//                                                                    Comments
#include<fstream>
#include<bits/stdc++.h>
#include "sic-xe.cpp"
using namespace std;
string programName, mainName, startAddress;
int programSize;

int main()
{
    preProcess();
    readInput();
    passOne();
    passTwo();
    print(objectCode);
    //cout<<"PROGRAM NAME => "<<programName<<endl;
    //cout<<"START ADDRESS => "<<hexToDec(startAddress)<<"\tHexa = "<<startAddress<<endl;
    //cout<<"PROGRAM SIZE => "<<programSize<<"\tHexa = "<<decToHex(programSize)<<endl;
}
