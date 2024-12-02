
# Huffman Encoding

Huffman Encoding is a project that implements a C++ algorithm for file compression and decompression using Huffman encoding. The project demonstrates the application of this type of data compression technique using C++, with a Python and HTML web applicaton for better user experience.

[![Google testing Documentation](https://img.shields.io/badge/Testing-Documentation-blue)](./testing.md)

## Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)

## Features

- File compression using Huffman encoding.
- Decompression that restores the original file structure.
- Python-based API for handling file I/O.
- C++ implementation for core encoding and decoding.

## Project Structure

```plaintext
Huffman-Encoding/
├── build/                # Build directory for compiled files
├── googletest/           # GoogleTest library
├── Huffman_C/            
│   ├── huffman_main.cpp  # main function
│   ├── huffman.cpp       # C++ implementation of Huffman Encoding
│   ├── huffman.h         # Header file
│   ├── main              # Compiled main executable (Linux/macOS)
│   ├── main.exe          # Compiled main executable (Windows)
├── templates/            
│   └── index.html        # HTML template for Flask web app
├── tests/    
│   ├── Test Cases        # Test cases
│   └── huffman_tests.cpp # GoogleTest
├── app.py                # Flask app
├── routes.py             # routes for webapp
├── README.md             # Project documentation
├── requirements.txt      
└──  testing.md           # Testing documentation
```

## Requirements

- **C++ Compiler**: Supports C++11 or higher.
- **Python**: Version 3.8 or higher.
- **Flask**: For running the web application.

## Installation

### WINDOWS

**Clone the Repository:** If Git is installed, open Command Prompt or PowerShell and run:


```bash
git clone https://github.com/BaptisteEtroy/Huffman-Encoding.git
cd Huffman-Encoding
```

Alternatively, download the repository as a ZIP file from GitHub and extract it.

**Set up the Python Environment:** Create and activate a virtual environment:

```bash
python -m venv env
env\Scripts\activate
```

**Install Dependencies:** Install the required Python packages:

```bash
pip install -r requirements.txt
```

**Run the Application:** Start the Flask application:

```bash
python app.py
```

Access the application at http://127.0.0.1:5000.

### MACOS

**Clone the Repository:** Open Terminal and run:

```bash
git clone https://github.com/BaptisteEtroy/Huffman-Encoding.git
cd Huffman-Encoding
```

**Set up the Python Environment:** Create and activate a virtual environment:
```bash
python3 -m venv env
source env/bin/activate
```

**Install Dependencies:** Install the required Python packages:
```bash
pip install -r requirements.txt
```

**Run the Application:** Start the Flask application:
```bash
python app.py
```

Access the application at http://127.0.0.1:5000.

# Run Using CMake
To run the project directly using the C++ executables:

## Set Up the Project:
```bash
mkdir build
cd build
cmake ..
make
```

## Run the Main Program:

### Compress a file:
```bash
./huffman_main c ../input.txt ../compressed.bin
```

### Decompress a file:
```bash
./huffman_main d ../compressed.bin ../output.txt
```


## Testing
```bash
./huffman_tests
```
[![Google testing Documentation](https://img.shields.io/badge/Testing-Documentation-blue)](./testing.md)

## License

This project is licensed under the MIT License.
