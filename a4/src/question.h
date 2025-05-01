#ifndef QUESTION_H
#define QUESTION_H

#include <vector>
#include <string>
#include <memory>

using namespace std;

class Question{
public:
    Question(string questionText_, string option1_, string option2_, string option3_, string option4_, int correctAnswer_, string difficulty_, string subject_);

    void setQuestion(string questionText_, string option1_, string option2_, string option3_, string option4_, int correctAnswer_, string difficulty_, string subject_);

    string printQuestion(int questionCounter);

    string evaluateTheQuestion();

    string getQuestionText() const;
    string getSubject() const;
    string getDifficulty() const;
    int getPriority() const;
    int getNumOfcorrect() const;
    int getNumOfIncorrect() const;
    int getNumOfBlank() const;
    int getNumOfOccurrence() const;

private:
    string questionText;
    vector<string> options;
    int correctAnswer;
    string difficulty;
    string subject;
    int numOfIncorrect;
    int numOfBlank;
    int numOfCorrect;
    int priority;
    int numOfOccurrence;
    string theAnswer;
};

void sortQuestions(vector<shared_ptr<Question>>& questions, bool needPriority);

vector<shared_ptr<Question>> processQuestionsFile(const string questionsFileAddress);

#endif //QUESTION_H