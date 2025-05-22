Parallel Sum with Shared Memory and Semaphore
📌 Description

This C program creates 100 child processes, each adding a unique number (1–100) to a shared memory variable. A System V semaphore ensures that only one process modifies the shared value at a time.
🎯 Goal

Calculate the sum 1 + 2 + ... + 100 = 5050 using 100 processes and protect access with a semaphore.
💡 Key Concepts

    Process creation (fork)

    Shared memory (shmget, shmat)

    Semaphore for synchronization (semget, semop, semctl)

⚙️ How to Compile & Run

gcc -o parallel_sum parallel_sum.c
./parallel_sum

Expected output:

sharedInt value at the end: 5050

✅ Features

    Proper semaphore locking/unlocking

    Shared memory initialized and cleaned up

    Final result printed by the parent process

🧹 Cleanup

Shared memory and semaphore are deleted automatically at the end.
