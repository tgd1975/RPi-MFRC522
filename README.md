# MFRC522
Raspberry Pi library for MFRC522 and other RFID RC522 based modules.

Read and write different types of Radio-Frequency IDentification (RFID) cards
on your Raspberry Pi using a RC522 based reader connected via the Serial Peripheral
Interface (SPI) interface.

Adapted from https://github.com/miguelbalboa/rfid
Inspired by https://github.com/paguz/RPi-RFID and many of its forks

This simply uses the same methods as https://github.com/miguelbalboa/rfid but mocks all Arduino only method with the bcm2835 library for raspberry pi.

Has the same methods, signatures, functions, and limitations as https://github.com/miguelbalboa/rfid (at least it should)

## Getting started
1. Install the bcm2835 library
1. Download the latest version of the bcm2835 library on http://www.airspayce.com/mikem/bcm2835/

2. Use ```cd``` to navigate to the folder where bcm2835-1.xx.tar.gz is downloaded

3. Run the following commands to install the library
```bash
tar zxvf bcm2835-1.xx.tar.gz

cd bcm2835-1.xx

./configure

make

sudo make check

sudo make install
```

2. Build the an example
Here is how you'd build the ReadNUID example
```bash
g++ src/*.cpp examples/ReadNUID/ReadNUID.cpp -std=c++20 -lbcm2835
```
