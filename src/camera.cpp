#define _USE_MATH_DEFINES
#include "camera.h"
#include <GL/glut.h>
#include <cmath>
#include <limits>

Camera::Camera(float radius, float phi, float theta)
: radius(radius), phi(phi), theta(theta) {
}

void Camera::move(float radius, float phi, float theta) {
    this->radius = radius;
    this->phi = phi;
    this->theta = theta;
}

void Camera::rotate_left() {
    theta -= camera_rotation_speed;
    if (theta < 0) {
        theta += 360;
    }
}

void Camera::rotate_up() {
    phi += camera_rotation_speed;
    if (phi >= 360) {
        phi -= 360;
    }
}

void Camera::rotate_right() {
    theta += camera_rotation_speed;
    if (theta >= 360) {
        theta -= 360;
    }
}

void Camera::rotate_down() {
    phi -= camera_rotation_speed;
    if (phi < 0) {
        phi += 360;
    }
}

void Camera::zoom_in() {
    radius -= camera_zoom_speed;
    if (radius < 0) {
        radius = 0;
    }
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

void preset_drawing(Camera* camera) {
    camera->move(25.0f, 90.0f, 0.0f);
}

void preset_disassembling(Camera* camera) {
    camera->move(10.0f, 90.0f, 45.0f);
}

void preset_observing(Camera* camera) {
    camera->move(7.0f, 90.0f, 45.0f);
}

void preset_printing(Camera* camera) {
    camera->move(30.0f, 45.0f, 90.0f);
}
