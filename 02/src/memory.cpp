#include "memory.hpp"

[[nodiscard]] bool MemoryManager::init(uintptr_t start, size_t size) {
    memory_start = start;
    memory_size = size;
    head = reinterpret_cast<MemoryChunk*>(memory_start);
    head->size = memory_size;
    head->prev = nullptr;
    head->is_free = true;

    //TODO: Check errores
    return true;
}

[[nodiscard]] void* MemoryManager::allocate(size_t size) {
    MemoryChunk* current = head;
    while (current) {
        // Bloque libre y con espacio?
        if (current->is_free && current->size >= size) {
            current->is_free = false;
            // Si es más grande, se divide y se crea un chunk libre.
            if (current->size > size + chunk_size) {
                uintptr_t new_chunk_addr = reinterpret_cast<uintptr_t>(current) + size + chunk_size;
                MemoryChunk* new_chunk = reinterpret_cast<MemoryChunk*>(new_chunk_addr);
                new_chunk->size = current->size - size - chunk_size;
                new_chunk->prev = current;
                new_chunk->next = current->next;
                new_chunk->is_free = true;                
                
                current->next = new_chunk;
                current->size = size;
            }
            // Devuelve la dirección física del bloque asignado.
            return reinterpret_cast<void*>(current + 1);

        }
        current = current->next;
    }
    return nullptr;
}

void MemoryManager::free(void* ptr) {
    // Obtener la dirección absoluta, restando el tamaño del bloque.
    MemoryChunk* chunk = reinterpret_cast<MemoryChunk*>(reinterpret_cast<uintptr_t>(ptr) - chunk_size);
    chunk->is_free = true;

    // Si el bloque anterior está libre, se fusiona.
    if (chunk->prev && chunk->prev->is_free) {
        chunk->prev->next = chunk->next;
        chunk->prev->size += chunk->size + chunk_size;
        if (chunk->next) {
            chunk->next->prev = chunk->prev;
        }
        chunk = chunk->prev;
    }

    // Lo mismo con el siguiente
    if (chunk->next && chunk->next->is_free) {
        chunk->size += chunk->next->size + chunk_size;
        chunk->next = chunk->next->next;
        if (chunk->next) {
            chunk->next->prev = chunk;
        }
    }
}
