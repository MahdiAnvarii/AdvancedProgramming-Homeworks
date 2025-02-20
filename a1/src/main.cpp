#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

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

void reportAll(map<string,string> taskState){
    int numberOfTodo = 0;
    int numberOfOngoing = 0;
    int numberOfDone = 0;
    const string todo = "TODO";
    const string ongoing = "ONGOING";
    const string done = "DONE";
    for (map<string,string>::iterator it = taskState.begin(); it != taskState.end(); ++it){
        if ((*it).second == todo){
            numberOfTodo+=1;
        }
        else if ((*it).second == ongoing){
            numberOfOngoing+=1;
        }
        else{
            numberOfDone+=1;
        }
    }
    cout << todo << ": " << numberOfTodo << "\n";
    cout << ongoing << ": " << numberOfOngoing << "\n";
    cout << done << ": " << numberOfDone << "\n";
}

bool customComparator(pair<string,int> a, pair<string,int> b) {
    if (a.second != b.second)
        return a.second < b.second;
    return a.first < b.first;
}

void printReportOngoing(string task, int prior, map<string,string> ongoingEmployee){
    vector<string> employeesForTheTask;
    for (map<string,string>::iterator it = ongoingEmployee.begin(); it != ongoingEmployee.end(); ++it){
        if ((*it).first == task){
            employeesForTheTask.push_back((*it).second);
        }
    }
    sort(employeesForTheTask.begin(), employeesForTheTask.end());
    string employeesForTheTaskString;
    for (vector<string>::iterator it = employeesForTheTask.begin(); it != employeesForTheTask.end(); ++it){
        if (it == employeesForTheTask.begin()){
            employeesForTheTaskString = employeesForTheTaskString + *it;
        }
        else{
            employeesForTheTaskString = employeesForTheTaskString + ", " + (*it);   
        }
    }  
    cout << task << " (" << prior << ") : "  << employeesForTheTaskString << "\n";
}

void organizeReportOngoing(map<string,int> ongoingPriorityMap, map<string,string> ongoingEmployee){
    vector<pair<string, int>> ongoingPriorityVec(ongoingPriorityMap.begin(), ongoingPriorityMap.end());
    sort(ongoingPriorityVec.begin(), ongoingPriorityVec.end(), customComparator);
    for (map<string,int>::iterator it = ongoingPriorityMap.begin(); it != ongoingPriorityMap.end(); ++it){
        printReportOngoing((*it).first, (*it).second, ongoingEmployee);
    }
}

void reportOngoing(map<string,string> taskState, map<string,int> priorityMap, map<string,string> taskToEmployee){
    const string ongoing = "ONGOING";
    set<string> ongoingTasks;
    for (map<string,string>::iterator it = taskState.begin(); it != taskState.end(); ++it){
        if ((*it).second == ongoing){
            ongoingTasks.insert((*it).first);
        }
    }
    map<string,int> ongoingPriorityMap;
    for (map<string,int>::iterator it = priorityMap.begin(); it != priorityMap.end(); ++it){
        if (ongoingTasks.count((*it).first) > 0){
            ongoingPriorityMap[(*it).first] = (*it).second;
        }
    }
    map<string,string> ongoingEmployee;
    for (map<string,string>::iterator it = taskToEmployee.begin(); it != taskToEmployee.end(); ++it){
        if (ongoingTasks.count((*it).first) > 0){
            ongoingEmployee[(*it).first] = (*it).second;
        }
    }
    organizeReportOngoing(ongoingPriorityMap, ongoingEmployee);
}

void printReportEmployee(string employee, int doneByEmployee, vector<string> employeeNotDone){
    cout << employee << " has done " << doneByEmployee << " tasks." << "\n";
    if (employeeNotDone.empty()){
        cout << employee << " is currently not working on any tasks." << "\n";     
    }
    else{
        cout << employee << " is currently working on these tasks:" << "\n";
        for (int i=0; i < employeeNotDone.size(); ++i){
            cout << i+1 << ". " << employeeNotDone[i] << "\n";     
        }
    }
}

void reportEmployee(string employee, map<string,string> taskState, map<string,string> taskToEmployee){
    vector<string> employeeTasks;
    for (map<string,string>::iterator it = taskToEmployee.begin(); it != taskToEmployee.end(); ++it){
        if ((*it).second == employee){
            employeeTasks.push_back((*it).first);
        }
    }
    const string done = "DONE";
    int doneByEmployee = 0;  
    vector<string> employeeNotDone;  
    for (map<string,string>::iterator it = taskState.begin(); it != taskState.end(); ++it){
        if (count(employeeTasks.begin(), employeeTasks.end(), (*it).first) > 0){
            if ((*it).second == done){
                doneByEmployee+=1;
            }
            else{
                employeeNotDone.push_back((*it).first);
            }
        }
    }
    printReportEmployee(employee, doneByEmployee, employeeNotDone);    
}

int main(){
    map<string,int> priorityMap;
    map<string,string> taskState;
    set<string> employees;
    map<string,string> taskToEmployee;
    const string todo = "TODO";
    const string ongoing = "ONGOING";
    const string done = "DONE";

    string order;
    while (getline(cin,order)){
        vector<string> ordersVector = splitTheOrder(order);
        if (ordersVector[0] == "add_task"){
            priorityMap[ordersVector[1]] = stoi(ordersVector[2]);
            taskState[ordersVector[1]] = todo;
        }

        if (ordersVector[0] == "add_employee"){
            employees.insert(ordersVector[1]);
        }

        if (ordersVector[0] == "assign_employee"){
            taskToEmployee[ordersVector[1]] = ordersVector[2];
            taskState[ordersVector[1]] = ongoing;
        }    

        if (ordersVector[0] == "finish_task"){
            taskState[ordersVector[1]] = done;
        }      

        if (ordersVector[0] == "report" && ordersVector[1] == "all"){
            reportAll(taskState);
        }  

        if (ordersVector[0] == "report" && ordersVector[1] == "ongoing"){
            reportOngoing(taskState, priorityMap, taskToEmployee);
        }
        
        if (ordersVector[0] == "report" && ordersVector[1] == "employee"){
            reportEmployee(ordersVector[2], taskState, taskToEmployee);
        }
    }    
}