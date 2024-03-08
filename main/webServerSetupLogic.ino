////////////////////////////////////////////FSBrowser2 code:///////////////////////////////////////////////////

// format bytes
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

void handleFileRead()
{
  Serial.println("handleFileRead");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

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

// todo: note for the handleFileUpload function:
/*
   need error handling
   size too big
   wrong file name
   only upload abcdefghij... no more!
   LittleFS must not exeed bounds
 * */
void handleFileUpload()
{
  if (server.uri() != "/edit")
    return;
  HTTPUpload &upload = server.upload();
  if (upload.status == UPLOAD_FILE_START)
  {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    Serial.println("uploadCounter is: "); 
    Serial.println(uploadCounter);
    uploadCounter++;
    String filename = upload.filename;
    if (!filename.startsWith("/")){
      filename = "/" + filename;
    }
    Serial.print("handleFileUpload Name: "); 
    Serial.println(filename);
    fsUploadFile = LittleFS.open(filename, "w");
    filename = String();
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    Serial.print("handleFileUpload Data: "); 
    Serial.println(upload.currentSize);
    if (fsUploadFile){
      fsUploadFile.write(upload.buf, upload.currentSize);
      } 
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    Serial.println("UPLOAD FINISHED");
    uploadCounter = 1;
    if (fsUploadFile){
      fsUploadFile.close();
    }
    
    server.send(200, "text/plain", "");
  }
}

void handleFileDelete()
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  if (server.args() == 0)
    return server.send(500, "text/plain", "BAD ARGS");
  String path = server.arg(0);
  // Serial.println("handleFileDelete: " + path);
  if (path == "/")
    return server.send(500, "text/plain", "BAD PATH");
  if (!LittleFS.exists(path))
    return server.send(404, "text/plain", "FileNotFound");
  LittleFS.remove(path);
  server.send(200, "text/plain", "");
  path = String();
}

void handleFileCreate()
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
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

void handleFileList()
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
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

/////////////////////////////////////////////////////////////////////////////////////////END FSBrowser2////////////////////////////////////////////////////////////

void webServerSetupLogic(String router, String pass)
{
  // Serial.print("webserver router is: ");
  // Serial.println(router);
  // Serial.print("webserver pass is: ");
  // Serial.println(pass);

  int checkChannel;
  checkChannel = int(EEPROM.read(13)); // change from ascii
  int newChannel = checkChannel;

  // html taking up too much room? moved to LittleFS!
  File html = LittleFS.open("/site.htm", "r");
  responseHTML = "";

  if (!html) {
      Serial.println("Failed to open file for reading");
  } else {
      size_t fileSize = html.size(); // Get the size of the file
      responseHTML.reserve(fileSize); // Reserve memory to store the file content

      // Read the entire file into responseHTML
      while (html.available()) {
          responseHTML += (char)html.read(); // Read one character at a time and append it to the string
      }

      html.close();
      Serial.println("Finished building html");
  }
  
  /////////////////////////////////////////////////FSBrowser2////////////////////////////////////////////////////////////////////////////////////
  // SERVER INIT
  // list directory
  server.on("/list", HTTP_GET, handleFileList);
  // load editor
  server.on("/edit", HTTP_GET, handleFileRead);
  // create file
  server.on("/edit", HTTP_PUT, handleFileCreate);
  // delete file
  server.on("/edit", HTTP_DELETE, handleFileDelete);
  // first callback is called after the request has ended with all parsed arguments
  // second callback handles file uploads at that location
  server.on(
      "/edit", HTTP_POST, []()
      {
        
      },
      handleFileUpload);

  // called when the url is not defined here
  // use it to load content from SPIFFS
  server.onNotFound([]()
                    {
                      server.sendHeader("Access-Control-Allow-Origin", "*");
                      server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
                      server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
                      server.send(200, "text/html", responseHTML); // my code here, should work for eeprom settings!
                      //     server.send(204, "text/html", responseHTML); //my code here, should work for eeprom settings!
                    });

  ///////////////////////////////////////////End FSBrowser2 code////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////return settings////////////////////////////////////////////////////////
  // works with httpPostToPoiSettings processing app (in internet folder)
  server.on("/returnsettings", []()
            {
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
              server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
              //    content = "huaweiRouter,password,more";
              statusCode = 200;
              /// //Serial.println(pass);

              // below converts char array to string
              // see https://stackoverflow.com/questions/17158890/transform-char-array-into-string
              // content = str((char*)router_array);
              // String rrr = String(router_array);/
              // String rrr = String(str((char*)router_array));
              /// String ppp = String(pwd_array); //wtf thought this worked???!!!!!!!!!
              // String ppp = pass;

              // content = rrr + "," + ppp + "," + String(apChannel);/
              // content = String(router_array) + "," + String(pwd_array) + "," + String(apChannel);

              // I know try open Spiffs settings.txt and read out to var:
              ///////////////////////////////////////////////////////////////////////read spiffs settings again://///////////////////////////////////////////////////////
              // why do I have to read it all again? ???????

              settings = LittleFS.open("/settings.txt", "r");
              String anotherSettingsFUP = settings.readStringUntil('\n');
              String anotherSettingsFUP2 = settings.readStringUntil('\n');

              //

              settings.close();
              // delay(100);
              int newChannelAgainFUP = int(EEPROM.read(13));
              // delay(30);
              // MUST WE READ ALL SETTINGS AGAIN? OR USE INITIALIZED ONES LIKE addrNumA?
              //////////////////////////////////////////end read spiffs settings again/////////////////////////////////////////////////////////////////////////////
              content = anotherSettingsFUP + "," + anotherSettingsFUP2 + "," + newChannelAgainFUP + "," + addrNumA + "," + addrNumB + "," + addrNumC + "," + addrNumD + "," + patternChooser;
              server.send(statusCode, "text/html", content);
            });
  //////////////////////////////////////////////////////////////////end return settings/////////////////////////////////////////////

  // replay to all requests with same HTML
  //  server.onNotFound([]() {
  //    server.send(200, "text/html", responseHTML);
  //  });

  /////////////////////////////////////////////////////////quick change Router://////////////////////////////////////////////////////////////////////////////////
  // to activate in browser: http://192.168.1.78/router?router=1
  // don't forget main: http://192.168.1.1/router?router=1
  // nothing happens, but router is now switched on.

  // to deactivate in browser: http://192.168.8.78/router?router=0
  // don't forget main: http://192.168.8.79/router?router=0

  server.on("/router", []()
            {
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
              server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
              //////////////////////////////////////////////////////change PatternChooser setting in EEPROM://///////////////////////////////////////////////////////////////////////////////
              String onRouter = server.arg("router"); // need to handle errors what if it's too big
              if (onRouter.length() > 0)
              {
                EEPROM.write(100, 0); // clearing... Is this necessary? make it some sort of default then in case of errors..?
                EEPROM.commit();
                int newRouter; // temp variable - not needed, use same one as previously:
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
                // Set CORS headers before sending the response
                server.sendHeader("Access-Control-Allow-Origin", "*");
                server.sendHeader("Access-Control-Allow-Methods", "POST");

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
              FastLED.showColor(CRGB::Black);
            });
  ////////////////////////////////////////////////////end change router setting in EEPROM////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////quick change Pattern://////////////////////////////////////////////////////////////////////////////////
  server.on("/pattern", []()
            {
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
              server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
              //////////////////////////////////////////////////////change PatternChooser setting in EEPROM://///////////////////////////////////////////////////////////////////////////////
              String onAddress = server.arg("patternChooserChange"); // need to handle errors what if it's too big
              if (onAddress.length() > 0)
              {
                EEPROM.write(10, 1); // clearing... Is this necessary? make it some sort of default then in case of errors..?
                EEPROM.commit();
                // change address:
                int newPatt; // temp variable - not needed, use same one as previously:
                newPatt = onAddress.toInt();
                // if newPatt < max patt and also > 0 { //add this check here or null()
                patternChooser = newPatt; // change on poi as well as saving
                EEPROM.write(10, newPatt);
                if (newPatt < 6 && newPatt > 0)
                {
                  pattern = patternChooser;
                  EEPROM.write(11, newPatt);
                }
                else
                {
                  // do something here??
                  //         pattern++; //for loadPatternChooser change pattern why not! (may remove this I don't know)
                }
                EEPROM.commit(); // save for next time?
                // Serial.println("10, patternChooser saved");
                // black, this could take a while, so save power? Also an indicator...
                //  FastLED.showColor(CRGB::Black);
                //  loadPatternChooser();
                content = "{\"Success\":\" your pattern is set \"}";
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
  ////////////////////////////////////////////////////end change Pattern Chooser setting in EEPROM////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////interval changer://///////////////////////////////////////////////////////////////////////////////
  server.on("/intervalChange", []()
            {
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
              server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
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
                server.sendHeader("Access-Control-Allow-Origin", "*");
                server.sendHeader("Access-Control-Allow-Methods", "POST");

                // Send the error response
                server.send(statusCode, "application/json", content);
              } // nothing
            });
  ////////////////////////////////////////////////////end change interval changer////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////quick change Brighness://////////////////////////////////////////////////////////////////////////////////
  server.on("/brightness", []()
            {
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
              server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
              //////////////////////////////////////////////////////change PatternChooser setting in EEPROM://///////////////////////////////////////////////////////////////////////////////
              String onNewBRT = server.arg("brt"); 
              if (onNewBRT.length() > 0)
              {                
                newBrightness = onNewBRT.toInt();
                if(newBrightness > 254){
                  newBrightness = 255;
                }
                if(newBrightness < 20){
                  newBrightness = 20;
                }
                
                FastLED.setBrightness(  newBrightness ); //should I be removing this becos https://github.com/FastLED/FastLED/wiki/FastLED-Temporal-Dithering
                FastLED.showColor( CRGB::Black );

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
  ////////////////////////////////////////////////////end change Pattern Chooser setting in EEPROM////////////////////////////////////////////////////////////////////////////////


  ////////////////////////////////////////////////////////settings form://////////////////////////////////////////////////////////////////////////////////////////
  server.on("/setting", []()
            {
              server.sendHeader("Access-Control-Allow-Origin", "*");
              server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
              server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
              /*  //test:
                //    String stip = webServer.arg("ip");
                //      if (stip.length() > 0 ) {
                //        for (int i = 0; i < 13; ++i) {
                //          EEPROM.write(i, 0); //clearing
                //        }
                //        EEPROM.commit();
                //
                //        for (int i = 0; i < 13 ; ++i)
                //        {
                //          EEPROM.write(i, stip[i]);
                //        }
                //        EEPROM.commit();
                //        //Serial.print("stip is: ");
                //        //Serial.println(stip);
                //        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
                //        statusCode = 200;
                //      }else{
                //      content = "{\"Error\":\"404 not found\"}";
                //        statusCode = 404;
                //        //Serial.println("Sending 404");
                //      //Serial.println("not found");
                //      }
              */
              // change router settings in Spiffs://////////////////////////////////////////////////////////////////////////////////
              // not working currently
              //  open file for writing
              settings = LittleFS.open("/settings.txt", "w");
              if (!settings)
              {
                // Serial.println("settings file open failed");
              }
              // Serial.println("====== Writing to SETTINGS file =========");

              // String pwd2 = server.arg("pwd");
              // int pwd2_len = pwd2.length() + 1;
              // char pwd2_array[pwd2_len];
              // pwd2.toCharArray(pwd2_array, pwd2_len);
              // settings.println(pwd2);
              //
              // settings.println("Smart_Poi_2");
              // settings.println("password");

              // old method with strings below:

              // try string.trim() as well... //
              settings.print(server.arg("ssid"));
              settings.print("\n"); // finally solved not loading problem

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

                // test: use address field for patternChooser:
                // make another field here for this, has it's own eeprom address and everything!
                // ok can't use html because of auxillary poi not working with this. Use other method. Just a test delete this!
                //       patternChooser = onAddress.toInt();
                //       //error check:
                //       if(patternChooser > 3){
                //         patternChooser = 3;
                //       }
                //       if(patternChooser < 1) {
                //         patternChooser = 1;
                //       }
                //       loadPatternChooser();
                // end patternChooser Change test

                //        content = "huaweiRouter,password,more";
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
              //////////////////////////////////////////////////////change addrA setting in EEPROM://///////////////////////////////////////////////////////////////////////////////
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
              ////////////////////////////////////////////////////end change addrA setting in EEPROM////////////////////////////////////////////////////////////////////////////////

              //////////////////////////////////////////////////////change addrB setting in EEPROM://///////////////////////////////////////////////////////////////////////////////
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
              ////////////////////////////////////////////////////end change addrB setting in EEPROM////////////////////////////////////////////////////////////////////////////////

              //////////////////////////////////////////////////////change addrC setting in EEPROM://///////////////////////////////////////////////////////////////////////////////
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
              ////////////////////////////////////////////////////end change addrC setting in EEPROM////////////////////////////////////////////////////////////////////////////////

              //////////////////////////////////////////////////////change PatternChooser setting in EEPROM://///////////////////////////////////////////////////////////////////////////////
              onAddress = server.arg("patternChooserChange"); // need to handle errors what if it's too big
              if (onAddress.length() > 0)
              {
                EEPROM.write(10, 1); // clearing... Is this necessary? make it some sort of default then in case of errors..?
                EEPROM.commit();
                // change address:
                int newPatt; // temp variable - not needed, use same one as previously:
                newPatt = onAddress.toInt();
                // if newPatt < max patt and also > 0 { //add this check here or null()
                patternChooser = newPatt; // change on poi as well as saving
                EEPROM.write(10, newPatt);
                if (newPatt < 6 && newPatt > 0)
                {
                  pattern = patternChooser;
                  EEPROM.write(11, newPatt);
                }
                else
                {
                  // do something here??
                  //         pattern++; //for loadPatternChooser change pattern why not! (may remove this I don't know)
                }
                EEPROM.commit(); // save for next time?
                // Serial.println("10, patternChooser saved");
                // black, this could take a while, so save power? Also an indicator...
                //  FastLED.showColor(CRGB::Black);
                //  loadPatternChooser();
              }
              else
              {
              } // nothing
              ////////////////////////////////////////////////////end change Pattern Chooser setting in EEPROM////////////////////////////////////////////////////////////////////////////////

              // Send the status code response
              server.send(statusCode, "application/json", content);
              // delay(50);
              //    ESP.restart(); //not using this right now but see https://github.com/esp8266/Arduino/issues/1722#issuecomment-192829825
            });

  server.begin();
}
