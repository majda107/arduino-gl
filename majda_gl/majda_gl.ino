//Technical support:goodtft@163.com
// Breakout/Arduino UNO pin usage:
// LCD Data Bit :   7   6   5   4   3   2   1   0
// Uno dig. pin :   7   6   5   4   3   2   9   8
// Uno port/pin : PD7 PD6 PD5 PD4 PD3 PD2 PB1 PB0
// Mega dig. pin:  29  28  27  26  25  24  23  22

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF



#define LCD_RD   A0
#define LCD_WR   A1     
#define LCD_RS   A2        
#define LCD_CS   A3       
#define LCD_REST A4
#define WR_MASK B00000010
#define RS_MASK B00000100

const unsigned short WIDTH = 320;
const unsigned short HEIGHT = 480;

//short min_x = 0;
//short min_y = 0;
//short max_x = 0;
//short max_y = 0;




struct vec3f
{
  float x;
  float y;
  float z;

  vec3f(float X, float Y, float Z) {
    x = X;
    y = Y;
    z = Z;
  }

  vec3f() {
    
  }
};

struct triangle 
{
  vec3f p[3];

  triangle() {
    
  }
};

struct mat4f 
{
  float m[4][4] = { 0 };
};



void Lcd_Writ_Bus(unsigned char d)
{
  PORTD = (PORTD & B00000011) | ((d) & B11111100); 
  PORTB = (PORTB & B11111100) | ((d) & B00000011); 
  PORTC &= ~WR_MASK;
  PORTC|=  WR_MASK;
}

void Lcd_Write_Com(unsigned char VH)  
{   
   PORTC &= ~RS_MASK;
    Lcd_Writ_Bus(VH);
}

void Lcd_Write_Data(unsigned char VH)
{
  PORTC |= RS_MASK;
  Lcd_Writ_Bus(VH);
}

void Lcd_Write_Com_Data(unsigned char com,unsigned char dat)
{
  Lcd_Write_Com(com);
  Lcd_Write_Data(dat);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
  Lcd_Write_Com(0x2a);
  Lcd_Write_Data(x1>>8);
  Lcd_Write_Data(x1);
  Lcd_Write_Data(x2>>8);
  Lcd_Write_Data(x2);
  Lcd_Write_Com(0x2b);
  Lcd_Write_Data(y1>>8);
  Lcd_Write_Data(y1);
  Lcd_Write_Data(y2>>8);
  Lcd_Write_Data(y2);
  Lcd_Write_Com(0x2c);               
}


void Lcd_Init(void)
{
  digitalWrite(LCD_REST,HIGH);
  delay(5); 
  digitalWrite(LCD_REST,LOW);
  delay(15);
  digitalWrite(LCD_REST,HIGH);
  delay(15);

  digitalWrite(LCD_CS,HIGH);
  digitalWrite(LCD_WR,HIGH);
  digitalWrite(LCD_CS,LOW);  //CS

  Lcd_Write_Com(0x11);
  delay(20);
  Lcd_Write_Com(0xD0);
  Lcd_Write_Data(0x07);
  Lcd_Write_Data(0x42);
  Lcd_Write_Data(0x18);
  
  Lcd_Write_Com(0xD1);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x07);//07
  Lcd_Write_Data(0x10);
  
  Lcd_Write_Com(0xD2);
  Lcd_Write_Data(0x01);
  Lcd_Write_Data(0x02);
  
  Lcd_Write_Com(0xC0);
  Lcd_Write_Data(0x10);
  Lcd_Write_Data(0x3B);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x02);
  Lcd_Write_Data(0x11);
  
  Lcd_Write_Com(0xC5);
  Lcd_Write_Data(0x03);
  Lcd_Write_Com(0x36);
  Lcd_Write_Data(0x0A);
  
  Lcd_Write_Com(0x3A);
  Lcd_Write_Data(0x55);
  
  Lcd_Write_Com(0x2A);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x01);
  Lcd_Write_Data(0x3F);
  
  Lcd_Write_Com(0x2B);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x01);
  Lcd_Write_Data(0xE0);
  delay(120);
  Lcd_Write_Com(0x29);
  Lcd_Write_Com(0x002c); 
}





void LCD_Pixel(short x, short y, unsigned int c)
{
  Address_set(x,y, x+ 1, y + 1);

  Lcd_Write_Data(c >> 8);
  Lcd_Write_Data(c);
}



void LCD_Clear(unsigned int j)                   
{  
  unsigned int i,m;
  digitalWrite(LCD_RS,HIGH);
  digitalWrite(LCD_CS,LOW);
  Address_set(0,0,WIDTH, HEIGHT);
  Lcd_Write_Com(0x02c); //write_memory_start



  for(i=0;i<HEIGHT;i++)
    for(m=0;m<WIDTH;m++)
    {
      Lcd_Write_Data(j>>8);
      Lcd_Write_Data(j);

    }
  digitalWrite(LCD_CS,HIGH);   
  digitalWrite(LCD_RS,LOW);
}





void SetPixel(int x, int y, unsigned int color) 
{ 
  //if(x < min_x) min_x = x;
  //if(y < min_y) min_y = y;

  //if(x > max_x) max_x = x;
  //if(y > max_y) max_y = y;

  
  //Lcd_Write_Com(0x02c);
  //digitalWrite(LCD_RS,HIGH);
  digitalWrite(LCD_CS,LOW);
  
  LCD_Pixel(x, y, color);
  digitalWrite(LCD_CS,HIGH);
}


//void ClearPixelMins(unsigned int color)
//{
//  for(int i = min_x; i <= max_x; i++)
//    for(int j = min_y; j <= max_y; j++)
//      SetPixel(i, j, color);
//
//  max_x = max_y = 0;
//  min_x = min_y = 1000;
//}



void DrawLine(short x1, short y1, short x2, short y2, unsigned int color)
{
  short dx = abs(x2 - x1);
  short sx = x1 < x2 ? 1 : -1;
  
  short dy = -abs(y2-y1);
  short sy = y1 < y2 ? 1 : -1;

  short err = dx + dy;
  short e2;

  while(true) 
  {
    SetPixel(x1, y1, color);
    if(x1 == x2 && y1 == y2)
      break;

      e2 = 2*err;
      if(e2 >= dy) 
      {
        err += dy;
        x1 += sx;
      }

      if(e2 <= dx)
      {
        err += dx;
        y1 += sy;
      }
  }
}

void DrawTriangle(triangle t, unsigned int color) {
  DrawTriangle((short)t.p[0].x, (short)t.p[0].y, (short)t.p[1].x, (short)t.p[1].y, (short)t.p[2].x, (short)t.p[2].y, color);  
}

void DrawTriangle(short x0, short y0, short x1, short y1, short x2, short y2, unsigned int color)
{
  DrawLine(x0, y0, x1, y1, color);
  DrawLine(x1, y1, x2, y2, color);
  DrawLine(x2, y2, x0, y0, color);
}




vec3f multiply_mat4_vec3(vec3f v, mat4f m) {
  vec3f r;
  
  r.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
  r.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
  r.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];

  float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

  if(w == 0.0f)
    return r;

  r.x /= w;
  r.y /= w;
  r.z /= w;

  return r;
}


const int mesh_len = 12;
triangle cube_mesh[mesh_len];

mat4f mat_proj;

void load() {
  float mesh [][9]{

    // SOUTH
    { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

    // EAST                                                      
    { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
    { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

    // NORTH                                                     
    { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
    { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

    // WEST                                                      
    { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

    // TOP                                                       
    { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
    { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

    // BOTTOM                                                    
    { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
    { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

   };


   for(int t = 0; t < mesh_len; t++)
   {
     for(int v = 0; v < 3; v++)
     {
       cube_mesh[t].p[v] = vec3f(mesh[t][v*3], mesh[t][v*3 + 1], mesh[t][v*3 + 2]);
     }
   }

   

  float f_near = 0.1f;
  float f_far = 1000.0f;
  float fov = 90.0f * 3.14159f / 180.0f;
  float aspect = (float)HEIGHT / (float)WIDTH;

  mat_proj.m[0][0] = fov * aspect;
  mat_proj.m[1][1] = fov;
  mat_proj.m[2][2] = f_far / (f_far - f_near);
  mat_proj.m[3][2] = (-f_far * f_near) / (f_far - f_near);
  mat_proj.m[2][3] = 1.0f;
  mat_proj.m[3][3] = 0.0f;
}

//float zt = 2.0f;
//float xt = 0.0f;

void populate_rotZ(mat4f &m, float f) {
 m.m[0][0] = cosf(f);
 m.m[0][1] = sinf(f);
 m.m[1][0] = -sinf(f);
 m.m[1][1] = cosf(f);
 m.m[2][2] = 1;
 m.m[3][3] = 1;
}

void populate_rotX(mat4f &m, float f) {
  m.m[0][0] = 1;
  m.m[1][1] = cosf(f * 0.5f);
  m.m[1][2] = sinf(f * 0.5f);
  m.m[2][1] = -sinf(f * 0.5f);
  m.m[2][2] = cosf(f * 0.5f);
  m.m[3][3] = 1;
}

float theta = 0;

void render_loop(unsigned int color) {
  mat4f mat_z, mat_x;
  
  populate_rotZ(mat_z, theta);
  populate_rotX(mat_x, theta);


  triangle t;

  triangle t_proj;
  
  for(int ti = 0; ti < mesh_len; ti++)
  {
    t = cube_mesh[ti];

    for(int v = 0; v < 3; v++)
    {
      // translation
      //t.p[v].z += zt;
      //t.p[v].x += 1.0;
      //t.p[v].y += xt;

      t_proj.p[v] = multiply_mat4_vec3(t.p[v], mat_z);
      t_proj.p[v] = multiply_mat4_vec3(t_proj.p[v], mat_x);

      t_proj.p[v].x += 1.0f;
      t_proj.p[v].y += 1.0f;
      t_proj.p[v].z += 3.0f;
      
      t_proj.p[v] = multiply_mat4_vec3(t_proj.p[v], mat_proj);

      // scale
      t_proj.p[v].x += 1.0f;
      t_proj.p[v].y += 1.0f;

      t_proj.p[v].x *= 0.5f * (float)WIDTH/2;
      t_proj.p[v].y *= 0.5f * (float)HEIGHT/2;
    }

    DrawTriangle(t_proj, color);
  }
}





void setup()
{
  //Serial.begin(9600);
  
  for(int p=0;p<10;p++)
    pinMode(p,OUTPUT);
  
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  pinMode(A4,OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  digitalWrite(A4, HIGH);
  Lcd_Init();

  LCD_Clear(BLACK);

  load();
}








void loop() {
  render_loop(BLACK);
  theta += 0.4f;
  render_loop(WHITE);

  delay(50);
}
