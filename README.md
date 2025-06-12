[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/ZRO8WLAc)
# Judge Script README

This script is used to compile and run tests for your code, and then compare the results against expected outputs.

## Setup

1. **Place `judge.sh` in the same directory** as your `src` folder.

2. **Make `judge.sh` executable**:
   - In the terminal, run the following command to make the script executable:

     ```bash
     chmod +x judge.sh
     ```

3. **Prepare your tests**:
   - Create a `tests` folder in the same directory as `judge.sh`.
   - Inside the `tests` folder, create subfolders for each test case. Each subfolder should contain:
     - An input file (e.g., `01.in`) that provides input to your program.
     - An output file (e.g., `01.out`) that contains the expected output for your program.

   Example directory structure:

   ```text
    .
    ├── README.md
    ├── judge.sh
    ├── src
    │   └── main.cpp
    └── tests
        ├── 01
        │   ├── 01.in
        │   └── 01.out
        ├── 02
        │   ├── 02.in
        │   └── 02.out
        └── ...
   ```

## How to Run Tests

1. Open a terminal.

2. **Run the tests** by executing the following command:

   ```bash
   ./judge.sh -t
   ```

3. The script will:
   - Compile your code.
   - Run each test case (using the `.in` file as input).
   - Compare the output against the expected `.out` file.
   - Print whether each test passed or failed, along with details for any failed tests.

## After Testing

- Once the script finishes running, check the output in the terminal.
  - **Passed tests** will be marked as "Accepted".
  - **Failed tests** will show "Wrong answer" along with a diff of the differences if available.

## Cleaning Up

-  **Remove test outputs and diffs** (if needed) by running:

   ```bash
   ./judge.sh -r
   ```

   This will delete any `.result` and `.diff` files generated during the tests.

## a5 assignment
- Check out [TBD Project](https://github.com/MahdiAnvarii/TBD)

## a6 assignment
- Check out [UTrello Project](https://github.com/MahdiAnvarii/UTrello)
