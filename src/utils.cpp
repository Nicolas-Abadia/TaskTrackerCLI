#include "../include/utils.hpp"
//Returns the current time as a string in the format "YYYY-MM-DD HH:MM:SS".
std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    time_t nowTimeT = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&nowTimeT);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    std::string currentTime = oss.str();
    return currentTime;
}

int getLastId(const std::string& filename) {
    int maxId = 0;
    std::vector<Task> tasks = parseJsonFile(filename);
    for (const auto& task : tasks) {
        if (task.id > maxId) {
            maxId = task.id;
        } 
    }
    return maxId;
}

