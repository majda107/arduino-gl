//ILI9163C registers-----------------------
#define CMD_NOP       0x00//Non operation
#define CMD_SWRESET   0x01//Soft Reset
#define CMD_SLPIN     0x10//Sleep ON
#define CMD_SLPOUT    0x11//Sleep OFF
#define CMD_PTLON     0x12//Partial Mode ON
#define CMD_NORML     0x13//Normal Display ON
#define CMD_DINVOF    0x20//Display Inversion OFF
#define CMD_DINVON    0x21//Display Inversion ON
#define CMD_GAMMASET  0x26//Gamma Set (0x01[1],0x02[2],0x04[3],0x08[4])
#define CMD_DISPOFF   0x28//Display OFF
#define CMD_DISPON    0x29//Display ON
#define CMD_IDLEON    0x39//Idle Mode ON
#define CMD_IDLEOF    0x38//Idle Mode OFF
#define CMD_CLMADRS     0x2A//Column Address Set
#define CMD_PGEADRS     0x2B//Page Address Set

#define CMD_RAMWR     0x2C//Memory Write
#define CMD_RAMRD     0x2E//Memory Read
#define CMD_CLRSPACE    0x2D//Color Space : 4K/65K/262K
#define CMD_PARTAREA  0x30//Partial Area
#define CMD_VSCLLDEF  0x33//Vertical Scroll Definition
#define CMD_TEFXLON   0x35//Tearing Effect Line ON
#define CMD_TEFXLOF   0x34//Tearing Effect Line OFF
#define CMD_MADCTL    0x36//Memory Access Control
#define CMD_VSSTADRS  0x37//Vertical Scrolling Start address
#define CMD_PIXFMT    0x3A//Interface Pixel Format
#define CMD_FRMCTR1   0xB1//Frame Rate Control (In normal mode/Full colors)
#define CMD_FRMCTR2   0xB2//Frame Rate Control(In Idle mode/8-colors)
#define CMD_FRMCTR3   0xB3//Frame Rate Control(In Partial mode/full colors)
#define CMD_DINVCTR   0xB4//Display Inversion Control
#define CMD_RGBBLK    0xB5//RGB Interface Blanking Porch setting
#define CMD_DFUNCTR   0xB6//Display Fuction set 5
#define CMD_SDRVDIR   0xB7//Source Driver Direction Control
#define CMD_GDRVDIR   0xB8//Gate Driver Direction Control 

#define CMD_PWCTR1    0xC0//Power_Control1
#define CMD_PWCTR2    0xC1//Power_Control2
#define CMD_PWCTR3    0xC2//Power_Control3
#define CMD_PWCTR4    0xC3//Power_Control4
#define CMD_PWCTR5    0xC4//Power_Control5
#define CMD_VCOMCTR1    0xC5//VCOM_Control 1
#define CMD_VCOMCTR2    0xC6//VCOM_Control 2
#define CMD_VCOMOFFS    0xC7//VCOM Offset Control
#define CMD_PGAMMAC   0xE0//Positive Gamma Correction Setting
#define CMD_NGAMMAC   0xE1//Negative Gamma Correction Setting
#define CMD_GAMRSEL   0xF2//GAM_R_SEL


// colors
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


#include <SPI.h>

#define __CS  10
#define __RS  8
#define __DC  9

volatile uint8_t* csport;
volatile uint8_t* rsport;

uint8_t csmask;
uint8_t rsmask;
uint8_t dcmask;


void LCD_spi(uint16_t c) {
  SPDR = c;
  while(!(SPSR & _BV(SPIF)));
}

void LCD_start_t() {
  //*csport &= ~csmask; //low
}

void LCD_end_t() {
  //*csport |= csmask; //hi 
}

void LCD_com(uint16_t c) {

  //*rsport &= ~dcmask; //low
  
  *rsport &= ~rsmask; //low
  *csport &= ~csmask; //low
  LCD_spi(c);
  *csport |= csmask; //hi
}

void LCD_data(uint16_t c) {

  //*rsport |=  dcmask; //hi
  
  *rsport |=  rsmask; //hi
  *csport &= ~csmask; //low
  LCD_spi(c);
  *csport |= csmask; //hi
}

void LCD_data16(uint16_t c) {

  *rsport |=  rsmask; //hi
  *csport &= ~csmask; //low
  LCD_spi(c >> 8);
  LCD_spi(c);
  *csport |= csmask; //hi
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


void ClearScreen(uint16_t c) {
  LCD_start_t();
  
  LCD_address(0, 0, 128, 160);
  
  for(int x = 0; x < 128; x++)
    for(int y = 0; y < 160; y++)
    {
      LCD_data16(c);
    }

  LCD_end_t();
}


void LCD_init() {
  pinMode(__RS, OUTPUT);
  pinMode(__CS, OUTPUT);

  csport = portOutputRegister(digitalPinToPort(__CS));
  rsport = portOutputRegister(digitalPinToPort(__RS));

  csmask = digitalPinToBitMask(__CS);
  rsmask = digitalPinToBitMask(__RS);
  dcmask = digitalPinToBitMask(__DC);



  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  
  //*csport &= ~csmask;// toggle CS low so it'll listen to us`
  *csport |= csmask;//hi
  
  //enable data stream
  *rsport |= dcmask;


  pinMode(__DC, OUTPUT);
  digitalWrite(__DC, HIGH);
  delay(500);
  digitalWrite(__DC, LOW);
  delay(500);
  digitalWrite(__DC, HIGH);
  delay(500);


  // CHIP INIT ~ !

  LCD_start_t();
  
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

  //LCD_end_t();
}


void setup() {
  LCD_init();
  ClearScreen(0x00);
}

void loop() {
  
  ClearScreen(RED);
  ClearScreen(BLUE);
  
}
