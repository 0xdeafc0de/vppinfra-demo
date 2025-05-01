/*
 * MIT License
 * 
 * Copyright (c) 2025 github.com/0xdeafc0de
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <vppinfra/vec.h>
#include <vppinfra/vector.h>
#include <vppinfra/mem.h> // For clib_mem_init
#include <stdio.h>

int main() {
    // Initialize the VPP memory heap with 1MB size.
    // Required for VPP vector/memory operations.
    clib_mem_init(0, 1<<20);

    // Declare a pointer to a VPP vector (initially NULL)
    u32 *my_vector = NULL;

    // Dynamically grow the vector and add elements
    vec_add1(my_vector, 10); // Append 10
    vec_add1(my_vector, 20); // Append 20
    vec_add1(my_vector, 30); // Append 30

    // Print all elements using VPP's vector length macro
    printf("Vector elements:\n");
    for (int i = 0; i < vec_len(my_vector); i++) {
        printf("my_vector[%d] = %u\n", i, my_vector[i]);
    }

    // Free memory associated with the vector
    vec_free(my_vector);

    return 0;
}
