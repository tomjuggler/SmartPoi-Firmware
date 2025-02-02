#include "Globals.h"                                                                                                                                                                  
                                                                                                                                                                                       
 // Add platform-specific includes at the top                                                                                                                                          
 #if defined(PLATFORM_ESP32)                                                                                                                                                           
   #include <WiFi.h>                                                                                                                                                                   
   #include <WebServer.h>                                                                                                                                                              
 #elif defined(PLATFORM_ESP8266)                                                                                                                                                       
   #include <ESP8266WiFi.h>                                                                                                                                                            
   #include <ESP8266WebServer.h>                                                                                                                                                       
 #endif                                                                                                                                                                                
                                                                                                                                                                                       
 // Then define the server instance                                                                                                                                                    
 #if defined(PLATFORM_ESP32)                                                                                                                                                           
   WebServer server(80);                                                                                                                                                               
 #elif defined(PLATFORM_ESP8266)                                                                                                                                                       
   ESP8266WebServer server(80);                                                                                                                                                        
 #endif          