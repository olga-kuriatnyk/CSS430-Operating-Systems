**This project asks to manage allocations within a memory pool of size MEMSIZE.  The program support six different requests:**

1. Allocate: N bytes for a process using one of the 3 allocation algorithms
    
    Format: A  'name'  'size'  'algorithm'
  
    Eg:   A P 20 F   Allocate 20 bytes for process P using First-Fit algorithm

2. Free: all allocations for a given process

    Eg:   F P     Free all the allocations owned by 'name'

3. Show: the status of the memory pool – allocated and free blocks

4. Read: a script – a sequence of commands from a file, and execute them

    Eg:     R 'file.txt'

5. Compact: the allocations, making them into one contiguous block.  (This somewhat resembles the operation of a mark-sweep garbage collector in C#)

6. Exit: exit the program
