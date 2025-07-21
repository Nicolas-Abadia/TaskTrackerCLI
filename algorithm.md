## task-cli add
// Receives the first command line argument for the command add and the second for the name of the task.

- if command_line_argument_1 is "add" and command_line_argument_2 is a string then
  write json file with properties
    id is assigned to num_tasks
    description is assigned to command_line_argument_2
    status is assigned to "todo"
    createdAt is assigned to current_time
    updatedAt is assigned to current_time
  close json file
- else
  print "Type the name of the task!"

## task-cli list

- if command_line_argument_1 is "list" then
  for each task in tasks
    print task description

## task-cli update

- if command_line_argument_1 is "update" and command_line_argument_2 is a valid id and command_line_argument_3 is a string then
  description is assigned to command_line_argument_3

## task-cli delete

## task-cli mark-in-progress

## task-cli mark-done

## task-cli list done

## task-cli list todo

## task-cli list in-progress

## Edge cases to handle later

User inputs wrong action
User inputs a task desc that already exists
User inputs wrong argument
User marks status to the same current status (should work normally)
Case if the user does not add "" to the task description