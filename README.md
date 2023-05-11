# Banker-s-Algorithm
This project is implemented in C and uses multithreading, synchronization, mutual exclusion, Deadlock Avoidance.

Operating system: Linux (Ubuntu)
About: The code implements banker’s algorithm using multithreading, mutex locking and deadlock concepts to deliver the output.
Predefined values: As described int the assignment, the values of NUM_CUSTOMERS and NUM_RESOURCES has already been defined as 5 and 3 respectively.
Steps to run the code:
1) Copy the file in 'Home’ directory.
2) Open terminal
3) Type the following commands:
gcc VineetBanker.c -lpthread (Press Enter)
./a.out 10 5 7 (Press Enter)
4) Give input on the terminal as shown in the terminal.
5) After giving input the code will start executing.
Output:
- It will give the safe sequence if the given inputs are schedulable.
- If the threads are not schedulable, then it will show the deadlock when occur during compilation.
Print on console: The code is printing all the instructions as given in the assignment and some additional instructions that help the user in better understanding of its working.
