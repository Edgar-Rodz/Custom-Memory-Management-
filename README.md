# Custom-Memory-Management-
This project implements custom memory management functions in C++, including malloc, realloc, and free, to improve memory allocation and deallocation processes. 
The goal of this project is to optimize dynamic memory management, enhance performance, and reduce memory fragmentation through efficient data structures.

Custom Memory Allocation (malloc): Allocates memory blocks dynamically, minimizing overhead and optimizing space usage.
Memory Reallocation (realloc): Adjusts the size of previously allocated memory blocks, ensuring efficient memory resizing.
Memory Deallocation (free): Frees up allocated memory blocks, preventing memory leaks and improving performance.

How It Works
The custom memory management functions are designed to replace the standard C++ malloc, realloc, and free functions. The implementation includes:

Free Lists: Tracks free memory blocks for reuse, reducing the need for frequent allocations.
Allocation Tables: Manages allocated memory blocks, ensuring efficient allocation and deallocation.
Fragmentation Minimization: Combines adjacent free blocks to minimize memory fragmentation and optimize space usage.
