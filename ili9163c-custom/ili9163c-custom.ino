#include "ILI9163_TFT.h"

// colors
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define __CS  10
#define __RS  8
#define __DC  9

volatile uint8_t* csport;
volatile uint8_t* rsport;

uint8_t csmask;
uint8_t rsmask;
//uint8_t dcmask;


void LCD_spi(uint16_t c) {
  SPDR = c;
  while(!(SPSR & _BV(SPIF)));
}

void LCD_trans() {
  *csport &= ~csmask; //low
}

void LCD_enableCom() {
  *rsport &= ~rsmask; //low
}

void LCD_enableDat() {
  *rsport |= rsmask;
}

void LCD_disableCS() {
  *csport |= csmask; // hi
}

void LCD_com(uint16_t c) {
  LCD_enableCom();
  LCD_spi(c);
}

void LCD_data(uint16_t c) {
  LCD_enableDat();
  LCD_spi(c);
}

void LCD_data16(uint16_t c) {
  LCD_enableDat();
  LCD_spi(c >> 8);
  LCD_spi(c);
}

void LCD_address(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  LCD_com(CMD_CLMADRS);
  LCD_data16(x0);
  LCD_data16(x1);

  LCD_com(CMD_PGEADRS);
  LCD_data16(y0);
  LCD_data16(y1);

  LCD_com(CMD_RAMWR);
}

void LCD_ClearMem() {
  LCD_trans();

  LCD_address(0, 0, 128, 160);

  LCD_enableDat();
  for(int x = 0; x < 128; x++)
    for(int y = 0; y < 160; y++)
    {
      LCD_data16(0x00);
    }
  
  LCD_disableCS();
}


void ClearScreen(uint16_t c) {
  LCD_trans();
  
  LCD_address(0, 0, 128, 160);

  LCD_enableDat();
  for(int x = 0; x < 128; x++)
    for(int y = 0; y < 160; y++)
    {
      LCD_data16(c);
    }

  LCD_disableCS();
}


void LCD_init() {
  pinMode(__RS, OUTPUT);
  pinMode(__CS, OUTPUT);

  csport = portOutputRegister(digitalPinToPort(__CS));
  rsport = portOutputRegister(digitalPinToPort(__RS));

  csmask = digitalPinToBitMask(__CS);
  rsmask = digitalPinToBitMask(__RS);
  //dcmask = digitalPinToBitMask(__DC);



  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  
  //*csport &= ~csmask;// toggle CS low so it'll listen to us`
  //*csport |= csmask;//hi MAYBE ENABLE THIS?
  
  //enable data stream
  LCD_enableDat();


  pinMode(__DC, OUTPUT);
  digitalWrite(__DC, HIGH);
  delay(500);
  digitalWrite(__DC, LOW);
  delay(500);
  digitalWrite(__DC, HIGH);
  delay(500);


  // CHIP INIT ~ !

  LCD_trans();
  
  LCD_com(CMD_SWRESET); //software reset
  delay(500);

  LCD_com(CMD_SLPOUT); //exit sleep
  delay(5);

  LCD_com(CMD_PIXFMT); //Set Color Format 16bit
  LCD_data(0x05);
  delay(5);


  LCD_com(CMD_FRMCTR1); //Frame Rate Control (In normal mode/Full colors)
  LCD_data(0x08);       //0x0C//0x08
  LCD_data(0x02);       //0x14//0x08
  delay(1);

  LCD_com(CMD_DINVCTR); //display inversion
  LCD_data(0x07);
  delay(1);

  LCD_com(CMD_PWCTR1); //Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD
  LCD_data(0x0A);      //4.30 - 0x0A
  LCD_data(0x02);      //0x05
  delay(1);

  LCD_com(CMD_PWCTR2); //Set BT[2:0] for AVDD & VCL & VGH & VGL
  LCD_data(0x02);
  delay(1);

  LCD_com(CMD_VCOMCTR1); //Set VMH[6:0] & VML[6:0] for VOMH & VCOML
  LCD_data(0x50);      //0x50
  LCD_data(99);        //0x5b
  delay(1);

  LCD_com(CMD_VCOMOFFS);
  LCD_data(0); //0x40
  delay(1);

  LCD_com(CMD_CLMADRS); //Set Column Address
  LCD_data16(0x00);
  LCD_data16(128);

  LCD_com(CMD_PGEADRS); //Set Page Address
  LCD_data16(0X00);
  LCD_data16(180);

  auto Mactrl = 0b00001000;
  bitSet(Mactrl,3);
  LCD_com(CMD_MADCTL);
  LCD_data(Mactrl);
  

  LCD_com(CMD_DISPON); //display ON
  delay(1);
  LCD_com(CMD_RAMWR); //Memory Write

  delay(1);

  LCD_ClearMem();
}


void setup() {
  LCD_init();
  ClearScreen(0x00);
}

void loop() {
  
  ClearScreen(RED);
  ClearScreen(BLUE);
  
}
