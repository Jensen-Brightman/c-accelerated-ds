# C-Accelerated Python Data Structures (`IntVector`)

A fast, memory optimized dynamic integer array built from scratch in C, with a Python wrapper implemented with the `cffi` foreign function interface.

This project was completed in three days, with the fourth for testing and polish, to learn about manual heap optimization and pointer arithmetic.

## Key Features
- **Amortized $O(1)$:** Uses geometric growth in order to minimize heavy reallocation.
- **Cache Friendly Layout:** Contiguous 4 byte integers in memory optimize CPU cache utilization.
- **Robust Memory Safety:** Boundary validation and NULL pointer checks, paired with automated destruction sequences to work with Python's garbage collector.
- **Pythonic Wrapper:** Fully encapsulated OOP wrapper allowing *seamless* integration with standard Python workflows.

## Benchmark
Results based on averages over multiple trials. Speedup factor is approximate.
| Operation | Elements | Standard Python Time (s) | `IntVector` Time (s) | Speedup Factor (Approx) |
| :--- | :--- | :--- | :--- | :--- |
| **Search** | 300,000,000 | 3.1 | 0.2 | 16x |
| **Concatenation** | 300,000,000 | 3.4 | 1.1 | 3x |
| **Initialization** | 3,000,000,000 | 12.7 | 4.8 | 2.5x |
| **Insertion** | 600,000,000 | 2.4 | 1.2 | 2x |
| **Removal** | 600,000,000 | 0.3 | 0.2 | 1.5x |

### Benchmark Analysis
Most notably, the **16x** gain in efficiency from the search operation shows the impact of hardware aware programming. Python lists store references to objects scattered across the heap, whereas my C implementation enforces data contiguity, allowing the CPU cache prefetcher to load adjacent locations faster.

For element shifting operations, like insertion & removal, performance metrics level off around **1.5x-2x** as both candidates are forced to move large blocks of memory around.

## API and Method Signatures (Python Wrapper)
- `append(self, value: int) -> None`
- `pop(self) -> int`
- `remove(self, index: int) -> None`
- `insert(self, index: int, value: int) -> None`
- `find(self, value: int, startFrom: int = 0) -> int`
- `__getitem__(self, index: int) -> int`
- `__setitem__(self, index: int, value: int) -> None`
- `__add__(self, other: IntVector) -> IntVector`
- `cPrint(self) -> None`
- `free(self) -> None`
- `__len__(self) -> int`

## API and Method Signatures (C)
- `IntVector* createIntVector(VecSize size, VecSize capacity);`
- `IntVector* createIntVectorInit(VecSize size, VecSize capacity, VectorElem initValue);`
- `ERR_CODE concatIntVector(IntVector** dest, const IntVector* a, const IntVector* b);`
- `IntVector* createIntVectorFromArray(const VectorElem* array, VecSize len);`
- `ERR_CODE popIntVector(IntVector* intVec, VectorElem* outValue);`
- `ERR_CODE appendIntVector(IntVector* intVec, VectorElem value);`
- `ERR_CODE removeIntVector(IntVector* intVec, VecSize index);`
- `ERR_CODE insertIntVector(IntVector* intVec, VecSize index, VectorElem value);`
- `int searchIntVector(const IntVector* intVec, VectorElem value, VecSize startFrom);`
- `void printIntVector(const IntVector* intVec);`
- `void freeIntVector(IntVector* intVec);`
- `char* errorToStr(ERR_CODE errorCode);`
- `ERR_CODE getIntVector(const IntVector* intVec, VecSize index, VectorElem* outValue);`
- `ERR_CODE setIntVector(IntVector* intVec, VecSize index, VectorElem value);`

## ⚠️ Platform Support
Strictly available on Windows (`.dll` binaries). Cross platform compatibility is planned by adding runtime checks to switch to `.dylib` (MacOS) or `.so` (Linux).

## How To Run
1. Clone repository:
    ```cmd
    cd [desired location]
    git clone https://github.com/Jensen-Brightman/c-accelerated-ds.git
    ```
2. CD into the new file:
   ```cmd
   cd c-accelerated-ds
   ```
3. Create a virtual environment:
   ```cmd
   python -m venv .venv
   .venv\Scripts\activate
   ```
4. Install the required external modules:
    ```cmd
    pip install -r src\python_frontend\requirements.txt
    ```
5. Run the automated testing suite:
    ```cmd
    python src\python_frontend\tests.py
    ```
6. Run the automated profiling suite:
    ```cmd
    python src\python_frontend\benchmark.py
    ```
