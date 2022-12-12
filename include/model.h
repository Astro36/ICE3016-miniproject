#pragma once
#include "bitmap.h"
#include "object.h"

class CubeMap {
public:
    CubeMap(float size = 50.0f);
    ~CubeMap() = default;
    void draw() const;

private:
    unsigned int texture;
    float size;
};

class MyPen {
public:
    MyPen() = default;
    ~MyPen() = default;
    void draw(float disassembled = 0.0f) const;
    const Color& get_line_color() const;
    void perform_click();

private:
    class Barrel {
    public:
        Barrel();
        ~Barrel();
        void draw() const;

    private:
        Object* obj;
    };

    class Cap {
    public:
        Cap();
        ~Cap();
        void draw() const;

    private:
        Object* obj;
    };

    class Clip {
    public:
        Clip();
        ~Clip();
        void draw() const;

    private:
        Object* obj;
    };

    class Grip {
    public:
        Grip();
        ~Grip();
        void draw() const;

    private:
        Object* obj;
    };

    class Socket {
    public:
        Socket();
        ~Socket();
        void draw() const;

    private:
        Object* obj;
    };

    class Spring {
    public:
        Spring();
        ~Spring();
        void draw() const;
        void draw_compressed() const;

    private:
        Object* obj;
    };

    class Tip {
    public:
        Tip();
        ~Tip();
        void draw() const;

    private:
        Object* obj;
    };

    class Reservoir {
    public:
        Reservoir();
        ~Reservoir();
        void draw() const;

    private:
        Object* obj;
    };

    Barrel pen_barrel;
    Cap pen_cap;
    Clip pen_clip;
    Grip pen_grip;
    Socket pen_socket;
    Spring pen_spring;
    Tip pen_tip;
    Reservoir pen_reservoir;

    bool clicked = false;
    Color line_color = { 0.0f, 0.0f, 0.0f };
};

class Paper {
public:
    Paper(float width = 20.0f, float height = 20.0f, int image_width = 512, int image_height = 512);
    ~Paper() = default;
    void draw() const;
    void fill_pixel(int x, int y, const Color& color);
    float get_width() const;
    float get_height() const;
    int get_image_width() const;
    int get_image_height() const;
    void save_as(const std::string& out_filename) const;
    void update_texture() const;

private:
    unsigned int texture;
    float width;
    float height;
    Bitmap* image;
};
