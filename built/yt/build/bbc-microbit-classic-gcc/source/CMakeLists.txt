# NOTE: This file is generated by yotta: changes will be overwritten!

cmake_minimum_required(VERSION 2.8.11)

include_directories("C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source")


set(YOTTA_AUTO_PXT-MICROBIT-APP_CPP_FILES
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/core/serial.cpp"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/core/buffer.cpp"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/PXT-CPP-ESP8266-WIFI/module_init.cpp"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/core/basic.cpp"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/core/input.cpp"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/core/pins.cpp"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/core/ManagedBuffer.cpp"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/PXT-CPP-ESP8266-WIFI/command_layer.cpp"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/main.cpp"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/core/control.cpp"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/core/pxt.cpp"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/core/images.cpp"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/PXT-CPP-ESP8266-WIFI/net_layer.cpp"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/pointers.cpp"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/radio/radio.cpp"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/core/core.cpp"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/core/led.cpp"
)
# force dependency on the config header for cpp files, which CMake otherwise wouldn't track:
set_property(SOURCE ${YOTTA_AUTO_PXT-MICROBIT-APP_CPP_FILES} PROPERTY OBJECT_DEPENDS "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/build/bbc-microbit-classic-gcc/yotta_config.h")
set(YOTTA_AUTO_PXT-MICROBIT-APP_HEADER_FILES
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/PXT-CPP-ESP8266-WIFI/net_layer.h"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/PXT-CPP-ESP8266-WIFI/module_config.h"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/pxt.h"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/ManagedBuffer.h"
    "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/source/PXT-CPP-ESP8266-WIFI/command_layer.h"
)
# force dependency on the config header for header files, which CMake otherwise wouldn't track:
set_property(SOURCE ${YOTTA_AUTO_PXT-MICROBIT-APP_HEADER_FILES} PROPERTY OBJECT_DEPENDS "C:/Users/John/Documents/pxt-esp8266iot/projects/PXT-CPP-ESP8266-WIFI/built/yt/build/bbc-microbit-classic-gcc/yotta_config.h")


add_executable (pxt-microbit-app
    ${YOTTA_AUTO_PXT-MICROBIT-APP_CPP_FILES}
    ${YOTTA_AUTO_PXT-MICROBIT-APP_HEADER_FILES}
)
set_target_properties(pxt-microbit-app PROPERTIES ENABLE_EXPORTS ON)

yotta_postprocess_target(EXECUTABLE pxt-microbit-app)



target_link_libraries(pxt-microbit-app
    microbit
)

