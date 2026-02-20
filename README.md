# Techshell

Bradley Duft

I worked on it without groupmates, but with the help of a friend.

Work done would include:
    designing structure of the program
    writing the parsing logic
    implementing fork() and execvp() with aid of friend
    implementing the built in cd command
    input and output redirection
    debugging program with aid of friend
    




Overall Description
This project is a very simplified version of a bash shell written in C. This program repeatedly displays a prompt that shows the current working directory. The user can then enter commands just like they would in a normal shell.




How to Clone, Compile, and Run

This program must be compiled and run in a Linux environment (or WSL on Windows, which is what I did to compile and run), since it uses linux calls like fork() and execvp().

1. Clone the Repository
git clone <https://github.com/brad110/Techshell.git>
cd <Techshell>

2. Compile the Program
(within terminal of repo folder)
gcc Techshell.c -o Techshell

3. Run the Program
./Techshell



Features That Work

    Prompt displays current working directory
    Executes commands using fork() and execvp()
    Built in cd command
    Input redirection using <
    Output redirection using >
    Proper error messages for invalid commands or invalid file paths/files
    exit using ctrl+d
            basic commands would be:
                pwd
                ls
                whoami
                cd ..
                ls > out.txt
                cat out.txt


Features that don't work within this shell program
    pipes
    quoted string
    
