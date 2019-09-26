import sys
import unittest
from a1ece650 import *
from Class import *


class MyTest(unittest.TestCase):
    def test_parser(self):
        with self.assertRaises(Exception):
            command_parser('a "Weber Street" 1,2,3,4,5,6')

        with self.assertRaises(Exception):
            command_parser('a "Weber Street" (1,2 (3,4) (5,6)')

        with self.assertRaises(Exception):
            command_parser('a"Weber Street" (1,2) (3,4) (5,6)')

        with self.assertRaises(Exception):
            command_parser('a "Weber Street"(1,2) (3,4) (5,6)')

        with self.assertRaises(Exception):
            command_parser('a "Weber Street (1,2) (3,4) (5,6)')

        with self.assertRaises(Exception):
            command_parser('a Weber Street (1,2) (3,4) (5,6)')

        with self.assertRaises(Exception):
            command_parser('a "Weber "Street" (1,2) (3,4) (5,6)')

        with self.assertRaises(Exception):
            command_parser('a "Weber 135Street" (1,2) (3,4) (5,6)')

        with self.assertRaises(Exception):
            command_parser('a "Weber Street" (1, 2)(3, -       4)')

        with self.assertRaises(Exception):
            command_parser('a "Weber Street" (1, 2    )(3, -4     )')

        with self.assertRaises(Exception):
            command_parser('a "Weber Street" (1,2)(3)')

        with self.assertRaises(Exception):
            command_parser('a "Weber Street" (1,+2)(3,4)')

        with self.assertRaises(Exception):
            command_parser('a "Weber Street" (2+3,4)(3,4)')

        with self.assertRaises(Exception):
            command_parser('a "Weber Street" (2,)(3,4)')

    def test_database(self):
        st_test = StreetDatabase()
        st_test.add(['Weber Street'], ['(1,2)', '(3,4)'])

        with self.assertRaises(Exception):
            st_test.add(['Street'], ['(1,2)', '(1,2)'])

        with self.assertRaises(Exception):
            st_test.add(['Weber Street'], ['(1,2)', '(3,4)'])

        with self.assertRaises(Exception):
            st_test.add(['WeBeR StrEeT'], ['(1,2)', '(3,4)'])

        with self.assertRaises(Exception):
            st_test.change(['Weber Street '], ['(1,2)', '(3,4)'])

        with self.assertRaises(Exception):
            st_test.remove([' Weber Street '])

    '''
    def test_isupper(self):
        """Test isupper() function of class string"""
        self.assertTrue('FOO'.isupper())
        self.assertFalse('foo'.isupper())
        self.assertFalse('foo'.isupper())
        self.assertFalse('Foo'.isupper())

    def test_failing(self):
        """A test that fails"""
        self.assertEqual(True, False)
    '''


if __name__ == '__main__':
    unittest.main()
