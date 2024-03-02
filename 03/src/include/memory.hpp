#pragma once
#include <cstdint>
#include <cstddef>

namespace memory {
    // Nuestro "heap" apunta al primer Bloque de RAM libre en PC 32 bits.
    // Ver tabla "Extended Memory" https://wiki.osdev.org/Memory_Map_(x86)
    constexpr size_t heap_address {0x02000000};
    constexpr size_t heap_size {0x04000000}; // 512 Megas. 67108864
}


// TODO:
struct MemoryStatus {
    size_t free;
    size_t chunks;
};

struct MemoryChunk {
    MemoryChunk* prev {nullptr};
    MemoryChunk* next {nullptr};
    size_t size {0};
    bool is_free {true};

};

class MemoryManager {
    public:
    [[nodiscard]] bool init(uintptr_t start, size_t size);
    [[nodiscard]] void* allocate(size_t size);
    void free(void* ptr);

    private:
    static constexpr unsigned int chunk_size {sizeof(MemoryChunk)};
    MemoryChunk* head {nullptr};
    size_t memory_start {0};
    size_t memory_size {0};
};