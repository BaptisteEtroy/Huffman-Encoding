from flask import Blueprint, request, jsonify, send_from_directory, render_template
import subprocess
import os

UPLOAD_FOLDER = os.path.dirname(os.path.abspath(__file__))  # Directory for uploaded files

# Create a Blueprint
routes = Blueprint('routes', __name__)
routes.config = {'UPLOAD_FOLDER': UPLOAD_FOLDER}

@routes.route('/')
def index():
    # Render the main web page
    return render_template('index.html')

@routes.route('/compress', methods=['POST'])
def compress():
    # Check if a file is uploaded
    uploaded_file = request.files.get('file')
    if not uploaded_file:
        return jsonify({'error': 'No file uploaded'}), 400

    # Save the uploaded file
    input_path = os.path.join(routes.config['UPLOAD_FOLDER'], 'input.txt')
    output_path = os.path.join(routes.config['UPLOAD_FOLDER'], 'compressed.bin')
    uploaded_file.save(input_path)

    try:
        # Run the Huffman compression executable
        subprocess.run(['./Huffman_C/main', 'c', input_path, output_path], check=True)
    except subprocess.CalledProcessError as e:
        return jsonify({'error': f'Compression failed: {str(e)}'}), 500

    response = {
        'message': 'Compression successful!',
        'downloadLink': f'/download/{os.path.basename(output_path)}',
        'filename': os.path.basename(output_path)
    }
    return jsonify(response)

@routes.route('/decompress', methods=['POST'])
def decompress():
    # Check if a file is uploaded
    uploaded_file = request.files.get('file')
    if not uploaded_file:
        return jsonify({'error': 'No file uploaded'}), 400

    # Save the uploaded file
    input_path = os.path.join(routes.config['UPLOAD_FOLDER'], 'compressed.bin')
    output_path = os.path.join(routes.config['UPLOAD_FOLDER'], 'output.txt')
    uploaded_file.save(input_path)

    try:
        # Run the Huffman decompression executable
        subprocess.run(['./Huffman_C/main', 'd', input_path, output_path], check=True)
    except subprocess.CalledProcessError as e:
        return jsonify({'error': f'Decompression failed: {str(e)}'}), 500

    response = {
        'message': 'Decompression successful!',
        'downloadLink': f'/download/{os.path.basename(output_path)}',
        'filename': os.path.basename(output_path)
    }
    return jsonify(response)

@routes.route('/download/<path:filename>', methods=['GET'])
def download(filename):
    # Securely serve the file for download
    file_path = os.path.join(routes.config['UPLOAD_FOLDER'], filename)
    if os.path.exists(file_path):
        return send_from_directory(
            routes.config['UPLOAD_FOLDER'],
            filename,
            as_attachment=True,
            download_name=filename  # Specify the download filename
        )
    return jsonify({'error': 'File not found'}), 404
