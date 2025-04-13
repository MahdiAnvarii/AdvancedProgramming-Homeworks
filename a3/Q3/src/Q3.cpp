#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <list>

using namespace std;

double CONSIDER_TAX_PERCENTAGE=1.1;

struct FoodMenuStruct{
    double foodPrices;
    string foodNames;
};

vector<int> splitTheLine(string line){
    istringstream iss(line);
    vector<int> numbers;
    int num;
    while (iss >> num) {
        numbers.push_back(num);
    }
    return numbers;
}

vector<FoodMenuStruct> processTheMenu(int n){
    vector<FoodMenuStruct> foodMenu;
    for (int i=0; i<n; i++){
        string food;
        string name;
        int price;
        getline(cin,food);
        istringstream iss(food);
        iss >> name >> price;
        foodMenu.push_back({price*CONSIDER_TAX_PERCENTAGE, name});
    }
    return foodMenu;
}

void findThesubset(int minimumFactor, int maximumFactor, vector<FoodMenuStruct>& foodMenu, vector<FoodMenuStruct>& currentMenu, vector<FoodMenuStruct>& bestMenu, 
    int& bestSum, int indx=0, int currentSum=0){
    if (currentSum > maximumFactor) return;
    if ((currentSum >= minimumFactor) && (currentSum > bestSum)){
        bestSum = currentSum;
        bestMenu = currentMenu;
    }
    for (int i=indx; i < foodMenu.size(); i++) {
        currentMenu.push_back(foodMenu[i]);
        findThesubset(minimumFactor, maximumFactor, foodMenu, currentMenu, bestMenu, bestSum, i+1, currentSum+foodMenu[i].foodPrices);
        currentMenu.pop_back();
    }
}

void takeTheRevenge(int numberOfFoods, int discountPercentage, int minimumFactor, int maximumDiscount, vector<FoodMenuStruct>& foodMenu){
    int maximumFactor = maximumDiscount / (static_cast<double>(discountPercentage)/100);
    int bestSum=0;
    vector<FoodMenuStruct> currentMenu;
    vector<FoodMenuStruct> bestMenu;
    findThesubset(minimumFactor, maximumFactor, foodMenu, currentMenu, bestMenu, bestSum);
    for (const auto [theFoodPrice, theFoodName] : bestMenu){
        cout << theFoodName << endl;
    } 
}

int main(){
    string optionsString;
    vector<int> optionsVector;
    getline(cin,optionsString);
    optionsVector = splitTheLine(optionsString);
    int n = optionsVector[0];
    int x = optionsVector[1];
    int l = optionsVector[2];
    int r = optionsVector[3];
    vector<FoodMenuStruct> foodMenu = processTheMenu(n);
    takeTheRevenge(n, x, l, r, foodMenu);
    return 0;
}