#include <GL/glut.h>
#include<bits/stdc++.h>
#ifndef CLASSES_HPP
#define CLASSES_HPP
//====================declearation start========================

// Vector class for 3D operations
class Vec3 {
public:
    float x, y, z;
    Vec3(): x(0), y(0), z(0) {}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vec3 operator+(const Vec3 &v) const { return Vec3(x+v.x, y+v.y, z+v.z); }
    Vec3 operator-(const Vec3 &v) const { return Vec3(x-v.x, y-v.y, z-v.z); }
    Vec3 operator*(float s) const { return Vec3(x*s, y*s, z*s); }
    Vec3 operator/(float s) const { return Vec3(x/s, y/s, z/s); }
    Vec3& operator+=(const Vec3 &v) { x+=v.x; y+=v.y; z+=v.z; return *this; }
    Vec3& operator-=(const Vec3 &v) { x-=v.x; y-=v.y; z-=v.z; return *this; }
    Vec3& operator*=(float s) { x*=s; y*=s; z*=s; return *this; }
    float length() const { return std::sqrt(x*x + y*y + z*z); }
    Vec3 normalized() const { float l = length(); return (l>0)? (*this)/l : Vec3(); }
    static float dot(const Vec3 &a, const Vec3 &b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
    static Vec3 cross(const Vec3 &a, const Vec3 &b) {
        return Vec3(a.y*b.z - a.z*b.y,
                    a.z*b.x - a.x*b.z,
                    a.x*b.y - a.y*b.x);
    }
};
// Camera with position, target (look-at point), and up-vector
class Camera {
public:
    Vec3 position;
    Vec3 target;
    Vec3 up;
    Camera();

    void apply();             // Set the view with gluLookAt
    void moveForward(float d);
    void moveRight(float d);
    void moveUpGlobal(float d);
    void moveUpLocal(float d);
    void yaw(float angleDeg);   // rotate around global Y
    void pitch(float angleDeg); // rotate around camera's right axis
    void roll(float angleDeg);  // rotate around camera's forward axis
};

// Ray class for ray tracing
class Ray {
public:
    Vec3 start;
    Vec3 dir;

    Ray(Vec3 start, Vec3 dir) {
        this->start = start;
        this->dir = dir;
        this->dir.normalized();
    }
};

// Base class for all drawable objects in the scene
class Object {
public:
    Vec3 reference_point; 
    double height, width, length;
    double color[3];
    double coEfficients[4]; // ambient, diffuse, specular, reflection
    int shine; 

    Object() {}
    virtual ~Object() {} // Virtual destructor for proper cleanup

    virtual void draw() = 0;
    virtual double intersect(Ray *r, double *color, int level) {
        return -1.0;
    }

    void setColor(double r, double g, double b) {
        color[0] = r; color[1] = g; color[2] = b;
    }
    void setShine(int s) { shine = s; }
    void setCoEfficients(double amb, double diff, double spec, double rec) {
        coEfficients[0] = amb; coEfficients[1] = diff;
        coEfficients[2] = spec; coEfficients[3] = rec;
    }
};

// --- Derived Object Classes ---

class Sphere : public Object {
public:
    Sphere(Vec3 center, double radius) {
        reference_point = center;
        length = radius;
    }
    void draw() override;
};

class Triangle : public Object {
public:
    Vec3 p1, p2, p3;
    Triangle(Vec3 p1, Vec3 p2, Vec3 p3) {
        this->p1 = p1; this->p2 = p2; this->p3 = p3;
    }
    void draw() override;
};

class General : public Object {
public:
    // A,B,C,D,E,F,G,H,I,J
    double quadric_coeffs[10];

    General(double coeffs[10], Vec3 ref, double l, double w, double h) {
        for(int i=0; i<10; ++i) quadric_coeffs[i] = coeffs[i];
        reference_point = ref;
        length = l; width = w; height = h;
    }
    // General quadric surfaces are not drawn in OpenGL, only in ray tracing
    void draw() override { /* Does nothing */ }
};

class Floor : public Object {
public:
    Floor(double floorWidth, double tileWidth) {
        reference_point = Vec3(-floorWidth/2, -floorWidth/2, 0);
        length = tileWidth;
    }
    void draw() override;
};


// --- Light Classes ---

class PointLight {
public:
    Vec3 light_pos;
    double color[3];

    PointLight(Vec3 pos, double r, double g, double b) {
        light_pos = pos;
        color[0] = r; color[1] = g; color[2] = b;
    }
    void draw();
};

class SpotLight {
public:
    PointLight point_light;
    Vec3 light_direction;
    double cutoff_angle;

    SpotLight(PointLight pl, Vec3 dir, double angle) 
        : point_light(pl), light_direction(dir), cutoff_angle(angle) {}
    void draw();
};






Camera::Camera() {
    // Default initialization (can be overridden in main)
    position = Vec3(0.0f, 0.0f, -5.0f);
    target   = Vec3(0.0f, 0.0f,  0.0f);
    up       = Vec3(0.0f, 1.0f,  0.0f);
}

void Camera::apply() {
    gluLookAt(position.x, position.y, position.z,
              target.x,   target.y,   target.z,
              up.x,       up.y,       up.z);
}

void Camera::moveForward(float d) {
    Vec3 forward = (target - position).normalized();
    Vec3 move = forward * d;
    position += move;
    target   += move;
}

void Camera::moveRight(float d) {
    Vec3 forward = (target - position).normalized();
    Vec3 right = Vec3::cross(forward, up).normalized();
    Vec3 move = right * d;
    position += move;
    target   += move;
}

void Camera::moveUpGlobal(float d) {
    Vec3 move=up.normalized()*d;
    position += move;
    target   += move;
}

void Camera::moveUpLocal(float d) {
    // Move camera up/down without moving the target (reference point)
    Vec3 move=up.normalized()*d;
    position += move;
    std::cout<<target.x<<","<<target.y<<","<<target.z<<std::endl;
  
}

void Camera::yaw(float angleDeg) {
    
    float rad =angleDeg *M_PI / 180.0f;
    float tanA= tanf(rad);
    Vec3 forward = (target - position);
    Vec3 right = Vec3::cross(forward, up).normalized();
    Vec3 move=right*tanA+forward;
    move=move.normalized();
    target=position+move;
    // Update the up vector to maintain orthogonality
}

void Camera::pitch(float angleDeg) {
    // Rotate camera around its right vector (perpendicular to forward and up)
    float rad =angleDeg *M_PI / 180.0f;
    float tanA= tanf(rad);
    Vec3 forward = (target - position);
    Vec3 move=up*tanA+forward;
    move=move.normalized();
    target=position+move;
    // Update the up vector to maintain orthogonality
}

void Camera::roll(float angleDeg) {
    // Rotate camera around its forward vector
    Vec3 forward = (target - position).normalized();
    float rad = angleDeg * M_PI / 180.0f;
    float cosA = cosf(rad), sinA = sinf(rad);
    Vec3 newUp;
    // Rodrigues' rotation for up-vector around forward axis
    newUp.x = up.x * cosA + Vec3::cross(forward, up).x * sinA + forward.x * (Vec3::dot(forward, up) * (1 - cosA));
    newUp.y = up.y * cosA + Vec3::cross(forward, up).y * sinA + forward.y * (Vec3::dot(forward, up) * (1 - cosA));
    newUp.z = up.z * cosA + Vec3::cross(forward, up).z * sinA + forward.z * (Vec3::dot(forward, up) * (1 - cosA));
    up = newUp.normalized();
}

#endif // CLASSES_HPP
