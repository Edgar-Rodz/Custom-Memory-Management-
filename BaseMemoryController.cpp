#include "BaseMemoryController.hpp"

/*
 * BaseMemoryController constructor
 * 
 * Done for you. You should not modify this method.
*/
BaseMemoryController::BaseMemoryController(uint32_t base_address, uint32_t total_bytes) :
    base_address(base_address),
    heap(new Heap(base_address, total_bytes))
{
    
}

/*
 * BaseMemoryController destructor
 * 
 * Done for you. You should not modify this method.
*/
BaseMemoryController::~BaseMemoryController()
{
    delete this->heap;
}


/*
 * Resets the memory system
 *
 * Done for you. You should not modify this method.
*/
void BaseMemoryController::reset()
{
    this->heap->reset();

}

/*
 * Prints out the contents of the heap
 *
 * Done for you. You should not modify this method.
*/
void BaseMemoryController::print_heap(uint32_t start_address, uint32_t end_address)
{
    this->heap->print_heap(start_address, end_address);
}

/*
 * TODO: Implement this method
 *
 * Converts a word (4 bytes) to a vector of bytes in little endian order (byte at smallest address first)
 * For example, if the word is: 0x6053890A,
 * word_to_bytes(0x6053890A) should return {0x0A, 0x89, 0x53, 0x60}
*/
vector<int8_t> BaseMemoryController::word_to_bytes(uint32_t word)
{
   vector<int8_t> hold;
    for(int i = 0; i < 4; i++){
        hold.push_back(word & 0xFF);
        word = word >> 8;
    }

    return hold;
}

/*
 * TODO: Implement this method
 *
 * Reads a full word (4 bytes) from the memory system
 * For example, if the memory is x2000: 0A 89 53 60,
 * read_full_word(0x2000) should return 0x6053890A
*/
int32_t BaseMemoryController::read_full_word(uint32_t address)
{
     int32_t word = 0;
     for(int i = 0; i < 4; i++){
         uint8_t byte = this->heap->read(address+i);
         word |= byte << (8*i);
     }

    return word;
}
