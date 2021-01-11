# PXT-CPP-ESP8266-WIFI

The goal was to rewrite <https://github.com/Interpause/pxt-esp8266iot> completely in C/C++ to decrease lag in the original package. In hindsight, having seen other esp8266-microbit libraries, I probably made the code too complex. While I am pretty sure the C/C++ code here would have worked (no guarantee about bugs though), ultimately, I ended up relying on std libraries, causing the compile size to be too large. Should have seen that coming.

As I have no plans to master C/C++ and relearn the microbit's API just to make this work, I will be archiving this repository. That said, this has been some of my most substantial C/C++ code ever, which is kind of a pity.
