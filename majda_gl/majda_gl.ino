#include "ILI9163C_TFT.h"
#include "majda_gl.h" // contains glm!

//#include "majda_glm.h"


// for UNO...

//#define __CS  10
//#define __RS  8
//#define __DC  9

//ILI9163C_TFT tft = ILI9163C_TFT(__CS, __RS, __DC);




#define __CS  16
#define __RS  5
#define __DC  4

ILI9163C_TFT tft = ILI9163C_TFT(__CS, __RS, __DC);





const int mesh_len = 12;
triangle cube_mesh[mesh_len];

vec3f camera = vec3f(0, 0, 0);

mat4f mat_proj;

void load() {
  float mesh [][9] {

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


  for (int t = 0; t < mesh_len; t++)
  {
    for (int v = 0; v < 3; v++)
    {
      cube_mesh[t].p[v] = vec3f(mesh[t][v * 3], mesh[t][v * 3 + 1], mesh[t][v * 3 + 2]);
    }
  }



  float f_near = 0.1f;
  float f_far = 1000.0f;
  float fov = 90.0f * 3.14159f / 180.0f;
  float aspect = (float)tft.WIDTH / (float)tft.HEIGHT;

  mat_proj = mat4f::projection(fov, f_near, f_far, aspect);
}



float theta = 0;

void render_loop(unsigned int color, bool clean) {
  mat4f mat_mod;

  mat_mod = mat4f::rotation_Z(theta);
  mat_mod = mat_mod * mat4f::rotation_X(theta);
  mat_mod = mat_mod * mat4f::translation(vec3f(0, 0, 3.25f));

  


  triangle t;

  triangle t_proj;
  vec3f l1, l2, normal, to_camera;
  float dot_camera, lum;

  for (int ti = 0; ti < mesh_len; ti++)
  {
    t = cube_mesh[ti];

    for (byte v = 0; v < 3; v++)
    {
      // model translation
      t_proj.p[v] = mat4f::mult_vec3f(t.p[v], mat_mod);
    }
    
    // normal calculation
    l1 = t_proj.p[1] - t_proj.p[0];
    l2 = t_proj.p[2] - t_proj.p[0];
    
    normal = vec3f::cross(l1, l2);
    normal.normalize();

    to_camera = t_proj.p[0] - camera;
    to_camera.normalize();
    
    dot_camera = vec3f::dot(to_camera, normal);
    if(dot_camera > 0.0f) 
    {
      continue;
    }

    for(byte v = 0; v < 3; v++)
    {
      // projection
      t_proj.p[v] = mat4f::mult_vec3f(t_proj.p[v], mat_proj);

      // scale
      t_proj.p[v].x += 1.0f;
      t_proj.p[v].y += 1.0f;

      t_proj.p[v].x *= 0.5f * (float)tft.WIDTH;
      t_proj.p[v].y *= 0.5f * (float)tft.HEIGHT;
    }
    

    lum = min(abs(dot_camera*0.75f) + 0.25f, 1.0f);
    
    unsigned int color = lum * 31;   // r (31)
    color = (color << 6) + lum * 63; // g (63)
    color = (color << 5) + lum * 31; // b (31)
 

    tft.draw_triangle((short)t_proj.p[0].x, (short)t_proj.p[0].y, (short)t_proj.p[1].x, (short)t_proj.p[1].y, (short)t_proj.p[2].x, (short)t_proj.p[2].y, color);
  }
}



#define __B1 0
#define __B2 2

bool moving = false;


void setup()
{
  pinMode(__B1, INPUT_PULLUP);
  pinMode(__B2, INPUT_PULLUP);
  
  tft.start();
  load();
}




void loop() {

  moving = ((digitalRead(__B1) != HIGH) || (digitalRead(__B2) != HIGH))? true : false;
  
  if(moving)
  {
    tft.fill_screen(BLACK);
    theta += 0.2f;
  }
  
  render_loop(WHITE, false);  

  delay(14);
}
