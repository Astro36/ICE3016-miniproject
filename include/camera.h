#pragma once

constexpr float camera_rotation_speed = 3.0f;
constexpr float camera_zoom_speed = 1.0f;

class Camera {
public:
    Camera(float radius, float phi, float theta);
    ~Camera() = default;
    void move(float radius, float phi, float theta);
    void rotate_left();
    void rotate_up();
    void rotate_right();
    void rotate_down();
    void zoom_in();
    void zoom_out();
    void setup();

private:
    float radius;
    float phi;
    float theta;

    float get_x() const;
    float get_y() const;
    float get_z() const;
    float get_upy() const;
};

void preset_drawing(Camera* camera);
void preset_disassembling(Camera* camera);
void preset_observing(Camera* camera);
void preset_printing(Camera* camera);

float radian(float degree);
