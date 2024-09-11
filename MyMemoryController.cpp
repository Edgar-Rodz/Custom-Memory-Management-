#include "MyMemoryController.hpp"

/*
 * MyMemoryController constructor. Calls the MemoryController constructor
 *
 * Done for you. You should not modify this method.
*/
MyMemoryController::MyMemoryController(uint32_t base_address, uint32_t total_bytes) : 
    BaseMemoryController(base_address, total_bytes)
{
    this->write(base_address, word_to_bytes(total_bytes-8)); // Write the size of the block at the start and end of the block
    this->write(base_address+total_bytes-4, word_to_bytes(total_bytes-8)); // -8 because we need to account for the size of the metadata
    return;
}

/*
 * MyMemoryController destructor. Calls the MemoryController destructor
 *
 * Done for you. You should not modify this method.
*/
MyMemoryController::~MyMemoryController()
{
    return;
}

/*
 * TODO: Implement this method
 * 
 * Reads 'size' consecutive bytes from memory
 * We do not need to check if the addresses being read from are valid; it is the user's responsibility to ensure that
 * @param address: The starting address to read from
 * @param size: The number of bytes to read
 * @return: A vector of bytes read from memory in little endian order (byte at smallest address first)
*/
vector<int8_t> BaseMemoryController::read(uint32_t address, size_t size)
{
    vector<int8_t> Brayden;
    for(int i = 0; i < size; i++){
        Brayden.push_back(this->heap->read(address + i));
    }
    return Brayden;
}


/*
 * TODO: Implement this method
 *
 * Writes each byte in data to memory
 * We do not need to check if the addresses being written to are valid; it is the user's responsibility to ensure that
 * @param address: The starting address to write to
 * @param data: A vector of bytes to write to memory in little endian order (byte at smallest address first)
*/
void BaseMemoryController::write(uint32_t address, vector<int8_t> data)
{
   for(int i = 0; i < data.size(); i++){
       this->heap->write(address + i, data[i]);
   }
    return;
}

/*
 * TODO: Implement this method
 *
 * Allocates a block of memory of the given size at the smallest possible address. Returns 0 if the allocation fails.
 * Should write the negative size of the block at the start and end of the block
 * Remember the returned pointer is the first address the user can write to,
 * which is not the same as the start of the block of memory you allocate.
 * @param size: The number of bytes to allocate
 * @return: A pointer to the start of the allocated block of memory
*/
//have an index that points to the headers for memory blocks
//compare that head number with 'size' if it is big enough... with space for header, data, and footer
//return the pointer to the first memory address of data

uint32_t BaseMemoryController::malloc(size_t size)
{
    if(size == 0){
        return 0;
    }

    uint32_t start = base_address;
    uint32_t end = start + heap->get_total_bytes();
    uint32_t totsize = heap->get_total_bytes();
    uint8_t found = 0;
    size_t blockSize = size;
    size_t freeBlock = 0;
    int32_t meta;
    int32_t oldBlock = 0;
    meta = read_full_word(start);
    oldBlock = -meta;
    while(blockSize % 4 != 0){
        blockSize++;
    }

    while(!found && start < end){
        meta = read_full_word(start);
        oldBlock = -meta;
        while(oldBlock % 4 != 0){
            oldBlock++;
        }
        if(blockSize > meta || meta < 0){
            start = start + abs(oldBlock) + 8;
            if(start > base_address + heap->get_total_bytes()){
                return 0;
            }
        }else{
            found = 1;
        }
    }
    if(found) {
        if((totsize - (blockSize + 8)) == 4){
            this->write(start, word_to_bytes(-size));
            this->write(start + blockSize + 4, word_to_bytes(-size));
            this->write(start + blockSize + 8, word_to_bytes(0));
            return start + 4;
        }
        if((totsize - (blockSize + 8)) == 8){
            this->write(start, word_to_bytes(-size));
            this->write(start + blockSize + 4, word_to_bytes(-size));
            this->write(start + blockSize + 8, word_to_bytes(0));
            this->write(start + blockSize + freeBlock + 12, word_to_bytes(0));
            return start + 4;
        }

        this->write(start, word_to_bytes(-size));
        this->write(start + blockSize + 4, word_to_bytes(-size));
        freeBlock = meta - blockSize - 8;
        if(meta - blockSize == 0){
            return start + 4;
        }
        this->write(start + blockSize + 8, word_to_bytes(freeBlock));
        this->write(start + blockSize + freeBlock + 12, word_to_bytes(freeBlock));
        return start + 4;
    }
    return 0;
}

/*
 * TODO: Implement this method
 *
 * Frees a block of memory at the given pointer
 * If the pointer is 0, this method should do nothing.
 * Should write the positive size of the block at the start and end of the block
 * You do not need to check if the pointer being freed is valid; it is the user's responsibility to ensure that
 * However, you should check if the pointer is within the memory system's range and return straight away if it is not
 * Remember that the pointer passed in is the first address the user can write to,
 * which is not the same as the start of the block of memory you allocated.
 * Remember to merge the new free block with the next free block if possible
 * @param ptr: The pointer to the block of memory to free
*/
void BaseMemoryController::free(uint32_t ptr)
{
    if(ptr == 0){
        return;
    }
    uint32_t start = ptr;
    int32_t meta = -this->read_full_word(ptr - 4);
    int32_t freeBlock = meta;

    while(freeBlock % 4 != 0){
        freeBlock++;
    }
    uint32_t end = ptr + freeBlock;
    this->write(start - 4, word_to_bytes(meta));
    this->write(ptr + freeBlock, word_to_bytes(meta));

    if((start - 8) >= base_address && this->read_full_word(start - 8) > 0 ){
        int32_t topBlock = read_full_word(start - 8);
        while(topBlock % 4 != 0){
            topBlock++;
        }
        this->write(start - 4, word_to_bytes(0));
        this->write(start - 8, word_to_bytes(0));
        uint32_t topStart = (start - 8) - topBlock - 4;
        this->write(topStart, word_to_bytes(freeBlock + topBlock + 8));
        this->write(end, word_to_bytes(freeBlock + topBlock + 8));
        freeBlock = topBlock + freeBlock + 8;
    }

     end += 4;
     if(end < heap->get_total_bytes() + base_address && this->read_full_word(end) >= 0){
        int32_t bottomBlock = read_full_word(end);
        uint32_t endOfStart = end + bottomBlock + 4;

        while(bottomBlock % 4 != 0){
            bottomBlock++;
        }

        int32_t newSize = read_full_word(end) + freeBlock + 8;
        this->write(end - freeBlock - 8, word_to_bytes(newSize));
        this->write(endOfStart, word_to_bytes(newSize));
    }

    return;
}

/*
 * TODO: Implement this method
 *
 * Reallocates a block of memory of the given size at the given pointer. Returns 0 if the allocation fails.
 * If the pointer is 0, this method should behave like malloc.
 * If the size is 0, this method should behave like free (and return 0).
 * If there is enough space to expand the block at the given pointer, you must do so.
 * Remember to copy the data from the old block of memory to the new block of memory if the pointer changes.
 * Remember the returned pointer is the first address the user can write to,
 * which is not the same as the start of the block of memory you allocated.
 * Remember to merge the new free block with the next free block if possible
 * You do not need to check if the pointer being reallocated is valid; it is the user's responsibility to ensure that
 * @param ptr: The pointer to the block of memory to reallocate
 * @param size: The new size of the block of memory
*/
uint32_t pad(size_t size){
    while(size % 4 != 0){
        size++;
    }
    return size;
}

uint32_t BaseMemoryController::realloc(uint32_t ptr, size_t new_size)
{
    if(new_size + 8 > base_address  + heap->get_total_bytes()){
        free(ptr);
        return 0;
    }
    if(ptr == 0){
        malloc(0);
    }
    if(new_size == 0){
        free(0);
    }
    uint32_t curr_size = -read_full_word(ptr - 4);


    if(pad(new_size) == pad(curr_size)){
        this->write(ptr - 4, word_to_bytes(-new_size));
        this->write(ptr + pad(new_size), word_to_bytes(-new_size));
        return ptr;

    }
    else if(pad(new_size) < pad(curr_size)){
        uint32_t writeData = curr_size + pad(new_size);
        uint32_t header = ptr - 4;
        uint32_t footer = ptr + pad(new_size);
        uint32_t header2 = ptr + pad(new_size) + 4;
        uint32_t footer2 = ptr + pad(-read_full_word(header));
        this->write(ptr - 4, word_to_bytes(-new_size));
        this->write(ptr + pad(new_size), word_to_bytes(-new_size));
        this->write(ptr + pad(new_size) + 4, word_to_bytes( curr_size - new_size - 8));
        uint32_t newFree = read_full_word(ptr + pad(new_size) + 4);
        this->write(ptr + pad(new_size) + 8 + newFree, word_to_bytes(newFree));
        if(read_full_word(footer2) >= 0 && read_full_word(footer2 + 4) >= 0 && read_full_word(footer2 + 4) < base_address + heap->get_total_bytes()){
            uint32_t topSize = read_full_word(header2) + read_full_word(footer2 + 4) + 8;
            this->write(header2, word_to_bytes(topSize));
            this->write(header2 + pad(topSize) + 4,word_to_bytes(topSize));
        }


    }else if(pad(new_size) > pad(curr_size)){
        bool flag = false;
        int32_t led = read_full_word(ptr - 4);
        int32_t checkBottom = read_full_word(ptr + pad(curr_size) + 4);
        uint32_t total = curr_size + pad(checkBottom) + 8;
        if(checkBottom > 0){
            flag = true;
        }
        if(checkBottom && pad(curr_size) + pad(checkBottom) + 8 > pad(new_size)){
            uint32_t total_size = pad(checkBottom) + pad(curr_size);
            this->write(ptr + pad(curr_size), word_to_bytes(0));
            this->write(ptr + pad(curr_size)+ 4, word_to_bytes(0));
            this->write(ptr - 4, word_to_bytes(-new_size));
            this->write(ptr + pad(new_size), word_to_bytes(-new_size));
            this->write(ptr + pad(new_size) + 4, word_to_bytes(total_size - pad(new_size)));
            uint32_t bottomOffset = read_full_word(ptr + pad(new_size) + 4);
            this->write(ptr + pad(new_size) + 8 + pad(bottomOffset), word_to_bytes(total_size - pad(new_size)));
        }else if(!flag && pad(curr_size) + 8 < pad(new_size)){
            uint32_t newMal = malloc(new_size);
            uint32_t Store = 0;
            uint32_t hold = 0;
            for(int i = 0; i < pad(curr_size); i++){
                Store = read_full_word(ptr + i*4);
                this->write(ptr+i*4, word_to_bytes(Store));


            }
            free(ptr);
        }
    }
    return ptr;
}
