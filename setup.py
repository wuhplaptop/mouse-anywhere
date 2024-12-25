from setuptools import setup, find_packages
import os

# Read the README for the long description
with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

setup(
    name='mouse-anywhere',
    version='0.1.2',
    description='A Python package for smooth mouse movement using a C library.',
    long_description=long_description,
    long_description_content_type="text/markdown",
    author='wuhp',
    author_email='wuhp@gmail.com',
    url='https://github.com/wuhplaptop/mouse-anywhere',
    packages=find_packages(),
    include_package_data=True,
    package_data={
        'mouse_anywhere': ['mouse-anywhere.dll'],  # Ensure the DLL is included
    },
    classifiers=[
        'Programming Language :: Python :: 3',
        'Operating System :: Microsoft :: Windows',
        'License :: OSI Approved :: MIT License',
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Libraries',
    ],
    keywords='mouse automation cursor movement',
    python_requires='>=3.6',
    install_requires=[],
    extras_require={
        'dev': [
            'pytest>=6.0.0',
            'wheel',
            'twine',
        ],
    },
    test_suite='tests',  # Explicitly define the test directory
    tests_require=[
        'pytest>=6.0.0',
    ],
    entry_points={
        'console_scripts': [],
    },
    project_urls={
        'Bug Reports': 'https://github.com/wuhplaptop/mouse-anywhere/issues',
        'Source': 'https://github.com/wuhplaptop/mouse-anywhere',
    },
)
