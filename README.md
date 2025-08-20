# 🐚 Holberton Shell (hsh)

This repository contains the implementation of **hsh (Holberton Shell)**.  
It is a basic **command line interpreter** written in C, designed to reproduce some of the behavior of shells like `bash` or `sh`.

---

## 📖 Description

- The shell works in both **interactive** and **non-interactive** mode:
  - **Interactive mode**: displays a prompt (`#cisfun$ `) and waits for user input.  
  - **Non-interactive mode**: executes commands provided through a pipeline or a script.
- After each command is executed, the prompt is displayed again (interactive mode).
- Handles special cases such as:
  - `Ctrl+D` (EOF) to exit the shell.
  - Empty lines or input with only spaces.
- Supports **commands with arguments**.
- Supports commands entered with:
  - **Absolute/relative path** (`/bin/ls`)
  - **PATH resolution** (`ls`)
- Built-in commands:
  - `exit` → exits the shell, returning the last command’s status.

---

## ⚙️ Allowed Functions and System Calls

This project is restricted to a specific set of system calls and C standard library functions:

- `access`
- `chdir`
- `close`
- `execve`
- `exit`
- `_exit`  
- `fork`
- `free`
- `getline`
- `isatty`
- `kill`
- `malloc`  
- `open`
- `opendir`
- `perror`
- `read`
- `readdir`
- `signal`  
- `stat`
- `lstat`
- `fstat`
- `strtok`
- `wait`
- `waitpid`
- `wait3`
- `wait4`
- `write`

---

## 📂 Project Structure

holbertonschool-simple_shell/

- `shelly.c` Main loop: prompt, getline, EOF handling
- `run_command.c` Tokenization, PATH resolution, command execution
- `trim_line.c` Utility: trims newline and spaces from input
- `shelly.h` Header file with prototypes and includes
- `man_1_simple_shell` Manual page for hsh
- `README.md` Project documentation
- `AUTHORS` Contributors

---

## 🛠️ Compilation

Compile with:
`gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh`

---

##▶️ Usage

Run with:
`./hsh`
