#include "ILI9163C_TFT.h"

#define __CS  10
#define __RS  8
#define __DC  9

ILI9163C_TFT tft = ILI9163C_TFT(__CS, __RS, __DC);



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






void DrawTriangle(triangle t, unsigned int color, bool clean) {
  //DrawTriangle((short)t.p[0].x, (short)t.p[0].y, (short)t.p[1].x, (short)t.p[1].y, (short)t.p[2].x, (short)t.p[2].y, color, clean);
  tft.draw_triangle((short)t.p[0].x, (short)t.p[0].y, (short)t.p[1].x, (short)t.p[1].y, (short)t.p[2].x, (short)t.p[2].y, color);
}

void DrawTriangle(short x0, short y0, short x1, short y1, short x2, short y2, unsigned int color, bool clean)
{
  tft.draw_line(x0, y0, x1, y1, color);
  tft.draw_line(x1, y1, x2, y2, color);
  tft.draw_line(x2, y2, x0, y0, color);
}




vec3f multiply_mat4_vec3(vec3f v, mat4f m) {
  vec3f r;

  r.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
  r.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
  r.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];

  float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

  if (w == 0.0f)
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
  float aspect = (float)tft.HEIGHT / (float)tft.WIDTH;

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

void render_loop(unsigned int color, bool clean) {
  mat4f mat_z, mat_x;

  populate_rotZ(mat_z, theta);
  populate_rotX(mat_x, theta);


  triangle t;

  triangle t_proj;

  for (int ti = 0; ti < mesh_len; ti++)
  {
    t = cube_mesh[ti];

    for (int v = 0; v < 3; v++)
    {
      // translation
      //t.p[v].z += zt;
      //t.p[v].x += 1.0;
      //t.p[v].y += xt;

      t_proj.p[v] = multiply_mat4_vec3(t.p[v], mat_z);
      t_proj.p[v] = multiply_mat4_vec3(t_proj.p[v], mat_x);

      t_proj.p[v].x += 0.0f;
      t_proj.p[v].y += 0.0f;
      t_proj.p[v].z += 3.25f;

      t_proj.p[v] = multiply_mat4_vec3(t_proj.p[v], mat_proj);

      // scale
      t_proj.p[v].x += 1.0f;
      t_proj.p[v].y += 1.0f;

      t_proj.p[v].x *= 0.5f * (float)tft.WIDTH;
      t_proj.p[v].y *= 0.5f * (float)tft.HEIGHT;
    }

    DrawTriangle(t_proj, color, clean);
  }
}





void setup()
{
  tft.start();
  load();
}








void loop() {
  tft.fill_screen(BLACK);

  theta += 0.2f;
  render_loop(WHITE, false);

  delay(14);
}
