from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
from Cython.Distutils import build_ext
import numpy as np


extensions = [
    Extension('average.average', 
              ['average/average.pyx', 'average/_average.cpp'],
              language='c++',
              library_dirs = [],
              libraries = [],
              runtime_library_dirs = [],
              extra_compile_args=["-Wno-unused-function", "-std=c++11"],
              include_dirs = [np.get_include()]),
]


setup(ext_modules = extensions, 
      cmdclass={'build_ext': build_ext},)
