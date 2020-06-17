#include "ILI9163_TFT.h"

ILI9163_TFT::ILI9163_TFT(uint8_t cspin, uint8_t rspin, uint8_t dcpin) {
  this->m_cspin = cspin;
  this->m_rspin = rspin;
  this->m_dcpin = dcpin;
}

void ILI9163_TFT::start() {
  pinMode(this->m_cspin, OUTPUT);
  pinMode(this->m_rspin, OUTPUT);

  this->m_csport = portOutputRegister(digitalPinToPort(this->m_cspin));
  this->m_rsport = portOutputRegister(digitalPinToPort(this->m_rspin));

  this->m_csmask = digitalPinToBitMask(this->m_cspin);
  this->m_rsmask = digitalPinToBitMask(this->m_rspin);


  // begin SPI
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);


  this->m_en_data();

  // reset display power
  pinMode(this->m_dcpin, OUTPUT);
  digitalWrite(this->m_dcpin, HIGH);
  delay(500);
  digitalWrite(this->m_dcpin, LOW);
  delay(500);
  digitalWrite(this->m_dcpin, HIGH);
  delay(500);


  this->m_chip();
}

void ILI9163_TFT::m_chip() {
  this->m_trans();

  this->m_com(CMD_SWRESET);
  delay(500);
}
