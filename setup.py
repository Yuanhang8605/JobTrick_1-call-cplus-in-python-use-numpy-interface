from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
from Cython.Distutils import build_ext
import numpy as np


extensions = [
    Extension('gen_geo_map.gen_geo_map', 
              ['gen_geo_map/gen_geo_map.pyx', 'gen_geo_map/_gen_geo_map.cpp'],
              language='c++',
              library_dirs = ['/home/liyuan/lib/opencv/lib'],
              libraries = ['opencv_world'],
              runtime_library_dirs = ['/home/liyuan/lib/opencv/lib'],
              extra_compile_args=["-Wno-unused-function", "-std=c++11"],
              include_dirs = [np.get_include(), '/home/liyuan/lib/opencv/include']),
]


setup(ext_modules = extensions, 
      cmdclass={'build_ext': build_ext},)
