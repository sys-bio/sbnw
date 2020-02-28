import unittest

from ctypes_test import API


class TestKyles(unittest.TestCase):
    whos = 'kyles'

    def setUp(self) -> None:
        self.api = API(self.whos)

    def tearDown(self) -> None:
        pass

    def test_version(self):
        version = self.api.getCurrentLibraryVersion()
        self.assertEqual(version, '1.0.0')


class TestCiarans(unittest.TestCase):
    whos = 'ciarans'

    def setUp(self) -> None:
        self.api = API(self.whos)

    def tearDown(self) -> None:
        pass

    def test_version(self):
        version = self.api.getCurrentLibraryVersion()
        self.assertEqual(version, '1.3.27')


if __name__ == '__main__':
    unittest.main()
