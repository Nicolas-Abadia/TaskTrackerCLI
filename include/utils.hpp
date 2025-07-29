#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <ctime>
#include <chrono>
#include <vector>
#include <iomanip>
#include "task.hpp"
#include "json.hpp"

std::string getCurrentTime();
int getLastId(const std::string& filename);


#endif // UTILS_H