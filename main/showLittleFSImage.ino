/**
 * @brief Displays an image stored on the ESP8266 using the LittleFS file system.
 * 
 * This function loads an image from the LittleFS file system, checks its size, 
 * and displays the image pixel-by-pixel on an LED strip using FastLED. The image
 * is stored in a compressed format and is decompressed into RGB values.
 * 
 * The function handles different cases such as missing files and oversized images.
 * It iterates through the pixels to display the image across the LED strip.
 * 
 * @note The image file should be stored in LittleFS as a binary file.
 * 
 * @todo Add converter from "bin" to filename from `listDir()` or implement image 
 * index to access the correct image. 
 * 
 * @todo Test if this version can work concurrently with older image display logic.
 * 
 * @return void
 */
int cnti = 0;

void showLittleFSImage()
{
    // Open the image file from LittleFS
    a = LittleFS.open(bin, "r"); // Open every time?

    if (!a)
    {
        // If no file is found, go to the next image
        imageToUse++;
        // FastLED.showColor(CRGB::Blue); // Display error color (optional)
        // Serial.println("Code Blue - no file found!");
    }
    else
    {
        size_t size = a.size(); // Get file size

        // Check if the image size is larger than the max allowed
        if (size > maxPX)
        {
            // Display error if the image is too large
            FastLED.showColor(CRGB::Blue); // Show blue color as error indicator
            imageToUse++;
        }
        else
        {
            // Calculate the number of pixels across based on the file size
            pxAcross = int(size / pxDown); // Should be an integer

            // Read image data into message1Data buffer
            a.read(message1Data, size);

            cnti++;
            if (cnti >= pxDown)
            {
                cnti = 0;
            }

            // Close the file after reading
            a.close();
        }
    }

    // Initialize counter for reading pixel data
    int counter = 0;

    // Loop through the pixels to display the image
    for (int j = 0; j < pxAcross; j++)
    {
        for (int i = 0; i < pxDown; i++)
        {
            byte X = message1Data[counter++]; // Get pixel data

            // Decompress and assign color values
            byte R1 = (X & 0xE0); // Extract Red value (3 bits)
            leds[i].r = R1;
            byte G1 = ((X << 3) & 0xE0); // Extract Green value (3 bits)
            leds[i].g = G1;
            byte M1 = (X << 6); // Extract Blue value (2 bits)
            leds[i].b = M1;

            // Optional: Print RGB values for debugging
            // Serial.print(R1); Serial.print(", "); Serial.print(G1); Serial.print(", "); Serial.println(M1);
        }
        
        // Display the current row of pixels on the LED strip
        FastLED.show();

        // Optional delay depending on the number of LEDs
        if (NUM_LEDS < 72)
        {
            // FastLED.delay(1); // For 160MHz clock - only applies to APA102 Strip
        }
        else
        {
            // No delay needed for larger LED strips
        }
        yield(); // let's make time here for WiFi operations! 
    }
}
