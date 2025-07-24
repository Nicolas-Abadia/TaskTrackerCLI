# TaskTrackerCLI - AI Agent Instructions

## Project Overview
A C++20 command-line task management application that stores tasks as individual JSON files in the working directory. Each task has an auto-incrementing numeric ID and is stored as `{id}.json`.

## Architecture & Data Model

### Core Data Storage Pattern
- **File-based storage**: Each task = one JSON file (`1.json`, `2.json`, etc.)
- **No database**: Tasks persist as individual files in the working directory
- **Sequential ID assignment**: `num_task()` counts existing files to determine next ID
- **JSON structure**: `{"id":1,"description":"...","status":"todo","createdAt":"...","updatedAt":"..."}`

### Key Architectural Decisions
- **String-based JSON parsing**: Manual string manipulation instead of JSON libraries
- **File existence checking**: Uses `std::filesystem::exists()` before operations
- **Gap-tolerant iteration**: `task_list()` iterates through potential IDs, skipping missing files
- **In-place updates**: Reads entire file, modifies content string, writes back

## Critical Code Patterns

### Filename Generation (Marked for Refactoring)
```cpp
std::string filename = std::to_string(id) + ".json"; // FILENAME TO CHANGE
```
All filename generation is marked with `// FILENAME TO CHANGE` comments - indicates planned naming scheme migration.

### Error Handling Pattern
```cpp
if (!std::filesystem::exists(filename)) {
    std::cout << "Task with ID " << id << " not found.\n";
    return;
}
```
Consistent pattern: check file existence → validate file operations → provide user feedback → early return on errors.

### JSON Field Extraction Pattern
```cpp
size_t desc_pos = content.find("\"description\":");
size_t desc_start = content.find("\"", desc_pos + 14) + 1;
size_t desc_end = content.find("\"", desc_start);
std::string description = content.substr(desc_start, desc_end - desc_start);
```
Manual JSON parsing with position-based string extraction. Critical: always check for `std::string::npos` before using positions.

## Build & Development Workflow

### Build System
- **CMake** with C++20 standard requirement
- **Build command**: `cmake -B build && cmake --build build`
- **Executable location**: `build/task-cli`
- **VS Code tasks**: Use "C/C++: g++ build active file" or "C/C++: g++-11 build active file"

### Command Interface
```bash
./task-cli add "description"           # Creates next ID.json
./task-cli list                        # Shows all tasks
./task-cli update <id> "new desc"      # Modifies existing task
./task-cli delete <id>                 # Removes task file
./task-cli print                       # Debug: lists all .json files
```

## Known Issues & Limitations

### Sequential ID Dependency
- `num_task()` assumes contiguous IDs starting from 1
- Deleting tasks creates gaps, breaking `task_list()` iteration
- `task_print()` uses `std::filesystem::directory_iterator` but needs filtering for task files vs other JSON files

### Filename Collision Risk
Current pattern `{id}.json` conflicts with build artifacts like `compile_commands.json`. Consider migrating to `task{id}.json` pattern.

### Manual JSON Parsing Fragility
String-based parsing is brittle. When adding fields or changing structure, update all parsing logic in `task_list()` and `task_update()`.

## Development Guidelines

### When Adding New Commands
1. Add function following `void task_<command>(char const *argv[])` pattern
2. Add command parsing in `main()` with argc validation
3. Follow existing error handling pattern: file existence → operation → user feedback
4. Use `// FILENAME TO CHANGE` comment when generating filenames

### When Modifying JSON Structure
1. Update both write operations (`task_add`, `task_update`) 
2. Update all read/parse operations (`task_list`, field extraction logic)
3. Test with existing task files to ensure backward compatibility

### File System Operations
- Always use `std::filesystem::exists()` before file operations
- Use `std::filesystem::remove()` for deletion, check return value
- Prefer reading entire file content for modifications rather than streaming
