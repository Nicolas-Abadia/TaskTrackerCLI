#ifndef CLI_H
#define CLI_H

#include <string>
#include <vector>
#include  <iostream>
#include "task.hpp"
#include "utils.hpp"
#include "json.hpp"

class CLI {
public:
    void addTask(const std::string& description);
    void listTasks();
    void deleteTask(int id);
    void updateTask(int id, std::string& description);
    void markInProgress(int id);
    void markDone(int id);
    void listTaskByStatus(const std::string &status);
private:  
    std::vector<Task> tasks;
    std::string jsonFile;
};


#endif