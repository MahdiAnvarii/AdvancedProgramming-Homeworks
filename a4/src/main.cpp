#include "test.h"
#include "utils.h"
#include "question.h"
#include "subject.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <memory>

using namespace std;

class GhalamUT{
public:
    GhalamUT(string questionFileAddress_){
        vector<shared_ptr<Question>> questions = processQuestionsFile(questionFileAddress_);
        vector<shared_ptr<Subject>> subjects = processSubjects(questions);
        takeTheOrders(questions, subjects);
    }

private:
    void createTemplate(const vector<string>& orderToVector, vector<shared_ptr<TestTemplate>>& testTemplates, bool isAutoTemplate){
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

        shared_ptr<TestTemplate> testTemplate = make_shared<TestTemplate>(testTemplateName, potentialQuestions, testTemplates, isAutoTemplate);
        testTemplates.push_back(testTemplate);
    }
    void generateTest(const vector<string>& orderToVector, vector<shared_ptr<Question>>& questions, const vector<shared_ptr<TestTemplate>>& testTemplates, vector<shared_ptr<Test>>& tests){
        string testName = orderToVector[1];
        string testTemplateName = orderToVector[2];
        sortQuestions(questions, true);
        bool isTestTemplateNameValid = false;
        shared_ptr<TestTemplate> testTemplate = nullptr;
        for (const auto& t : testTemplates) {
            if (t->getTestTemplateName() == testTemplateName){
                isTestTemplateNameValid = true;
                testTemplate = t;
            }
        }
        if (!isTestTemplateNameValid){
            cout << "Could not find template: \'" << testTemplateName << "\'" << endl;
            return;
        }
    
        shared_ptr<Test> test = make_shared<Test>(testName, *testTemplate, questions);
        test->sortTestQuestions();
        tests.push_back(test);
    }
    void attendToTest(const vector<string>& orderToVector, vector<shared_ptr<Test>>& tests){
        string testName = orderToVector[1];
        bool isTestNameValid = false;
        shared_ptr<Test> test = nullptr;
        for (const auto& t : tests) {
            if (t->getTestName() == testName){
                isTestNameValid = true;
                test = t;
            }
        }
        if (!isTestNameValid){
            cout << "Could not find test: \'" << testName << "\'" << endl;
            return;
        }
        test->attendTest();
    }
    void autoGenerateTest(const vector<string>& orderToVector, vector<shared_ptr<Question>>& questions, vector<shared_ptr<Test>>& tests, vector<shared_ptr<TestTemplate>> testTemplates, vector<shared_ptr<Subject>>& subjects){
        for (auto& subject : subjects){
            subject->calculateScoreForSubject();
        }
        sortSubjects(subjects, true);
        string testTemplateName = generateRandomTemplateName();
        string testName = orderToVector[1];
        vector<string> ordersOfTemplateCreation;
        ordersOfTemplateCreation.push_back("autoTemplateCreation");
        ordersOfTemplateCreation.push_back(testTemplateName);
        const string hardSubjectName1 = subjects[0]->getSubjectName();
        const string hardSubjectName2 = subjects[1]->getSubjectName();
        for (const auto& hardSubjectMaterial1 : HARD_SUBJECT_FOR_USER_1){
            ordersOfTemplateCreation.push_back(hardSubjectName1 + hardSubjectMaterial1);
        }
        for (const auto& hardSubjectMaterial2 : HARD_SUBJECT_FOR_USER_2){
            ordersOfTemplateCreation.push_back(hardSubjectName2 + hardSubjectMaterial2);
        }
        createTemplate(ordersOfTemplateCreation, testTemplates, true);
    
        vector<string> ordersOfTestGeneration;
        ordersOfTestGeneration.push_back("autoTestGeneration");
        ordersOfTestGeneration.push_back(testName);
        ordersOfTestGeneration.push_back(testTemplateName);
        generateTest(ordersOfTestGeneration, questions, testTemplates, tests);
    }
    void reportAll(const vector<shared_ptr<Question>> questions, vector<shared_ptr<Subject>> subjects){
        for (auto& subject : subjects){
            subject->calculateScoreForSubject();
        }
        sortSubjects(subjects, false);
        cout << "Total report: " << endl << endl;
        double allSubjectsCorrects=0.0;
        double allSubjectsIncorrects=0.0;
        double allSubjectsBlank=0.0;
        double allSubjectsOccurrence=0.0;
        for (const auto& subject : subjects){
            subject->totalReport();
            allSubjectsCorrects += subject->getSubjectTotalCorrects();
            allSubjectsIncorrects += subject->getSubjectTotalIncorrects();
            allSubjectsBlank += subject->getSubjectTotalBlank();
            allSubjectsOccurrence += subject->getSubjectTotalOccurrence();
        }
    
        double allSubjectScore=0.0;
        if (allSubjectsOccurrence) allSubjectScore=allSubjectsCorrects/allSubjectsOccurrence;
        double allSubjectsScoreWithThreeDigits = truncationWithThreeDigits(allSubjectScore);
        cout << endl << "Total results: " << allSubjectsCorrects << " corrects, " << allSubjectsIncorrects << " incorrects and " << allSubjectsBlank << " blanks." << endl;
        cout << "Total score: " << fixed << setprecision(3) << allSubjectsScoreWithThreeDigits << "%." << endl;
        unsetThreeDigits();
    }
    void reportTest(const vector<string>& orderToVector, vector<shared_ptr<Test>>& tests){
        string testName = orderToVector[2];
        shared_ptr<Test> test = nullptr;
        for (auto& t : tests) {
            if (t->getTestName() == testName){
                test = t;
            }
        }
        cout << "Results for " << test->getTestName() << ":" << endl << endl;
        test->reportTest();
    }
    void reportTests(vector<shared_ptr<Test>>& tests){
        cout << "Results per attended tests: " << endl << endl;
        sortTests(tests);
        for (auto& test : tests) {
            if (test->getTestTime()){
                test->reportTests();
            }
        }
    }
    void reportSubject(const vector<string>& orderToVector, vector<shared_ptr<Subject>>& subjects){
        string subjectName = orderToVector[2];
        shared_ptr<Subject> subject = nullptr;
        for (auto& s : subjects) {
            if (s->getSubjectName() == subjectName){
                subject = s;
            }
        }
        subject->calculateScoreForSubject();
        cout << "Results for " << subject->getSubjectName() << ":" << endl << endl;
        subject->reportSubject();
    }
    void takeTheOrders(vector<shared_ptr<Question>>& questions, vector<shared_ptr<Subject>>& subjects){
        string theOrder;
        vector<shared_ptr<TestTemplate>> testTemplates;
        vector<shared_ptr<Test>> tests;
        while (getline(cin,theOrder)){
            if (theOrder.empty()) continue;
            vector<string> orderToVector = splitTheOrder(theOrder);
            if (orderToVector.empty()) continue;
            if (orderToVector[0] == CREATE_TEMPLATE){
                createTemplate(orderToVector, testTemplates, false);
            } else if (orderToVector[0] == GENERATE_TEST){
                generateTest(orderToVector, questions, testTemplates, tests);
            } else if (orderToVector[0] == ATTEND){
                attendToTest(orderToVector, tests);          
            } else if (orderToVector[0] == AUTO_GENERATE){
                autoGenerateTest(orderToVector, questions, tests, testTemplates, subjects);
            } else if (orderToVector[0] == REPORT){
                if (orderToVector[1] == ALL) reportAll(questions, subjects);
                else if (orderToVector[1] == TEST) reportTest(orderToVector, tests);
                else if (orderToVector[1] == TESTS) reportTests(tests);
                else if (orderToVector[1] == SUBJECT) reportSubject(orderToVector, subjects);
            } 
        }
    }
    vector<shared_ptr<Question>> questions;
    vector<shared_ptr<Subject>> subjects;
};

void checkArguments(int argc) {
    if (argc < 2) {
        throw logic_error(NO_INPUT_FILE);
    }
}

int main(int argc, char* argv[]){
    checkArguments(argc);
    const string questionFileAddress = argv[1];
    GhalamUT ghalamUT(questionFileAddress);
}