#include<algorithm>
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<limits.h>
#include<sstream>
#include<stdio.h>
#include<ctype.h>
#include<math.h>
#include<vector>
#include<string>
#include<queue>
#include<stack>
#include<map>
#include<set>
#include "myAssembler_AUX.cpp"

using namespace std;

vector<Instruction> programCode;
vector<string> objectCode;
vector<string> HTEformat;
vector<string> modifications;
string BASE,programLength;

void readInput()
{
    Instruction inst;
    while(true){
        inst = scanInstruction();
     //   cout << inst.tLabel << " " << inst.tOpcode << " " << inst.tOprnd << endl;
        if(inst.tOpcode == "END")
            break;
        programCode.push_back(inst);
    }
}

int getFormat(Instruction inst){
    if(inst.tOpcode[0]=='.')
        return 0;
    if(inst.tOpcode[0]=='+')
        return 4;
    if(inst.tOpcode[inst.tOpcode.size()-1]=='R' || inst.tOpcode=="SHIFTL" ||inst.tOpcode=="RMO")
        return 2;
    if(inst.tOpcode=="RSUB" ||
       inst.tOpcode=="FIX" ||
       inst.tOpcode=="FIO" ||
       inst.tOpcode=="FLOAT" ||
       inst.tOpcode=="HIO" ||
       inst.tOpcode=="SIO" ||
       inst.tOpcode=="TIO" )
        return 1;
    if(inst.tOpcode == "WORD" ||
       inst.tOpcode == "RESW" ||
       inst.tOpcode == "RESB" ||
       inst.tOpcode == "BYTE"
       )
        return -1;
    return 3;
}

int curLocValue()
{

}

void passOne()
{
    int i,format,curLoc = programCode[0].iOprnd;
    string temp;
    Instruction inst;
    for(i=1;i<programCode.size();i++)
    {
        programCode[i].loc=curLoc;
        //cout << curLoc << " " <<programCode[i].loc<< endl;
        inst=programCode[i];
        format=getFormat(inst);
       // cout << inst.tLabel<<endl;
        if(format==0)
            continue;
        if(inst.tOpcode=="BASE")
        {
            BASE=inst.tOprnd;
            continue;
        }
        if(inst.tLabel != "")
        {
            if(!SymTab.count(inst.tLabel)){
                temp=decToHex(curLoc);
                while(temp.size()<3)temp.insert(0,"0");
                SymTab[inst.tLabel] = temp;
            }
        }
        if(format==4 || format==3 || format==2)
            curLoc+=format;
        else if(format==1)
            curLoc+=3;
        else if(format==-1){
            if(inst.tOpcode=="WORD")
                curLoc+=3;
            else if(inst.tOpcode=="RESW")
                curLoc+=(inst.iOprnd)*3 ;
            else if(inst.tOpcode == "RESB")
                curLoc+=inst.iOprnd;
            else if(inst.tOpcode == "BYTE")
                curLoc+=sizeBytes(inst.tOprnd);
        }
    }
    programLength=decToHex(curLoc-programCode[0].iOprnd);
    programLength=six_places(programLength);
}

bool isImmediate(string x){
    return x[0]=='#';
}
bool isIndirect(string x){
    return x[0]=='@';
}
bool isIndexed(string x){
    return (x[x.size()-2]==',' && x[x.size()-1]=='X');
}

string makeFormat3(Instruction inst,int curLoc){
    int middle=0,disp=-curLoc,opcode=hexToDec(OpTab[inst.tOpcode]);
    string __final,xx;
    bool in=0,im=0,b=0,pc=1;
    char temp[100];
    if(isImmediate(inst.tOprnd)){
        im=1;
        opcode+=1;
        inst.tOprnd=inst.tOprnd.substr(1,inst.tOprnd.size());
        if(SymTab.count(inst.tOprnd)){
            disp+=hexToDec(SymTab[inst.tOprnd]);
            if(disp<-2048 || disp>2047){
                disp+=curLoc;
                disp-=hexToDec(SymTab[BASE]);
                b=1;pc=0;
            }
        }
        else{
            disp=convertIntStringToInt(inst.tOprnd);
            pc=0;
        }
    }
    else if(isIndirect(inst.tOprnd)){
        in=1;
        opcode+=2;
        inst.tOprnd=inst.tOprnd.substr(1,inst.tOprnd.size());
        disp+=hexToDec(SymTab[inst.tOprnd]);
        if(disp<-2048 || disp>2047){
            disp+=curLoc;
            disp-=hexToDec(SymTab[BASE]);
            b=1;pc=0;
        }
    }
    else
        opcode+=3;

    if(isIndexed(inst.tOprnd)){
        middle=8;
        inst.tOprnd=inst.tOprnd.substr(0,inst.tOprnd.size()-2);
        disp+=hexToDec(SymTab[inst.tOprnd]);
        if(disp<-2048 || disp>2047){
            disp+=curLoc;
            disp-=hexToDec(SymTab[BASE]);
            b=1;pc=0;
        }
    }
    else
        if(!in  &&  !im){
            disp+=hexToDec(SymTab[inst.tOprnd]);
            if(disp<-2048 || disp>2047){
                disp+=curLoc;
                disp-=hexToDec(SymTab[BASE]);
                pc=0;b=1;
            }
        }

    if(pc)middle+=2;
    if(b)middle+=4;
    __final=decToHex(opcode);
    __final+=decToHex(middle);
    itoa(disp,temp,16);
    xx=temp;while(xx.size()<3)xx.insert(0,"0");
    __final+=xx.substr(xx.size()-3,xx.size());
    return toUpperCase(six_places(__final));
}

string makeFormat4(Instruction inst){
    inst.tOpcode=inst.tOpcode.substr(1, inst.tOpcode.size());
    int middle=1,opcode=hexToDec(OpTab[inst.tOpcode]);
    string __final,disp;
    bool in=0,im=0,im_value=0;
    if(isImmediate(inst.tOprnd)){
        im=1;
        opcode+=1;
        inst.tOprnd=inst.tOprnd.substr(1,inst.tOprnd.size());
        if(SymTab.count(inst.tOprnd))
            disp=SymTab[inst.tOprnd];
        else{
            disp=convertIntToHexString(convertIntStringToInt(inst.tOprnd));
            im_value=1;
        }
    }
    else if(isIndirect(inst.tOprnd)){
        in=1;
        opcode+=2;
        inst.tOprnd=inst.tOprnd.substr(1,inst.tOprnd.size());
        disp=SymTab[inst.tOprnd];
    }
    else
        opcode+=3;

    if(isIndexed(inst.tOprnd)){
        middle=9;
        inst.tOprnd=inst.tOprnd.substr(0,inst.tOprnd.size()-2);
        disp=SymTab[inst.tOprnd];
    }
    else
        if(!in && !im)
            disp=SymTab[inst.tOprnd];

    //adding modification record
    if(!im_value)
        modifications.push_back(six_places(decToHex(inst.loc+1)));

    __final=decToHex(opcode);
    __final+=decToHex(middle);
    while(disp.size()<5)disp.insert(0,"0");
    __final+=disp;
    while(__final.size()<8)__final.insert(0,"0"); //kanet ma3moola 6
    return toUpperCase(__final);
}

void passTwo(){
    Instruction inst;
    int z,curLoc = programCode[0].iOprnd,m;
    string temp,charr,n;
    pair<string,int> x;
    for(int i=1;i<programCode.size();i++)
    {
        temp="";
        inst=programCode[i];
        if(inst.tOpcode=="BASE"){
            temp="BASE";
        }
        else{
        switch (getFormat(inst)){
            case 0 :temp="COMMENT";
                    break;
            case 1 :curLoc+=3;
                    temp=decToHex(hexToDec(OpTab[inst.tOpcode])+3);
                    temp+="0000";
                    break;

            case 2 :curLoc+=2;
                    temp=OpTab[inst.tOpcode];
                    if(inst.tOprnd.size()<2)
                        inst.tOprnd+=",A";
                    charr=inst.tOprnd[0];
                    temp+=Register[charr];
                    charr=inst.tOprnd[2];
                    temp+=Register[charr];
                    break;

            case 3 :curLoc+=3;
                    temp=makeFormat3(inst,curLoc);
                    break;

            case 4 :curLoc+=4;
                    temp=makeFormat4(inst);
                    break;

            case -1:if(inst.tOpcode=="WORD"){
                        curLoc+=3;
                        temp=convertIntToHexString(inst.iOprnd);
                        temp=six_places(temp);
                    }
                    if(inst.tOpcode=="BYTE"){
                        if(inst.tOprnd[0]=='C')
                            temp=convertCharsToAsciiString(inst.tOprnd.substr(2,inst.tOprnd.size()-3));
                        else
                            temp=inst.tOprnd.substr(2,inst.tOprnd.size()-3);
                        curLoc+=sizeBytes(inst.tOprnd);

                    }
                    if(inst.tOpcode == "RESB"){
                        curLoc+=inst.iOprnd;
                        temp="RESERVED";
                    }
                    if(inst.tOpcode=="RESW"){
                        curLoc+=(inst.iOprnd)*3;
                        temp="RESERVED";
                    }
                    break;
            }
        }
        if(temp.size())objectCode.push_back(temp);
    }
}

/*void printSymbol(map<string,string> stable)
{
    map<string,string>::iterator it=stable.begin();
    for(it=stable.begin();it!=stable.end();++it)
        cout<< it->first << "-->" << it->second << endl;

}*/
void print(vector<string> x){
    for(int i=0;i<x.size();i++)
        cout << x[i]<< endl;
}


void addHpart()
{
    string temp,progName;
    temp+="H.";
    progName=programCode[0].tLabel;
    while(progName.size()<6)progName+=' ';
    temp+=progName+".";//program name
    temp+="00"+programCode[0].tOprnd+'.';//starting address;
    temp+=programLength;
    HTEformat.push_back(temp);
}

void addEpart()
{
    string temp="E.00"+programCode[0].tOprnd;
    HTEformat.push_back(temp);
}

void addMpart(){
    string temp;
    for(int i=0;i<modifications.size();i++){
        temp="M."+modifications[i]+".05."+"+"+programCode[0].tLabel;
        HTEformat.push_back(temp);
    }
}

void addTpart(){
    string cur;
    int counter=0;
    bool newRecord=0;
    Instruction inst;
    string temp="T."+six_places(programCode[0].tOprnd),record,siz;
    for(int i=0;i<objectCode.size();i++){
        cur=objectCode[i];
        inst=programCode[i+1];
        if(getFormat(inst)==0 || inst.tOpcode=="BASE"){
            continue;
        }
        if(inst.tOpcode=="RESW" || inst.tOpcode=="RESB")
        {
            newRecord=1;
            continue;
        }
        if(counter+getFormat(inst)>30)
            newRecord=1;
        if(newRecord){
            siz=two_places(decToHex(counter));
            counter=0;
            temp+="."+siz+record;
            HTEformat.push_back(temp);
            newRecord=0;
            record.clear();
            temp="T."+six_places(decToHex(inst.loc));
        }
        record+="."+cur;

        if(getFormat(inst)==4 || getFormat(inst)==3 || getFormat(inst)==2)
            counter+=getFormat(inst);
        else if(getFormat(inst)==1)
            counter+=3;
        else if(getFormat(inst)==-1){
            if(inst.tOpcode=="WORD")
                counter+=3;
            else if(inst.tOpcode == "BYTE")
                counter+=sizeBytes(inst.tOprnd);
        }
    }
    if(record.size()!=0)
    {
        siz=two_places(decToHex(counter));
        counter=0;
        temp+="."+siz+record;
        HTEformat.push_back(temp);
    }
}

void generateHTEformat(){
    addHpart();
    addTpart();
    addMpart();
    addEpart();
}

vector<Instruction> getInstructions(){
    return programCode;
}

vector<string> getObjectCode(){
    return objectCode;
}

vector<string> getHTERecord(){
    return HTEformat;
}

/*int main()
{
    preProcess();
    readInput();
    //for(i=0;i<programCode.size();i++)
      //  printInstruction(programCode[i]);
    passOne();
    passTwo();
    generateHTEformat();
   // printSymbol(SymTab);
    //print(objectCode);
    print(HTEformat);
    //vector<Instruction> getInstructions();
    //vector<string> getObjectCode();
    //vector<string> getHTERecord();
	return 0;
}*/

