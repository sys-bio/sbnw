import ctypes
import os
import platform
import unittest

class API:

    def __init__(self, whos):
        self.slib = self._load_sbnw(whos)

        # Library Info Functions
        self.slib.gf_getCurrentLibraryVersion.restype = ctypes.c_char_p

    def _load_sbnw(self, whos='kyles'):
        if whos not in ['kyles', 'ciarans']:
            raise ValueError('"whos" argument must be either "kyles" or "ciarans"')
        lib = os.path.join(os.path.dirname(__file__), f'sbnw-{whos}.dll')
        if not os.path.isfile(lib):
            raise FileNotFoundError('Did not find the libsbml-draw c api "{}"'.format(lib))
        return ctypes.CDLL(lib)

    # Library Info Functions
    def getCurrentLibraryVersion(self):
        return self.slib.gf_getCurrentLibraryVersion().decode('utf-8')



if __name__ == '__main__':
    unittest.main()
