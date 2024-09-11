#include "MyMemoryController.hpp"
void testMallocZero(){
    MyMemoryController* mem = new MyMemoryController(0x2000, 20);
    uint32_t ptr = mem->malloc(8);
    delete mem;
}
void testMaxMalloc(){
    MyMemoryController* mem = new MyMemoryController(0x2000, 20);
    uint32_t ptr = mem->malloc(12);
    delete mem;
}
void testMallocBounds(){
    MyMemoryController* mem = new MyMemoryController(0x2000, 20);
    uint32_t ptr = mem->malloc(12);
    mem->malloc(100);
    delete mem;
}
void testMallZeroSize(){
    MyMemoryController* mem = new MyMemoryController(0x2000, 20);
    uint32_t ptr = mem->malloc(0);
    delete mem;
}
int main() {
    testMallocZero();
    testMaxMalloc();
    testMallocBounds();
    testMallZeroSize();
    return 0;
}