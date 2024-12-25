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
    url='https://github.com/wuhplaptop/mouse-anywhere',  # Replace with your repository URL
    packages=find_packages(),
    include_package_data=True,
    package_data={
        'mouse_anywhere': ['mouse-anywhere.dll'],
    },
    classifiers=[
        'Programming Language :: Python :: 3',
        'Operating System :: Microsoft :: Windows',
        'License :: OSI Approved :: MIT License',  # Adjust license as needed
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Libraries',
    ],
    keywords='mouse automation cursor movement',
    python_requires='>=3.6',
    install_requires=[
        # List runtime dependencies here
        # Example: 'numpy>=1.18.0'
    ],
    extras_require={
        'dev': [
            'pytest>=6.0.0',  # For running tests
            'wheel',          # For building wheels
            'twine',          # For uploading to PyPI
        ],
    },
    test_suite='tests',  # Explicitly define the test directory
    tests_require=[
        'pytest>=6.0.0',  # Add pytest for testing
    ],
    entry_points={
        'console_scripts': [
            # Add any CLI tools your package might provide
            # Example: 'mouse-anywhere-cli=mouse_anywhere.cli:main',
        ]
    },
    project_urls={
        'Bug Reports': 'https://github.com/wuhplaptop/mouse-anywhere/issues',
        'Source': 'https://github.com/wuhplaptop/mouse-anywhere',
    },
)
