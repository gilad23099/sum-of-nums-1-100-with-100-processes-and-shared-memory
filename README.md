# Parallel Sum with Shared Memory and Semaphore

## Description

This C program creates 100 child processes. Each process adds a unique number (1–100) to a shared integer stored in System V shared memory. A semaphore is used to ensure mutual exclusion.

## Goal

Calculate the sum of numbers 1 to 100 in parallel with process synchronization. Final result should be:


## Key Concepts

- `fork()` for process creation  
- `shmget`, `shmat` for shared memory  
- `semget`, `semop`, `semctl` for semaphore control  

## Build and Run

```bash
gcc assignment3.c -o assignment3
./assignment3

