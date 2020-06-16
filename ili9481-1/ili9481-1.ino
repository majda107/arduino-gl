#define RD_MASK B00000001
#define WR_MASK B00000010
#define RS_MASK B00000100
#define CS_MASK B00001000


#define LCD_RD  A0 // ? data read
#define LCD_WR  A1 // ? data write
#define LCD_RS  A2 // ? data-or-command ? low if command, DC/X
#define LCD_CS  A3 // chip select, low enable
#define LCD_RST A4 // reset


void LCD_Bus(unsigned char d)
{
  PORTD = (PORTD & B00000011) | ((d) & B11111100);
  PORTB = (PORTB & B11111100) | ((d) & B00000011);
  PORTC &= ~WR_MASK;
  PORTC |=  WR_MASK;
}

void LCD_Com(unsigned char vh)
{
  PORTC &= ~RS_MASK;
  LCD_Bus(vh);
}

void LCD_Data(unsigned char vh)
{
  PORTC |= RS_MASK;
  LCD_Bus(vh);
}



void LCD_Address(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
{
  LCD_Com(0x2a);
  LCD_Data(x0 >> 8);
  LCD_Data(x0);
  LCD_Data(x1 >> 8);
  LCD_Data(x1);

  LCD_Com(0x2b);
  LCD_Data(y0 >> 8);
  LCD_Data(y0);
  LCD_Data(y1 >> 8);
  LCD_Data(y1);
}



void LCD_Init()
{
  for (int p = 0; p < 10; p++)
    pinMode(p, OUTPUT);
    
  pinMode(LCD_RD, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RST, OUTPUT);

  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RST, HIGH);


  /* Bring up the LCD */
  digitalWrite(LCD_RST, HIGH);
  delay(5); 
  digitalWrite(LCD_RST, LOW);
  delay(15);
  digitalWrite(LCD_RST, HIGH);
  delay(15);

  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_CS, LOW);  //CS


  LCD_Com(0x11); // exit sleep
  delay(20);

  LCD_Com(0xD0); // power settings
  LCD_Data(0x07);
  LCD_Data(0x42);
  LCD_Data(0x18);

  LCD_Com(0xD1); // vcom control
  LCD_Data(0x00);
  LCD_Data(0x07);
  LCD_Data(0x10);

  LCD_Com(0xD2); // power settings for normal mode
  LCD_Data(0x01);
  LCD_Data(0x02);
  
  LCD_Com(0xC0); // panel driving setting
  LCD_Data(0x10);
  LCD_Data(0x3B);
  LCD_Data(0x00);
  LCD_Data(0x02);
  LCD_Data(0x11);

  LCD_Com(0xC5); // refresh rate
  LCD_Data(0x03); // 72hz
  //LCD_Data(0x03); // 125hz


  LCD_Com(0x36); // address mode
  LCD_Data(0x0A); // page-address-order | page/column-selection |  horizontal flip

  LCD_Com(0x3A); // set pixel format
  LCD_Data(0x55);


  //LCD_Com(0x2A); // set col addr
  //LCD_Data(0x00);
  //LCD_Data(0x00);
  //LCD_Data(0x01);
  //LCD_Data(0x3F);
  
  //LCD_Com(0x2B); // set page addr
  //LCD_Data(0x00);
  //LCD_Data(0x00);
  //LCD_Data(0x01);
  //LCD_Data(0xE0);

  delay(120);
  LCD_Com(0x29); // display on

  //LCD_Com(0x002c);
}


void setup() 
{
  LCD_Init();
}

void loop() {

  unsigned int c = 0xF800;

  
  LCD_Address(0, 0, 100, 100);
  LCD_Com(0x2c); //write_memory_start
  
  for(unsigned int x = 0; x < 100; x++)
    for(unsigned int y = 0; y < 100; y++)
    {
      LCD_Data(c);
      LCD_Data(c >> 8);
    }


  LCD_Address(25, 25, 75, 75);
  
  LCD_Com(0x2c); //write_memory_start

  LCD_Data(0x02);
  
  for(unsigned int x = 0; x < 50; x++)
    for(unsigned int y = 0; y < 50; y++)
    {
      LCD_Data(c);
      LCD_Data(c >> 8);
    }


  //delay(2000);

  //LCD_Com(0x01);

  //delay(14);

  //LCD_Com(0x20);
}
