#include "utils.h"
#include "question.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <sstream>
#include <fstream>

using namespace std;

Question::Question(string questionText_, string option1_, string option2_, string option3_, string option4_, int correctAnswer_, string difficulty_, string subject_){
    setQuestion(questionText_, option1_, option2_, option3_, option4_, correctAnswer_, difficulty_, subject_);
}
void Question::setQuestion(string questionText_, string option1_, string option2_, string option3_, string option4_, int correctAnswer_, string difficulty_, string subject_){
    if (correctAnswer_ > 4 || correctAnswer_ < 1 || (find(VALID_DIFFICULTIES.begin(), VALID_DIFFICULTIES.end(), difficulty_) == VALID_DIFFICULTIES.end()))
        throw invalid_argument("Invalid question parameters");
    questionText = questionText_;
    vector<string> options_;
    options_.push_back(option1_);
    options_.push_back(option2_);
    options_.push_back(option3_);
    options_.push_back(option4_);
    options = options_;
    correctAnswer = correctAnswer_;
    difficulty = difficulty_;
    subject = subject_;
    numOfIncorrect = 0;
    numOfBlank = 0;
    numOfCorrect = 0;
    priority = 0;
    numOfOccurrence = 0;
}
string Question::printQuestion(int questionCounter){
    cout << questionCounter << ") " << questionText << endl;
    int answerIndex = -1;
    if (find(VALID_ANSWERS.begin(), VALID_ANSWERS.end(), theAnswer) != VALID_ANSWERS.end()){
        if (theAnswer == "1") answerIndex = 0;
        else if (theAnswer == "2") answerIndex = 1;
        else if (theAnswer == "3") answerIndex = 2;
        else if (theAnswer == "4") answerIndex = 3;
    } 
    for (int i=0; i<4; ++i) {
        cout << "    " << (i+1) << ". " << options[i];
        if (i == answerIndex) {
            cout << " <-";
        }
        cout << endl;
    }
    cout << "Your answer: ";
    string theAnswer_;
    getline(cin, theAnswer_);
    theAnswer_.erase(remove(theAnswer_.begin(), theAnswer_.end(), '\r'), theAnswer_.end());
    while ((find(VALID_ANSWERS.begin(), VALID_ANSWERS.end(), theAnswer_) == VALID_ANSWERS.end()) || (theAnswer_ == PREVIOUS && questionCounter==1)){
        cout << "Invalid answer, please try again." << endl;
        cout << "Your answer: ";
        getline(cin, theAnswer_);
        theAnswer_.erase(remove(theAnswer_.begin(), theAnswer_.end(), '\r'), theAnswer_.end());
    }
    if (theAnswer_ != PREVIOUS) theAnswer = theAnswer_;
    cout << endl;
    return theAnswer_;
}
string Question::evaluateTheQuestion(){
    string theResult_;
    if (theAnswer.empty()){
        numOfBlank+=1;
        theResult_ = BLANK;
    } else if (stoi(theAnswer)==correctAnswer){
        numOfCorrect+=1;
        theResult_=CORRECT;
    } else{
        numOfIncorrect+=1;
        theResult_ = INCORRECT;
    }
    priority = 3*numOfIncorrect + 1*numOfBlank - 2*numOfCorrect;
    numOfOccurrence+=1;
    theAnswer = "N";
    return theResult_;
}
string Question::getQuestionText() const { return questionText; }
string Question::getSubject() const { return subject; }
string Question::getDifficulty() const { return difficulty; }
int Question::getPriority() const { return priority; }
int Question::getNumOfcorrect() const { return numOfCorrect; }
int Question::getNumOfIncorrect() const { return numOfIncorrect; }
int Question::getNumOfBlank() const { return numOfBlank; }
int Question::getNumOfOccurrence() const { return numOfOccurrence; }

void sortQuestions(vector<shared_ptr<Question>>& questions, bool needPriority){
    sort(questions.begin(), questions.end(), [needPriority](const shared_ptr<Question> a, const shared_ptr<Question> b) {
        if (a->getSubject() != b->getSubject()) {
            return a->getSubject() < b->getSubject();
        }
        if (needPriority && a->getPriority() != b->getPriority()) {
            return a->getPriority() > b->getPriority();
        }
        return a->getQuestionText() < b->getQuestionText();
    });
}

vector<shared_ptr<Question>> processQuestionsFile(const string questionsFileAddress){
    ifstream questionsFile(questionsFileAddress);
    string questionsFileLine;
    vector<shared_ptr<Question>> questions;
    getline(questionsFile, questionsFileLine);
    while(getline(questionsFile, questionsFileLine)){
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
        
        shared_ptr<Question> question = make_shared<Question>(questionText, option1, option2, option3, option4, stoi(correctAnswer), difficulty, subject);
        questions.push_back(question);
    }
    sortQuestions(questions, true);
    return questions;
}