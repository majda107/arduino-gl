// VECTORS
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

  float get_length() {
    return sqrt(x*x + y*y + z*z);
  }

  void normalize() {
    auto len = this->get_length();
    
    x /= len;
    y /= len;
    z /= len;
  }

  
  static float dot(const vec3f& v1, const vec3f& v2)
  {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
  }

  static vec3f cross(const vec3f& v1, const vec3f& v2)
  {
    vec3f v = vec3f(0, 0, 0);
  
    v.x = v1.y * v2.z - v1.z * v2.y;
    v.y = v1.z * v2.x - v1.x * v2.z;
    v.z = v1.x * v2.y - v1.y * v2.x;

    return v;
  }
  
};


vec3f operator+(const vec3f& v1, const vec3f& v2)
{
  return vec3f(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

vec3f operator-(const vec3f& v1, const vec3f& v2)
{
  return vec3f(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}







// MATRICES
struct mat4f
{
  float m[4][4] = { { 0 } };

  // multiplication
  static vec3f mult_mat4f_vec3f(const vec3f &v, const mat4f &m)
  {
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

  static vec3f mult_mat4f_mat4f(const mat4f &m, const mat4f &n)
  {
    mat4f r;

    // idk, steal somewhere, it's 4 AM DUH GO SLEEP LOL
    
    return r;
  }
  

  static mat4f projection(float fov, float near, float far, float aspect)
  {
    mat4f m;
    
    m.m[0][0] = fov * aspect;
    m.m[1][1] = fov;
    m.m[2][2] = f_far / (f_far - f_near);
    m.m[3][2] = (-f_far * f_near) / (f_far - f_near);
    m.m[2][3] = 1.0f;
    m.m[3][3] = 0.0f;
  }

  static mat4f identity()
  {
    mat4f m;
    m.m[0][0] = m.m[1][1] = m.m[2][2] = m.m[3][3] = 1;
    return m;
  }


  // rotations
  static mat4f rotation_X(float a)
  {
    mat4f m;
    
    m.m[0][0] = 1;
    m.m[1][1] = cosf(a);
    m.m[1][2] = sinf(a);
    m.m[2][1] = -sinf(a);
    m.m[2][2] = cosf(a);
    m.m[3][3] = 1;
    
    return m;
  }

  static mat4f rotation_Y(float a)
  {
    mat4f m;
    
    m.m[0][0] = cosf(a);
    m.m[2][0] = sinf(a);
    m.m[1][1] = 1;
    m.m[0][2] = -sinf(a);
    m.m[2][2] = cosf(a);
    m.m[3][3] = 1;
    
    return m;
  }

  static mat4f rotation_Z(float a)
  {
    mat4f m;
    
    m.m[0][0] = cosf(a);
    m.m[0][1] = sinf(a);
    m.m[1][0] = -sinf(a);
    m.m[1][1] = cosf(a);
    m.m[2][2] = 1;
    m.m[3][3] = 1;
    
    return m;
  }
};
