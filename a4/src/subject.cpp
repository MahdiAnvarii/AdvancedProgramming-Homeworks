#include "utils.h"
#include "question.h"
#include "subject.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;

Subject::Subject(string subjectName_, const vector<shared_ptr<Question>>& questions){
    setSubjectName(subjectName_);
    vector<shared_ptr<Question>> subjectQuestions_ = selectQuestionsForSubject(questions);
    setSubjectQuestions(subjectQuestions_);
}
void Subject::setSubjectName(string subjectName_){
    subjectName = subjectName_;
}
void Subject::setSubjectQuestions(const vector<shared_ptr<Question>>& subjectQuestions_){
    subjectQuestions = subjectQuestions_;
    totalCorrects=0.0;
    totalIncorrects=0.0;
    totalBlank=0.0;
    totalOccurrence=0.0;
    score=0.0;
}
vector<shared_ptr<Question>> Subject::selectQuestionsForSubject(const vector<shared_ptr<Question>> questions){
    vector<shared_ptr<Question>> subjectQuestions_;
    for (const auto& question : questions){
        if (subjectName == question->getSubject()){
            subjectQuestions_.push_back(question);
        }
    }
    return subjectQuestions_;
}
void Subject::calculateScoreForSubject(){
    totalCorrects=0.0;
    totalIncorrects=0.0;
    totalBlank=0.0;
    totalOccurrence=0.0;
    for (const auto difficulty_ : VALID_DIFFICULTIES){
        eachDifficultyCorrects[difficulty_]=0.0;
        eachDifficultyIncorrects[difficulty_] =0.0;
        eachDifficultyBlank[difficulty_]=0.0;
    }
    for (const auto& subjectQuestion : subjectQuestions){
        totalCorrects+=subjectQuestion->getNumOfcorrect();
        totalIncorrects+=subjectQuestion->getNumOfIncorrect();
        totalBlank+=subjectQuestion->getNumOfBlank();
        totalOccurrence+=subjectQuestion->getNumOfOccurrence();

        string difficulty_ = subjectQuestion->getDifficulty();
        eachDifficultyCorrects[difficulty_]+=subjectQuestion->getNumOfcorrect();
        eachDifficultyIncorrects[difficulty_]+=subjectQuestion->getNumOfIncorrect();
        eachDifficultyBlank[difficulty_]+=subjectQuestion->getNumOfBlank();
    }
    if (totalOccurrence) score = totalCorrects/totalOccurrence;
}
void Subject::totalReport(){
    scoreWithThreeDigits = truncationWithThreeDigits(score);
    cout << subjectName << ": " << totalCorrects << " corrects, " << totalIncorrects << " incorrects and " <<
    totalBlank << " blanks. Score: " << fixed << setprecision(3) << scoreWithThreeDigits << "%." << endl;
    unsetThreeDigits();
}
void Subject::reportSubject(){
    for (auto difficulty_ : VALID_DIFFICULTIES){
        string difficultyUpperCased_ = difficulty_;
        difficultyUpperCased_[0] = toupper(difficultyUpperCased_[0]);
        cout << difficultyUpperCased_ << ": " << eachDifficultyCorrects[difficulty_] << " corrects, " << eachDifficultyIncorrects[difficulty_] 
        << " incorrects and " << eachDifficultyBlank[difficulty_] << " blanks." << endl;
    }
    scoreWithThreeDigits = truncationWithThreeDigits(score);
    cout << endl << "Total score: " << fixed << setprecision(3) << scoreWithThreeDigits << "%." << endl;
    unsetThreeDigits();
}
string Subject::getSubjectName() const { return subjectName; }
double Subject::getSubjectScore() const { return score; }
double Subject::getSubjectTotalCorrects() const { return totalCorrects; }
double Subject::getSubjectTotalBlank() const { return totalBlank; }
double Subject::getSubjectTotalIncorrects() const { return totalIncorrects; }
double Subject::getSubjectTotalOccurrence() const { return totalOccurrence; }

void sortSubjects(vector<shared_ptr<Subject>>& subjects, bool needScore){
    sort(subjects.begin(), subjects.end(), [needScore](const shared_ptr<Subject> a, const shared_ptr<Subject> b) {
        if (needScore && a->getSubjectScore() != b->getSubjectScore()) {
            return a->getSubjectScore() < b->getSubjectScore();
        }
        return a->getSubjectName() < b->getSubjectName();
    });
}

vector<shared_ptr<Subject>> processSubjects(const vector<shared_ptr<Question>> questions){
    vector<shared_ptr<Subject>> subjects;
    vector<string> subjectNames;
    for (auto& question : questions){
        string subjectName = question->getSubject();
        if (find(subjectNames.begin(), subjectNames.end(), subjectName) == subjectNames.end()){
            subjectNames.push_back(subjectName);
            shared_ptr<Subject> subject = make_shared<Subject>(subjectName, questions);
            subjects.push_back(subject);
        }
    }
    sortSubjects(subjects, false);
    return subjects;
}