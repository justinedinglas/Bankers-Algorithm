This program implements the *Banker’s Algorithm* used in operating systems to avoid deadlock.  
It checks if the system is currently in a safe state and allows users to make resource requests that are granted only if they keep the system safe.

The program is written in **C++**. Compiling the code will be stated in **MacOS** as that is what we used to code on. 

First, ensure that you are in the current directory where the .cpp file is and run the following command:
```
clang++ -std=c++11 bankers.cpp -o bankers
```

Once the program is successfully created, run the following command to execute:
```
./bankers
```

A menu will be displayed upon execution.

1. **Option 1: *Check safe sequence.***  
    - Type 1 and press Enter.
    - If the system is safe, you’ll see:
    - System is in a safe state. and a sequence like [1, 3, 4, 0, 2].
    - If it’s not safe, it will say so.

2. **Option 2: *Make a resource request.***  
    - Type 2 and press Enter.
    - Enter a process ID (e.g., 0–4) and press Enter.
    - Enter the request vector as three integers (format: r1 r2 r3) and press Enter.
   
Repeat as needed. You return to the menu after each action.

3. To **exit** the program, type 0 and press Enter.
