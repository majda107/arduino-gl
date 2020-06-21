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


/*
uint8_t z_buff[128][128] = { { 0 } };

void clear_z_buff(uint8_t val = 0)
{
  for(byte x = 0; x < 128; x++)
    for(byte y = 0; y < 128; y++)
      z_buff[x][y] = val;
}

void process_z_buff()
{
  for(byte x = 0; x < 128; x++)
    for(byte y = 0; y < 128; y++)
    {
      if(z_buff[x][y] == 0)
        continue;
      
      if((z_buff[x][y] & 1 << 0) > 0)
      {
        z_buff[x][y] &= ~(1 << 0);
      }
      else
      {
        tft.set_pixel(x, y, BLACK);
        z_buff[x][y] = 0;
      }
    }
      
}
*/




const int mesh_len = 12;
triangle cube_mesh[mesh_len];

Camera camera = Camera(vec3f(0, 0, -4));
mat4f mat_proj;

MeshEntity cube = MeshEntity(cube_mesh, mesh_len, mat4f::translation(vec3f(-1.25f, -1.25f, 0)));
MeshEntity cube2 = MeshEntity(cube_mesh, mesh_len, mat4f::identity());
MeshEntity cube3 = MeshEntity(cube_mesh, mesh_len, mat4f::translation(vec3f(1.25f, 1.25f, 0)));

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



  
  // projection init
  float f_near = 0.1f;
  float f_far = 1000.0f;
  float fov = 90.0f * 3.14159f / 180.0f;
  float aspect = (float)tft.WIDTH / (float)tft.HEIGHT;

  mat_proj = mat4f::projection(fov, f_near, f_far, aspect);



  // camera init
  camera.build_view();
}


void render_entity(const MeshEntity& entity)
{
  triangle t;

  vec3f l1, l2, normal, to_camera;
  float dot_camera, lum;

  unsigned int color;

  mat4f world_mat = entity.model_mat * camera.view;

  for (int ti = 0; ti < entity.mesh_len; ti++)
  {
    t = entity.mesh[ti];

    // transformation
    t.p[0] = mat4f::mult_vec3f(t.p[0], world_mat);
    t.p[1] = mat4f::mult_vec3f(t.p[1], world_mat);
    t.p[2] = mat4f::mult_vec3f(t.p[2], world_mat);
    
    
    // normal calculation
    l1 = t.p[1] - t.p[0];
    l2 = t.p[2] - t.p[0];
    
    normal = vec3f::cross(l1, l2);
    normal.normalize();

    to_camera = t.p[0];
    to_camera.normalize();
    
    dot_camera = vec3f::dot(to_camera, normal);
    if(dot_camera > 0.0f) 
    {
      continue;
    }

    for(byte v = 0; v < 3; v++)
    {
      // projection
      t.p[v] = mat4f::mult_vec3f(t.p[v], mat_proj);

      // scale
      t.p[v].x += 1.0f;
      t.p[v].y += 1.0f;

      t.p[v].x *= 0.5f * (float)tft.WIDTH;
      t.p[v].y *= 0.5f * (float)tft.HEIGHT;
    }

    if(t.p[0].z > 1 || t.p[1].z > 1 || t.p[2].z > 1) // drawing behind camera
      continue;
    

    lum = min(dot_camera*-0.85f + 0.15f, 1.0f);
    
    color = lum * 31;   // r (31)
    color = (color << 6) + lum * 63; // g (63)
    color = (color << 5) + lum * 31; // b (31)
 
    
    //tft.draw_triangle_buff((short)t.p[0].x, (short)t.p[0].y, (short)t.p[1].x, (short)t.p[1].y, (short)t.p[2].x, (short)t.p[2].y, color, z_buff, (color & 0xfe));
    tft.draw_triangle((short)t.p[0].x, (short)t.p[0].y, (short)t.p[1].x, (short)t.p[1].y, (short)t.p[2].x, (short)t.p[2].y, color);
  }
}

void render_loop() {

  triangle t, t_proj;

  vec3f l1, l2, normal, to_camera;
  float dot_camera, lum;

  unsigned int color;

  for (int ti = 0; ti < mesh_len; ti++)
  {
    t = cube_mesh[ti];

    for (byte v = 0; v < 3; v++)
    {
      // model translation
      //t_proj.p[v] = mat4f::mult_vec3f(t.p[v], mat_mod);
      
      t_proj.p[v] = mat4f::mult_vec3f(t.p[v], camera.view);
    }
    
    // normal calculation
    l1 = t_proj.p[1] - t_proj.p[0];
    l2 = t_proj.p[2] - t_proj.p[0];
    
    normal = vec3f::cross(l1, l2);
    normal.normalize();

    //to_camera = t_proj.p[0] - camera.pos;
    to_camera = t_proj.p[0];
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

    //Serial.println(t_proj.p[0].z);
    
    if(t_proj.p[0].z > 1 || t_proj.p[1].z > 1 || t_proj.p[2].z > 1) // drawing behind camera
      continue;
    

    lum = min(dot_camera*-0.85f + 0.15f, 1.0f);
    
    color = lum * 31;   // r (31)
    color = (color << 6) + lum * 63; // g (63)
    color = (color << 5) + lum * 31; // b (31)
 
    
    //tft.draw_triangle_buff((short)t_proj.p[0].x, (short)t_proj.p[0].y, (short)t_proj.p[1].x, (short)t_proj.p[1].y, (short)t_proj.p[2].x, (short)t_proj.p[2].y, color, z_buff, (color & 0xfe));
    tft.draw_triangle((short)t_proj.p[0].x, (short)t_proj.p[0].y, (short)t_proj.p[1].x, (short)t_proj.p[1].y, (short)t_proj.p[2].x, (short)t_proj.p[2].y, color);
  }
}



#define __B1 0
#define __B2 2
#define __B3 12

bool render;

void setup()
{    
  pinMode(__B1, INPUT_PULLUP);
  pinMode(__B2, INPUT_PULLUP);
  
  
  tft.start();
  load();

  render = true;




  //clear_z_buff(0);
  tft.fill_screen(BLACK);

  pinMode(__B3, INPUT_PULLUP);
}



float mov;

void loop() {

  mov = 0;
  if(digitalRead(__B1) != HIGH)
    mov += 0.2f;

  if(digitalRead(__B2) != HIGH)
    mov -= 0.2f;

  if(mov != 0)
  {
    if(digitalRead(__B3) != HIGH)
    {
      camera.yaw -= mov;
    }
    else                                               
    {
      //auto camera_step = camera.strafe() * mov;
      //camera.pos = camera.pos - camera_step;
      camera.pos.x -= mov;
    }

    camera.build_view();
    render = true;
  }


  
  
  
  if(render)
  {
    tft.fill_screen(BLACK);

    //render_loop();
    
    render_entity(cube);
    render_entity(cube2);
    render_entity(cube3);
    
    //process_z_buff();
   
    render = false;
  } 

  //delay(14);
}
