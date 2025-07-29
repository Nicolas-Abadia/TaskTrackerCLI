#ifndef JSON_H
#define JSON_H

#include "fstream" // what is that??? 
#include "cli.hpp"



void writeTaskToJson(struct Task task, const char* filename);
std::vector<Task> parseJsonFile(const std::string& filename);

#endif  // JSON_H