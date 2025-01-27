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

print("LittleFS image 'image.bin' created successfully.")
