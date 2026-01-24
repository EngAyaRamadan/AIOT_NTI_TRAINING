# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS 
    #============== Hal files ============
    "main.c"         # list the source files of this component
    "../Src/Hal/GPIO/gpio.c"
    "../Src/Hal/NVS/Nvs.c"
    "../Src/Hal/WIFI/wifi.c"
    "../Src/Hal/ADC/adc.c"
    "../Src/Hal/UART/uart.c"
    "../Src/Hal/I2C/i2c.c"
    
   #============== App files ============
     "../Src/App/POT/pot.c"
     "../Src/App/ULTRASONIC/ultrasonic.c"
     "../Src/App/HTTP_CLIENT/http_client.c"
     "../Src/App/HTTP_SERVER/http_server.c"
     "../Src/App/MQTT/mqtt.c"
    INCLUDE_DIRS       
   #============== Hal direcory ============
    "."     # optional, add here public include directories
    "../Src/Hal/GPIO"
    "../Src/Hal/NVS"
    "../Src/Hal/WIFI"
    "../Src/Hal/ADC"
    "../Src/Hal/UART"
    "../Src/Hal/I2C"
    #============== App direcory ============
    "../Src/App/POT"
    "../Src/App/ULTRASONIC"
    "../Src/App/HTTP_CLIENT"
    "../Src/App/HTTP_SERVER"
    "../Src/App/MQTT"
    PRIV_INCLUDE_DIRS   # optional, add here private include directories
    REQUIRES            # optional, list the public requirements (component names)
    PRIV_REQUIRES       # optional, list the private requirements
    
    
)
#============== Https server certificate  private key ============
target_add_binary_data(${COMPONENT_TARGET} "../Src/App/HTTP_SERVER/server_cer_key/cert.pem" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "../Src/App/HTTP_SERVER/server_cer_key/key.pem" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "../Src/App/MQTT/mqtt_cer.pem" TEXT)

