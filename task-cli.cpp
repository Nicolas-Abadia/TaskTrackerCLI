#include <iostream>
#include <fstream>
#include <chrono>

/** 
 class task
  {
  private:
    int id;

  public:
    // constructor and destructor
    task();
    ~task();
  };
 **/
  
int num_task(){
  int num_task = 1; 
  std::string filename = "1.json";
  std::ifstream json(filename);
  while (json)
  {
    num_task++;
    filename = std::to_string(num_task)+".json"; 
    
    json.close();
    std::ifstream json(filename);
  }
  json.close();
  return num_task;
}


int main(int argc, char const *argv[]) {
  int num_tasks = 0;
  std::cout << "CLI Task Tracker\n";
  if (argc < 2) {
    std::cout << "Usage: task-cli <command>\n";
    return 1;
  }
  if (std::string(argv[1]) == "add") {
    if (argc != 3)
    {
      std::cout << "Type a task description!\n";
    }
    auto desc = argv[2];
    // Write json file
    // I have a problem. how do i know the id of the task i am adding every time i run the program?
    // it should be based on the previous tasks added or the json files that were created. but how do i get this info?
    // maybe i should loop over the files? 
    int num = num_task();
    
    std::string filename = std::to_string(num) + ".json";
    std::ofstream json(filename);

    if (!json) {
        std::cerr << "Error opening file for writing.\n";
        return 1;
    }
    
    json << "{" << std::endl;
    json << "\"id\":" << num << "," << std::endl;
    json << "\"description\":" << "\"" << desc << "\""<< "," <<  std::endl;
    json << "\"status\":" << "\"todo\""<< "," << std::endl;
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);
    std::string time = std::ctime(&timestamp);
    time.pop_back();
    json << "\"createdAt\":"<< "\"" << time << "\""<< "," << std::endl;
    json << "\"updatedAt\":"<< "\"" << time << "\"" << std::endl;
    json << "}";
    json.close();
    std::cout << "Task " << desc << " recorded!\n";

  } 
  return 0;
}