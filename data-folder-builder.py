import os
from littlefs import LittleFS

# Create a LittleFS filesystem object
fs = LittleFS(block_size=4096, block_count=512)  # 2MB filesystem

# Directory to create the image from
data_dir = "main/data"

# Iterate through files in the data directory
for filename in os.listdir(data_dir):
    filepath = os.path.join(data_dir, filename)
    # Open each file and write it to the LittleFS image
    with open(filepath, "rb") as f, fs.open(filename, "wb") as outfile:
        outfile.write(f.read())

# Write the LittleFS image to a file
with open("image.bin", "wb") as fh:
    fh.write(fs.context.buffer)

import subprocess
import os

print("LittleFS image 'image.bin' created successfully.")

# Construct the path to esptool.py within the virtual environment
venv_path = "utilities/venv"  # Replace with your virtual environment directory if different
esptool_path = os.path.join(venv_path, "Scripts", "esptool.py") # Windows
if not os.path.exists(esptool_path):
    esptool_path = os.path.join(venv_path, "bin", "esptool.py") # Linux/macOS

# Upload using esptool.py from the virtual environment
try:
    esptool_process = subprocess.run([
        esptool_path, "--chip", "esp32", "--port", "/dev/ttyACM0", "--baud", "921600",
        "write_flash", "0x200000", "image.bin"
    ], check=True, capture_output=True, text=True)
    print("esptool.py output:")
    print(esptool_process.stdout)
    if esptool_process.stderr:
        print("esptool.py errors:")
        print(esptool_process.stderr)

except subprocess.CalledProcessError as e:
    print(f"Error uploading with esptool.py: {e}")
    print(e.stderr)  # Print esptool's error output
except FileNotFoundError:
    print(f"Error: esptool.py not found at '{esptool_path}'. Check your virtual environment path.")

