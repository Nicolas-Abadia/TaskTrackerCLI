# Task Tracker CLI

![C++](https://img.shields.io/badge/C++-20-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.10+-green.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

A simple command-line task management application built in C++. This project was developed as part of the [roadmap.sh Task Tracker project](https://roadmap.sh/projects/task-tracker).

## 📋 Features

- ✅ Add new tasks with descriptions
- 📝 Update existing task descriptions
- 🗑️ Delete tasks
- 🔄 Change task status (todo, in-progress, done)
- 📊 List tasks by status or view all tasks
- 💾 JSON-based storage

## 🚀 Getting Started

### Prerequisites

- **C++ Compiler**: GCC 9+ or Clang 10+ with C++20 support
- **CMake**: Version 3.10 or higher
- **Google Test**: For running unit tests (optional)

### Installation

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Nicolas-Abadia/TaskTrackerCLI.git
   cd TaskTrackerCLI
   ```

2. **Build the project:**
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

3. **Run the application:**
   ```bash
   ./task-cli
   ```

## 💡 Usage

### Basic Commands

```bash
# Add a new task
./task-cli add "Buy groceries"

# Update a task description
./task-cli update 1 "Buy groceries and cook dinner"

# Delete a task
./task-cli delete 1

# Mark task status
./task-cli mark-todo 1
./task-cli mark-in-progress 1
./task-cli mark-done 1

# List tasks
./task-cli list                    # List all tasks
./task-cli list done               # List completed tasks
./task-cli list todo               # List pending tasks
./task-cli list in-progress        # List tasks in progress
```

### Example Workflow

```bash
# Add some tasks
./task-cli add "Learn C++"
./task-cli add "Build a CLI app"
./task-cli add "Write documentation"

# Start working on a task
./task-cli mark-in-progress 2

# Complete a task
./task-cli mark-done 1

# View your progress
./task-cli list
```

## 📁 Project Structure

```
TaskTrackerCLI/
├── task-cli.cpp           # Main application source code
├── test_task-cli.cpp      # Unit tests
├── CMakeLists.txt         # Build configuration
├── README.md              # Project documentation
├── build/                 # Build artifacts (generated)
└── *.json                 # Task data files (generated)
```

## 🤝 Contributing

Contributions are welcome! This is a learning project, and I'm open to feedback to improve my coding practices.


### Areas for Improvement

- Error handling and input validation
- Configuration file support
- Task filtering and search functionality
- Better JSON parsing/generation

## 📬 Contact

Nicolas Abadia - [@Nicolas-Abadia](https://github.com/Nicolas-Abadia)

Project Link: [https://github.com/Nicolas-Abadia/TaskTrackerCLI](https://github.com/Nicolas-Abadia/TaskTrackerCLI)