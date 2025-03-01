# Process and Thread Synchronization

A Linux implementation of a hierarchical process structure with inter-process and inter-thread synchronization mechanisms.

## Overview

This project demonstrates the creation of a specific process hierarchy and the synchronization of threads both within the same process and across different processes. The implementation uses POSIX threads, semaphores, and Linux process management functions.

## Process Hierarchy

The following process hierarchy is implemented:
```
        P1
       /  \
     P2    P3
           |
           P4
           |
           P5
           |
           P6
           |
           P7
```

P1 is the main process that creates P2 and P3 as children. P3 creates P4, P4 creates P5, P5 creates P6, and P6 creates P7. Each parent process waits for its children to finish before terminating.

## Thread Synchronization Requirements

### P4 Thread Synchronization
- P4 creates 4 threads: T4.1, T4.2, T4.3, and T4.4
- Thread T4.2 must start before T4.1 and terminate after T4.1

### P3 Thread Barrier
- P3 creates 47 threads: T3.1 through T3.47
- At most 5 threads can run simultaneously (not counting the main thread)
- Thread T3.10 must end while exactly 5 threads are running

### Cross-Process Thread Synchronization
- P7 creates 6 threads: T7.1 through T7.6
- Thread T7.1 must terminate before T4.3 starts
- Thread T7.6 must start only after T4.3 terminates

## Implementation Details

The implementation uses:
- `fork()` for process creation
- POSIX threads (`pthread_create()`, `pthread_join()`) for thread management
- Semaphores for thread synchronization
- The provided helper functions (`init()` and `info()`) for tracking execution

## Building and Running

Compile the program with:
```bash
gcc -Wall a2.c a2_helper.c -o a2 -lpthread
```

Run the program:
```bash
./a2
```

## Testing

The program can be tested using the provided `tester.py` script:
```bash
python3 tester.py
```

For Docker-based testing:
```bash
python3 tester.py --docker
```

## Requirements

- Linux operating system
- GCC compiler
- POSIX threads library
- Python 3 (for testing)
