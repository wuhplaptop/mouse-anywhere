from setuptools import setup, find_packages

setup(
    name='mouse-anywhere',
    version='0.1.0',
    description='A Python package for smooth mouse movement using a C library.',
    author='wuhp',
    author_email='wuhp@gmail.com',
    url='https://github.com/yourusername/mouse-anywhere',
    packages=find_packages(),
    include_package_data=True,
    package_data={
        'mouse_anywhere': ['mouse-anywhere.dll'],
    },
    classifiers=[
        'Programming Language :: Python :: 3',
        'Operating System :: Microsoft :: Windows',
    ],
    python_requires='>=3.6',
)
