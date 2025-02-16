#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> Split(string InputString){
    vector<string> Key_Operation;
    string Key;
    string Operation;
    int flag=0;
    for (int i=0; i<InputString.length(); i++){
        if (InputString[i]!=' '){
            if (flag==0){
                Key = Key + InputString[i];
            }
            else{
                Operation = Operation + InputString[i];
            }
        }
        else{
            flag=1;
        }
    }
    Key_Operation.push_back(Key);
    Key_Operation.push_back(Operation);
    return Key_Operation;
}

string Encrypt(string InputString, int Key){
    string OutputString;
    for (char c : InputString){
        int AsciiCode = static_cast<int>(c);
        int AsciiCode2;
        if (AsciiCode==32){
            AsciiCode2=32;
        }
        else if ((AsciiCode+Key>90 && AsciiCode<97) || (AsciiCode+Key>122)){
            AsciiCode2=AsciiCode+Key-26;
        }
        else{
            AsciiCode2=AsciiCode+Key;
        }
        char Character = static_cast<char>(AsciiCode2);
        OutputString+=Character;
    }
    return OutputString;
}

string Decrypt(string InputString, int Key){
    string OutputString;
    for (char c : InputString){
        int AsciiCode = static_cast<int>(c);
        int AsciiCode2;
        if (AsciiCode==32){
            AsciiCode2=32;
        }
        else if ((AsciiCode-Key<97 && AsciiCode>90) || (AsciiCode-Key<65)){
            AsciiCode2=AsciiCode-Key+26;
        }
        else{
            AsciiCode2=AsciiCode-Key;
        }
        char Character = static_cast<char>(AsciiCode2);
        OutputString+=Character;
    }
    return OutputString;
}

int main(){
    string s1;
    getline(cin,s1);
    string s2;
    getline(cin,s2);
    vector<string> Key_Operation;    
    Key_Operation = Split(s2);
    string Key=Key_Operation[0];
    string Operation=Key_Operation[1];
    if (Operation=="Encrypt"){
        string Output=Encrypt(s1,stoi(Key));
        cout << Output << endl;
    }
    else{
        string Output=Decrypt(s1,stoi(Key));
        cout << Output << endl;        
    }
    return 0;
}
