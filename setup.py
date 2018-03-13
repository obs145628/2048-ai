from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext_modules=[
Extension(name = 'env2048',
sources = ['pyx/env2048.pyx'],
library_dirs = ['.'],
libraries=['env2048native'],
language='c++',
include_dirs=['ext/ai-cpp/include']
)]

setup(
name = 'env2048',
cmdclass = {'build_ext': build_ext},
ext_modules = ext_modules    
)
