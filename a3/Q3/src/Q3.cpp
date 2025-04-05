#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <list>

using namespace std;

struct FoodMenuStruct{
    vector<int> foodPrices;
    vector<string> foodNames;
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

FoodMenuStruct processTheMenu(int n){
    FoodMenuStruct foodMenu;
    double considerTaxPercentage=1.1;
    for (int i=0; i<n; i++){
        string food;
        string name;
        int price;
        getline(cin,food);
        istringstream iss(food);
        iss >> name >> price;
        foodMenu.foodNames.push_back(name);
        foodMenu.foodPrices.push_back(price*considerTaxPercentage);
    }
    return foodMenu;
}

void findThesubset(int minimumFactor, int maximumFactor, FoodMenuStruct foodMenu, FoodMenuStruct& currentMenu, FoodMenuStruct& bestMenu, int& bestSum, int indx=0, int currentSum=0){
    if (currentSum > maximumFactor) return;
    if ((currentSum >= minimumFactor) && (currentSum > bestSum)){
        bestSum = currentSum;
        bestMenu = currentMenu;
    }
    for (int i=indx; i < foodMenu.foodPrices.size(); i++) {
        currentMenu.foodNames.push_back(foodMenu.foodNames[i]);
        currentMenu.foodPrices.push_back(foodMenu.foodPrices[i]);
        findThesubset(minimumFactor, maximumFactor, foodMenu, currentMenu, bestMenu, bestSum, i+1, currentSum+foodMenu.foodPrices[i]);
        currentMenu.foodNames.pop_back();
        currentMenu.foodPrices.pop_back();
    }
}

void takeTheRevenge(int numberOfFoods, int discountPercentage, int minimumFactor, int maximumDiscount, FoodMenuStruct foodMenu){
    int maximumFactor = maximumDiscount / (static_cast<double>(discountPercentage)/100);
    int bestSum=0;
    FoodMenuStruct currentMenu;
    FoodMenuStruct bestMenu;
    findThesubset(minimumFactor, maximumFactor, foodMenu, currentMenu, bestMenu, bestSum);
    for (vector<string>::iterator it = bestMenu.foodNames.begin(); it != bestMenu.foodNames.end(); ++it){
        cout << *it << endl;
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
    FoodMenuStruct foodMenu = processTheMenu(n);
    takeTheRevenge(n, x, l, r, foodMenu);
    return 0;
}