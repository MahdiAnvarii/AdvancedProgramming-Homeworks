#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

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

class Question{
public:
    Question(string questionText_, string option1_, string option2_, string option3_, string option4_, int correctAnswer_, string difficulty_, string subject_){
        setQuestion(questionText_, option1_, option2_, option3_, option4_, correctAnswer_, difficulty_, subject_);
    }

    void setQuestion(string questionText_, string option1_, string option2_, string option3_, string option4_, int correctAnswer_, string difficulty_, string subject_){
        if (correctAnswer_ > 4 || correctAnswer_ < 1 || (find(VALID_DIFFICULTIES.begin(), VALID_DIFFICULTIES.end(), difficulty_) == VALID_DIFFICULTIES.end()))
            throw invalid_argument("Invalid question parameters");
    questionText = questionText_;
    option1 = option1_;
    option2 = option2_;
    option3 = option3_;
    option4 = option4_;
    correctAnswer = correctAnswer_;
    difficulty = difficulty_;
    subject = subject_;
    }
private:
    string questionText;
    string option1;
    string option2;
    string option3;
    string option4;
    int correctAnswer;
    string difficulty;
    string subject;
};

vector<string> splitTheOrder(string order){
    istringstream iss(order);
    vector<string> orderToVector;
    string word;
    while (iss >> word) {
        orderToVector.push_back(word);
    }
    return orderToVector;
}

vector<Question*> processQuestionsFile(const string questionsFileAddress){
    ifstream questionsFile(questionsFileAddress);
    string questionsFileLine;
    vector<Question*> questions;
    getline(questionsFile, questionsFileLine);
    while(getline(questionsFile, questionsFileLine)){
        cout << questionsFileLine << endl;
        istringstream iss(questionsFileLine);
        string questionText, option1, option2, option3, option4, correctAnswer, difficulty, subject;
        getline(iss, questionText, ',');
        getline(iss, option1, ',');
        getline(iss, option2, ',');
        getline(iss, option3, ',');
        getline(iss, option4, ',');
        getline(iss, correctAnswer, ',');
        getline(iss, difficulty, ',');
        getline(iss, subject, ',');
        
        Question* question = new Question(questionText, option1, option2, option3, option4, stoi(correctAnswer), difficulty, subject);
        questions.push_back(question);
    }
    return questions;
}

void takeTheOrders(){
    string theOrder;
    while (getline(cin,theOrder)){
        vector<string> orderToVector = splitTheOrder(theOrder);

    }
}

int main(int argc, char* argv[]){
    const string questionFileAddress = argv[1];
    vector<Question*> questions = processQuestionsFile(questionFileAddress);
    takeTheOrders();
}