# Deque-Toolkit

## Project Overview

This repository contains a templated `deque<T>` implementation using a dynamic double array (`T** blockmap`). The container supports efficient push and pop operations at both ends, random access with `operator[]`, and automatic map resizing.

## Files

- `deque.h` - Header-only templated deque implementation using fixed-size blocks.
- `deque.cpp` - Placeholder file that includes `deque.h` to satisfy assignment structure.
- `main.cpp` - Automated test driver exercising integer and string uses, including randomized stress tests.
- `Makefile` - Build script for the test executable `deque_test`.

## Design Summary

The deque stores elements in fixed `BLOCK_SIZE` blocks. A dynamic block map manages pointers to blocks and expands when front or back growth reaches the current map boundary.

Key features:
- `T** blockmap` internal representation
- constructor / destructor handling both block pointers and blockmap
- `push_front`, `push_back`, `pop_front`, `pop_back`
- `front`, `back`, `empty`, `size`, `operator[]`
- templated to support numeric and non-numeric types

## Build and Run

To build the test program:

```bash
make
```

To run the automated tests:

```bash
./deque_test
```

## Testing

The driver performs:
- deterministic integer sequence tests
- randomized mixed push/pop/front/back/`operator[]` comparisons against `std::deque`
- string template validation

## Notes

This implementation is intentionally header-only for the templated deque, while also including a `deque.cpp` file to satisfy the assignment file structure requirement.

## Work Division

This repository is a single-author submission. All design, implementation, and testing work was completed by the same author.
