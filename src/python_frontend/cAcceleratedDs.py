import os
from cffi import FFI

ffi = FFI()

ffi.cdef("""
    typedef int VectorElem;
    typedef size_t VecSize;
    typedef uint8_t ERR_CODE;
         
    typedef struct {
        VectorElem* ptr;
        VecSize size;     // Current number of elements
        VecSize capacity; // Total allocated slots
    } IntVector;

    // Instantiate methods
    IntVector* createIntVector(VecSize size, VecSize capacity);
    IntVector* createIntVectorInit(VecSize size, VecSize capacity, VectorElem initValue);
    ERR_CODE concatIntVector(IntVector** dest, const IntVector* a, const IntVector* b);
    IntVector* createIntVectorFromArray(const VectorElem* array, VecSize len);


    // Push/Pop
    ERR_CODE popIntVector(IntVector* intVec, VectorElem* outValue);
    ERR_CODE appendIntVector(IntVector* intVec, VectorElem value);

    // Remove/insert
    ERR_CODE removeIntVector(IntVector* intVec, VecSize index);
    ERR_CODE insertIntVector(IntVector* intVec, VecSize index, VectorElem value);

    // Search
    int searchIntVector(const IntVector* intVec, VectorElem value, VecSize startFrom);

    // Others
    void printIntVector(const IntVector* intVec);
    //char* stringifyIntVector(const IntVector* intVec); [NOTE] NOT IMPLEMENTED
    void freeIntVector(IntVector* intVec);
    char* errorToStr(ERR_CODE errorCode);

    // Get & Set
    ERR_CODE getIntVector(const IntVector* intVec, VecSize index, VectorElem* outValue);
    ERR_CODE setIntVector(IntVector* intVec, VecSize index, VectorElem value);
""")

dll_path = os.path.abspath("dlls/c-accelerated-ds.dll")

C = ffi.dlopen(dll_path)

class IntVector:
    ERR_CODE_MAP = {
        # 0: "VEC_SUCCESS",
        1: (ValueError, "VEC_NULL_PTR_ERR"),
        2: (MemoryError, "VEC_ALLOC_MEM_ERR"),
        3: (IndexError, "VEC_RANGE_ERR"),


        99: (RuntimeError, "VEC_ALLOC_MEM_ERR OR VEC_RANGE_ERR")
    }

    @classmethod
    def raiseIntVectorError(cls, code: int) -> None:
        errClass, msg = cls.ERR_CODE_MAP.get(code, (KeyError, f"ERROR CODE {code} not recognized."))
        raise errClass(msg)

    def __init__(self, capacity: int = None, size: int = 0, initValue: int = None, literals: list[int] = None) -> None:                
        self._vecPtr = ffi.NULL
        self._outPtr = ffi.new("VectorElem *")
        
        if initValue is not None and capacity is not None:
            self._vecPtr = C.createIntVectorInit(size, capacity, initValue)
            if self._vecPtr == ffi.NULL:
                IntVector.raiseIntVectorError(2)
        
        elif capacity is not None:
            self._vecPtr = C.createIntVector(size, capacity)
            if self._vecPtr == ffi.NULL:
                IntVector.raiseIntVectorError(2)

        elif literals is not None:
            if not literals: # Literal == []
                self._vecPtr = C.createIntVector(0, 0)
                if self._vecPtr == ffi.NULL:
                    IntVector.raiseIntVectorError(2)
                return
            
            literalArray = ffi.new("VectorElem[]", literals)
            self._vecPtr = C.createIntVectorFromArray(literalArray, len(literalArray))
            if self._vecPtr == ffi.NULL:
                IntVector.raiseIntVectorError(2)

        else:
            raise ValueError("Arguments inconsistent, IntVector could not be created.")

    def append(self, value) -> None:
        errorCode = C.appendIntVector(self._vecPtr, value)
        if errorCode!=0:
            IntVector.raiseIntVectorError(errorCode)
    
    def pop(self) -> int:
        errorCode = C.popIntVector(self._vecPtr, self._outPtr)
        if errorCode!=0:
            IntVector.raiseIntVectorError(errorCode)

        return self._outPtr[0]

    def remove(self, index: int) -> None:
        errorCode = C.removeIntVector(self._vecPtr, index)
        if errorCode!=0:
            IntVector.raiseIntVectorError(errorCode)

    def insert(self, index: int, value: int) -> None:
        errorCode = C.insertIntVector(self._vecPtr, index, value)
        if errorCode!=0:
            IntVector.raiseIntVectorError(errorCode)
            
    def find(self, value: int, startFrom: int = 0) -> int:
        result = C.searchIntVector(self._vecPtr, value, startFrom)
        
        return result

    def __getitem__(self, index) -> int:
        errorCode = C.getIntVector(self._vecPtr, index, self._outPtr)
        if errorCode!=0:
            IntVector.raiseIntVectorError(errorCode)

        return self._outPtr[0]
            
    def __setitem__(self, index, value) -> None:
        errorCode = C.setIntVector(self._vecPtr, index, value)
        if errorCode!=0:
            IntVector.raiseIntVectorError(errorCode)

    def __add__(self, other: "IntVector"):
        if type(other) is not IntVector:
            raise ValueError("Cannot concatenate IntVec with other types.")
        
        intVecPtr = ffi.new("IntVector **")
        errorCode = C.concatIntVector(intVecPtr, self._vecPtr, other._vecPtr)
        if errorCode!=0:
            IntVector.raiseIntVectorError(errorCode)
        
        result = IntVector.__new__(IntVector)
        result._vecPtr = intVecPtr[0]
        result._outPtr = ffi.new("VectorElem *")

        return result
        
    def cPrint(self) -> None:
        C.printIntVector(self._vecPtr)
    
    def free(self) -> None:
        if self._vecPtr != ffi.NULL:
            C.freeIntVector(self._vecPtr)

            self._vecPtr = ffi.NULL

    def __del__(self) -> None:
        self.free()

    def __len__(self) -> int:
        return self._vecPtr.size

if __name__ == "__main__":
    aVec = IntVector(literals=[1,2,3,4,5,6,7,8])
    bVec = IntVector(literals=[7,6,5,4,3,2,1])

    aVec.cPrint()
    bVec.cPrint()    

    cVec = aVec + bVec

    cVec.cPrint()

    print(f"Length of cVec: {len(cVec)}")