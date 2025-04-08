# Using VPP's `vppinfra` Library in an External Application

Vector Packet Processing (VPP) is a high-performance packet processing framework that provides a rich set of libraries for building network applications. One of its core libraries, `vppinfra`, offers powerful data structures like vectors, hash tables, and more, which can be used in external applications. This article walks you through the process of compiling a test application that uses VPP's vector implementation from the `vppinfra` library.

## Prerequisites

Before starting, ensure you have the following:

1. **VPP Installed**:
   - Install VPP on your system. On Ubuntu, you can install it using:
     ```bash
     sudo apt-get install vpp-dev
     ```
   - If you are building VPP from source, ensure the `vppinfra` library is built and available.

2. **Development Tools**:
   - A C compiler (e.g., `gcc` or `clang`).
   - `make` or a similar build tool.

3. **VPP Source Code** (Optional):
   - If you are using a custom build of VPP, ensure you have the source code available.

## Step 1: Write a Test Application

Let’s create a simple C program that uses VPP's vector implementation. Save the following code in a file named `test_vec.c`:

```c
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
```

## Step 2: Compile the Application
To compile the application, you need to link it with the vppinfra library. Here’s how you can do it:

1. Locate the VPP Library and Headers
Headers: The VPP headers are typically located in /usr/include/vppinfra or /ws/vpp/src/ (if you built VPP from source).

Library: The libvppinfra.so library is usually located in /usr/lib/x86_64-linux-gnu/ or /ws/vpp/build-root/build-vpp_debug-native/vpp/lib/.

2. Compile the Program
Use the following command to compile the program:

```bash
gcc -o test_vec test_vec.c -I. -I/ws/vpp/src/ -L/ws/vpp/build-root/build-vpp_debug-native/vpp/lib/ -lvppinfra
```

-I/ws/vpp/src/:  Specifies the directory containing the VPP headers.

-L/ws/vpp/build-root/build-vpp_debug-native/vpp/lib/: Specifies the directory containing the libvppinfra.so library.

-lvppinfra: Links the program with the vppinfra library.

## Step 3: Set the Library Path
If the libvppinfra.so library is not in a standard system library path (e.g., /usr/lib), you need to set the LD_LIBRARY_PATH environment variable to include the directory containing the library:

```bash
export LD_LIBRARY_PATH=/ws/vpp/build-root/build-vpp_debug-native/vpp/lib/:$LD_LIBRARY_PATH
```

## Step 4: Run the Application
Now, you can run the compiled application:

```bash
$ ./test_vec
Vector elements:
my_vector[0] = 10
my_vector[1] = 20
my_vector[2] = 30
```

## Step 5: Debugging (Optional)
If the program crashes or behaves unexpectedly, you can use debugging tools like gdb or valgrind to diagnose the issue.

Using gdb
Compile the program with debug symbols:

```bash
gcc -g -o test_vec test_vec.c -I/ws/vpp/src/ -L/ws/vpp/build-root/build-vpp_debug-native/vpp/lib/ -lvppinfra
```
Run the program with gdb:

```bash
gdb ./test_vec
```
Set breakpoints and step through the code:

```bash
(gdb) break main
(gdb) run
(gdb) next
```

## Step 6: Advanced Usage
1. Embedding the Library Path
To avoid setting LD_LIBRARY_PATH, you can embed the library path directly into the executable using the -Wl,-rpath linker flag:

```bash
gcc -o test_vec test_vec.c -I/ws/vpp/src/ -L/ws/vpp/build-root/build-vpp_debug-native/vpp/lib/ -lvppinfra -Wl,-rpath,/ws/vpp/build-root/build-vpp_debug-native/vpp/lib/
```

2. Using pkg-config
If VPP provides a pkg-config file, you can simplify the compilation process:

```bash
gcc -o test_vec test_vec.c $(pkg-config --cflags --libs vppinfra)
```

### Conclusion
Using VPP's vppinfra library in an external application is straightforward once you understand the compilation and linking process. By following the steps in this article, you can leverage VPP's powerful data structures, such as vectors, in your own applications. Whether you are building network applications or need high-performance data structures, vppinfra provides a robust foundation for your projects.

## References
https://wiki.fd.io/view/VPP

https://github.com/FDio/vpp

