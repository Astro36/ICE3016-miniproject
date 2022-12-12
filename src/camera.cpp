#define _USE_MATH_DEFINES
#include "camera.h"
#include <GL/glut.h>
#include <cmath>
#include <limits>

Camera::Camera(float radius, float phi, float theta)
: radius(radius), phi(phi), theta(theta) {
}

void Camera::rotate_left() {
    phi -= camera_rotation_speed;
    if (phi < 0) {
        phi += 360;
    }
}

void Camera::rotate_up() {
    theta -= camera_rotation_speed;
    if (theta < 0) {
        theta += 360;
    }
}

void Camera::rotate_right() {
    phi += camera_rotation_speed;
    if (phi >= 360) {
        phi -= 360;
    }
}

void Camera::rotate_down() {
    theta += camera_rotation_speed;
    if (theta >= 360) {
        theta -= 360;
    }
}

void Camera::zoom_in() {
    radius -= camera_zoom_speed;
}

void Camera::zoom_out() {
    radius += camera_zoom_speed;
}

void Camera::setup() {
    if (theta == 0 || theta == 180) {
        theta += std::numeric_limits<float>::epsilon();
    }
    float eyex = get_x();
    float eyey = get_y();
    float eyez = get_z();
    float upy = get_upy();
    gluLookAt(eyex, eyey, eyez, 0.0, 0.0, 0.0, 0.0, upy, 0.0);
}

float Camera::get_x() const {
    return radius * std::sin(radian(theta)) * std::cos(radian(phi));
}

float Camera::get_y() const {
    return radius * std::sin(radian(phi));
}

float Camera::get_z() const {
    return radius * std::cos(radian(theta)) * std::cos(radian(phi));
}

float Camera::get_upy() const {
    return std::cos(radian(phi));
}

float radian(float degree) {
    return degree * M_PI / 180;
}
