from setuptools import setup

setup(
    name="face-pipeline",        # <--- Your package name on PyPI
    version="0.0.1",             # <--- Increment to release new versions
    py_modules=["face_pipeline"],# Because all code is in face_pipeline.py
    install_requires=[
        "requests",
        "numpy",
        "opencv-python",
        "torch",
        "Pillow",
        "gradio",
        "ultralytics",
        "facenet-pytorch",
        "torchvision",
        "deep_sort_realtime",
        "mediapipe",
    ],
    entry_points={
        "console_scripts": [
            # This installs a CLI command: "face-pipeline"
            # that calls face_pipeline:main()
            "face-pipeline = face_pipeline:main"
        ]
    },
    author="YourName",           # Adjust as needed
    author_email="you@example.com",
    description="Single-file face pipeline with YOLO, Mediapipe, and Gradio",
    url="https://github.com/yourusername/face-pipeline",   # Adjust or remove
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",          # or whatever
        "Operating System :: OS Independent",
    ],
    python_requires=">=3.7",     # Minimum Python version
)
