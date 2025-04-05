#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

vector<int> numberGeneration(int n){
    vector<int> theNumbers;
    for (int i=pow(2,n-1) ; i<pow(2,n) ; i++){
        theNumbers.push_back(i);
    }
    return theNumbers;
}

vector<int> replacementIndices(string currentString){
    vector<int> theIndices;
    int maxDigitNumbers=10;
    for (int i=0; i<currentString.size(); i++){
        if ((currentString[i]=='{') && (currentString[i+2]!='+')){
            for (int j=i+1; j<=i+maxDigitNumbers; j++){
                if (currentString[j]=='}'){
                    theIndices.push_back(j);
                    break;
                }
            }
        }
    }
    return theIndices;
}

string createFraction(vector<int> theNumbers, int i){
    string theFraction = "+\\frac{" + to_string(theNumbers[2*i]) + "}{" + to_string(theNumbers[2*i+1]) + "}";
    return theFraction;
}

string latexFraction(int n){
    if (n==1) return "1";
    else if (n==2) return "1+\\frac{2}{3}";
    
    string currentString = latexFraction(n-1);
    string nextString;
    vector<int> theIndices = replacementIndices(currentString);
    vector<int> theNumbers = numberGeneration(n);

    int previousIndex=0;
    int theIndex=theIndices[0];
    for (int i=1; i<=theIndices.size(); i++){
        nextString = nextString + currentString.substr(previousIndex, theIndex-previousIndex) + createFraction(theNumbers, i-1);
        previousIndex = theIndex;
        theIndex = theIndices[i];
    }
    nextString+="}";
    return nextString;
}

int main() {
    int n;
    cin >> n;
    cout << latexFraction(n) << endl;
    return 0;
}