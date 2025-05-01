#include "utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <sstream>

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
const vector<string> VALID_DIFFICULTIES = {"easy", "medium", "hard"};
const string PREVIOUS = "previous";
const string CORRECT = "correct";
const string INCORRECT = "incorrect";
const string BLANK = "blank";
const vector<string> VALID_ANSWERS = {"1", "2", "3", "4", "", "previous"};
const double TRUNCATION_FACTOR = 1000.0;
const double PERCENTAGE_FACTOR = 100.0;
const vector<string> HARD_SUBJECT_FOR_USER_1 = {":easy:3", ":medium:2", ":hard:1"};
const vector<string> HARD_SUBJECT_FOR_USER_2 = {":easy:2", ":medium:1", ":hard:1"};
const string RANDOM_TEMPLATE_CHARACTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const int RANDOM_TEMPLATE_LENGTH = 20;
const string NO_INPUT_FILE =  "No input file provided.\n"
                            "Usage: ./main <question_file_path>\n"
                            "Example: ./main ../csv/questions.csv";

void unsetThreeDigits(){
    cout.unsetf(ios::fixed);
    cout.precision(6);
}

double truncationWithThreeDigits(double score){
    return trunc(score*PERCENTAGE_FACTOR*TRUNCATION_FACTOR)/TRUNCATION_FACTOR;
}

string generateRandomTemplateName() {
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> distribution(0, RANDOM_TEMPLATE_CHARACTERS.size() - 1);
    string randomTemplateName;
    for (size_t i=0; i<RANDOM_TEMPLATE_LENGTH; ++i) {
        randomTemplateName += RANDOM_TEMPLATE_CHARACTERS[distribution(generator)];
    }
    return randomTemplateName;
}

vector<string> splitTheOrder(string order){
    istringstream iss(order);
    vector<string> orderToVector;
    bool insideQuote = false;
    string word;
    string temp;
    while (iss >> word) {
        if (word.front() == '\'' && word.back() == '\''){
            orderToVector.push_back(word.substr(1, word.length() - 2));
        }
        else if (word.front() == '\''){
            temp = word.substr(1);
            insideQuote = true;
        } else if (insideQuote && word.back() == '\''){
            temp += " " + word.substr(0, word.length() - 1);
            orderToVector.push_back(temp);
            insideQuote = false;
        } else if (insideQuote) {
            temp += " " + word;
        } else {
            orderToVector.push_back(word);
        }
    }
    return orderToVector;
}