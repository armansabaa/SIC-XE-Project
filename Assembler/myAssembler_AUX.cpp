#include<bits/stdc++.h>
#pragma region Macros
#define WORD 3
#define ZERO 0
#define ONE 1
#define MEM_SIZE 1<<15
#pragma endregion
#define testCase2
using namespace std;
#ifdef testCase1
    ifstream file1("sicxe_INPUT.txt"), file2("sicxe_INPUT.txt");
    ofstream fout ("Out1.txt");
#endif
#ifdef testCase2
    ifstream file1("sicxe_INPUT.txt"), file2("sicxe_INPUT.txt");
    ofstream fout("Out2.txt");
#endif
map<string, string> SymTab, OpTab,Register;
int i, curLoc;

char hexAux[] ={'A','B','C','D','E','F'};
char decAux[] ={ 10, 11, 12, 13, 14, 15};
void preProcess()
{
    OpTab["ADD"] = "18";
    OpTab["ADDF"] = "58";
    OpTab["ADDR"] = "90";
    OpTab["AND"] = "40";
    OpTab["CLEAR"] = "B4";
    OpTab["COMP"] = "28";
    OpTab["COMPR"] = "A0";
    OpTab["COMPF"] = "88";
    OpTab["DIV"] = "24";
    OpTab["DIVF"] = "64";
    OpTab["DIVR"] = "9C";
    OpTab["FIX"] = "C4";
    OpTab["FLOAT"] = "C0";
    OpTab["HIO"] = "F4";
    OpTab["J"] = "3C";
    OpTab["JEQ"] = "30";
    OpTab["JGT"] = "34";
    OpTab["JLT"] = "38";
    OpTab["LDA"] = "00";
    OpTab["LDB"] = "68";
    OpTab["LDF"] = "70";
    OpTab["LDL"] = "08";
    OpTab["LDT"] = "74";
    OpTab["LDX"] = "04";
    OpTab["LDS"] = "6C";
    OpTab["JSUB"] = "48";
    OpTab["LDCH"] = "50";
    OpTab["LPS"] = "D0";
    OpTab["MUL"] = "20";
    OpTab["MULF"] = "60";
    OpTab["MULR"] = "98";
    OpTab["NORM"] = "C8";
    OpTab["OR"] = "44";
    OpTab["RD"] = "D8";
    OpTab["RMO"] = "AC";
    OpTab["RSUB"] = "4C";
    OpTab["SHIFTL"] = "A4";
    OpTab["SHIFTR"] = "A8";
    OpTab["SIO"] = "F0";
    OpTab["STA"] = "0C";
    OpTab["STB"] = "78";
    OpTab["STCH"] = "54";
    OpTab["STL"] = "14";
    OpTab["STF"] = "80";
    OpTab["STI"] = "D4";
    OpTab["STS"] = "7C";
    OpTab["STT"] = "84";
    OpTab["STSW"] = "E8";
    OpTab["STX"] = "10";
    OpTab["SUB"] = "1C";
    OpTab["SUBF"] = "5C";
    OpTab["SSK"] = "EC";
    OpTab["SUBR"] = "94";
    OpTab["SVC"] = "B0";
    OpTab["TD"] = "E0";
    OpTab["TIO"] = "F8";
    OpTab["TIX"] = "2C";
    OpTab["TIXR"] = "B8";
    OpTab["WD"] = "DC";
    Register["A"]="0";
    Register["X"]="1";
    Register["L"]="2";
    Register["B"]="3";
    Register["S"]="4";
    Register["T"]="5";
    Register["F"]="6";
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
string toUpperCase(string x)
{
    for(int i=0;i<x.size();i++)
        x[i] = toupper(x[i]);
    return x;
}
string decToHex(int number)
{
    char x[10];
    itoa(number,x,16);
    string y(x);
    y=toUpperCase(y);
    return y;
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

string convertIntToHexString(int number)
{
    char x[10];
    itoa(number,x,16);
    string y(x);
    y=toUpperCase(y);
    return y;
}
string convertCharsToAsciiString(string x)
{
    int y=0;
    x=toUpperCase(x);
    string ret,temp;
    char arr[5];
    for(int i=0;i<x.size();i++)
    {
        itoa(x[i],arr,16);
        temp=arr;
        if(temp.size()<2)temp.insert(0,"0");
        ret+=temp;
    }
    return ret;
}
int myPow(int x,int power)
{
    int result=1;
    for(int i=0;i<power;i++)
        result*=x;
    return result;
}

int convertIntStringToInt(string x)
{
    int result=0,c=0;
    for(int i=x.size()-1;i>=0;i--)
    {
        result+=( (x[i]-'0') * myPow(10,c)  );
        c++;
    }
    return result;
}

