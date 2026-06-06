import os
from cffi import FFI


class IntVector:
    _ffi = FFI()
    _ffi.cdef("""
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

    _C = _ffi.dlopen(os.path.abspath("dlls/c-accelerated-ds.dll"))

    _createIntVector = _C.createIntVector
    _createIntVectorInit = _C.createIntVectorInit
    _concatIntVector = _C.concatIntVector
    _createIntVectorFromArray = _C.createIntVectorFromArray
    _popIntVector = _C.popIntVector
    _appendIntVector = _C.appendIntVector
    _removeIntVector = _C.removeIntVector
    _insertIntVector = _C.insertIntVector
    _searchIntVector = _C.searchIntVector
    _printIntVector = _C.printIntVector
    _freeIntVector = _C.freeIntVector
    _errorToStr = _C.errorToStr
    _getIntVector = _C.getIntVector
    _setIntVector = _C.setIntVector

    _new = _ffi.new
    _NULL = _ffi.NULL

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
        self._vecPtr = IntVector._NULL
        self._outPtr = IntVector._new("VectorElem *")
        
        if initValue is not None and capacity is not None:
            self._vecPtr = IntVector._createIntVectorInit(size, capacity, initValue)
            if self._vecPtr == IntVector._NULL:
                IntVector.raiseIntVectorError(2)
        
        elif capacity is not None:
            self._vecPtr = IntVector._createIntVector(size, capacity)
            if self._vecPtr == IntVector._NULL:
                IntVector.raiseIntVectorError(2)

        elif literals is not None:
            if not literals: # Literal == []
                self._vecPtr = IntVector._createIntVector(0, 0)
                if self._vecPtr == IntVector._NULL:
                    IntVector.raiseIntVectorError(2)
                return
            
            literalArray = IntVector._new("VectorElem[]", literals)
            self._vecPtr = IntVector._createIntVectorFromArray(literalArray, len(literalArray))
            if self._vecPtr == IntVector._NULL:
                IntVector.raiseIntVectorError(2)

        else:
            raise ValueError("Arguments inconsistent, IntVector could not be created.")

    def append(self, value) -> None:
        errorCode = IntVector._appendIntVector(self._vecPtr, value)
        if errorCode!=0:
            IntVector.raiseIntVectorError(errorCode)
    
    def pop(self) -> int:
        errorCode = IntVector._popIntVector(self._vecPtr, self._outPtr)
        if errorCode!=0:
            IntVector.raiseIntVectorError(errorCode)

        return self._outPtr[0]

    def remove(self, index: int) -> None:
        errorCode = IntVector._removeIntVector(self._vecPtr, index)
        if errorCode!=0:
            IntVector.raiseIntVectorError(errorCode)

    def insert(self, index: int, value: int) -> None:
        errorCode = IntVector._insertIntVector(self._vecPtr, index, value)
        if errorCode!=0:
            IntVector.raiseIntVectorError(errorCode)
            
    def find(self, value: int, startFrom: int = 0) -> int:
        result = IntVector._searchIntVector(self._vecPtr, value, startFrom)
        
        return result

    def __getitem__(self, index) -> int:
        errorCode = IntVector._getIntVector(self._vecPtr, index, self._outPtr)
        if errorCode!=0:
            IntVector.raiseIntVectorError(errorCode)

        return self._outPtr[0]
            
    def __setitem__(self, index, value) -> None:
        errorCode = IntVector._setIntVector(self._vecPtr, index, value)
        if errorCode!=0:
            IntVector.raiseIntVectorError(errorCode)

    def __add__(self, other: "IntVector"):
        if type(other) is not IntVector:
            raise ValueError("Cannot concatenate IntVec with other types.")
        
        intVecPtr = IntVector._new("IntVector **")
        errorCode = IntVector._concatIntVector(intVecPtr, self._vecPtr, other._vecPtr)
        if errorCode!=0:
            IntVector.raiseIntVectorError(errorCode)
        
        result = IntVector.__new__(IntVector)
        result._vecPtr = intVecPtr[0]
        result._outPtr = IntVector._new("VectorElem *")

        return result
        
    def cPrint(self) -> None:
        IntVector._printIntVector(self._vecPtr)
    
    def free(self) -> None:
        if self._vecPtr != IntVector._NULL:
            IntVector._freeIntVector(self._vecPtr)

            self._vecPtr = IntVector._NULL

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