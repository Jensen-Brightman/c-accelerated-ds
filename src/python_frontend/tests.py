from wrapper import IntVector
import unittest

class IntVectorTest(unittest.TestCase):
    def testInitialization(self):
        vec = IntVector(0)
        
        self.assertEqual(len(vec), 0)

    def testSearchFunctional(self):
        vec = IntVector(literals=[10, 20, 30, 40])

        self.assertEqual(vec.find(30), 2)
        self.assertEqual(vec.find(999), -1)

    def testRangeErrorGetItem(self):
        vec = IntVector(literals=[10, 20, 30])

        with self.assertRaises(IndexError) as context:
            _ = vec[5]

        self.assertIn("VEC_RANGE_ERR", str(context.exception))

    def testRangeErrorSetItem(self):
        vec = IntVector(literals=[10, 20, 30])

        with self.assertRaises(IndexError) as context:
            vec[99] = 400

        self.assertIn("VEC_RANGE_ERR", str(context.exception))

    def testPopEmptyVector(self):
        vec = IntVector(capacity=3, size=0)

        with self.assertRaises(IndexError) as context:
            _ = vec.pop()

        self.assertIn("VEC_RANGE_ERR", str(context.exception))


    def testRemoveInvalidIndex(self):
        vec = IntVector(literals=[10, 20, 30])

        with self.assertRaises(IndexError) as context:
            vec.remove(10)

        self.assertIn("VEC_RANGE_ERR", str(context.exception))

    def testInsertInvalidIndex(self):
        vec = IntVector(literals=[10, 20, 30])

        with self.assertRaises(IndexError) as context:
            vec.insert(10, 99)

        self.assertIn("VEC_RANGE_ERR", str(context.exception))

    def testInconsistentArguments(self):
        with self.assertRaises(ValueError):
            _ = IntVector()

    def testConcatTypeValidation(self):
        vec = IntVector(literals=[10, 20, 30])

        with self.assertRaises(ValueError):
            _ = vec + [4, 5, 6]

    def testNullPointer(self):
        vec = IntVector(literals=[10, 20, 30])

        vec._vecPtr = IntVector._NULL

        with self.assertRaises(ValueError) as context:
            vec.append(999)

        self.assertIn("VEC_NULL_PTR_ERR", str(context.exception))


if __name__ == "__main__":
    unittest.main()