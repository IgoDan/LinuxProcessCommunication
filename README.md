# Introduction

Implementation of Linux inter-process communication between 3 child processes using FIFO and FILE machanisms. Secondary program affects process operation using signals and process' PID numbers to kill, stop or continue any of the 3 child processes.

# Programs start

First program sends information through communication mechanisms from child process 1 to child process 2 (FIFO) and then from child process 2 to child process 3 (FILE). After running both programs, first one (in the left console) lets user select source of sent information: 

- From keyboard
- From txt file
- From /dev/urandom file

The second file (in the right console) gets PID numbers from 3 child processes and allows to send signals to each of them.

![image](https://user-images.githubusercontent.com/87280929/225780402-175be149-6a86-4005-8b98-3ff255712f95.png)

# Process communication

After selecting "From txt file" option (in the left console), information is being sent forward by child process 1 and child processes 2 and 3 print out data obtainment confirmation:

![image](https://user-images.githubusercontent.com/87280929/225782192-324144c8-8b8e-4bdf-8e8c-2a74950c13b5.png)

# Sending signals

To send signal to one of the 3 child process, console on the right side is used to select type of signal and child process PID. Screenshot below presents stopping child process 2 while the /dev/urandom information is sent via communication mechanisms of child processes:

![image](https://user-images.githubusercontent.com/87280929/225783435-cdaff77f-6642-4cf3-90fd-6d7b6f013501.png)




