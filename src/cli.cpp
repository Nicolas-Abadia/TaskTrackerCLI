#include "../include/cli.hpp"

void CLI::addTask(const std::string& description){
  Task newTask;
  newTask.id = getLastId("tasks.json")+1;
  newTask.description = description;
  newTask.status = "todo";
  newTask.createdAt = getCurrentTime();
  newTask.updatedAt = getCurrentTime();
  writeTaskToJson(newTask, "tasks.json");
  std::cout << "Task added!" << std::endl;
}

void CLI::listTasks()
{
    std::cout << "List of Tasks: " << std::endl;
    std::vector<Task> tasks = parseJsonFile("tasks.json");
    for (const auto& task : tasks) {
        if (!task.description.empty()) {
            std::cout << "ID: " << task.id <<  " | Status: " << task.status << " | Description: " << task.description << " | createdAt: " << task.createdAt << " | updatedAt: "  << task.updatedAt << std::endl;
        } else {
            std::cout << "Task has no description" << std::endl;
        }
    }
}
void CLI::deleteTask(int id)
{
    std::vector<Task> updatedTasks;
    std::vector<Task> tasks = parseJsonFile("tasks.json");
    //copy tasks before the target id
    for (int i = 1; i < id;  i++) {
        if (i != id) {
            updatedTasks.push_back(tasks[i-1]);
        }
    }
    for (int i = id + 1; i <= (int)tasks.size();  i++) {
        //tasks[i - 1].id = i - 1;
        updatedTasks.push_back(tasks[i-1]);
    }
    // write to file safely using a temp file
    for  (const auto& task : updatedTasks) {
        writeTaskToJson(task, "tasks.json.tmp");
    }

    remove("tasks.json");
    rename("tasks.json.tmp", "tasks.json");
    std::cout << "Task deleted!" << std::endl;
}

void CLI::updateTask(int id, std::string &description)
{
    std::vector<Task> updatedTasks;
    std::vector<Task> tasks = parseJsonFile("tasks.json");
    for (auto & task : tasks) {
        if(task.id == id) {
            task.description = description;
            task.updatedAt =  getCurrentTime();
            updatedTasks.push_back(task);
        } else {
            updatedTasks.push_back(task);
        }
    }
    for  (const auto& task : updatedTasks) {
        writeTaskToJson(task, "tasks.json.tmp");
    }
    remove("tasks.json");
    rename("tasks.json.tmp", "tasks.json");

    std::cout << "Task updated" << std::endl;
}

void CLI::markInProgress(int id)
{
    std::vector<Task> updatedTasks;
    std::vector<Task> tasks = parseJsonFile("tasks.json");
    for (auto & task : tasks) {
        if(task.id == id) {
            task.status = "in-progress";
            task.updatedAt =  getCurrentTime();
            updatedTasks.push_back(task);
        } else {
            updatedTasks.push_back(task);
        }
    }
    for  (const auto& task : updatedTasks) {
        writeTaskToJson(task, "tasks.json.tmp");
    }
    remove("tasks.json");
    rename("tasks.json.tmp", "tasks.json");
    std::cout << "Task was marked as in-progress!" << std::endl;
}

void CLI::markDone(int id)
{
    std::vector<Task> updatedTasks;
    std::vector<Task> tasks = parseJsonFile("tasks.json");
    for (auto & task : tasks) {
        if(task.id == id) {
            task.status = "done";
            task.updatedAt =  getCurrentTime();
            updatedTasks.push_back(task);
        } else {
            updatedTasks.push_back(task);
        }
    }
    for  (const auto& task : updatedTasks) {
        writeTaskToJson(task, "tasks.json.tmp");
    }
    remove("tasks.json");
    rename("tasks.json.tmp", "tasks.json");
    std::cout << "Task was marked as done!" << std::endl;
}

void CLI::listTaskByStatus(const std::string &status)
{
    std::cout << "List Of Tasks " << status << ":" << std::endl;
    std::vector<Task> tasks = parseJsonFile("tasks.json");
    for (const auto& task : tasks) {
        if (task.status == status) { // if tasks status and status passed by the user to be used as a filter match.
            std::cout << "ID: " << task.id <<  " | Status: " << task.status << " | Description: " << task.description << " | createdAt: " << task.createdAt << " | updatedAt: "  << task.updatedAt << std::endl;
        }
    }
}