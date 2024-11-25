
# Huffman Encoding

Huffman Encoding is a project that implements file compression and decompression using the Huffman encoding algorithm. The project demonstrates the application of data compression techniques in both Python and C++.

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
├── Huffman_C/
│   ├── huffman.cpp      # C++ implementation of Huffman Encoding
│   └── huffman          # Compiled executable
├── templates/
│   └── index.html       # HTML template for web-based interface
├── app.py               # Flask application
├── routes.py            # Flask routes for web API
└── README.md            # Project documentation
```

## Requirements

- **C++ Compiler**: Supports C++11 or higher.
- **Python**: Version 3.8 or higher.
- **Flask**: For running the web application.

## Installation

### Python Environment

1. Clone the repository:

   ```bash
   git clone https://github.com/<username>/Huffman-Encoding.git
   cd Huffman-Encoding
   ```

2. Create a virtual environment and install dependencies:

   ```bash
   python3 -m venv env
   source env/bin/activate
   pip install -r requirements.txt
   ```

### C++ Compilation

1. Navigate to the `Huffman_C` directory:

   ```bash
   cd Huffman_C
   ```

2. Compile the C++ code:

   ```bash
   g++ -o huffman huffman.cpp
   ```

## Usage

### Command-Line

Run the compiled Huffman executable:

```bash
./huffman input.txt output.huf
```

To decompress:

```bash
./huffman -d output.huf decompressed.txt
```

### Web Application

1. Start the Flask server:

   ```bash
   python app.py
   ```

2. Access the web interface at `http://127.0.0.1:5000`.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
