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




//====================declearation end========================

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
