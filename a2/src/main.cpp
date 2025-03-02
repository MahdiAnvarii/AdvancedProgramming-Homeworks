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
    list<int> peopleAtTableID;
    list<string> peopleWaiting;
    list<int> peopleWaitingID;
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

vector<string> splitTheOrder(string orderString){
    vector<string> orderToVector;
    string orderSection;
    for (string::iterator it = orderString.begin(); it != orderString.end(); ++it){
        if (*it == ' '){
            orderToVector.push_back(orderSection);
            orderSection.clear();
        }
        else{
            orderSection = orderSection + *it;
        }
    }
    orderToVector.push_back(orderSection);
    return orderToVector;
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
                tableInfo.peopleAtTableID = {};
                tableInfo.peopleWaitingID = {};
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

void showTableInfo(int tableID, TableInfo tableInfo){
    cout << "Table ID: " << tableID << endl;
    string peopleAtTheTable;
    tableInfo.peopleAtTable.sort();
    for (list<string>::iterator it = tableInfo.peopleAtTable.begin(); it != tableInfo.peopleAtTable.end(); ++it){
        if (it == tableInfo.peopleAtTable.begin()){
            peopleAtTheTable = peopleAtTheTable + *it;
        }
        else{
            peopleAtTheTable = peopleAtTheTable + ", " + (*it);   
        }
    } 
    cout << "People at the table: " << peopleAtTheTable << endl;
    cout << "Table remaining capacity: " << tableInfo.tableCapacity << endl;
    cout << "Waiting queue length: " << tableInfo.peopleWaiting.size() << endl;
}

int scoreCalculation(int friendTable, int enemyTable, map<int,TableInfo> tablesInformationMap, TableInfo theTable){
    int x1, y1, x2, y2, dist1, dist2;
    map<string,int> bonusPoint = {{"window", 6}, {"door", 4}, {"middle", 2}};
    if (friendTable==0){
        dist1 = 0;
    } else{
        x1 = tablesInformationMap[friendTable].tableX;
        y1 = tablesInformationMap[friendTable].tableY;
        dist1 = abs(x1-theTable.tableX) + abs(y1-theTable.tableY);
    }
    if (enemyTable==0){
        dist2 = 0;
    } else{
        x2 = tablesInformationMap[enemyTable].tableX;
        y2 = tablesInformationMap[enemyTable].tableY;
        dist2 = abs(x2-theTable.tableX) + abs(y2-theTable.tableY);
    }
    int score =  dist2 - dist1 + bonusPoint[theTable.tableType];
    return score;
}

bool customComparator(pair<int,int> a, pair<int,int> b) {
    if (a.second != b.second)
        return a.second > b.second;
    return a.first < b.first;
}

int enterTheStudent(int studentID, map<int,StudentInfo> studentsInformationMap, map<int,TableInfo> tablesInformationMap, bool needDescription = true){
    map<int,int> tablePriority;
    int friendTable = studentsInformationMap[studentsInformationMap[studentID].friendID].assignedTable;
    int enemyTable = studentsInformationMap[studentsInformationMap[studentID].enemyID].assignedTable;
    for (map<int,TableInfo>::iterator it = tablesInformationMap.begin(); it != tablesInformationMap.end(); ++it){
        int score = scoreCalculation(friendTable, enemyTable, tablesInformationMap, (*it).second);
        tablePriority[(*it).first] = score;
    }
    vector<pair<int, int>> tablePriorityVec(tablePriority.begin(), tablePriority.end());
    sort(tablePriorityVec.begin(), tablePriorityVec.end(), customComparator);
    if (needDescription){
        for (vector<pair<int,int>>::iterator it = tablePriorityVec.begin(); it != tablePriorityVec.end(); ++it){
            cout << "Table " << (*it).first << ": " << tablesInformationMap[(*it).first].tableCapacity << " " << tablesInformationMap[(*it).first].peopleWaiting.size() << endl;
        }
    }
    return tablePriorityVec[0].first;
}

void reserveTable(map<int,StudentInfo> &studentsInformationMap, map<int,TableInfo> &tablesInformationMap, int studentID, int tableID = 0, bool needDescription = true){
    if (!tableID){
        tableID = enterTheStudent(studentID , studentsInformationMap, tablesInformationMap, false);
    }
    studentsInformationMap[studentID].assignedTable = tableID;
    if (tablesInformationMap[tableID].tableCapacity > 0){
        tablesInformationMap[tableID].tableCapacity--;
        tablesInformationMap[tableID].peopleAtTable.push_back(studentsInformationMap[studentID].studentName);
        tablesInformationMap[tableID].peopleAtTableID.push_back(studentID);
        studentsInformationMap[studentID].isWaiting = false;
        if (needDescription){
            cout << studentsInformationMap[studentID].studentName << " sits at table " << tableID << endl;
        }
    } else{
        tablesInformationMap[tableID].peopleWaiting.push_back(studentsInformationMap[studentID].studentName);
        tablesInformationMap[tableID].peopleWaitingID.push_back(studentID);
        studentsInformationMap[studentID].isWaiting = true;
        cout << studentsInformationMap[studentID].studentName << " enters the waiting queue of table " << tableID << endl;
    }
}

void exitTheStudent(int studentID, map<int,StudentInfo> &studentsInformationMap, map<int,TableInfo> &tablesInformationMap){
    if (studentsInformationMap[studentID].assignedTable > 0){
        int tableID = studentsInformationMap[studentID].assignedTable;
        if (studentsInformationMap[studentID].isWaiting){
            tablesInformationMap[tableID].peopleWaiting.remove(studentsInformationMap[studentID].studentName);
            tablesInformationMap[tableID].peopleWaitingID.remove(studentID);
        } else{
            tablesInformationMap[tableID].peopleAtTable.remove(studentsInformationMap[studentID].studentName);
            tablesInformationMap[tableID].peopleAtTableID.remove(studentID);
            tablesInformationMap[tableID].tableCapacity++;
            string friendName = studentsInformationMap[studentsInformationMap[studentID].friendID].studentName;
            if (find(tablesInformationMap[tableID].peopleWaiting.begin(), tablesInformationMap[tableID].peopleWaiting.end(), friendName) != tablesInformationMap[tableID].peopleWaiting.end()) {
                tablesInformationMap[tableID].peopleWaiting.remove(studentsInformationMap[studentsInformationMap[studentID].friendID].studentName);
                tablesInformationMap[tableID].peopleWaitingID.remove(studentsInformationMap[studentID].friendID);       
                reserveTable(studentsInformationMap, tablesInformationMap, studentsInformationMap[studentID].friendID, tableID, false);
            } else{
                tablesInformationMap[tableID].peopleWaiting.remove(studentsInformationMap[tablesInformationMap[tableID].peopleWaitingID.front()].studentName);
                tablesInformationMap[tableID].peopleWaitingID.remove(tablesInformationMap[tableID].peopleWaitingID.front());       
                reserveTable(studentsInformationMap, tablesInformationMap, tablesInformationMap[tableID].peopleWaitingID.front(), tableID, false);
            }
        }
        studentsInformationMap[studentID].assignedTable = 0;
        studentsInformationMap[studentID].isWaiting = false;
    }
    cout << studentsInformationMap[studentID].studentName << " exits!" << endl;
}

void switchTheStudents(int studentID, map<int,StudentInfo> &studentsInformationMap, map<int,TableInfo> &tablesInformationMap){
    int theStudentTable = studentsInformationMap[studentID].assignedTable;
    int theFriendTable = studentsInformationMap[studentsInformationMap[studentID].friendID].assignedTable;
    tablesInformationMap[theStudentTable].peopleAtTable.remove(studentsInformationMap[studentID].studentName);
    tablesInformationMap[theStudentTable].peopleAtTableID.remove(studentID);
    tablesInformationMap[theStudentTable].peopleAtTable.push_back(studentsInformationMap[studentsInformationMap[studentID].friendID].studentName);
    tablesInformationMap[theStudentTable].peopleAtTableID.push_back(studentsInformationMap[studentID].friendID);
    tablesInformationMap[theFriendTable].peopleAtTable.remove(studentsInformationMap[studentsInformationMap[studentID].friendID].studentName);
    tablesInformationMap[theFriendTable].peopleAtTableID.remove(studentsInformationMap[studentID].friendID);
    tablesInformationMap[theFriendTable].peopleAtTable.push_back(studentsInformationMap[studentID].studentName);
    tablesInformationMap[theFriendTable].peopleAtTableID.push_back(studentID);
    studentsInformationMap[studentID].assignedTable = theFriendTable;
    studentsInformationMap[studentsInformationMap[studentID].friendID].assignedTable = theStudentTable;
    cout << studentsInformationMap[studentID].studentName << " switches seats with " << studentsInformationMap[studentsInformationMap[studentID].friendID].studentName << "!" << endl;
}

int main(int argc, char* argv[]){
    map<int,TableInfo> tablesInformationMap = processTablesFile(argv[1]);
    map<int,StudentInfo> studentsInformationMap = processStudentsFile(argv[2]);

    string order;
    while (getline(cin,order)){
        vector<string> ordersVector = splitTheOrder(order);
        if (ordersVector[0] == "show_table_info"){
            showTableInfo(stoi(ordersVector[1]), tablesInformationMap[stoi(ordersVector[1])]);
        }

        if (ordersVector[0] == "enter"){
            enterTheStudent(stoi(ordersVector[1]), studentsInformationMap, tablesInformationMap);
        }

        if (ordersVector[0] == "reserve_table"){
            if (ordersVector.size() >= 3) {
                reserveTable(studentsInformationMap, tablesInformationMap, stoi(ordersVector[1]), stoi(ordersVector[2]));
            } else {
                reserveTable(studentsInformationMap, tablesInformationMap, stoi(ordersVector[1]));
            }
        }

        if (ordersVector[0] == "exit"){
            exitTheStudent(stoi(ordersVector[1]), studentsInformationMap, tablesInformationMap);
        }

        if (ordersVector[0] == "switch"){
            switchTheStudents(stoi(ordersVector[1]), studentsInformationMap, tablesInformationMap);
        }
    }
    return 0;
}
