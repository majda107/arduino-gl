#pragma once
#include "majda_glm.h"


// mesh rendering things

struct triangle
{  
  vec3f p[3];

  triangle() {

  }

  triangle(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2) {
    p[0].x = x0;
    p[0].y = y0;
    p[0].z = z0;

    p[1].x = x1;
    p[1].y = y1;
    p[1].z = z1;

    p[2].x = x2;
    p[2].y = y2;
    p[2].z = z2;
  }
};



// "game" objects

class Camera 
{
public:
  mat4f view;

  vec3f forward()
  {
    return vec3f(this->view.m[0][2], this->view.m[1][2], this->view.m[2][2]);
  }

  vec3f strafe()
  {
    return vec3f(this->view.m[0][0], this->view.m[1][0], this->view.m[2][0]);
  }

  vec3f pos;
  float pitch;
  float yaw;

  vec3f look_at;

  Camera(vec3f p)
  {
    this->pos = p;

    this->pitch = 0;
    this->yaw = 0;

    this->look_at = vec3f(0, 0, 0);
  }

  void build_view()
  {
    //this->view = mat4f::translation(-(this->pos));
    
    //this->view = this->view * mat4f::rotation_X(-(this->pitch));
    //this->view = this->view * mat4f::rotation_Y(-(this->yaw));
    
    this->view = mat4f::look_at(this->pos, this->look_at, vec3f(0, 1, 0));
    //this->view = mat4f::quick_inverse(this->view);
  }

};





class MeshEntity
{
public:
  triangle* mesh;
  uint16_t mesh_len;

  mat4f model_mat;

  MeshEntity(triangle* mesh, uint16_t mesh_len, mat4f model_mat)
  {
    this->mesh = mesh;
    this->mesh_len = mesh_len;
    this->model_mat = model_mat;
  }
};
