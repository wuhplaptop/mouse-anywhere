from setuptools import setup, find_packages
import os

# Read the README for the long description
with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

setup(
    name="mouse-anywhere",
    version="0.2.6",  # Incremented version for updates
    description="A Python package for smooth mouse movement using a C library.",
    long_description=long_description,
    long_description_content_type="text/markdown",
    author="wuhp",
    author_email="wuhp@gmail.com",
    url="https://github.com/wuhplaptop/mouse-anywhere",
    packages=find_packages(),
    include_package_data=True,
    package_data={
        "mouse_anywhere": ["mouse_anywhere2.dll"],  # Corrected DLL filename
    },
    classifiers=[
        "Programming Language :: Python :: 3",
        "Operating System :: Microsoft :: Windows",
        "License :: OSI Approved :: MIT License",
        "Intended Audience :: Developers",
        "Topic :: Software Development :: Libraries",
        "Topic :: Software Development :: User Interfaces",
    ],
    keywords="mouse automation cursor movement DLL",
    python_requires=">=3.6",
    install_requires=[],  # Add dependencies if needed
    extras_require={
        "dev": [
            "pytest>=6.0.0",
            "wheel",
            "twine",
        ],
    },
    test_suite="tests",
    tests_require=[
        "pytest>=6.0.0",
    ],
    entry_points={
        "console_scripts": [
            # Example: "mouse-anywhere-cli=mouse_anywhere.cli:main",
        ],
    },
    project_urls={
        "Bug Reports": "https://github.com/wuhplaptop/mouse-anywhere/issues",
        "Source": "https://github.com/wuhplaptop/mouse-anywhere",
        "Documentation": "https://github.com/wuhplaptop/mouse-anywhere/wiki",
    },
)
