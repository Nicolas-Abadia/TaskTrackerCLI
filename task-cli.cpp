#include <iostream>
#include <fstream>
#include <chrono>
#include <filesystem>

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


// Returns the number of existing tasks
int num_task(){
  int count = 0;
  int task_id = 1; 
  std::string filename = std::to_string(task_id) + ".json";
  
  // Count existing task files
  while (std::filesystem::exists(filename))
  {
    count++;
    task_id++;
    filename = std::to_string(task_id) + ".json"; 
  }
  
  return count;
}


void task_add(char const *argv[]){
  auto desc = argv[2];
    // Write json file
    // I have a problem. how do i know the id of the task i am adding every time i run the program?
    // it should be based on the previous tasks added or the json files that were created. but how do i get this info?
    // maybe i should loop over the files? 
    int num = num_task() + 1;
    
    std::string filename = std::to_string(num) + ".json";
    std::ofstream json(filename);

    if (!json) {
        std::cerr << "Error opening file for writing.\n";
        return;
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

void task_list(char const *argv[]) {
  int total_tasks = num_task(); // Get the actual number of existing tasks
  bool found_any = false;
  
  std::cout << "Tasks:\n";
  std::cout << "------\n";
  
  if (total_tasks == 0) {
    std::cout << "No tasks found.\n";
    return;
  }
  
  // Loop through all possible task IDs starting from 1
  int found_count = 0;
  for (int i = 1; found_count < total_tasks; i++) {
    std::string filename = std::to_string(i) + ".json";
    
    // Check if the file exists before trying to open it
    if (std::filesystem::exists(filename)) {
      found_count++;
      std::ifstream task(filename);
      if (task.is_open()) {
        std::string line;
        std::string content;
        
        // Read the entire file content
        while (std::getline(task, line)) {
          content += line;
        }
        task.close();
        
        // Parse and display basic info (simple string parsing)
        // Extract ID
        size_t id_pos = content.find("\"id\":");
        if (id_pos != std::string::npos) { //error handling
          size_t id_start = content.find(":", id_pos) + 1;
          size_t id_end = content.find(",", id_start);
          std::string id = content.substr(id_start, id_end - id_start);
          
          // Extract description
          size_t desc_pos = content.find("\"description\":");
          std::string description = "No description";
          if (desc_pos != std::string::npos) {
            size_t desc_start = content.find("\"", desc_pos + 14) + 1;
            size_t desc_end = content.find("\"", desc_start);
            description = content.substr(desc_start, desc_end - desc_start);
          }
          
          // Extract status
          size_t status_pos = content.find("\"status\":");
          std::string status = "unknown";
          if (status_pos != std::string::npos) {
            size_t status_start = content.find("\"", status_pos + 9) + 1;
            size_t status_end = content.find("\"", status_start);
            status = content.substr(status_start, status_end - status_start);
          }
          
          std::cout << "ID: " << id << " | Status: " << status << " | Description: " << description << std::endl;
          found_any = true;
        }
      }
    }
  }
}

void task_update(char const *argv[]) {
  std::string id = argv[2];
  std::string new_description = argv[3]; // Fixed: should be argv[3], not argv[4]
  std::string filename = id + ".json";
  
  if (!std::filesystem::exists(filename)) {
    std::cout << "Task with ID " << id << " not found.\n";
    return;
  }

  
  std::ifstream task_file(filename);


  if (!task_file.is_open()) {
    std::cout << "Error: Could not open task file.\n";
    return;
  }
  
  std::string line;
  std::string content;

  
  // Read the entire file content
  while (std::getline(task_file, line)) {
    content += line + "\n"; // Preserve line breaks
  }
  task_file.close();
  
  // Find and replace the description
  size_t desc_pos = content.find("\"description\":");
  if (desc_pos == std::string::npos) {
    std::cout << "Error: Could not find description field in task file.\n";
    return;
  }
  
  size_t desc_start = content.find("\"", desc_pos + 14) + 1;
  size_t desc_end = content.find("\"", desc_start);
  
  if (desc_start == std::string::npos || desc_end == std::string::npos) {
    std::cout << "Error: Malformed JSON in task file.\n";
    return;
  }
  
  // Replace the description
  content.replace(desc_start, desc_end - desc_start, new_description);
  
  // Update the updatedAt timestamp
  auto now = std::chrono::system_clock::now();
  auto timestamp = std::chrono::system_clock::to_time_t(now);
  std::string time = std::ctime(&timestamp);
  time.pop_back(); // Remove newline
  
  size_t updated_pos = content.find("\"updatedAt\":");
  if (updated_pos != std::string::npos) {
    size_t updated_start = content.find("\"", updated_pos + 12) + 1;
    size_t updated_end = content.find("\"", updated_start);
    if (updated_start != std::string::npos && updated_end != std::string::npos) {
      content.replace(updated_start, updated_end - updated_start, time);
    }
  }
  
  // Write the updated content back to the file
  std::ofstream output_file(filename);
  if (!output_file.is_open()) {
    std::cout << "Error: Could not write to task file.\n";
    return;
  }
  
  output_file << content;
  output_file.close();
  
  std::cout << "Task " << id << " updated successfully!\n";
}


int main(int argc, char const *argv[]) {
  int num_tasks = 0;
  std::cout << "CLI Task Tracker\n";
  if (argc < 2) {
    std::cout << "Usage: task-cli <command>\n";
    return 1;
  }

  // ADD COMMAND
  if (std::string(argv[1]) == "add") {
    if (argc != 3)
    {
      std::cout << "Usage: task-cli add \"task description\"\n";
      return 1;
    }
    task_add(argv);
  } 

  // LISTING COMMAND
  if (std::string(argv[1])== "list"){
    task_list(argv);
  }

  // UPDATE COMMAND
  if (std::string(argv[1]) == "update") {
    if (argc != 4)  // argv[0]=program, argv[1]=update, argv[2]=id, argv[3]=description
    {
      std::cout << "Usage: task-cli update <task_id> \"new description\"\n";
      return 1;
    }
    task_update(argv);
  }

  // DELETE COMMAND

  
  return 0;
}