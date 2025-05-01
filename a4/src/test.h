#ifndef TEST_H
#define TEST_H

#include "utils.h"
#include "question.h"
#include "subject.h"
#include <vector>
#include <string>
#include <memory>
#include <map>

using namespace std;

class TestTemplate{
public:
    TestTemplate(string testTemplateName_, const vector<QuestionsTemplateStruct>& potentialQuestions_, const vector<shared_ptr<TestTemplate>>& testTemplates, bool isAutoTemplate_);

    void setTestTemplate(string testTemplateName_, const vector<QuestionsTemplateStruct>& potentialQuestions_, const vector<shared_ptr<TestTemplate>>& testTemplates, bool isAutoTemplate_);

    string getTestTemplateName() const;
    vector<QuestionsTemplateStruct> getPotentialQuestions() const;

private:
    string testTemplateName;
    vector<QuestionsTemplateStruct> potentialQuestions;
    bool isAutoTemplate;
};

class Test{
public:
    Test(string testName_, const TestTemplate& testTemplate_, const vector<shared_ptr<Question>>& questions);

    void setTest(string testName_, const TestTemplate& testTemplate_, const vector<shared_ptr<Question>>& testQuestions_);

    vector<shared_ptr<Question>> selectQuestionsForTest(const TestTemplate& testTemplate_, const vector<shared_ptr<Question>> questions);

    void attendTest();

    void reportTest();

    void reportTests();

    void sortTestQuestions();

    string getTestName() const;
    time_t getTestTime() const;
    vector<shared_ptr<Question>> getTestQuestions() const;

private:
    string testName;
    TestTemplate testTemplate;
    vector<shared_ptr<Question>> testQuestions;
    vector<shared_ptr<Subject>> testSubjects;
    double totalCorrects;
    double totalIncorrects;
    double totalBlank;
    double totalOccurrence;
    double totalScore;
    double totalScoreWithThreeDigits;
    map<string, double> eachSubjectCorrects;
    map<string, double> eachSubjectIncorrects;
    map<string, double> eachSubjectBlank;
    map<string, double> eachSubjectOccurrence;
    map<string, double> eachSubjectScore;
    time_t timeOfAttendance;
};

void sortTests(vector<shared_ptr<Test>>& tests);

#endif //TEST_H