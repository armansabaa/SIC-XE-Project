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
    passTwo();//object code have values "RESERVED" and "BASE"
    generateHTEformat();
    //print(objectCode);
    print(HTEformat);
    //cout<<"PROGRAM NAME => "<<programName<<endl;
    //cout<<"START ADDRESS => "<<hexToDec(startAddress)<<"\tHexa = "<<startAddress<<endl;
    //cout<<"PROGRAM SIZE => "<<programSize<<"\tHexa = "<<decToHex(programSize)<<endl;
}
