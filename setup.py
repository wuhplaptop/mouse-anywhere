from setuptools import setup, find_packages
from setuptools.command.build_py import build_py as build_py_orig
import os

class BuildPy(build_py_orig):
    def run(self):
        # Ensure the DLL is included in the package
        build_py_orig.run(self)
        dll_source = 'mouse-anywhere.dll'
        dll_dest = os.path.join(self.build_lib, 'mouse_anywhere', 'mouse-anywhere.dll')
        self.copy_file(dll_source, dll_dest)

setup(
    name='mouse-anywhere',
    version='0.1.0',
    description='A Python package for smooth mouse movement using a C library.',
    author='wuhp',
    author_email='wuhp@gmail.com',
    packages=find_packages(),
    include_package_data=True,
    package_data={
        'mouse_anywhere': ['mouse-anywhere.dll'],
    },
    cmdclass={'build_py': BuildPy},
    classifiers=[
        'Programming Language :: Python :: 3',
        'Operating System :: Microsoft :: Windows',
    ],
    python_requires='>=3.6',
)
