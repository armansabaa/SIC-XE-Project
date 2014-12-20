#include<bits/stdc++.h>
#pragma region Macros
#define WORD 3
#define ZERO 0
#define ONE 1
#define MEM_SIZE 1<<15
#pragma endregion
#define testCase2
using namespace std;
void hamedFunction()
{
    printf("hamed function");

}
#ifdef testCase1
    ifstream file1("TC1.txt"), file2("TC1_.txt");
    ofstream fout ("Out1.txt");
#endif
#ifdef testCase2
    ifstream file1("TC2.txt"), file2("TC2_.txt");
    ofstream fout("Out2.txt");
#endif
map<string, string> SymTab, OpTab;
int i, curLoc;

char hexAux[] ={'A','B','C','D','E','F'};
char decAux[] ={ 10, 11, 12, 13, 14, 15};
void preProcess()
{
    OpTab["ADD"] = "18";
    OpTab["AND"] = "40";
    OpTab["COMP"] = "28";
    OpTab["DIV"] = "24";
    OpTab["J"] = "3C";
    OpTab["JEQ"] = "30";
    OpTab["JGT"] = "34";
    OpTab["JLT"] = "38";
    OpTab["LDA"] = "00";
    OpTab["JSUB"] = "48";
    OpTab["LDCH"] = "50";
    OpTab["LDL"] = "08";
    OpTab["LDX"] = "04";
    OpTab["MUL"] = "20";
    OpTab["OR"] = "44";
    OpTab["RD"] = "D8";
    OpTab["RSUB"] = "4C";
    OpTab["STA"] = "0C";
    OpTab["STCH"] = "54";
    OpTab["STL"] = "14";
    OpTab["STSW"] = "E8";
    OpTab["STX"] = "10";
    OpTab["SUB"] = "1C";
    OpTab["TD"] = "E0";
    OpTab["TIX"] = "2C";
    OpTab["WD"] = "DC";
}
struct Instruction
{
    string tLabel, tOprnd, tOpcode;
    int iOprnd;
    Instruction (){}
    Instruction (string tLabel, string tOpcode, string tOprnd, int iOprnd)
    {
        this->tLabel = tLabel;
        this->tOpcode = tOpcode;
        this->tOprnd = tOprnd;
        this->iOprnd = iOprnd;
    }
};
string _toUpper(string inp)                                          //Rises all string's characters to upercase ones
{
    for(i=0;i<inp.size();i++)
    {
        if(inp[i]<='z' && inp[i]>='a')
        {
            inp[i] = inp[i]-'a'+'A';
        }
    }
    return inp;
}
int stringToInt(string s)
{
    int val=0;
    for(i=0;i<s.size();i++)
    {
        val*=10;
        val+=s[i]-'0';
    }
    return val;
}
string scanAux(string line, int *placeHolder)                        //Auxilary scanning function
{
    string temp, ret ="";
    if(line[*placeHolder]==' ')
    {
        *(placeHolder)+=1;
        return ret;
    }
    for(i=*placeHolder;i<line.size()&&line[i]!=' ';i++)
    {
        ret+=line[i];
    }
    *placeHolder = i+1;                                              //Assume that there is only one space between two substrings.
    ret = _toUpper(ret);
    return ret;
}
Instruction scanInstruction()                                        //(|)Main scanning function
{
    int iOprnd, ph=0;
    string line="", tLabel="", tOpcode="", tOprnd="";
    getline(file1, line);
    tLabel = scanAux(line, &ph);
    tOpcode = scanAux(line, &ph);
    tOprnd = scanAux(line, &ph);
    iOprnd = stringToInt(tOprnd);
    return Instruction (tLabel, tOpcode, tOprnd, iOprnd);
}
Instruction scanInstruction_()                                       //(||) Main scanning function
{
    int iOprnd, ph=0;
    string line="", tLabel="", tOpcode="", tOprnd="";
    getline(file2, line);
    tLabel = scanAux(line, &ph);
    tOpcode = scanAux(line, &ph);
    tOprnd = scanAux(line, &ph);
    iOprnd = stringToInt(tOprnd);
    return Instruction (tLabel, tOpcode, tOprnd, iOprnd);
}
int sizeBytes(string x)                                              //Returns the number of bytes accused by the number
{
    if(x[0] == 'C')
        return x.size() - 3 ;
    if(x[0] == 'X')
        return ((x.size()-3)/2)+((x.size()-3)%2);
    else
        printf("EORROR!!!");
}
string decToHex(int x)
{
    int temp;
    string ret="";
    while(x)
    {
        temp = x%16;
        if(temp>9)
            ret+=hexAux[temp-10];
        else
            ret+=temp+'0';
        x/=16;
    }
    reverse(ret.begin(),ret.end());
    return ret;
}
void printInstruction (Instruction inst)
{
    cout<<"Label = "<<inst.tLabel<<"\tOpcode = "<<inst.tOpcode<<"\tOperand ="<<inst.tOprnd<<"\t"<<decToHex(curLoc);
    if(inst.tOpcode == "RESW" || inst.tOpcode == "RESB")
        cout<<"   "<<inst.iOprnd;
    cout<<endl;
}
int hexToDec(string x)
{
    int ret=0;
    for(i=0;i<x.size();i++)
    {
        ret*=16;
        if(x[i]>='A' && x[i]<='F')
            ret+=decAux[x[i]-'A'];
        else
            ret+=x[i]-'0';
    }
    return ret;
}
string threeBytes(string x)
{
    string temp="";
    for(i=0;i<6-x.size();i++)
        temp+="0";
    for(i=0;i<x.size();i++)
    {
        temp+=x[i];
    }
    return temp;
}
string twoBytes(string x)
{
    string temp="";
    for(i=0;i<4-x.size();i++)
        temp+="0";
    for(i=0;i<x.size();i++)
    {
        temp+=x[i];
    }
    return temp;
}
string oneByte(string x)
{
    if(x.size() == 1)
    {
        return '0'+x;
    }
    return x;
}
string raiseX(string x)
{
    int tempo = hexToDec(x);
    tempo |= 1<<15;
    x = decToHex(tempo);
    return x;
}
