# Shell Implemntation

In this project I implemented a shell. The program displays a prompt ($) and allows the user to type commands in Linux and run them. When non-built-in commands are executed (except for "ls" and "ls -l" commands that I implemnted separately), the program will do this by creating a child process that will execute the command by calling the command from the exec commands family. In other commands that are built-in such as "history", "jobs", "cd" and "exit", the program executes the command alone without creating a child process.

![shell](https://user-images.githubusercontent.com/73079447/142916769-898f9dd4-8fe6-4b03-b418-b8eccb0fa9d1.png)
