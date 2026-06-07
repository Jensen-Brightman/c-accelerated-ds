from wrapper import IntVector
import time, gc

def cAcceleratedDs_ConcatTest(size) -> float:
    a = IntVector(capacity=size, size=size, initValue=555)
    b = IntVector(capacity=size, size=size, initValue=555)

    startTime = time.time()
    c = a + b
    return time.time() - startTime

def standard_ConcatTest(size) -> float:
    a = [555]*size
    b = [555]*size

    startTime = time.time()
    c = a + b
    return time.time() - startTime

def cAcceleratedDs_SearchTest(size) -> float:
    a = IntVector(capacity=size, size=size, initValue=555)

    a[size-1] = 999

    startTime = time.time()
    a.find(999)
    return time.time() - startTime

def standard_SearchTest(size) -> float:
    a = [555]*size

    a[size-1] = 999

    startTime = time.time()
    a.index(999)
    return time.time() - startTime

def cAcceleratedDs_InitializationTest(size) -> float:
    startTime = time.time()
    a = IntVector(capacity=size, size=size, initValue=999)
    return time.time() - startTime

def standard_InitializationTest(size) -> float:
    startTime = time.time()
    a = [999]*size
    return time.time() - startTime

def cAcceleratedDs_InsertTest(size) -> float:
    a = IntVector(capacity=size, size=size, initValue=999)

    startTime = time.time()
    a.insert(size//2, 555)
    return time.time() - startTime

def standard_InsertTest(size) -> float:
    a = [999]*size

    startTime = time.time()
    a.insert(size//2, 555)
    return time.time() - startTime

def cAcceleratedDs_RemoveTest(size) -> float:
    a = IntVector(capacity=size, size=size, initValue=999)

    startTime = time.time()
    a.remove(size//2)
    return time.time() - startTime

def standard_RemoveTest(size) -> float:
    a = [999]*size

    startTime = time.time()
    del a[size//2]
    return time.time() - startTime


scale = 1
testPairs = [
    (cAcceleratedDs_ConcatTest, standard_ConcatTest, 100_000_000*scale),
    (cAcceleratedDs_SearchTest, standard_SearchTest, 100_000_000*scale),
    (cAcceleratedDs_InitializationTest, standard_InitializationTest, 1_000_000_000*scale),
    (cAcceleratedDs_InsertTest, standard_InsertTest, 200_000_000*scale),
    (cAcceleratedDs_RemoveTest, standard_RemoveTest, 200_000_000*scale),
]

if __name__ == "__main__":
    for pair in testPairs:
        a, b, size = pair

        aName, aTime = a.__name__, a(size)
        gc.collect() # Force GC to complete before next test

        bName, bTime = b.__name__, b(size)
        gc.collect()
        
        print(f"Tested with {size} elements")
        print(f"{aName}: {aTime:.2f}")
        print(f"{bName}: {bTime:.2f}")
        print(f"Speedup Factor {(bTime/aTime):.2f}x\n")