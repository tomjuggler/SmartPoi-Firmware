import os
import sys
import logging
import argparse
from pathlib import Path
from littlefs import LittleFS
import subprocess

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
            data_path = Path(data_dir)
            if not data_path.exists():
                raise FileNotFoundError(f"Data directory not found: {data_dir}")

            for filename in data_path.iterdir():
                if filename.is_file():
                    self._add_file_to_image(filename)
            
            self._write_image()
            logger.info("LittleFS image created successfully")
            return True
        except Exception as e:
            logger.error(f"Failed to create image: {str(e)}")
            return False

    def _add_file_to_image(self, filepath):
        """Add a single file to the filesystem"""
        try:
            with filepath.open("rb") as f:
                with self.fs.open(filepath.name, "wb") as outfile:
                    outfile.write(f.read())
            logger.debug(f"Added file: {filepath.name}")
        except Exception as e:
            logger.error(f"Failed to add file {filepath.name}: {str(e)}")
            raise

    def _write_image(self):
        """Write filesystem to output file"""
        try:
            with open(self.config['output_file'], "wb") as fh:
                fh.write(self.fs.context.buffer)
        except Exception as e:
            logger.error(f"Failed to write image: {str(e)}")
            raise

def upload_image(config):
    """Upload image using esptool"""
    try:
        esptool_path = Path(config['venv_path']) / ("Scripts" if sys.platform == "win32" else "bin") / "esptool.py"
        
        if not esptool_path.exists():
            raise FileNotFoundError(f"esptool.py not found at {esptool_path}")

        result = subprocess.run(
            [
                str(esptool_path),
                "--chip", config['chip_type'],
                "--port", config['serial_port'],
                "--baud", str(config['baud_rate']),
                "write_flash", config['flash_address'], config['output_file']
            ],
            check=True,
            capture_output=True,
            text=True
        )
        
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

def main():
    # Configuration (could be moved to a separate config file)
    config = {
        'block_size': 4096,
        'block_count': 512,
        'output_file': 'image.bin',
        'venv_path': 'utilities/venv',
        'chip_type': 'esp32s3',
        'serial_port': '/dev/ttyACM0',
        'baud_rate': 921600,
        'flash_address': '0x200000'
    }

    # Parse command line arguments
    parser = argparse.ArgumentParser(description="Build and upload LittleFS image")
    parser.add_argument('data_dir', help="Directory containing files to include")
    parser.add_argument('--port', help="Serial port to use")
    args = parser.parse_args()

    if args.port:
        config['serial_port'] = args.port

    # Create and upload image
    builder = LittleFSBuilder(config)
    if builder.create_image(args.data_dir):
        if upload_image(config):
            logger.info("Process completed successfully")
        else:
            logger.error("Upload failed")
    else:
        logger.error("Image creation failed")

if __name__ == "__main__":
    main()