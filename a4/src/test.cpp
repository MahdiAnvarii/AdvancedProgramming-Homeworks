#include "test.h"
#include "utils.h"
#include "question.h"
#include "subject.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <map>
#include <ctime>
#include <iomanip>

using namespace std;

TestTemplate::TestTemplate(string testTemplateName_, const vector<QuestionsTemplateStruct>& potentialQuestions_, const vector<shared_ptr<TestTemplate>>& testTemplates, bool isAutoTemplate_){
    setTestTemplate(testTemplateName_, potentialQuestions_, testTemplates, isAutoTemplate_);
}
void TestTemplate::setTestTemplate(string testTemplateName_, const vector<QuestionsTemplateStruct>& potentialQuestions_, const vector<shared_ptr<TestTemplate>>& testTemplates, bool isAutoTemplate_){
    for (const auto t : testTemplates) {
        if (t->testTemplateName == testTemplateName_){
            cout << "Duplicate name: \'" << testTemplateName_ << "\'" << endl;
            return;
        }
    }
    testTemplateName = testTemplateName_;
    potentialQuestions = potentialQuestions_;
    isAutoTemplate = isAutoTemplate_;
    if (!isAutoTemplate) cout << "Template \'" << testTemplateName_ << "\' was created successfully." << endl;
}
string TestTemplate::getTestTemplateName() const { return testTemplateName; }
vector<QuestionsTemplateStruct> TestTemplate::getPotentialQuestions() const { return potentialQuestions; }

Test::Test(string testName_, const TestTemplate& testTemplate_, const vector<shared_ptr<Question>>& questions) 
    : testTemplate(testTemplate_){
    vector<shared_ptr<Question>> testQuestions_ = selectQuestionsForTest(testTemplate_, questions);
    setTest(testName_, testTemplate_, testQuestions_);
}
void Test::setTest(string testName_, const TestTemplate& testTemplate_, const vector<shared_ptr<Question>>& testQuestions_){
    testName = testName_;
    testTemplate = testTemplate_;
    testQuestions = testQuestions_;
    testSubjects = processSubjects(testQuestions);
    totalCorrects=0.0;
    totalIncorrects=0.0;
    totalBlank=0.0;
    totalOccurrence=0.0;
    totalScore=0.0;
    timeOfAttendance=0;
    cout << "Test \'" << testName_ << "\' was generated successfully." << endl;
}
vector<shared_ptr<Question>> Test::selectQuestionsForTest(const TestTemplate& testTemplate_, const vector<shared_ptr<Question>> questions){
    vector<shared_ptr<Question>> testQuestions_;
    vector<QuestionsTemplateStruct> potentialQuestions = testTemplate_.getPotentialQuestions();
    for (const auto& potentialQuestion : potentialQuestions){
        int theCount = potentialQuestion.count;
        int theCounter = 0;
        string theSubject = potentialQuestion.subject;
        string theDifficulty = potentialQuestion.difficulty;
        for (const auto& question : questions){
            if (theCounter >= theCount) break;
            else if (theSubject == question->getSubject() && theDifficulty == question->getDifficulty()){
                testQuestions_.push_back(question);
                theCounter+=1;
            }
        }
    }
    return testQuestions_;
}
void Test::attendTest(){
    timeOfAttendance = time(nullptr);
    cout << testName << ":" << endl << endl;
    int questionCounter = 0;
    string theAnswer;
    while (questionCounter < testQuestions.size()){
        theAnswer = testQuestions[questionCounter]->printQuestion(questionCounter+1);
        if (theAnswer == PREVIOUS){
            questionCounter-=1;
        } else {
            questionCounter+=1;
        }
    }
    cout << "Finished " << testName << "." << endl;

    for (int k=0; k< testQuestions.size(); ++k){
        string theResult_ = testQuestions[k]->evaluateTheQuestion();
        string subjectName_ = testQuestions[k]->getSubject();
        if (theResult_==CORRECT){
            totalCorrects+=1;
            eachSubjectCorrects[subjectName_]+=1;
        } else if (theResult_==BLANK){
            totalBlank+=1;
            eachSubjectBlank[subjectName_]+=1;
        } else{
            totalIncorrects+=1;
            eachSubjectIncorrects[subjectName_]+=1;
        }
        totalOccurrence+=1;
        eachSubjectOccurrence[subjectName_]+=1;
    }
}
void Test::reportTest(){
    for (const auto& testSubject : testSubjects){
        string subjectName_ = testSubject->getSubjectName();
        if (eachSubjectOccurrence[subjectName_]) eachSubjectScore[subjectName_] = eachSubjectCorrects[subjectName_]/eachSubjectOccurrence[subjectName_];

        double scoreWithThreeDigits = truncationWithThreeDigits(eachSubjectScore[subjectName_]);
        cout << subjectName_ << ": " << eachSubjectCorrects[subjectName_] << " corrects, " << eachSubjectIncorrects[subjectName_] << " incorrects and " <<
        eachSubjectBlank[subjectName_] << " blanks. Score: " << fixed << setprecision(3) << scoreWithThreeDigits << "%." << endl;
        unsetThreeDigits();
    }
        
    if (totalOccurrence) totalScore=totalCorrects/totalOccurrence;
    totalScoreWithThreeDigits = truncationWithThreeDigits(totalScore);
    cout << endl << "Total results: " << totalCorrects << " corrects, " << totalIncorrects << " incorrects and " << totalBlank << " blanks." << endl;
    cout << "Total score: " << fixed << setprecision(3) << totalScoreWithThreeDigits << "%." << endl;
    unsetThreeDigits();
}
void Test::reportTests(){
    if (totalOccurrence) totalScore=totalCorrects/totalOccurrence;
    totalScoreWithThreeDigits = truncationWithThreeDigits(totalScore);
    cout << testName << ": " << totalCorrects << " corrects, " << totalIncorrects << " incorrects and " << totalBlank << " blanks." 
    << " Score: " << fixed << setprecision(3) << totalScoreWithThreeDigits << "%." << endl;
    unsetThreeDigits();
}
void Test::sortTestQuestions(){
    sortQuestions(testQuestions, false);
}
string Test::getTestName() const { return testName; }
time_t Test::getTestTime() const { return timeOfAttendance; }
vector<shared_ptr<Question>> Test::getTestQuestions() const { return testQuestions; }

void sortTests(vector<shared_ptr<Test>>& tests){
    sort(tests.begin(), tests.end(), [](const shared_ptr<Test> a, const shared_ptr<Test> b) {
        return a->getTestTime() < b->getTestTime();
    });
}