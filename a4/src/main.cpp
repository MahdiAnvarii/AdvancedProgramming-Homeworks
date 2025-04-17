#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

const string CREATE_TEMPLATE = "create_template";
const string GENERATE_TEST = "generate_test";
const string ATTEND = "attend";
const string AUTO_GENERATE = "auto_generate";
const string REPORT = "report";
const string ALL = "all";
const string TEST = "test";
const string TESTS = "tests";
const string SUBJECT = "subject";

vector<string> splitTheOrder(string order){
    istringstream iss(order);
    vector<string> orderToVector;
    string word;
    while (iss >> word) {
        orderToVector.push_back(word);
    }
    return orderToVector;
}

void takeTheOrders(){
    string theOrder;
    while (getline(cin,theOrder)){
        vector<string> orderToVector = splitTheOrder(theOrder);

    }
}

int main(int argc, char* argv[]){
    //const string questionFileAddress = argv[1];
    //processQuestionsFile(questionFileAddress);
    takeTheOrders();
}