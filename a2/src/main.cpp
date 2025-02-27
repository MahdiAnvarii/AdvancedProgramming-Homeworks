#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>

using namespace std;

struct TableInfo{
    int tableX;
    int tableY;
    int tableCapacity;
    string tableType;
    list<string> peopleAtTable;
    list<string> peopleWaiting;
};

struct StudentInfo{
    string studentName;
    int friendID;
    int enemyID;
    int assignedTable;
    bool isWaiting;
};

vector<string> splitTheLine(string lineString){
    vector<string> lineToVector;
    string lineSection;
    for (string::iterator it = lineString.begin(); it != lineString.end(); ++it){
        if (*it == ',' || *it == lineString.back()){
            lineToVector.push_back(lineSection);
            lineSection.clear();
        } else{
            lineSection = lineSection + *it;
        }
    }
    lineToVector.push_back(lineSection);
    return lineToVector;
}

map<int,TableInfo> processTablesFile(string tablesFileName){
    ifstream tablesFile(tablesFileName);
    string tablesFileLine;
    map <int,TableInfo> tablesInformationMap;
    getline(tablesFile, tablesFileLine);
    while (getline(tablesFile, tablesFileLine)){
        int tableID;
        TableInfo tableInfo;
        vector<string> tableInfoVector = splitTheLine(tablesFileLine);
        for (int i=0 ; i<tableInfoVector.size() ; i++){
            if (i==0){
                tableID = stoi(tableInfoVector[0]);
            } else if (i==1){
                tableInfo.tableX = stoi(tableInfoVector[1]);
            } else if (i==2){
                tableInfo.tableY = stoi(tableInfoVector[2]);
            } else if (i==3){
                tableInfo.tableCapacity = stoi(tableInfoVector[3]);
            } else{
                tableInfo.tableType = tableInfoVector[4];
                tableInfo.peopleAtTable = {};
                tableInfo.peopleWaiting = {};
            }
        }
        tablesInformationMap[tableID] = tableInfo;
    }
    tablesFile.close();
    return tablesInformationMap;
}

map<int,StudentInfo> processStudentsFile(string studentsFileName){
    ifstream studentsFile(studentsFileName);
    string studentsFileLine;
    map <int,StudentInfo> studentsInformationMap;
    getline(studentsFile, studentsFileLine);
    while (getline(studentsFile, studentsFileLine)){
        int studentID;
        StudentInfo studentInfo;
        vector<string> studentInfoVector = splitTheLine(studentsFileLine);
        for (int i=0 ; i<studentInfoVector.size() ; i++){
            if (i==0){
                studentID = stoi(studentInfoVector[0]);
            } else if (i==1){
                studentInfo.studentName = studentInfoVector[1];
            } else if (i==2){
                studentInfo.friendID = stoi(studentInfoVector[2]);
            } else{
                studentInfo.enemyID = stoi(studentInfoVector[3]);
                studentInfo.assignedTable = 0;
                studentInfo.isWaiting = false;
            } 
        }
        studentsInformationMap[studentID] = studentInfo;
    }
    studentsFile.close();
    return studentsInformationMap;
}

int main(int argc, char* argv[]){
    map<int,TableInfo> tablesInformationMap = processTablesFile(argv[1]);
    map<int,StudentInfo> studentsInformationMap = processStudentsFile(argv[2]);

    return 0;
}
