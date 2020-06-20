#pragma once
#include "majda_glm.h"


// mesh rendering things

struct triangle
{  
  vec3f p[3];

  triangle() {

  }
};



// "game" objects

class Camera 
{
public:
  mat4f view;

  vec3f pos;
  float pitch;
  float yaw;

  Camera(vec3f p)
  {
    this->pos = p;

    this->pitch = 0;
    this->yaw = 0;
  }

  void build_view()
  {
    this->view = mat4f::rotation_Z(this->yaw);
    this->view = this->view * mat4f::rotation_X(this->pitch);

    this->view = this->view * mat4f::translation(this->pos);
  }

};
