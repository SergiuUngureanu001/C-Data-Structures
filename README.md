# Lista Container

`Lista` is a dynamic array template container for C++ offering random-access and reverse iterators, usage similar to `std::vector`. It supports efficient growth, insertion, and utility methods for flexible list management.

---

## Table of Contents

1. [Constructors](#constructors)
2. [Destructor](#destructor)
3. [Modifiers](#modifiers)
   - [push](#push)
   - [pop](#pop)
   - [assigned](#assigned)
   - [insert\_at](#insert_at)
   - [cleared](#cleared)
   - [resized](#resized)
   - [swap](#swap)
4. [Accessors](#accessors)
   - [get\_front](#get_front)
   - [get\_end](#get_end)
   - [isEmpty](#isempty)
   - [get\_size](#get_size)
   - [get\_cap](#get_cap)
   - `operator[]`
5. [Iterators](#iterators)
   - [Iterator](#iterator)
   - [ReverseIterator](#reverseiterator)
   - [begin/end/rbegin/rend](#beginend-rbegin-rend)
6. [Static Members](#static-members)
7. [Non-member Functions](#non-member-functions)
   - [print](#print)
8. [Example Usage](#example-usage)

---

## Constructors

- `Lista()`

  - Default constructs an empty list with initial capacity = 1.

- `Lista(size_t n, const T &val)`

  - Constructs a list of size `n`, each element copy-initialized to `val`.

- `Lista(size_t n)`

  - Constructs a list of size `n`, each element default-initialized (`T()`).

- `Lista(initializer_list<T> init)`

  - Constructs a list with elements from the initializer list.

- `Lista(const Lista<T> &other)`

  - Copy constructor: deep-copies contents from `other`.

---

## Destructor

- `~Lista()`
  - Default destructor, automatically releases allocated memory.

---

## Modifiers

### push

```cpp
void push(const T &val);
```

Appends `val` to the end of the list, growing capacity (doubling) if needed. Throws `std::out_of_range` on overflow (when size approaches `NMAX/2`).

### pop

```cpp
void pop();
```

Removes the last element. Throws `std::out_of_range` if the list is empty.

### assigned

```cpp
void assigned(size_t new_cap, const T &val);
```

Resizes the list to `new_cap`, filling each position with `val`. Preserves no existing data beyond assignment.

### insert\_at

```cpp
void insert_at(Iterator it, const Lista<T> &values);
```

Inserts all elements from `values` at the position pointed to by `it`. Shifts existing elements right. Throws `std::out_of_range` if `it` is out of valid range.

### cleared

```cpp
void cleared();
```

Clears all elements, setting size to 0 (capacity remains unchanged).

### resized

```cpp
void resized(size_t new_cap);
```

Resizes the list to `new_cap`. If `new_cap` < old size, truncates extra elements; if `new_cap` > old size, default-initializes new slots.

### swap

```cpp
friend void swap(Lista<T> &a, Lista<T> &b);
```

Swaps contents, size, and capacity of two lists in O(1) time.

---

## Accessors

### get\_front

```cpp
T &get_front() const;
```

Returns reference to the first element. Undefined if the list is empty.

### get\_end

```cpp
T &get_end() const;
```

Returns reference to the last element. Undefined if the list is empty.

### isEmpty

```cpp
bool isEmpty() const;
```

Returns `true` if the list contains no elements.

### get\_size

```cpp
size_t get_size() const;
```

Returns the current number of elements.

### get\_cap

```cpp
size_t get_cap() const;
```

Returns the current capacity (allocated storage size).

### operator[]

```cpp
T &operator[](size_t index);
const T &operator[](size_t index) const;
```

Provides unchecked random-access to elements by index.

---

## Iterators

### Iterator

Random-access iterator supporting:

- Increment/decrement (`++`, `--`)
- Arithmetic (`+`, `-`, `+=`, `-=`)
- Comparison (`==`, `!=`, `<`, `>`, `<=`, `>=`)
- Offset dereference (`it[n]`)

Usage in range-based for loops and STL algorithms (e.g., `sort`, `max_element`).

### ReverseIterator

Random-access reverse iterator with similar operations, iterating from end to begin.

### begin/end/rbegin/rend

```cpp
Iterator begin();
Iterator end();
ReverseIterator rbegin();
ReverseIterator rend();
```

Return iterators to the start/end of the list for forward and reverse traversal.

---

## Static Members

- `static const T NMAX`
  - Maximum value of type `T` (from `numeric_limits<T>::max()`). Used to detect potential overflow on growth.

---

## Non-member Functions

### print

```cpp
friend void print(Lista<T> &lst);
```

Prints all elements to `std::cout`, formatting floating-point types to 3 decimal places and pairs as `(first, second)`.

---

## Example Usage

```cpp
#include "lista.h"
#include <algorithm>

int main() {
    Lista<int> nums = {1, 2, 3, 4, 5};
    nums.push(6);
    nums.pop();
    nums.assigned(4, 10);      // [10, 10, 10, 10]
    nums.insert_at(nums.begin() + 2, Lista<int>{7, 8});
    for (auto &x : nums)
        std::cout << x << " ";  // 10 10 7 8 10 10
    std::sort(nums.begin(), nums.end());
    print(nums);
    return 0;
}
```


## Stiva<T> Stack Container

`Stiva` is a linked-list-based stack template container in C++ offering LIFO operations with dynamic capacity management. It supports push/pop/top operations, copying and assignment, and a helper to batch erase elements.

---

### Table of Contents
1. [Constructors](#constructors-1)
2. [Destructor](#destructor-1)
3. [Copy & Assignment](#copy--assignment)
4. [Modifiers](#modifiers-1)
   - [push](#push-1)
   - [pop](#pop-1)
   - [eraseAndSave](#eraseandsave)
5. [Accessors](#accessors-1)
6. [Helper Structures](#helper-structures)
   - [Node](#node)
   - [NodeListHelper](#nodelisthelper)
7. [Non-member Functions](#non-member-functions-1)
   - [print](#print-1)
8. [Example Usage](#example-usage-1)

---

## Constructors

- `Stiva(int n)`
  - Constructs an empty stack with initial capacity = `n`.

- `Stiva()`
  - Constructs an empty stack with initial capacity = 1.

---

## Destructor

- `~Stiva()`
  - Default destructor, automatically releases all nodes.

---

## Copy & Assignment

- `Stiva(const Stiva<T>& other)`
  - Copy constructor: performs a deep copy of `other` by reversing twice to maintain original order.

- `Stiva& operator=(const Stiva<T>& other)`
  - Copy assignment: uses the copy-and-swap idiom for exception safety.

---

## Modifiers

### push
```cpp
void push(T val);
```
Pushes `val` onto the top of the stack. If size equals capacity, doubles capacity (unless at `NMAX/2` threshold, where it prints "Overflow" and exits).

### pop
```cpp
void pop();
```
Removes the top element. If the stack is empty, prints "Invalid" without throwing.

### eraseAndSave
```cpp
NodeListHelper eraseAndSave(int k);
```
Pops up to `k` elements from the stack, saving them into a `NodeListHelper` that preserves removal order. Returns the helper containing the removed nodes.

---

## Accessors

### top
```cpp
T top() const;
```
Returns the top element by value. If the stack is empty, prints "Invalid" and returns `NMAX`.

### get_size
```cpp
size_t get_size() const;
```
Returns the number of elements in the stack.

### isEmpty
```cpp
bool isEmpty() const;
```
Returns `true` if the stack is empty.

---

## Helper Structures

### Node
Internal struct holding `T data` and a `unique_ptr<Node> next` pointer for linked-list chaining.

### NodeListHelper
Supports batch removal:
- `add(unique_ptr<Node> node)`: pushes `node` onto its own list (LIFO).
- `getData()`: returns a `vector<T>` of stored node data in removal order.

---

## Non-member Functions

### print
```cpp
friend void print(const Stiva<T>& obj);
```
Prints all elements from top to bottom to `std::cout`, separated by spaces.

---

## Example Usage
```cpp
#include "stiva.h"
#include <iostream>

int main() {
    Stiva<int> s(5);
    s.push(10);
    s.push(20);
    print(s);            // 20 10
    s.pop();
    std::cout << s.top(); // 10

    auto saved = s.eraseAndSave(1);
    for (auto x : saved.getData())
        std::cout << x << " "; // 10

    return 0;
}
```
