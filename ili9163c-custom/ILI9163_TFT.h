#include "Arduino.h"
#include "ILI9163_Commands.h"
#include <SPI.h>

class ILI9163_TFT {
private:
  uint8_t m_cspin;
  uint8_t m_rspin;
  uint8_t m_dcpin;

  volatile uint8_t* m_csport;
  volatile uint8_t* m_rsport;

  uint8_t m_csmask;
  uint8_t m_rsmask;

  // data flow
  void m_trans();
  void m_en_data();
  void m_en_com();
  void m_dis_cs();

  // communication
  void m_spi(uint8_t c);
  void m_com(uint16_t c);
  void m_data(uint8_t d);
  void m_data16(uint16_t d);


  void m_chip(void);
  
public:
  const uint8_t WIDTH = 128;
  const uint8_t HEIGHT = 160;
  
  ILI9163_TFT(uint8_t cspin, uint8_t rspin, uint8_t dcpin);

  void start();
};
