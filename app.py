from flask import Flask, request, jsonify
from flask_cors import CORS
import os
import subprocess

app = Flask(__name__)
CORS(app)  # Allow external requests

UPLOAD_FOLDER = os.path.abspath("uploads")  # Directory for uploaded files
os.makedirs(UPLOAD_FOLDER, exist_ok=True)  # Ensure the directory exists

@app.route('/api/compress', methods=['POST'])
def compress():
    # Check for uploaded file
    uploaded_file = request.files.get('file')
    if not uploaded_file:
        return jsonify({'error': 'No file uploaded'}), 400

    # Save the uploaded file
    input_path = os.path.join(UPLOAD_FOLDER, 'input.txt')
    output_path = os.path.join(UPLOAD_FOLDER, 'compressed.bin')
    uploaded_file.save(input_path)

    try:
        # Run the compression subprocess
        subprocess.run(
            [r'Huffman_C\huffman.exe', 'c', input_path, output_path],
            check=True
        )
    except subprocess.CalledProcessError as e:
        return jsonify({'error': 'Compression failed', 'details': str(e)}), 500
    except FileNotFoundError:
        return jsonify({'error': 'Executable not found. Check the path.'}), 500

    # Return response with the compressed file path
    return jsonify({'message': 'Compression successful!', 'output_path': output_path})

@app.route('/api/decompress', methods=['POST'])
def decompress():
    # Check for uploaded file
    uploaded_file = request.files.get('file')
    if not uploaded_file:
        return jsonify({'error': 'No file uploaded'}), 400

    # Save the uploaded file
    input_path = os.path.join(UPLOAD_FOLDER, 'compressed.bin')
    output_path = os.path.join(UPLOAD_FOLDER, 'output.txt')
    uploaded_file.save(input_path)

    try:
        # Run the decompression subprocess
        subprocess.run(
            [r'Huffman_C\huffman.exe', 'd', input_path, output_path],
            check=True
        )
    except subprocess.CalledProcessError as e:
        return jsonify({'error': 'Decompression failed', 'details': str(e)}), 500
    except FileNotFoundError:
        return jsonify({'error': 'Executable not found. Check the path.'}), 500

    # Return response with the decompressed file path
    return jsonify({'message': 'Decompression successful!', 'output_path': output_path})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
