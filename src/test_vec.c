#include <vppinfra/vec.h>
#include <vppinfra/vector.h>
#include <vppinfra/mem.h> // For clib_mem_init
#include <stdio.h>

int main() {
    // Initialize the VPP memory heap
    clib_mem_init(0, 1<<20);

    // Declare and initialize a VPP vector
    u32 *my_vector = NULL;

    // Add elements to the vector
    vec_add1(my_vector, 10);
    vec_add1(my_vector, 20);
    vec_add1(my_vector, 30);

    // Print the vector elements
    printf("Vector elements:\n");
    for (int i = 0; i < vec_len(my_vector); i++) {
        printf("my_vector[%d] = %u\n", i, my_vector[i]);
    }

    // Free the vector
    vec_free(my_vector);

    return 0;
}
