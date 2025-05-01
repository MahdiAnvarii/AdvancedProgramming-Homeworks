#ifndef SUBJECT_H
#define SUBJECT_H

#include "question.h"
#include <vector>
#include <string>
#include <memory>
#include <map>

using namespace std;

class Subject{
public:
    Subject(string subjectName_, const vector<shared_ptr<Question>>& questions);

    void setSubjectName(string subjectName_);

    void setSubjectQuestions(const vector<shared_ptr<Question>>& subjectQuestions_);

    vector<shared_ptr<Question>> selectQuestionsForSubject(const vector<shared_ptr<Question>> questions);

    void calculateScoreForSubject();

    void totalReport();

    void reportSubject();

    string getSubjectName() const;
    double getSubjectScore() const;
    double getSubjectTotalCorrects() const;
    double getSubjectTotalBlank() const;
    double getSubjectTotalIncorrects() const;
    double getSubjectTotalOccurrence() const;

private:
    string subjectName;
    vector<shared_ptr<Question>> subjectQuestions;
    double totalCorrects;
    double totalIncorrects;
    double totalBlank;
    double totalOccurrence;
    double score;
    double scoreWithThreeDigits;
    map<string, double> eachDifficultyCorrects;
    map<string, double> eachDifficultyIncorrects;
    map<string, double> eachDifficultyBlank;
};

void sortSubjects(vector<shared_ptr<Subject>>& subjects, bool needScore);

vector<shared_ptr<Subject>> processSubjects(const vector<shared_ptr<Question>> questions);

#endif //SUBJECT_H