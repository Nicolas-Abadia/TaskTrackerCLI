#include "../include/json.hpp"

//Writes a Task object to a JSON file.
void writeTaskToJson(struct Task task, const char* filename) {
  std::ifstream file(filename);
  std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  file.close();
  size_t startPos = jsonContent.find_last_of('[') + 1;
  size_t endPos = jsonContent.find_last_of(']');

  std::string newTaskJson =
    "\n    {\n"
    "        \"id\": " + std::to_string(task.id) + ",\n"
    "        \"description\": \"" + task.description + "\",\n"
    "        \"status\": \"" + task.status + "\",\n"
    "        \"createdAt\": \"" + task.createdAt + "\",\n"
    "        \"updatedAt\": \"" + task.updatedAt + "\"\n"
    "    }\n";
    if (startPos != std::string::npos && endPos != std::string::npos) {
        jsonContent.insert(endPos, "," + newTaskJson);
    } else {
        jsonContent = "[" + newTaskJson + "]";
    }

    std::ofstream fileOut(filename, std::ios::trunc);
    fileOut << jsonContent;
    fileOut.close();
}

//Parses a JSON file and returns a vector of Task objects.
std::vector<Task> parseJsonFile(const std::string& filename) {
    std::vector<Task> tasks;

    std::ifstream file(filename);
    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    size_t startPos = jsonContent.find('[') + 1;
    size_t endPos = jsonContent.find_last_of(']');

    if (startPos != std::string::npos && endPos != std::string::npos) {
        std::string tasksJson = jsonContent.substr(startPos, endPos - startPos);

        size_t taskStartPos = 0;
        size_t taskEndPos = tasksJson.find('}');

        while (taskEndPos != std::string::npos) {
            std::string taskJson = tasksJson.substr(taskStartPos, taskEndPos - taskStartPos + 1);

            Task task;
            size_t idStartPos = taskJson.find("\"id\": ") + 6;
            size_t idEndPos = taskJson.find(',', idStartPos);
            task.id = std::stoi(taskJson.substr(idStartPos, idEndPos - idStartPos));

            size_t descStartPos = taskJson.find("\"description\": \"") + 16;
            size_t descEndPos = taskJson.find('\"', descStartPos);
            task.description = taskJson.substr(descStartPos, descEndPos - descStartPos);


            size_t statusStartPos = taskJson.find("\"status\": \"") + 11;
            size_t statusEndPos = taskJson.find('\"', statusStartPos);
            task.status = taskJson.substr(statusStartPos, statusEndPos - statusStartPos);

            size_t createdAtStartPos = taskJson.find("\"createdAt\": \"") + 14;
            size_t createdAtEndPos = taskJson.find('\"', createdAtStartPos);
            task.createdAt = taskJson.substr(createdAtStartPos, createdAtEndPos - createdAtStartPos);

            size_t updatedAtStartPos = taskJson.find("\"updatedAt\": \"") + 14;
            size_t updatedAtEndPos = taskJson.find('\"', updatedAtStartPos);
            task.updatedAt = taskJson.substr(updatedAtStartPos, updatedAtEndPos - updatedAtStartPos);

            tasks.push_back(task);

            taskStartPos = taskEndPos + 1;
            taskEndPos = tasksJson.find('}', taskStartPos);
        }
    }

    return tasks;
}