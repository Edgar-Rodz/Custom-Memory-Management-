#include "Heap.hpp"

/*
 * Heap constructor
 * 
 * Done for you. You should not modify this method.
 */
Heap::Heap(uint32_t base_address, size_t total_bytes) :
    base_address(base_address),
    total_bytes(total_bytes),
    heap((int8_t*) malloc(total_bytes))
{
    assert(base_address % 4 == 0); // Heap must be word aligned
    this->num_reads = 0;
    this->reset();
}

/*
 * Heap destructor
 * 
 * Done for you. You should not modify this method.
 */
Heap::~Heap() {
    free((void*) this->heap);
}

/*
 * Resets the Heap to all 0s
 * 
 * Done for you. You should not modify this method.
 */
void Heap::reset() {
    memset((void*) this->heap, 0, this->total_bytes);
    this->num_reads = 0;
}

/*
 * Reads a single byte from Heap
 * 
 * Done for you. You should not modify this method.
 */
int8_t Heap::read(uint32_t address) {
    this->num_reads++;
    return this->heap[address - base_address];
}

/*
 * Writes a single byte to Heap
 * 
 * Done for you. You should not modify this method.
 */
void Heap::write(uint32_t address, int8_t data) {
    this->heap[address - base_address] = data;
}

/*
 * Gets the number of reads done for Heap
 * 
 * Done for you. You should not modify this method.
 */
uint32_t Heap::get_num_reads() {
    return this->num_reads;
}

/*
 * Gets the total number of bytes in Heap
 * 
 * Done for you. You should not modify this method.
 */
size_t Heap::get_total_bytes() {
    return this->total_bytes;
}

/*
 * Prints out the contents of the Heap from start_address to end_address
 * 
 * Done for you. You should not modify this method.
 */
void Heap::print_heap(uint32_t start_address, uint32_t end_address) {
    cout << "Heap contents:" << endl;
    for(uint32_t i = start_address & ~0x3; i < ((end_address + 3) & ~0x3); i++) {
        if(i % 4 == 0) {
            cout << "0x" << hex << i << ": ";
        }
        cout << std::setfill('0') << std::setw(2) 
              << std::hex << static_cast<int>((uint8_t)this->heap[i - this->base_address]) << " ";
        if(i % 4 == 3) {
            cout << endl;
        }
    }
}
