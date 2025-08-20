# 🐚 Holberton Shell (hsh)

This repository contains the implementation of **hsh (Holberton Simple Shell)**.  
It is a basic **command line interpreter** written in C, designed to reproduce some of the behavior of shells like `bash` or `sh`.

---

## 📖 Description

First of all, the program checks if the user runs the shell in interactive mode or not.

- If not interactive, `hsh` will execute the command only once and then exit.
- If interactive, a prompt will be displayed (#cisfun$ ) and hsh will wait for the user to enter a command. Once the command finishes executing, the prompt will show up again.

The shell also manages border cases such as:

- `Ctrl+D` (EOF)
- Empty input lines `(\n)`

It supports:

- Commands with arguments
- Absolute or relative paths (e.g., `/bin/ls`)
- If the command is not written with a path, `hsh` will search for it across all the directories defined in the PATH environment variable.

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

`holbertonschool-simple_shell/`

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

---

##👥 Authors

Bruno Salle (854113)
