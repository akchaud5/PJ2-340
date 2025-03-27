# CSE 340 Project 2 Guidelines

## Build Commands
- `g++ -std=c++11 -o parser project2.cc inputbuf.cc lexer.cc` - Compile the parser
- `./parser <task_number> < <input_file>` - Run a specific task with input
- `./run_tests.sh` - Run all tests for all tasks
- `./testTask1.sh` - Run tests specifically for Task 1

## Run Single Test Example
```bash
# Run test01.txt for Task 2
./parser 2 < tests/test01.txt
```

## Code Style Guidelines
- **Formatting**: Use 4-space indentation, no tabs
- **Naming**: CamelCase for structs, snake_case for variables/functions
- **Types**: Use STL containers (vector, map, set) and algorithms
- **Comments**: Document functions with purpose, parameters, and return values
- **Error Handling**: Use cout to report errors and exit() for fatal errors
- **Organization**: Group related functions together
- **Terminal Symbols**: Use "#" for terminal symbols in output
- **Output Format**: Follow exact spacing in expected output files

## Expected Tasks
Tasks include printing terminals/non-terminals, calculating nullable sets, computing FIRST/FOLLOW sets, left factoring, and eliminating left recursion.