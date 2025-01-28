import os
import sys
import logging
import argparse
from pathlib import Path
from littlefs import LittleFS
import subprocess
import time

# Configure logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

class LittleFSBuilder:
    def __init__(self, config):
        self.config = config
        self.fs = LittleFS(block_size=config['block_size'], block_count=config['block_count'])

    def create_image(self, data_dir):
        """Create LittleFS image from directory"""
        try:
            logger.info("Starting the LittleFS image creation process...")
            if Path(self.config['output_file']).exists():
                logger.info("LittleFS image already exists. Skipping creation.")
                return True

            data_path = Path(data_dir)
            if not data_path.exists():
                logger.error(f"Data directory not found: {data_dir}")
                raise FileNotFoundError(f"Data directory not found: {data_dir}")

            logger.info(f"Data directory located: {data_path.resolve()}")
            file_count = 0

            for filename in data_path.iterdir():
                if filename.is_file():
                    logger.info(f"Adding file to image: {filename}")
                    self._add_file_to_image(filename)
                    file_count += 1

            logger.info(f"Total files added to the image: {file_count}")

            self._write_image()
            logger.info(f"LittleFS image successfully written to {self.config['output_file']}")
            return True
        except Exception as e:
            logger.error(f"Failed to create image: {str(e)}")
            return False

    def _add_file_to_image(self, filepath):
        """Add a single file to the filesystem"""
        try:
            logger.debug(f"Opening file for reading: {filepath}")
            with filepath.open("rb") as f:
                with self.fs.open(filepath.name, "wb") as outfile:
                    logger.debug(f"Writing file to LittleFS: {filepath.name}")
                    outfile.write(f.read())
            logger.debug(f"Successfully added file: {filepath.name}")
        except Exception as e:
            logger.error(f"Failed to add file {filepath.name}: {str(e)}")
            raise

    def _write_image(self):
        """Write filesystem to output file"""
        try:
            logger.info(f"Writing LittleFS image to {self.config['output_file']}")
            with open(self.config['output_file'], "wb") as fh:
                fh.write(self.fs.context.buffer)
            logger.info("LittleFS image write completed.")
        except Exception as e:
            logger.error(f"Failed to write image: {str(e)}")
            raise

def upload_image(config):
    try:
        # Determine the serial port
        serial_port = config['serial_port']

        # Determine the LittleFS offset
        littlefs_offset = config['littlefs_offset']

        # Determine the path to esptool.py
        esptool_path = config['esptool_path']

        # Determine the path to the LittleFS image
        image_path = config['output_file']

        # Upload the LittleFS image
        upload_command = [
            "python3",
            str(esptool_path),
            "--chip", config['chip_type'],
            "--port", serial_port,
            "--baud", str(config['baud_rate']),
            "--before", "default_reset",
            "--after", "hard_reset",
            "write_flash",
            "-z",
            f"{littlefs_offset}", image_path
        ]

        logger.info("Starting upload of LittleFS image...")
        result = subprocess.run(upload_command, check=True, capture_output=True, text=True)

        logger.info("esptool.py output:\n" + result.stdout)
        if result.stderr:
            logger.warning("esptool.py warnings:\n" + result.stderr)

        return True
    except subprocess.CalledProcessError as e:
        logger.error(f"Upload failed: {e.stderr}")
        return False
    except Exception as e:
        logger.error(f"Upload error: {str(e)}")
        return False

def compile_and_upload_sketch(config):
    """Compile and upload Arduino sketch using arduino-cli"""
    try:
        output_dir = Path(config['sketch_path']) / "build"
        arduino_cli_path = Path(config['arduino_cli_path'])

        if not arduino_cli_path.exists():
            raise FileNotFoundError(f"arduino-cli not found at {arduino_cli_path}")

        # Ensure the output directory exists
        if not output_dir.exists():
            output_dir.mkdir(parents=True, exist_ok=True)

        # Compile the sketch
        logger.info("Starting Arduino sketch compilation...")
        logger.info(f"Expected output directory: {output_dir}")
        # Identify the generated binary file
        binary_files = list(output_dir.glob("*.bin"))
        if not binary_files:
            compile_result = subprocess.run(
                [
                    str(arduino_cli_path),
                    "compile",
                    "--fqbn", config['fqbn'],
                    "--libraries", config['library_path'],
                    "--output-dir", str(output_dir),
                    "--build-property", f"partitions={config['partition_file']}",
                    config['sketch_path']
                ],
                check=True,
                capture_output=True,
                text=True
            )
            
            logger.info("arduino-cli compile output:\n" + compile_result.stdout)
            if compile_result.stderr:
                logger.warning("arduino-cli compile warnings:\n" + compile_result.stderr)

        # Identify the generated binary file
        binary_files = list(output_dir.glob("*.bin"))
        if not binary_files:
            logger.error("Compilation completed, but no binary was generated.")
            return False

        compiled_binary = binary_files[0]
        logger.info(f"Binary file found: {compiled_binary} (Size: {compiled_binary.stat().st_size} bytes)")

        # Upload the sketch
        logger.info("Starting Arduino sketch upload...")
        upload_result = subprocess.run(
            [
                str(config['arduino_cli_path']),
                "upload",
                "--port", config['serial_port'],
                "--fqbn", config['fqbn'],
                "--verbose",
                config['sketch_path']
            ],
            check=True,
            capture_output=True,
            text=True
        )
        
        logger.info("arduino-cli upload output:\n" + upload_result.stdout)
        if upload_result.stderr:
            logger.warning("arduino-cli upload warnings:\n" + upload_result.stderr)
            
        return True
    except subprocess.CalledProcessError as e:
        logger.error(f"Arduino operation failed: {e.stderr}")
        return False
    except Exception as e:
        logger.error(f"Arduino error: {str(e)}")
        return False


def main():
    # Configuration (could be moved to a separate config file)
    config = {
        'block_size': 4096,
        'block_count': 512,
        'output_file': 'image.bin',
        'venv_path': 'utilities/venv',
        'esptool_path':'/home/tom/.arduino15/packages/esp32/tools/esptool_py/4.6/esptool.py',
        'chip_type': 'esp32s3',
        'serial_port': '/dev/ttyACM0',
        'baud_rate': 921600,
        'arduino_cli_path': '/usr/bin/arduino-cli',  # Path to arduino-cli executable
        'fqbn': 'esp32:esp32:esp32s3',  # Fully Qualified Board Name
        'sketch_path': 'main',  # Path to the Arduino sketch
        'littlefs_offset': '0x210000',
        'library_path': '/run/media/tom/9109f38b-6b5f-4e3d-a26f-dd920ac0edb6/Manjaro-Home-Backup/3717d0b5-ba54-4c0a-8e8d-407af5c801bd/@home/tom/Arduino/libraries',  # Path to your custom libraries
        'partition_file': '/home/tom/.arduino15/packages/esp32/hardware/esp32/3.0.4/tools/partitions/no_ota.csv'
    }

    # Parse command line arguments
    parser = argparse.ArgumentParser(description="Build and upload LittleFS image and Arduino sketch")
    parser.add_argument('data_dir', help="Directory containing files to include in LittleFS image")
    parser.add_argument('--port', help="Serial port to use")
    args = parser.parse_args()

    if args.port:
        config['serial_port'] = args.port

    # Compile and upload Arduino sketch
    if compile_and_upload_sketch(config):
        logger.info("Arduino sketch uploaded successfully")
    else:
        logger.error("Arduino sketch upload failed")
    time.sleep(10)
    # Create and upload LittleFS image
    builder = LittleFSBuilder(config)
    if builder.create_image(args.data_dir):
        if upload_image(config):
            logger.info("LittleFS image uploaded successfully")
        else:
            logger.error("LittleFS image upload failed")

if __name__ == "__main__":
    main()
