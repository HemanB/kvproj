# KV Store Project

## Objectives


This project involves implementing a map of key-value pairs using a linked list, and loading the map
from a *serialized* representation of the key-value
pairs in a memory buffer.   Here are some objectives:

- Gain familiarity with data representation and memory layout of data.
- Gain experience with pointer and memory manipulations in C.
- Gain experience with dynamic memory allocation and data
  deserialization.
- Develop caution with pointer arithmetic.

Please read this entire `README` before you begin working on the project. It
contains important information about the data format of the project and has
helpful debugging advice in case you run into an issue.

---

### Key-value maps and serialization 

You are familiar with the Java `Map` interface from CS 201.   It is an API for a
map of keys to values.   A lookup operation on the key returns the value.
Key-value maps are common in systems software, especially as a foundation
for scalable service applications in data centers.  

This project defines a serialized representation of key-value pairs.
The idea of *serialization* here is similar to the `Serializable` interface in Java.
In this context, serializing means
to traverse a data structure and convert it into a stream (sequence) of bytes suitable
for transmitting on a network or writing into a file.  It is also called *linearizing*, *flattening*, and *pickling*.   

Another process that receives
the serialized data or reads the file can then *deserialize* (reconstitute, unpack) it back into a new local copy
of the original data structure.   Various standards exist for serialized formats to permit
transmission of data among software running on machines of different types.

This project asks you to deserialize a set of key-value pairs that are serialized in a memory
buffer, and create the map as a linked list of the key-value pairs.  You also implement functions to
search the list (`lookup`) and to `delete` a pair element from the list.   

---

###  Deserializing the key-value map

Deserialization means reading through the serialized buffer, starting at the
beginning,  and extracting the relevant data items.  In this case, it means
extracting key-value pairs and creating a linked list where each node represents
one pair. 

The serialized buffer contains key value pairs in the following layout:

```
|key|size|value string|key|size|value string|...
```

In this project, the keys
are instances of C type `long`
and the values are strings.   The
value strings have various lengths.   The size of each string (in bytes)  is specified by the `size` value
immediately preceding the string.   Each `size` is an 
unsigned integer (`unsigned int`).   

These fields occupy space in the buffer of their corresponding sizes.
The fields occupy directly adjacent and contiguous space in the buffer.   In particular, the `|` delimiter characters in the figure
do not exist in the buffer.  *Note: the value strings are not null-terminated.*

The linked list is made up of nodes of the type `KVPAIR`, which is a structure
defined in `kv_store.h` and shown below.

```C
typedef struct KVP {
    long key;           // the key 
    unsigned int size;  // the size of the value string
    char *val;          // pointer to the value string
    struct KVP *next;   // pointer to the next node
} KVPAIR;
```

You do not need to change `KVPAIR` or any definitions in `kv_store.h`.
Make sure you understand the buffer memory layout and `KVPAIR`
structure. If you have any questions, please post on Ed or come to office hours
before you begin working on the code.

### Operating on the list

Once you understand the data representation, you can begin to implement the
skeleton functions in `kv_store.c`.  The are three functions there for you to complete:

1. `deserialize()`,
2. `lookup()`,
3. `delete()`.

Those functions are the only code to change for this project.  Each has a comment
specifying its correct behavior.

In the arguments for `deserialize()`, the buffer is
specified by its starting address in memory and its total size in bytes. 
The `deserialize` function should read each key-value pair from
this buffer and insert a node for the pair into the list. 

**You must copy the string values
from the memory buffer to a new memory location**.   Consider where to
put them.   They need their space.   Don't use string functions e.g. `strncpy` to copy them: 
those functions just keep going
until they encounter a null terminator, but string values in the buffer are not null-terminated.

The C library function
`memcpy()` is good for this.  [Here](https://man7.org/linux/man-pages/man3/memcpy.3.html) is one of the Man
pages for this function. You should read it carefully so you understand how to
use it!   Be sure also to null-terminate the unpacked strings so the rest of your code can
use them safely as strings.

You also need to copy the other fields from the buffer into a `KVPAIR`.   You can use the C
`sizeof` operator to find their sizes in bytes.

The `delete` function is tricky in that it passes a *handle* (a double pointer) to the list node at the
head of the list.   The reason to use a handle here is that the list head might be a different
node after the `delete`.   The list might even be empty.   The handle argument permits `delete` to
update the caller's list head, which is not possible with a single pointer.

When you `delete`, you must take care to `free` any/every heap block you allocated for the pair,
and to comply with all aspects of the contract for safe and responsible use of heap data.

---

### Checking your work

We have included a few tools to help you check your work.

- The function `print_list()` takes a pointer to a list of
  `KVPAIR`s and prints the list. This may be useful in debugging your code, but
  its use is optional and **all calls to this function should be commented out
  in your final submission**.

- You can also use `printf()` to debug your functions. Its use is optional, and
  **all calls to `printf()` should be commented out in your final submission**.

- The `kv_test` executable incorporates the code
  in `kv_store.c` checks correctness.  To build and use it, type these commands at your
  prompt (the `$` represents the Linux prompt):

  ```bash
  $ make
  $ ./kv_test
  ```

  Note that you must make `kv_test` each time you modify your code.   Don't
  forget to save your changes before each `make`.
  
  You may
  find it helpful to work through the functions one at a time, testing each one
  as you go.   There are two optional "flag" arguments available in `kv_test`: The optional
  `-f <function-name>` argument instructs `kv_test` to test only a single
  function, and the `-n <num-pairs>` argument changes the number of key-value
  pairs tested:

  ```
  $ ./kv_test -f lookup
  $ ./kv_test -n 256 -f delete
  ```

- `kv_test` is used with the test kit `hwtest.py` to test your solutions in
  various cases, and then on a wide range of inputs.

You should run the local test script before submitting to Gradescope:

```
$ python3 hwtest.py ALL
```

Any test you pass in the local tester should also pass on Gradescope. If this is
not the case, post on Ed or come to office hours.

The autograder may also use some test cases that are hidden.
Each function is tested separately to allow for partial credit.


---

### Valgrind

Unlike `kv_test`, the Python test script uses the memory checking tool *Valgrind*
(sounds like grin) to check if your program has any memory leaks or other
memory errors. The test kit reports if Valgrind finds an error; if so it deducts
points in the corresponding test.  See Section 3.3 of *Dive Into Systems* (DIS)
about memory errors and Valgrind.  You can run Valgrind yourself on `kv_test` to
get more detail about the problem:

```
$ valgrind --leak-check=full ./kv_test <arguments for kv_test>
```

Valgrind detects all memory leaks, use of uninitalized values, and reads of
invalid memory locations. If you have no memory leaks, it reports the
following at the end of the output:

```
All heap blocks were freed -- no leaks are possible
```

---

### Submission

- Submit your project on Gradescope using the GitLab submission process.

- ***Team specific instructions*** 
  - Teams should only do one submission via a group submission.
  - Please also edit the file called `statement.txt` to acknowledge sources of help and summarize each team member's contributions to the project.
  
