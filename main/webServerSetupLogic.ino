bool updateCurrentImagesForPattern(int pattern);

/**
 * @brief Handles requests to get the number of pixels.
 *
 * This route returns the value of NUM_PX, which represents the number of LEDs.
 *
 * @return Sends a 200 OK response with the number of pixels as a plain text string.
 */
void handleGetPixels()
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Credentials", "true");
  server.send(200, "text/plain", String(NUM_PX));
}

/**
 * @brief Checks if a file size exceeds the maximum allowed size on LittleFS.
 *
 * Calculates the maximum allowed file size based on the total available space on LittleFS,
 * reserving 24,000 bytes (maxPX) and some extra buffer (1024 bytes) to prevent running out of space.
 *
 * @param fileSize The size of the file to check.
 *
 * @return True if the file size is within the maximum allowed size, false otherwise.
 *
 * @note This function is used to prevent writing files that exceed the maximum allowed size on LittleFS.
 * @note Prints an error message to the serial console if the file size exceeds the maximum allowed size.
 * @example checkFileSpace(10000) returns true if the file size is within the maximum allowed size
 * @example checkFileSpace(30000) returns false if the file size exceeds the maximum allowed size
 */

bool checkFileSpace(size_t fileSize)
{
  // Get total available space on LittleFS
  FSInfo fs_info;
  LittleFS.info(fs_info);
  size_t totalSpace = fs_info.totalBytes;
  size_t availableSpace = fs_info.usedBytes;

  // Calculate maximum allowed file size
  size_t maxAllowedSize = totalSpace - maxPX - 1024; // reserve 24,000 bytes and some extra buffer

  // Check if file size exceeds the max allowed size
  if (fileSize > maxAllowedSize)
  {
    Serial.println("Error: File size exceeds max allowed size");
    return false;
  }
  return true;
}

/**
 * @brief Formats a size in bytes into a human-readable string.
 *
 * Converts a size in bytes into a string with appropriate units (B, KB, MB, GB).
 * The conversion is done by dividing the size by the appropriate power of 1024.
 *
 * @param bytes The size in bytes to format.
 *
 * @return A string representing the size with appropriate units.
 *
 * @note This function is used to convert a size in bytes into a human-readable string.
 * @note The function uses floating-point division to ensure accurate results.
 * @example formatBytes(1024) returns "1.00KB"
 * @example formatBytes(1024 * 1024) returns "1.00MB"
 * @example formatBytes(1024 * 1024 * 1024) returns "1.00GB"
 */
String formatBytes(size_t bytes)
{
  if (bytes < 1024)
  {
    return String(bytes) + "B";
  }
  else if (bytes < (1024 * 1024))
  {
    return String(bytes / 1024.0) + "KB";
  }
  else if (bytes < (1024 * 1024 * 1024))
  {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }
  else
  {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

/**
 * @brief Determines the MIME content type for a given file.
 *
 * Returns the MIME content type based on the file extension.
 * Supports common file types such as HTML, CSS, JavaScript, images, and archives.
 *
 * @param filename The name of the file to determine the content type for.
 *
 * @return The MIME content type as a string.
 *
 * @note This function is used to determine the content type of files served by the web server.
 * @note If the "download" argument is present, returns "application/octet-stream" to force download.
 * @note Defaults to "text/plain" for unknown file types.
 * @example getContentType("index.html") returns "text/html"
 * @example getContentType("image.png") returns "image/png"
 * @example getContentType("unknown.ext") returns "text/plain"
 */
String getContentType(String filename)
{
  if (server.hasArg("download"))
    return "application/octet-stream";
  else if (filename.endsWith(".htm"))
    return "text/html";
  else if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".gif"))
    return "image/gif";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".xml"))
    return "text/xml";
  else if (filename.endsWith(".pdf"))
    return "application/x-pdf";
  else if (filename.endsWith(".zip"))
    return "application/x-zip";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  else if (filename.endsWith(".bin"))
    return "application/octet-stream";
  return "text/plain";
}

/**
 * @brief Handles CORS preflight (OPTIONS) requests.
 *
 * Sends the necessary headers to respond to CORS preflight requests.
 * This allows cross-origin requests from web applications.
 *
 * @note This function is used to handle CORS preflight requests.
 * @note Sends the following headers:
 *  - Access-Control-Allow-Origin: * (allows requests from all origins)
 *  - Access-Control-Allow-Methods: GET, POST, OPTIONS (allows GET, POST, and OPTIONS methods)
 *  - Access-Control-Allow-Headers: Content-Type (allows Content-Type header in requests)
 * @note Returns a 204 No Content response to indicate successful preflight.
 */
void handleOptions()
{
  // This is needed to respond to CORS preflight (OPTIONS) requests
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(204); // No content response
}

/**
 * @brief Handles file read requests from the server.
 *
 * Sends the requested file to the client with the appropriate headers.
 * Supports CORS requests and sends the necessary headers to allow cross-origin requests.
 *
 * @note This function is used to handle file read requests from the server.
 * @note Sends the following headers:
 *  - Access-Control-Allow-Origin: * (allows requests from all origins)
 *  - Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS, FETCH (allows multiple methods)
 *  - Access-Control-Allow-Headers: Content-Type (allows Content-Type header in requests)
 *  - Access-Control-Allow-Credentials: true (allows credentials in requests)
 * @note Returns a 500 error if no file argument is provided.
 * @note Returns a 404 error if the file is not found.
 * @note Uses LittleFS to read the file from storage.
 */
void handleFileRead()
{
  Serial.println("handleFileRead");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Credentials", "true");

  if (!server.hasArg("file"))
  {
    Serial.println("no args detected");
    server.send(500, "text/plain", "BAD ARGS");
    return;
  }
  Serial.print("handleFileRead: ");
  String path = server.arg("file");
  Serial.println("handleFileRead: " + path);
  // if (path.endsWith("/"))
  //   path += "index.htm";
  String contentType = getContentType(path);
  Serial.println("contentType: " + contentType);
  // String pathWithGz = path + ".gz";
  if (LittleFS.exists(path))
  {
    File file = LittleFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    Serial.println("Found File: " + path);
    return;
  }
  Serial.println("Couldn't find file " + path);
  return;
}

// test function to clear memory while uploading.
void clearArray()
{
  memset(message1Data, 0, sizeof(message1Data));
}



/**
 * @brief Get the total space in LittleFS (ESP8266)
 *
 * @return size_t Total space in bytes
 */
size_t getTotalSpace()
{
    FSInfo fs_info;
    LittleFS.info(fs_info);
    return fs_info.totalBytes;
}

/**
 * @brief Get the remaining space in LittleFS (ESP8266)
 *
 * @return size_t Remaining space in bytes
 */
size_t getRemainingSpace()
{
    FSInfo fs_info;
    LittleFS.info(fs_info);
    return fs_info.totalBytes - fs_info.usedBytes;
}

/**
 * @brief Get the used space in LittleFS (ESP8266)
 *
 * @return size_t Used space in bytes
 */
size_t getUsedSpace()
{
    FSInfo fs_info;
    LittleFS.info(fs_info);
    return fs_info.usedBytes;
}


// todo: notes for the handleFileUpload function:
/*
   need more error handling
   size too big - Done
   LittleFS must not exeed bounds - Done

   wrong file name only upload abcdefghij... no more!- todo: did my solution to this work?


 * */

/**
 * @brief Handles file uploads to LittleFS File System.
 *
 * Manages the file upload process, including:
 *  - Checking for valid filenames (single character present in images string)
 *  - Tracking file size and checking against maximum allowed size (checkFileSpace())
 *  - Writing data to the file (if within size limits)
 *  - Handling upload completion, abortion, and errors
 *
 * @note This function is called by the ESP8266 server to handle file uploads.
 * @note Supports CORS requests and sends the necessary headers to allow cross-origin requests.
 * @note Uses LittleFS to read and write files.
 */
void handleFileUpload()
{
  clearArray(); // todo: did this help?

  if (server.uri() != "/edit")
    return;

  HTTPUpload &upload = server.upload();

  // Initialize file size tracking variable
  static size_t fileSize = 0;

  if (upload.status == UPLOAD_FILE_START)
  {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.sendHeader("Access-Control-Allow-Credentials", "true");

    Serial.println("uploadCounter is: ");
    Serial.println(uploadCounter);
    uploadCounter++;

    String filename = upload.filename;
    if (!filename.startsWith("/"))
    {
      filename = "/" + filename;
    }
    Serial.print("handleFileUpload Name: ");
    Serial.println(filename);

    // Check if filename is a single character present in images string eg "/a.bin"
    if (filename.length() != 6 || images.indexOf(filename[1]) == -1)
    {
      Serial.println("Error: Invalid filename");
      server.send(400, "text/plain", "Invalid filename");
      return;
    }

    // Reset file size tracking
    fileSize = 0;

    // Open file for writing
    fsUploadFile = LittleFS.open(filename, "w");
    filename = String();
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    // Track total size of the file being written
    fileSize += upload.currentSize;
    // Check if file size exceeds the max allowed size
    if (!checkFileSpace(fileSize))
    {
      // Close the file and delete it
      if (fsUploadFile)
      {
        fsUploadFile.close();
        LittleFS.remove(upload.filename); // Remove the partially written file
      }

      // Send error response and return
      server.send(500, "text/plain", "File size exceeds limit");
      return;
    }

    // Check if file exceeds the maxPX size
    if (fileSize > maxPX)
    {
      Serial.println("Error: File size exceeds maxPX limit");

      // Close the file and delete it
      if (fsUploadFile)
      {
        fsUploadFile.close();
        LittleFS.remove(upload.filename); // Remove the partially written file
      }

      // Send error response and return
      server.send(500, "text/plain", "File size exceeds limit");
      return;
    }

    // Check remaining space before upload: 
    if (fileSize > getRemainingSpace())
    {
      Serial.println("Error: Not enough space on disk to save file");

      // Close the file and delete it
      if (fsUploadFile)
      {
        fsUploadFile.close();
        LittleFS.remove(upload.filename); // Remove the partially written file
      }

      // Send error response and return
      server.send(500, "text/plain", "Not enough space remaining to save the file");
      return;
    }

    // Proceed with writing data if within the size limit and we have space:
    if (fsUploadFile)
    {
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    Serial.println("UPLOAD FINISHED");

    if (fsUploadFile)
    {
      fsUploadFile.close();
    }

    // Update current images for the current pattern after file upload
    updateCurrentImagesForPattern(pattern);

    // Reset upload counter and file size tracker
    uploadCounter = 1;
    fileSize = 0;

    server.send(200, "text/plain", "Upload successful");
  }
  else if (upload.status == UPLOAD_FILE_ABORTED)
  {
    // Handle aborted uploads by closing and removing the partially written file
    Serial.println("UPLOAD ABORTED");

    if (fsUploadFile)
    {
      fsUploadFile.close();
      LittleFS.remove(upload.filename); // Remove the incomplete file
      Serial.println("Aborted file removed");
    }

    // Send an error response for aborted upload
    server.send(500, "text/plain", "Upload aborted");

    // Reset upload counter and file size tracker
    uploadCounter = 1;
    fileSize = 0;
  }
}

/**
 * @brief Handles file deletion requests to the server.
 *
 * Manages the file deletion process, including:
 *  - Checking for valid request arguments (at least one argument required)
 *  - Verifying the file path is not the root directory ("/") to prevent accidental deletion
 *  - Checking if the file exists before deletion using LittleFS
 *  - Deleting the file using LittleFS.remove()
 *  - Returning a 200 response on successful deletion
 *  - Returning error responses for:
 *    - Invalid requests (500, "BAD ARGS")
 *    - Bad paths (500, "BAD PATH")
 *    - File not found (404, "FileNotFound")
 *
 * @note This function is called by the server to handle file deletion requests.
 * @note Supports CORS requests and sends the necessary headers to allow cross-origin requests.
 */
void handleFileDelete()
{
  Serial.print("attempt at deleting file: ");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Credentials", "true");
  if (server.args() == 0)
    return server.send(500, "text/plain", "BAD ARGS");
  String path = server.arg(0);
  // Serial.println("handleFileDelete: " + path);
  if (path == "/")
    return server.send(500, "text/plain", "BAD PATH");
  if (!LittleFS.exists(path))
    return server.send(404, "text/plain", "FileNotFound");
  Serial.println(path);
  LittleFS.remove(path);
  server.send(200, "text/plain", "");
  path = String();
}

/**
 * @brief Handles file creation requests to the server.
 *
 * Manages the file creation process, including:
 *  - Checking for valid request arguments (at least one argument required)
 *  - Verifying the file path is not the root directory ("/") to prevent accidental creation
 *  - Checking if the file already exists using LittleFS.exists()
 *  - Creating the file using LittleFS.open() in write mode ("w")
 *  - Closing the file after creation
 *  - Returning a 200 response on successful creation
 *  - Returning error responses for:
 *    - Invalid requests (500, "BAD ARGS")
 *    - Bad paths (500, "BAD PATH")
 *    - File existence (500, "FILE EXISTS")
 *    - Creation failure (500, "CREATE FAILED")
 *
 * @note This function is not used at all in SmartPoi file uploading. todo: delete?
 * @note This function is called by the server to handle file creation requests.
 * @note Supports CORS requests and sends the necessary headers to allow cross-origin requests.
 */
void handleFileCreate()
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Credentials", "true");
  if (server.args() == 0)
    return server.send(500, "text/plain", "BAD ARGS");
  String path = server.arg(0);
  // Serial.println("handleFileCreate: " + path);
  if (path == "/")
    return server.send(500, "text/plain", "BAD PATH");
  if (LittleFS.exists(path))
    return server.send(500, "text/plain", "FILE EXISTS");
  File file = LittleFS.open(path, "w");
  if (file)
    file.close();
  else
    return server.send(500, "text/plain", "CREATE FAILED");
  server.send(200, "text/plain", "");
  path = String();
}

/**
 * @brief Handles file list requests to the server.
 *
 * Manages the file list process, including:
 *  - Checking for valid request arguments (dir) and returning an error response if missing
 *  - Opening the directory using LittleFS.openDir() and iterating through entries
 *  - Constructing a JSON response with file information (type and name)
 *  - Returning a 200 response with the JSON file list
 *  - Supports CORS requests and sends the necessary headers to allow cross-origin requests
 *  - Example JSON response: [{"type":"dir","name":"directory"},{"type":"file","name":"file.txt"}]
 *
 * @note This function is called by the server to handle file list requests.
 * @example handleFileList() handles a file list request
 */
void handleFileList()
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.sendHeader("Access-Control-Allow-Credentials", "true");
  if (!server.hasArg("dir"))
  {
    server.send(500, "text/plain", "BAD ARGS");
    return;
  }

  String path = server.arg("dir");
  // Serial.println("handleFileList: " + path);
  Dir dir = LittleFS.openDir(path);
  path = String();

  String output = "[";
  while (dir.next())
  {
    File entry = dir.openFile("r");
    if (output != "[")
      output += ',';
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += String(entry.name());
    output += "\"}";
    entry.close();
  }

  output += "]";

  server.send(200, "text/json", output);
}

/**
 * @brief Sets up the web server with routes for handling file operations, settings, and LED control.
 *
 * Initializes the web server with routes for:
 *  - Handling file operations:
 *    - Listing files (GET /list)
 *    - Reading files (GET /edit)
 *    - Creating files (PUT /edit)
 *    - Deleting files (DELETE /edit)
 *  - Managing settings:
 *    - Router setting (GET /router)
 *    - Pattern setting (GET /pattern)
 *    - Interval setting (GET /intervalChange)
 *    - Brightness setting (GET /brightness)
 *  - Controlling LED behavior
 *
 * @param router The router setting to use
 * @param pass The password to use
 * @note This function sets up the web server and its routes, but does not start the server.
 */
void webServerSetupLogic(String router, String pass)
{
  int checkChannel;
  checkChannel = int(EEPROM.read(13));
  int newChannel = checkChannel;

  File html = LittleFS.open("/site.htm", "r");
  responseHTML = "";

  if (!html)
  {
    Serial.println("Failed to open file for reading");
  }
  else
  {
    size_t fileSize = html.size();
    responseHTML.reserve(fileSize);

    // Read the entire file into responseHTML
    while (html.available())
    {
      responseHTML += (char)html.read(); // Read one character at a time and append it to the string
    }

    html.close();
    Serial.println("Finished building html");
  }

  // SERVER INIT
  server.on("/", HTTP_OPTIONS, handleOptions); // Handle CORS preflight requests
  // list directory
  server.on("/list", HTTP_GET, handleFileList);

  // load editor
  server.on("/edit", HTTP_GET, handleFileRead);

  // create file
  server.on("/edit", HTTP_PUT, handleFileCreate);

  // delete file
  server.on("/edit", HTTP_DELETE, handleFileDelete);

  // Handle file uploads
  server.on(
      "/edit", HTTP_POST, []() {

      },
      handleFileUpload);

  // called when the url is not defined here - captive portal
  server.onNotFound([]()
                    {
                      server.sendHeader("Access-Control-Allow-Origin", "*");
                      server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
                      server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
                      server.sendHeader("Access-Control-Allow-Credentials", "true");
                      server.send(200, "text/html", responseHTML); // sends webpage for eeprom settings if url not defined. Captive portal.
                    });

  // Add the /get-pixels route
  server.on("/get-pixels", HTTP_GET, handleGetPixels);

  // trying to sync poi after one re-starts. todo: did it work?
  server.on("/resetimagetouse", []()
            {
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
              server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
              server.sendHeader("Access-Control-Allow-Credentials", "true");
              statusCode = 200;
              imageToUse = 0;
              previousMillis3 = millis(); //re-set timer here!
              server.send(200, "text/plain", ""); });

  // settings - returns in format SSID, PASS, Channel, A, B, C, D, Pattern - ABCD is IP address numbers
  server.on("/returnsettings", []()
            {
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
              server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
              server.sendHeader("Access-Control-Allow-Credentials", "true");
              statusCode = 200;

              settings = LittleFS.open("/settings.txt", "r");
              String settingsSSID = settings.readStringUntil('\n');
              String settingsPASS = settings.readStringUntil('\n');

              settings.close();
              // delay(100);
              int newChannel = int(EEPROM.read(13));
              content = settingsSSID + "," + settingsPASS + "," + newChannel + "," + addrNumA + "," + addrNumB + "," + addrNumC + "," + addrNumD + "," + patternChooser;
              server.send(statusCode, "text/html", content); });

  // to activate in browser: http://192.168.1.78/router?router=1
  // don't forget main: http://192.168.1.1/router?router=1
  // nothing happens, but router is now switched on.

  // to deactivate in browser: http://192.168.8.78/router?router=0  *use actual ip address
  // don't forget main: http://192.168.8.79/router?router=0 *use actual ip address

  // Router settings changes
  server.on("/router", []()
            {
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
              server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
              server.sendHeader("Access-Control-Allow-Credentials", "true");
              String onRouter = server.arg("router"); // todo: need to handle errors what if it's too big
              if (onRouter.length() > 0)
              {
                EEPROM.write(100, 0); // clearing... Is this necessary? make it some sort of default then in case of errors..?
                EEPROM.commit();
                int newRouter;
                newRouter = onRouter.toInt();
                if (newRouter == 0 || newRouter > 1)
                { // set to not work here! Deliberate?
                  newRouter = 0;
                  routerOption = false;
                }
                else
                { // 1 for router
                  routerOption = true;
                }

                EEPROM.write(100, newRouter);
                content = "{\"Success\":\" your pattern is set \"}";
                statusCode = 200;               

                // Send the response based on the logic above
                server.send(statusCode, "application/json", content);
              }
              else
              {
                content = "{\"Error\":\"404 not found\"}";
                statusCode = 404;
                

                // Send the error response
                server.send(statusCode, "application/json", content);
              }
              EEPROM.commit(); // save for next time?
              // Serial.println("10, patternChooser saved");
              // black, this could take a while, so save power? Also an indicator...
              FastLED.showColor(CRGB::Black); });

  // Pattern settings changes
  server.on("/pattern", []()
            {
              Serial.print("pattern change requested: ");
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
              server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
              server.sendHeader("Access-Control-Allow-Credentials", "true");
              String onAddress = server.arg("patternChooserChange"); // need to handle errors what if it's too big
              if (onAddress.length() > 0)
              {
                int newPatt = onAddress.toInt();
                Serial.println(newPatt);
                
                if(newPatt < 0 || newPatt > 62) {
                    content = "{\"Error\":\"Invalid pattern\"}";
                    statusCode = 400;
                    server.send(statusCode, "application/json", content);
                    return;
                }
                
                patternChooser = newPatt;
                EEPROM.write(10, newPatt);
                
                if(newPatt > 0 && newPatt < 6) {
                  pattern = patternChooser;
                  EEPROM.write(11, newPatt);
                  // Update currentImages for the new pattern
                  if (!updateCurrentImagesForPattern(newPatt)) {
                    // No files available for this pattern, switch to pattern 1
                    pattern = 1;
                    patternChooser = 1;
                    EEPROM.write(10, 1);
                    EEPROM.write(11, 1);
                  }
                }
                else if(newPatt == 7) {
                  FastLED.showColor(CRGB::Black);
                  pattern = patternChooser;
                } else {
                  pattern = patternChooser;
                  // For patterns 8+, update currentImages and verify file exists
                  if (pattern >= 8 && pattern <= 69) {
                    if (!updateCurrentImagesForPattern(pattern)) {
                      // File doesn't exist for this pattern, switch to pattern 1
                      pattern = 1;
                      patternChooser = 1;
                      EEPROM.write(10, 1);
                      EEPROM.write(11, 1);
                    }
                  }
                }
                
                EEPROM.commit();
                content = "{\"Success\":\"Pattern set\"}";
                statusCode = 200;
                server.send(statusCode, "application/json", content);
              }
              else
              {
                Serial.println("error");
                content = "{\"Error\":\"Missing parameter\"}";
                statusCode = 400;
                server.send(statusCode, "application/json", content);
              } // nothing
            });

  // Interval settings changes
  server.on("/intervalChange", []()
            {
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
              server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
              server.sendHeader("Access-Control-Allow-Credentials", "true");
              String newInterval = server.arg("interval");
              if (newInterval.length() > 0)
              {
                // Todo: save and change this in EEPROM?
                //  EEPROM.write(10, 1); //clearing... Is this necessary? make it some sort of default then in case of errors..?
                //  EEPROM.commit();
                // change address:
                int tmp = newInterval.toInt();
                if (tmp < 1)
                {
                  interval = 500; // change every half second - shortest interval available
                }
                else if (tmp > 1800)
                { // 30 minutes maximum
                  interval = 1800 * 1000;
                }
                else
                {
                  interval = tmp * 1000; // how many seconds
                }
                content = "{\"Success\":\" your interval is set \"}";
                statusCode = 200;

                // Send the response
                server.send(statusCode, "application/json", content);
              }
              else
              {
                content = "{\"Error\":\"404 not found\"}";
                statusCode = 404;
                // Send the error response
                server.send(statusCode, "application/json", content);
              } // nothing
            });

  // Brightness Settings changes
  server.on("/brightness", []()
            {
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
              server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
              server.sendHeader("Access-Control-Allow-Credentials", "true");
              String onNewBRT = server.arg("brt");
              if (onNewBRT.length() > 0)
              {
                newBrightness = onNewBRT.toInt();
                if (newBrightness > 254)
                {
                  newBrightness = 255;
                }
                if (newBrightness < 20)
                {
                  newBrightness = 20;
                }

                FastLED.setBrightness(newBrightness); // should I be removing this becos https://github.com/FastLED/FastLED/wiki/FastLED-Temporal-Dithering
                FastLED.showColor(CRGB::Black);

                EEPROM.write(15, newBrightness);
                EEPROM.commit(); // save for next time

                content = "{\"Success\":\" your brightness is set \"}";
                statusCode = 200;

                // Send the response
                server.send(statusCode, "application/json", content);
              }
              else
              {

                content = "{\"Error\":\"404 not found\"}";
                statusCode = 404;

                // Send the error response
                server.send(statusCode, "application/json", content);
              } // nothing
            });
  // Update settings in format SSID, PASS, Channel, A, B, C, D, Pattern - ABCD is IP address numbers
  server.on("/setting", []()
            {
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, FETCH");
              server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
              server.sendHeader("Access-Control-Allow-Credentials", "true");

              settings = LittleFS.open("/settings.txt", "w");
              if (!settings)
              {
                // Serial.println("settings file open failed");
                // todo: return something useful here
              }

              settings.print(server.arg("ssid"));
              // todo: in order to change channel below we need to overwrite the router name and password? FIX!
              settings.print("\n");
              // note: never use settings.println, use print("\n") instead

              // Serial.print("saving ssid: ");
              // Serial.println(server.arg("ssid"));

              settings.print(server.arg("pwd"));
              settings.print("\n");

              // Serial.print("saving pwd: ");
              // Serial.println(server.arg("pwd"));

              settings.close();
              /////////////////////////////////////////////end spiffs settings write//////////////////////////////////////////////////

              // change channel setting in EEPROM://///////////////////////////////////////////////////////////////////////////////
              String onChannel = server.arg("channel"); // need to handle errors what if it's too big
              if (onChannel.length() > 0)
              {

                EEPROM.write(13, 1); // clearing

                EEPROM.commit();
                int newChannel2; // temp variable
                newChannel2 = onChannel.toInt();
                EEPROM.write(13, newChannel2);

                EEPROM.commit();
              }
              else
              {
                content = "{\"Error\":\"404 not found\"}";
                statusCode = 404;
                ////Serial.println("Sending 404");
                ////Serial.println("not found");
              }
              // change address setting in EEPROM://///////////////////////////////////////////////////////////////////////////////
              String onAddress = server.arg("address"); // need to handle errors what if it's too big
              if (onAddress.length() > 0)
              {

                EEPROM.write(14, 1); // clearing

                EEPROM.commit();

                // change address:
                int newAddr2; // temp variable
                newAddr2 = onAddress.toInt();
                EEPROM.write(14, newAddr2);

                EEPROM.commit();

                ////Serial.print("onChannel is now: ");
                ////Serial.println(newChannel2);

                content = "{\"Success\":\" now switch both poi off and on again\"}";
                statusCode = 200;
                // add in patternChooser variable here, choose offline patterns in app!
                //       ESP.restart(); //not using this right now but see https://github.com/esp8266/Arduino/issues/1722#issuecomment-192829825
              }
              else
              {
                content = "{\"Error\":\"404 not found\"}";
                statusCode = 404;
                ////Serial.println("Sending 404");
                ////Serial.println("not found");
              }
              onAddress = server.arg("addressA"); // need to handle errors what if it's too big
              if (onAddress.length() > 0)
              {
                EEPROM.write(16, 1); // clearing... Is this necessary? make it some sort of default then in case of errors..?
                EEPROM.commit();
                // change address:
                int newAddrA; // temp variable - not needed, use same one as previously:
                newAddrA = onAddress.toInt();
                EEPROM.write(16, newAddrA);
                EEPROM.commit();
                // Serial.println("16");
              }
              else
              {
              } // nothing

              onAddress = server.arg("addressB"); // need to handle errors what if it's too big
              if (onAddress.length() > 0)
              {
                EEPROM.write(17, 1); // clearing... Is this necessary? make it some sort of default then in case of errors..?
                EEPROM.commit();
                // change address:
                int newAddrB; // temp variable - not needed, use same one as previously:
                newAddrB = onAddress.toInt();
                EEPROM.write(17, newAddrB);
                EEPROM.commit();
                // Serial.println("17");
              }
              else
              {
              } // nothing

              onAddress = server.arg("addressC"); // need to handle errors what if it's too big
              if (onAddress.length() > 0)
              {
                EEPROM.write(18, 1); // clearing... Is this necessary? make it some sort of default then in case of errors..?
                EEPROM.commit();
                // change address:
                int newAddrC; // temp variable - not needed, use same one as previously:
                newAddrC = onAddress.toInt();
                EEPROM.write(18, newAddrC);
                EEPROM.commit();
                // Serial.println("18");
              }
              else
              {
              } // nothing

              onAddress = server.arg("patternChooserChange"); // need to handle errors what if it's too big
              if (onAddress.length() > 0)
              {
                int newPatt = onAddress.toInt();
                
                if(newPatt >= 0 && newPatt <= 62) {
                  patternChooser = newPatt;
                  EEPROM.write(10, newPatt);
                  EEPROM.commit();
                  
                  if(newPatt > 0 && newPatt < 6) {
                    pattern = patternChooser;
                    EEPROM.write(11, newPatt);
                    EEPROM.commit();
                    // Update currentImages for the new pattern
                    if (!updateCurrentImagesForPattern(newPatt)) {
                      // No files available for this pattern, switch to pattern 1
                      pattern = 1;
                      patternChooser = 1;
                      EEPROM.write(10, 1);
                      EEPROM.commit();
                      EEPROM.write(11, 1);
                      EEPROM.commit();
                    }
                  }
                  else if(newPatt == 7) {
                    FastLED.showColor(CRGB::Black);
                    pattern = patternChooser;
                  } else {
                    pattern = patternChooser;
                    // For patterns 8+, update currentImages and verify file exists
                    if (pattern >= 8 && pattern <= 69) {
                      if (!updateCurrentImagesForPattern(pattern)) {
                        // File doesn't exist for this pattern, switch to pattern 1
                        pattern = 1;
                        patternChooser = 1;
                        EEPROM.write(10, 1);
                        EEPROM.commit();
                        EEPROM.write(11, 1);
                        EEPROM.commit();
                      }
                    }
                  }
                }
              }
              else
              {
              } // nothing

              // Send the status code response
              server.send(statusCode, "application/json", content);
              // delay(50);
              //    ESP.restart(); //not using this right now but see https://github.com/esp8266/Arduino/issues/1722#issuecomment-192829825
            });

  server.begin();
}