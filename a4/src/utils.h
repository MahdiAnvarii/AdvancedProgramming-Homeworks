#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

using namespace std;

extern const string CREATE_TEMPLATE;
extern const string GENERATE_TEST;
extern const string ATTEND;
extern const string AUTO_GENERATE;
extern const string REPORT;
extern const string ALL;
extern const string TEST;
extern const string TESTS;
extern const string SUBJECT;
extern const vector<string> VALID_DIFFICULTIES;
extern const string PREVIOUS;
extern const string CORRECT;
extern const string INCORRECT;
extern const string BLANK;
extern const vector<string> VALID_ANSWERS;
extern const double TRUNCATION_FACTOR;
extern const double PERCENTAGE_FACTOR;
extern const vector<string> HARD_SUBJECT_FOR_USER_1;
extern const vector<string> HARD_SUBJECT_FOR_USER_2;
extern const string RANDOM_TEMPLATE_CHARACTERS;
extern const int RANDOM_TEMPLATE_LENGTH;
extern const string NO_INPUT_FILE;

struct QuestionsTemplateStruct{
    string subject;
    string difficulty;
    int count;
};

void unsetThreeDigits();

double truncationWithThreeDigits(double score);

string generateRandomTemplateName();

vector<string> splitTheOrder(string order);

#endif //UTILS_H
