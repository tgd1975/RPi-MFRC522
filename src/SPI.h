/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <cstddef>

#include "bcm2835.h"

// SPI_HAS_TRANSACTION means SPI has beginTransaction(), endTransaction(),
// usingInterrupt(), and SPISetting(clock, bitOrder, dataMode)
#define SPI_HAS_TRANSACTION 1

// SPI_ATOMIC_VERSION means that SPI has atomicity fixes and what version.
// This way when there is a bug fix you can check this define to alert users
// of your code if it uses better version of this library.
// This also implies everything that SPI_HAS_TRANSACTION as documented above is
// available too.
#define SPI_ATOMIC_VERSION 1

// Uncomment this line to add detection of mismatched begin/end transactions.
// A mismatch occurs if other libraries fail to use SPI.endTransaction() for
// each SPI.beginTransaction().  Connect an LED to this pin.  The LED will turn
// on if any mismatch is ever detected.
//#define SPI_TRANSACTION_MISMATCH_LED 5

#ifndef LSBFIRST
#define LSBFIRST 0
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK 0x0C     // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03    // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

class SPISettings {
 public:
  SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
    _clock = clock;
    _bitOrder = bitOrder;
    _dataMode = dataMode;
  }
  SPISettings() {
    _clock = BCM2835_SPI_CLOCK_DIVIDER_64;
    _bitOrder = BCM2835_SPI_BIT_ORDER_MSBFIRST;
    _dataMode = BCM2835_SPI_MODE0;
  }

 private:
  uint8_t _clock;
  uint8_t _bitOrder;
  uint8_t _dataMode;
  friend class SPIClass;
};

class SPIClass {
 public:
  SPIClass() { begin(); }

  ~SPIClass() { end(); }

  // Initialize the SPI library
  inline static void begin() { bcm2835_spi_begin(); }

  // Before using SPI.transfer() or asserting chip select pins,
  // this function is used to gain exclusive access to the SPI bus
  // and configure the correct settings.
  inline static void beginTransaction(SPISettings settings) {
    bcm2835_spi_setBitOrder(settings._bitOrder);
    bcm2835_spi_setDataMode(settings._dataMode);
    bcm2835_spi_setClockDivider(settings._clock);
    bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE);
  }

  // Write to the SPI bus (MOSI pin) and also receive (MISO pin)
  inline static uint8_t transfer(uint8_t data) {
    return bcm2835_spi_transfer(data);
  }

  inline static void transfer(void *buf, size_t count) {
    bcm2835_spi_transfern((char *)buf, count);
  }
  // After performing a group of transfers and releasing the chip select
  // signal, this function allows others to access the SPI bus
  inline static void endTransaction(void) {}

  // Disable the SPI bus
  inline static void end() { bcm2835_spi_end(); }
};

extern SPIClass SPI;

#endif
