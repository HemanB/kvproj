# KV Store Project

## Objectives

- Gain familiarity with data representation and memory layout of data.
- Gain practical knowledge of pointer and memory manipulations in C.
- Gain practical knowledge of dynamic memory allocation and data
  deserialization.

You will interact with a compact memory (serialized) representation of key-value
pairs in a buffer and an implementation of linked lists.

Please read this entire `README` before you begin working on the project. It
contains important information about the data format of the project and has
helpful debugging advice in case you run into an issue.

---

### Key-value maps and serialization 

You are familiar with the Java `Map` interface from CS 201.   It is an API for
map of keys to values.   A lookup operation on the key returns the value.
Key-value maps are common in systems software, especially as a foundation
for scalable service applications in data centers.   In this project, the keys
are instances of C type `long` (8-byte signed integer) on your machines, 
and the values are variable-length strings. 

This project defines a serialized representation of key-value pairs.
It asks requires you to perform pointer arithmetic and memory operations to
deserialize the data into a linked list implementing the map.   The file `kv_store.c`
contains skeleton functions for you to complete, each with comments describing
its behavior. 

The idea of *serialization* here is similar to the `Serializable` interface in Java.
It is also called *linearizing*, *flattening*, and *pickling*.   In this context, it means
to traverse a data structure and convert into a stream (sequence) of bytes suitable
for transmitting on a network or writing into a file.

Another process that receives
the data or reads the file can then *deserialize* (reconstitute, unpack) it back into a local copy
of the original data structure.   Various standards exist for serialized forms to permit
transmission of data among software running on machines of different types.

This project asks you to deserialize a serialized 


---

###  Deserializing the key-value map

The serialized buffer contains key value pairs in the following layout:

```
|key|size|value string|key|size|value string|...
```

Note that `key` is an 8-byte signed integer (`long`) and `size` is a 4-byte
unsigned integer (`unsigned int`).   *Note: these strings are not null-terminated.*

Deserialization is the process of reading through this buffer, starting at the
very beginning, and extracting the relevant information. In this case, it is the
process of reading key-value pairs.

In the arguments of the `deserialize()` function you must write, the buffer is
specified by its starting address in memory and its total size in bytes. The
size of each value string is variable and is specified by the size value
immediately preceding the string. Your job is to read each key-value pair from
this buffer and insert it into a linked list. **You must copy the string values
from the memory buffer to a new memory location**. The C library function
`memcpy()` is usually good for this.
[Here](https://man7.org/linux/man-pages/man3/memcpy.3.html) is one of the Man
pages for this function. You should read it carefully so you understand how to
use it!   Be sure also to null-terminate the unpacked strings so they can be used safely as strings.

The linked list is comprised of nodes of the type `KVPAIR`, which is a structure
defined in `kv_store.h` and can be seen below.

```C
typedef struct KVP {
    long key;           // the key 
    unsigned int size;  // the size of the value string
    char *val;          // pointer to the value string
    struct KVP *next;   // pointer to the next node
} KVPAIR;
```

Make sure you **fully** understand the buffer memory layout and `KVPAIR`
structure. If you have any questions, please post on Ed or come to office hours
before you begin working on the code.

Once you understand the data representation, you can begin to implement the
skeleton functions in `kv_store.c`. You have three functions to complete: 

1. `deserialize()`,
2. `lookup()`,
3. `delete()`.

Once you complete these three functions you should be able to pass all tests in
the `kv_test` executable! 

---

### Checking your work

We have included a few tools to help you check your work.

- We have included the function `print_list()`, which takes a pointer to a list of
  `KVPAIR`'s and prints the list. This may be useful in debugging your code, but
  its use is optional and **all calls to this function should be commented out
  in your final submission**.

- You can also use `printf()` to debug your functions. Its use is optional, and
  **all calls to `printf()` should be commented out in your final submission**.

- `kv_test` is an executable that checks the functional correctness of the code
  in `kv_store.c`. To build and use it, type the following two commands at your
  prompt (the `$` represents the Linux prompt):

  ```bash
  $ make
  $ ./kv_test
  ```

  Note that you must rebuild `kv_test` each time you modify your code. You may
  find it helpful to work through the functions one at a time, testing each one
  as you go. 

  There are two optional "flag" arguments available in `kv_test`: The optional
  `-f <function-name>` argument instructs `kv_test` to test only a single
  function, and the `-n <num-pairs>` argument changes the number of key-value
  pairs tested:

  ```
  $ ./kv_test -f lookup
  $ ./kv_test -n 256 -f delete
  ```

- `kv_test` is used with the test kit `hwtest.py` to test your solutions in
  various cases, and then on a wide range of inputs.

- Each function is tested separately to allow for the opportunity for partial
  credit should you only complete some of the functions.

- Some test cases used for autograding in Gradescope may be hidden.

You should run the local test script before submitting to Gradescope:

```
$ python3 hwtest.py ALL
```

Any test you pass in the local tester should also pass on Gradescope. If this is
not the case, post on Ed or come to office hours.

---

### Valgrind

Unlike `kv_test`, the Python test script will use the memory checking tool
**Valgrind** to check if your program has any memory leaks or interacts unsafely
with memory. The test kit will report if a Valgrind error occurs and will deduct
points in the corresponding test. Valgrind is briefly covered in section 13.6 of
C for Java Programmers, and you can run it yourself on `kv_test` to figure out
the problem:

```
$ valgrind --leak-check=full ./kv_test <arguments for kv_test>
```

Valgrind will detect all memory leaks, use of uninitalized values, and reads of
invalid memory locations. If you have no memory leaks, it will report the
following at the end of the output:

```
All heap blocks were freed -- no leaks are possible
```

---

### Submission

- Submit your project on Gradescope using the GitLab submission process.

- ***Team specific instructions*** 
  - Teams should only do one submission via a group submission.
  - Please also edit the file called `statement.txt` and summarize each team member's contributions to the project.
  
