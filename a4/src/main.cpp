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
        numOfIncorrect = 0;
        numOfBlank = 0;
        numOfCorrect = 0;
        priority = 0;
    }
    void setPriority(int numOfIncorrect_, int numOfBlank_, int numOfCorrect_){
        numOfIncorrect = numOfIncorrect_;
        numOfBlank = numOfBlank_;
        numOfCorrect = numOfCorrect_;
        priority = 3*numOfIncorrect_ + 1*numOfBlank_ - 2*numOfCorrect_;
    }
    string getQuestionText() const { return questionText; }
    string getSubject() const { return subject; }
    string getDifficulty() const { return difficulty; }
    int getPriority() const { return priority; }
private:
    string questionText;
    string option1;
    string option2;
    string option3;
    string option4;
    int correctAnswer;
    string difficulty;
    string subject;
    int numOfIncorrect;
    int numOfBlank;
    int numOfCorrect;
    int priority;
};

struct QuestionsTemplateStruct{
    string subject;
    string difficulty;
    int count;
};

class TestTemplate{
public:
    TestTemplate(string testTemplateName_, const vector<QuestionsTemplateStruct>& potentialQuestions_, const vector<TestTemplate*>& testTemplates){
        setTestTemplate(testTemplateName_, potentialQuestions_, testTemplates);
    }
    void setTestTemplate(string testTemplateName_, const vector<QuestionsTemplateStruct>& potentialQuestions_, const vector<TestTemplate*>& testTemplates){
        for (const auto t : testTemplates) {
            if (t->testTemplateName == testTemplateName_)
                throw invalid_argument("Duplicate name: \'" + testTemplateName_ + "\'" + "\n");
        }
        testTemplateName = testTemplateName_;
        potentialQuestions = potentialQuestions_;
        cout << "Template \'" << testTemplateName_ << "\' was created successfully." << endl;
    }
    string getTestTemplateName() const { return testTemplateName; }
    vector<QuestionsTemplateStruct> getPotentialQuestions() const { return potentialQuestions; }
private:
    string testTemplateName;
    vector<QuestionsTemplateStruct> potentialQuestions;
};

void sortQuestions(vector<Question*>& questions, bool needPriority){
    sort(questions.begin(), questions.end(), [needPriority](const Question* a, const Question* b) {
        if (a->getSubject() != b->getSubject()) {
            return a->getSubject() < b->getSubject();
        }
        if (needPriority && a->getPriority() != b->getPriority()) {
            return a->getPriority() > b->getPriority();
        }
        return a->getQuestionText() < b->getQuestionText();
    });
}

vector<Question*> selectQuestionsForTest(const TestTemplate& testTemplate_, const vector<Question*> questions){
    vector<Question*> testQuestions;
    vector<QuestionsTemplateStruct> potentialQuestions = testTemplate_.getPotentialQuestions();
    for (const auto& potentialQuestion : potentialQuestions){
        int theCount = potentialQuestion.count;
        int theCounter = 0;
        string theSubject = potentialQuestion.subject;
        string theDifficulty = potentialQuestion.difficulty;
        for (const auto& question : questions){
            if (theCounter >= theCount) break;
            else if (theSubject == question->getSubject() && theDifficulty == question->getDifficulty()){
                testQuestions.push_back(question);
                theCounter+=1;
            }
        }
    }
    return testQuestions;
}

class Test{
public:
    Test(string testName_, const TestTemplate& testTemplate_, const vector<Question*>& questions)
    : testTemplate(testTemplate_){
        vector<Question*> testQuestions_ = selectQuestionsForTest(testTemplate_, questions);
        setTest(testName_, testTemplate_, testQuestions_);
    }
    void setTest(string testName_, const TestTemplate& testTemplate_, const vector<Question*>& testQuestions_){
        testName = testName_;
        testTemplate = testTemplate_;
        testQuestions = testQuestions_;
        cout << "Test \'" << testName_ << "\' was generated successfully." << endl;
    }
private:
    string testName;
    TestTemplate testTemplate;
    vector<Question*> testQuestions;
};

vector<string> splitTheOrder(string order){
    istringstream iss(order);
    vector<string> orderToVector;
    bool insideQuote = false;
    string word;
    string temp;
    while (iss >> word) {
        if (word.front() == '\''){
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

vector<Question*> processQuestionsFile(const string questionsFileAddress){
    ifstream questionsFile(questionsFileAddress);
    string questionsFileLine;
    vector<Question*> questions;
    getline(questionsFile, questionsFileLine);
    while(getline(questionsFile, questionsFileLine)){
        //cout << questionsFileLine << endl;
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
    sortQuestions(questions, true);
    return questions;
}

void createTemplate(const vector<string>& orderToVector, vector<TestTemplate*>& testTemplates){
    string testTemplateName = orderToVector[1];
    vector<QuestionsTemplateStruct> potentialQuestions;
    for (int i=2; i<orderToVector.size(); i++){
        istringstream iss(orderToVector[i]);
        string subject, difficulty, count;
        getline(iss, subject, ':');
        getline(iss, difficulty, ':');
        getline(iss, count, ':');
        potentialQuestions.push_back({subject, difficulty, stoi(count)});
    }

    TestTemplate* testTemplate = new TestTemplate(testTemplateName, potentialQuestions, testTemplates);
    testTemplates.push_back(testTemplate);
}

void generateTest(const vector<string>& orderToVector, vector<Question*>& questions, const vector<TestTemplate*>& testTemplates, vector<Test*> tests){
    string testName = orderToVector[1];
    string testTemplateName = orderToVector[2];
    sortQuestions(questions, true);
    bool isTestTemplateNameValid = false;
    TestTemplate* testTemplate = nullptr;
    for (const auto& t : testTemplates) {
        if (t->getTestTemplateName() == testTemplateName){
            isTestTemplateNameValid = true;
            testTemplate = t;
        }
    }
    if (!isTestTemplateNameValid){
        throw invalid_argument("Could not find template: \'" + testTemplateName + "\'" + "\n");
    }

    Test* test = new Test(testName, *testTemplate, questions);
    tests.push_back(test);
}

void takeTheOrders(vector<Question*>& questions){
    string theOrder;
    vector<TestTemplate*> testTemplates;
    vector<Test*> tests;
    while (getline(cin,theOrder)){
        vector<string> orderToVector = splitTheOrder(theOrder);
        if (orderToVector[0] == CREATE_TEMPLATE){
            createTemplate(orderToVector, testTemplates);
        } else if (orderToVector[0] == GENERATE_TEST){
            generateTest(orderToVector, questions, testTemplates, tests);
        }
    }
}

int main(int argc, char* argv[]){
    const string questionFileAddress = argv[1];
    vector<Question*> questions = processQuestionsFile(questionFileAddress);
    takeTheOrders(questions);
}