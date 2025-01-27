# Setting up the Virtual Environment for data-folder-builder.py

This script requires a Python virtual environment to manage dependencies. Here's how to set it up:

1. **Navigate to the utilities directory:**

   ```bash
   cd utilities
   ```

2. **Create the virtual environment:**

   ```bash
   python3 -m venv venv
   ```

3. **Activate the virtual environment:**

   ```bash
   source venv/bin/activate  # On Linux/macOS
   venv\Scripts\activate  # On Windows
   ```

4. **Install the required packages:**

   ```bash
   pip install -r requirements.txt
   ```

Now you can run the `data-folder-builder.py` script within the activated virtual environment.
