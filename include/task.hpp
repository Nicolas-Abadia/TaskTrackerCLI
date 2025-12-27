#ifndef TASK_H
#define TASK_H

#include <string>
#include <ctime>
#include <chrono>


// Classes can substitue structs in almost all cases. The main difference is that struct members are public by default while in classes they are private. 
struct Task {
  int id;
  std::string description;
  std::string status;
  std::string createdAt;
  std::string updatedAt;
};

#endif