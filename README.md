# Shell Implementation

In this project I implemented a shell. The program displays a prompt ($) and allows the user to type commands in Linux and run them. When non-built-in commands are executed, the program will do this by creating a child process that will execute the command by calling the command from the exec commands family. In other commands that are built-in such as "history", "jobs", "cd" and "exit", the program executes the command alone without creating a child process.

![shell](https://user-images.githubusercontent.com/73079447/142921542-01804a96-6be7-4dcc-8829-ad761d772a65.png)
