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


void task_add(char const *argv[], int next_id){
  auto desc = argv[2];
    
    int num = next_id;
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

void task_list(char const *argv[], int num_tasks) {
  // Get the actual number of existing tasks
  int total_tasks = num_tasks;
  
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
  std::string new_description = argv[3]; 
  std::string filename = id + ".json"; 
  
  // Error Handling
  if (!std::filesystem::exists(filename)) {
    std::cout << "Task with ID " << id << " not found.\n";
    return;
  }

  
  std::ifstream task_file(filename);

  // Error Handling
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
  // Searches for the literal string ["description":] in the JSON content
  // content.find() returns the starting position of the string
  size_t desc_pos = content.find("\"description\":"); // position of the word description
  if (desc_pos == std::string::npos) {
    std::cout << "Error: Could not find description field in task file.\n"; // this could happen if the json file is corrupted or malformed.
    return;
  }
  
  /*
    find the opening quote ["] after de starting position of the string ["description":]
    desp_pos + 14: skips past ["description":] (14 characters)
    + 1: Moves to the character after the opening quote (start of actual description text)
  */
  size_t desc_start = content.find("\"", desc_pos + 14) + 1;
  /*
    Searches for the closing quote starting from desc_start
    This finds the end of the description text (before the closing quote)
  */
  size_t desc_end = content.find("\"", desc_start);
  

  // This npos error checking would never fail for desc start since it always adds 1 to the desc_start variable
  // should correct this
  if (desc_start == std::string::npos || desc_end == std::string::npos) {
    std::cout << "Error: Malformed JSON in task file.\n";
    return;
  }
  
  // Replace the description
  /*
    desc_start: Starting position of replacement
    desc_end - desc_start: Length of text to replace
    new_description: The new text to insert 
  */
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


void task_delete(char const *argv[]) {

  std::string id = argv[2];
  std::string filename = id + ".json"; 

  if (!std::filesystem::exists(filename)) {
    std::cout << "Task with ID " << id << " not found.\n";
    return;
  }

  if (!std::filesystem::remove(filename)) {
    std::cout << "Task was not deleted\n";
    return;
  }
  std::cout << "Task " << id << " deleted successfully!\n";

}

void task_print(char const* argv[]) {
  // DIRECTORY ITERATOR
  for (const auto& entry : std::filesystem::directory_iterator(".")) {
    std::string path = entry.path(); 
    // Step 1: Remove .json
    std::string without_json = path.substr(0, path.find(".json"));

    // Step 2: Get filename only
    size_t last_slash = without_json.rfind("/");
    std::string filename = (last_slash != std::string::npos) ? 
                          without_json.substr(last_slash + 1) : without_json;

    // Step 3: Check if all characters are digits
    bool is_number = true;
    for (char c : filename) {
        if (!std::isdigit(c)) {
            is_number = false;
            break;
        }
    }
    if (is_number) std::cout << entry.path() << std::endl;
    
  }
}


void directory_iterator(int& num_tasks, int& next_id){
  int id = 0;
  int greatest_id = 0;
  for (const auto& entry : std::filesystem::directory_iterator(".")) {
    std::string path = entry.path(); 
    // Step 1: Remove .json
    std::string without_json = path.substr(0, path.find(".json"));

    // Step 2: Get filename only
    size_t last_slash = without_json.rfind("/");
    std::string filename = (last_slash != std::string::npos) ? 
                          without_json.substr(last_slash + 1) : without_json;

    // Step 3: Check if all characters are digits
    bool is_number = true;
    for (char c : filename) {
        if (!std::isdigit(c)) {
            is_number = false;
            break;
        }
    }
    
    if (is_number){
      id = std::stoi(filename);
      if (id > greatest_id) {
      greatest_id = id;
      } 
      num_tasks++;
    }
  }
  next_id = greatest_id + 1; 
}

int main(int argc, char const *argv[]) {
  int num_tasks = 0;
  int next_id = 0;
  directory_iterator(num_tasks, next_id);
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
    task_add(argv, next_id);
  } 

  // LISTING COMMAND
  if (std::string(argv[1])== "list"){
    task_list(argv, num_tasks);
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

  if (std::string(argv[1]) == "delete")
  {
    if (argc != 3) {
      std::cout << "Usage: task-cli delete <task_id>\n";
      return 1;
    }
    task_delete(argv);
  }
  
  // PRINT JSON

  if (std::string(argv[1]) == "print")
  {
    if (argc != 2) {
      std::cout << "Usage: task-cli print\n";
      return 1;
    }
    task_print(argv);
  }

  // UPDATE WITH FILTERS

  
  return 0;
}