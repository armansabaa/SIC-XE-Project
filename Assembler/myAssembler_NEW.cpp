//myAssembler.cpp
//11102054
//AbdelRahman ElSabaa
//                                                                    Comments
#include<fstream>
#include<bits/stdc++.h>
#include "myAssembler_AUX.cpp"
using namespace std;
string programName, mainName, startAddress;
int programSize;
void Pass_1()
{
    Instruction inst;
    bool firstline = 1;
    curLoc = 0;                                                           //curloc is always holding a decimal value for the address.
    while(true){
        inst = scanInstruction();
        if(inst.tOpcode == "END")
        {
            mainName = inst.tOprnd;
            programSize = curLoc - hexToDec(startAddress);
            break;
        }
        if(inst.tLabel[0]=='.')                                         //Validating comment line.
            continue;
        //printInstruction(inst);
        if(inst.tOpcode == "START")
        {
            if(firstline){                                               //Assumed start is always in the first line.
                startAddress = inst.tOprnd;
                curLoc = hexToDec(startAddress);
                programName = inst.tLabel;
            }
            else
                printf("ERROR!!\nFirst line doesnt contain START.\n");  //ERROR
        }
        if(inst.tLabel != "" && !firstline)
        {
            if(SymTab.count(inst.tLabel))                               //If Label exists.
                printf("ERROR!\nSame label is being used.\n");          //ERROR!!
            else
                SymTab[inst.tLabel] = decToHex(curLoc);                 //Insert Label.
        }
        if(OpTab.count(inst.tOpcode))                                   //If Instruction exists.
            curLoc+=3;
        else if(inst.tOpcode == "WORD")                                 //Checking Directives.
            curLoc+=3;
        else if(inst.tOpcode == "RESW")
            curLoc+=(inst.iOprnd)*3;
        else if(inst.tOpcode == "RESB")
            curLoc+=inst.iOprnd;
        else if(inst.tOpcode == "BYTE"){
            if(inst.tOprnd[0]=='X')
            {
//  %
            }
            curLoc+=sizeBytes(inst.tOprnd);
            if(inst.tOprnd[0]=='C')
            {
 // %
            }
        }
        else if (firstline)
            firstline=0;
        else if(inst.tOpcode != "END"){
            printInstruction(inst);
            printf("EORROR..................................\n");
        }
    }
}
void Pass_2()
{
    bool fg=0;
    int cnt=0;
    Instruction inst;
    string omKalthoum;
    curLoc = hexToDec(startAddress);
    while(true)
    {
        inst = scanInstruction_();
        printInstruction(inst);
        //cout<<decToHex(curLoc)<<"  "<<omKalthoum.size()<<endl;
        if(inst.tOpcode == "RESW" || inst.tOpcode == "RESB")
        {
            fg=1;
            if(inst.tOpcode == "RESW")
                curLoc+=(inst.iOprnd)*3;
            else if(inst.tOpcode == "RESB")
                curLoc+=inst.iOprnd;
        }
        else if(fg || cnt==10)                                          //Either Resw/Resb or 10 continuous inst
        {
            fg=0;
            cnt=0;
            for(i=0;i<9;i++)
                fout<<omKalthoum[i];
            fout<<oneByte(decToHex((omKalthoum.size()-9)/2))<<" ";
            for(i=9;i<omKalthoum.size();i++)
                fout<<omKalthoum[i];
            fout<<endl;
            omKalthoum = "T "+ threeBytes(decToHex(curLoc))+" ";
        }
        if(inst.tOpcode == "END")
        {
            for(i=0;i<9;i++)
                fout<<omKalthoum[i];
            fout<<oneByte(decToHex((omKalthoum.size()-9)/2))<<" ";
            for(i=9;i<omKalthoum.size();i++)
                fout<<omKalthoum[i];
            fout<<endl;
            mainName = inst.tOprnd;
            programSize = curLoc - hexToDec(startAddress);
            fout<<"E 00"<<startAddress<<endl;
            break;
        }
        else if(inst.tOpcode == "START")
        {
            //cout<<"**"<<decToHex(curLoc)<<"**"<<threeBytes(decToHex(curLoc))<<'I'<<endl;
            omKalthoum = "T "+ threeBytes(decToHex(curLoc))+" ";
            fout<<"H "<<programName<<" "<<threeBytes(startAddress)<<" "<<threeBytes(decToHex(programSize))<<endl;
            curLoc = hexToDec(startAddress);
        }
        else if(inst.tOpcode == "WORD")
        {
            omKalthoum+=threeBytes(decToHex(inst.iOprnd));
            curLoc+=3;
            cnt++;
        }
        else if(inst.tOpcode == "BYTE")
        {
            if(inst.tOprnd[0] == 'X'){
                omKalthoum+=inst.tOprnd[2];
                omKalthoum+=inst.tOprnd[3];
            }
            else for(i=0;i<sizeBytes(inst.tOprnd);i++)
            {
                omKalthoum+=decToHex(inst.tOprnd[i+2]);
            }
            curLoc+=sizeBytes(inst.tOprnd);
            cnt++;
        }
        else if(inst.tOpcode == "RSUB"){
            omKalthoum+="4C0000";
            curLoc+=3;
            cnt++;
        }
        else if(inst.tOprnd[inst.tOprnd.size()-1] == 'X' && inst.tOprnd[inst.tOprnd.size()-2] == ',')   //Indexed
        {
            //cout<<"*"<<SymTab[inst.tOprnd.substr(0,inst.tOprnd.size()-2)]<<inst.tOprnd.substr(0,inst.tOprnd.size()-2)<<endl;
            omKalthoum+=OpTab[inst.tOpcode] + raiseX(SymTab[inst.tOprnd.substr(0,inst.tOprnd.size()-2)]);
            curLoc+=3;
            cnt++;
        }
        else if(!fg)
        {
            omKalthoum+=OpTab[inst.tOpcode]+twoBytes(SymTab[inst.tOprnd]);
            curLoc+=3;
            cnt++;
        }
    }
}
int main()
{
    preProcess();
    Pass_1();
    cout<<"PROGRAM NAME => "<<programName<<endl;
    cout<<"START ADDRESS => "<<hexToDec(startAddress)<<"\tHexa = "<<startAddress<<endl;
    cout<<"PROGRAM SIZE => "<<programSize<<"\tHexa = "<<decToHex(programSize)<<endl;
    Pass_2();
}
