#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

vector<int> splitTheLine(string line){
    istringstream iss(line);
    vector<int> numbers;
    int num;
    while (iss >> num) {
        numbers.push_back(num);
    }
    return numbers;
}

string joinTheLine(const vector<int>& numbers){
    string line;
    for (int n : numbers) {
        line = line + to_string(n) + " ";
    }
    return line;
}

int checkEachSide(const vector<int>& sideAges, int theAge, bool isAfter){
    int maxValidAge = 0;
    if (!sideAges.empty()){
        auto sideMaxIt = max_element(sideAges.begin(), sideAges.end());
        maxValidAge = *sideMaxIt;
        int maxValidIndex = distance(sideAges.begin(), sideMaxIt);
        if (isAfter){
            for (int i=0; i<maxValidIndex; i++){
                if (sideAges[i]==0){
                    return 0;
                }
            }
        } else{
            for (int i=maxValidIndex; i<sideAges.size(); i++){
                if (sideAges[i]==0){
                    return 0;
                }
            }
        }

        if (theAge < maxValidAge) return 0;
    }
    return maxValidAge;
}

vector<int> eideeCalculator(int n, vector<int>& ages, vector<int>& eideesValues, vector<int>& currentEidees){
    auto maxIt = max_element(ages.begin(), ages.end());
    int maxAge = *maxIt;
    int maxIndex = distance(ages.begin(), maxIt);
    int eideeValue = eideesValues[maxIndex];

    if (count(ages.begin(), ages.end(), 0) == n) return currentEidees;
    else{
        vector<int> agesAfter(ages.begin() + maxIndex+1, ages.end());
        vector<int> agesBefore(ages.begin(), ages.begin() + maxIndex);
        vector<int> eideesValuesAfter(eideesValues.begin() + maxIndex+1, eideesValues.end());
        vector<int> eideesValuesBefore(eideesValues.begin(), eideesValues.begin() + maxIndex);
        int maxAgeAfter = checkEachSide(agesAfter, maxAge, true);
        int maxAgeBefore = checkEachSide(agesBefore, maxAge, false);
        auto maxItAfter = agesAfter.end();
        auto maxItBefore = agesBefore.end();

        if (maxAgeAfter && maxAgeBefore){
            maxItAfter = max_element(agesAfter.begin(), agesAfter.end());
            maxItBefore = max_element(agesBefore.begin(), agesBefore.end());
            int maxIndexAfter = distance(agesAfter.begin(), maxItAfter) + maxIndex+1;
            int maxIndexBefore = distance(agesBefore.begin(), maxItBefore);
            currentEidees[maxIndexAfter]+=(eideeValue/2);
            currentEidees[maxIndexBefore]+=(eideeValue/2);
        } else if (maxAgeAfter){
            maxItAfter = max_element(agesAfter.begin(), agesAfter.end());
            int maxIndexAfter = distance(agesAfter.begin(), maxItAfter) + maxIndex+1;
            currentEidees[maxIndexAfter]+=eideeValue;
        } else if (maxAgeBefore){
            maxItBefore = max_element(agesBefore.begin(), agesBefore.end());
            int maxIndexBefore = distance(agesBefore.begin(), maxItBefore);
            currentEidees[maxIndexBefore]+=eideeValue;
        } else{
            currentEidees[maxIndex]+=eideeValue;
        }

        eideesValues[maxIndex]=0;
        ages[maxIndex]=0;
        return eideeCalculator(n, ages, eideesValues, currentEidees);
    }
}

int main(){
    int n;
    string agesLine;
    string eideesLine;
    cin >> n;
    cin.ignore();
 
    getline(cin, agesLine);
    vector<int> ages = splitTheLine(agesLine);
    getline(cin, eideesLine);
    vector<int> eideesValues = splitTheLine(eideesLine);

    vector<int> startingEidees(n,0);
    vector<int> finalEidees = eideeCalculator(n, ages, eideesValues, startingEidees);
    cout << joinTheLine(finalEidees) << endl;
    return 0;
}