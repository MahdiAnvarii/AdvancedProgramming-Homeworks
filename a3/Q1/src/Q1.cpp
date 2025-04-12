#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int MAX_INDEX_DIGITS = 10;

vector<int> generateNumbers(int n){
    vector<int> theNumbers;
    for (int i= 1 << (n - 1) ; i< 1 << n ; i++){
        theNumbers.push_back(i);
    }
    return theNumbers;
}

vector<int> findReplacementIndices(string currentString){
    vector<int> theIndices;
    for (int i=0; i<currentString.size(); i++){
        if ((currentString[i]=='{') && (currentString[i+2]!='+')){
            for (int j=i+1; j<=i+MAX_INDEX_DIGITS; j++){
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

string generateLatex(int n){
    if (n==1) return "1";
    else if (n==2) return "1+\\frac{2}{3}";
    
    string currentString = generateLatex(n-1);
    string nextString;
    vector<int> theIndices = findReplacementIndices(currentString);
    vector<int> theNumbers = generateNumbers(n);

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
    cout << generateLatex(n) << endl;
    return 0;
}