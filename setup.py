# setup.py
from setuptools import setup, find_packages
import os
import platform
import shutil
from setuptools.command.build_py import build_py as build_py_orig

class BuildPy(build_py_orig):
    def run(self):
        # Ensure the DLL is included in the package
        build_py_orig.run(self)
        dll_source = os.path.join('mouse-anywhere', 'mouse-anywhere.dll')
        dll_dest = os.path.join(self.build_lib, 'mouse-anywhere', 'mouse-anywhere.dll')
        self.copy_file(dll_source, dll_dest)

setup(
    name='mouse-anywhere_pkg',
    version='0.1.0',
    description='A Python package for smooth mouse movement using a C library.',
    author='wuhp',
    author_email='wuhp@gmail.com',
    packages=find_packages(),
    include_package_data=True,
    package_data={
        'mouse-anywhere': ['mouse-anywhere.dll'],
    },
    cmdclass={'build_py': BuildPy},
    classifiers=[
        'Programming Language :: Python :: 3',
        'Operating System :: Microsoft :: Windows',
    ],
)
