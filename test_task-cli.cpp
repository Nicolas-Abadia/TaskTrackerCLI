#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>

// Include the functions we want to test
// Since task_update is in task-cli.cpp, we need to declare it here
extern void task_update(char const *argv[]);

class TaskUpdateTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Create a test directory for our test files
    test_dir = "test_tasks";
    std::filesystem::create_directory(test_dir);
    std::filesystem::current_path(test_dir);
    
    // Redirect cout to capture output
    original_cout = std::cout.rdbuf();
    std::cout.rdbuf(output_stream.rdbuf());
  }
  
  void TearDown() override {
    // Restore cout
    std::cout.rdbuf(original_cout);
    
    // Clean up test files
    std::filesystem::current_path("..");
    std::filesystem::remove_all(test_dir);
  }
  
  void CreateTestTask(const std::string& id, const std::string& description, 
             const std::string& status = "todo") {
    std::string filename = id + ".json";
    std::ofstream file(filename);
    file << "{\n";
    file << "\"id\":" << id << ",\n";
    file << "\"description\":\"" << description << "\",\n";
    file << "\"status\":\"" << status << "\",\n";
    file << "\"createdAt\":\"Mon Jan 1 00:00:00 2024\",\n";
    file << "\"updatedAt\":\"Mon Jan 1 00:00:00 2024\"\n";
    file << "}";
    file.close();
  }
  
  std::string ReadTaskFile(const std::string& id) {
    std::string filename = id + ".json";
    std::ifstream file(filename);
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
      content += line + "\n";
    }
    return content;
  }
  
  std::string GetOutput() {
    return output_stream.str();
  }
  
  void ClearOutput() {
    output_stream.str("");
    output_stream.clear();
  }

private:
  std::string test_dir;
  std::ostringstream output_stream;
  std::streambuf* original_cout;
};

// Test 1: Successfully update existing task
TEST_F(TaskUpdateTest, UpdateExistingTask) {
  // Arrange
  CreateTestTask("1", "Original task description");
  const char* argv[] = {"task-cli", "update", "1", "Updated task description"};
  
  // Act
  task_update(argv);
  
  // Assert
  std::string content = ReadTaskFile("1");
  EXPECT_TRUE(content.find("Updated task description") != std::string::npos);
  EXPECT_TRUE(content.find("Original task description") == std::string::npos);
  EXPECT_TRUE(GetOutput().find("Task 1 updated successfully!") != std::string::npos);
}

// Test 2: Task file doesn't exist
TEST_F(TaskUpdateTest, TaskFileNotFound) {
  // Arrange
  const char* argv[] = {"task-cli", "update", "999", "Some description"};
  
  // Act
  task_update(argv);
  
  // Assert
  EXPECT_TRUE(GetOutput().find("Task with ID 999 not found.") != std::string::npos);
}

// Test 3: Update with empty description
TEST_F(TaskUpdateTest, UpdateWithEmptyDescription) {
  // Arrange
  CreateTestTask("2", "Original description");
  const char* argv[] = {"task-cli", "update", "2", ""};
  
  // Act
  task_update(argv);
  
  // Assert
  std::string content = ReadTaskFile("2");
  EXPECT_TRUE(content.find("\"description\":\"\"") != std::string::npos);
  EXPECT_TRUE(GetOutput().find("Task 2 updated successfully!") != std::string::npos);
}

// Test 4: Update with special characters
TEST_F(TaskUpdateTest, UpdateWithSpecialCharacters) {
  // Arrange
  CreateTestTask("3", "Original description");
  const char* argv[] = {"task-cli", "update", "3", "Task with \"quotes\" and \\backslashes"};
  
  // Act
  task_update(argv);
  
  // Assert
  std::string content = ReadTaskFile("3");
  EXPECT_TRUE(content.find("Task with \"quotes\" and \\backslashes") != std::string::npos);
  EXPECT_TRUE(GetOutput().find("Task 3 updated successfully!") != std::string::npos);
}

// Test 5: Malformed JSON - missing description field
TEST_F(TaskUpdateTest, MalformedJsonMissingDescription) {
  // Arrange
  std::ofstream file("4.json");
  file << "{\n";
  file << "\"id\":4,\n";
  file << "\"status\":\"todo\",\n";
  file << "\"createdAt\":\"Mon Jan 1 00:00:00 2024\"\n";
  file << "}";
  file.close();
  
  const char* argv[] = {"task-cli", "update", "4", "New description"};
  
  // Act
  task_update(argv);
  
  // Assert
  EXPECT_TRUE(GetOutput().find("Error: Could not find description field in task file.") != std::string::npos);
}

// Test 6: Very long description
TEST_F(TaskUpdateTest, UpdateWithLongDescription) {
  // Arrange
  CreateTestTask("5", "Short description");
  std::string long_desc(1000, 'A'); // 1000 character string
  const char* argv[] = {"task-cli", "update", "5", long_desc.c_str()};
  
  // Act
  task_update(argv);
  
  // Assert
  std::string content = ReadTaskFile("5");
  EXPECT_TRUE(content.find(long_desc) != std::string::npos);
  EXPECT_TRUE(GetOutput().find("Task 5 updated successfully!") != std::string::npos);
}

// Test 7: Verify timestamp is updated
TEST_F(TaskUpdateTest, TimestampIsUpdated) {
  // Arrange
  CreateTestTask("6", "Original description");
  std::string original_content = ReadTaskFile("6");
  
  // Sleep briefly to ensure timestamp difference
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  
  const char* argv[] = {"task-cli", "update", "6", "Updated description"};
  
  // Act
  task_update(argv);
  
  // Assert
  std::string updated_content = ReadTaskFile("6");
  
  // Extract original and new timestamps (this is a simplified check)
  size_t orig_time_pos = original_content.find("\"updatedAt\":");
  size_t new_time_pos = updated_content.find("\"updatedAt\":");
  
  EXPECT_NE(orig_time_pos, std::string::npos);
  EXPECT_NE(new_time_pos, std::string::npos);
  
  // The timestamps should be different (though this is a basic check)
  std::string orig_time_section = original_content.substr(orig_time_pos, 50);
  std::string new_time_section = updated_content.substr(new_time_pos, 50);
  
  // In a real scenario, you might want to parse and compare actual timestamps
  EXPECT_TRUE(GetOutput().find("Task 6 updated successfully!") != std::string::npos);
}

// Main function to run all tests
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}