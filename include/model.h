#pragma once
#include "bitmap.h"
#include "object.h"

class CubeMap {
public:
    CubeMap(float size = 50.0f);
    ~CubeMap() = default;
    void render() const;

private:
    unsigned int texture;
    float size;
};

class Paper {
public:
    Paper(float width = 20.0f, float height = 20.0f, int image_width = 512, int image_height = 512);
    ~Paper();
    void clear();
    void fill_pixel(int x, int y, const Color& color);
    float get_width() const;
    float get_height() const;
    int get_image_width() const;
    int get_image_height() const;
    void render() const;
    void save_as(const std::string& out_filename) const;
    void update_texture() const;

private:
    unsigned int texture;
    float width;
    float height;
    Bitmap* image;
};

class MyPen {
public:
    MyPen() = default;
    ~MyPen() = default;
    const Color& get_line_color() const;
    bool is_clicked() const;
    bool is_drawing_mode() const;
    void move(float x, float y);
    void disable_drawing_mode();
    void enable_drawing_mode();
    void perform_click();
    void perform_draw_line(Paper* paper, int x1, int y1, int x2, int y2);
    void render(float disassembled = 0.0f) const;
    void set_line_color(const Color& color);

private:
    class Barrel {
    public:
        Barrel();
        ~Barrel();
        void render() const;
        void set_color(const Color& color);

    private:
        unsigned int texture;
        Bitmap* image;
        Object* obj;
    };

    class Cap {
    public:
        Cap();
        ~Cap();
        void render() const;

    private:
        Object* obj;
    };

    class Clip {
    public:
        Clip();
        ~Clip();
        void render() const;

    private:
        Object* obj;
    };

    class Grip {
    public:
        Grip();
        ~Grip();
        void render() const;

    private:
        Object* obj;
    };

    class Socket {
    public:
        Socket();
        ~Socket();
        void render() const;

    private:
        Object* obj;
    };

    class Spring {
    public:
        Spring();
        ~Spring();
        void render() const;
        void render_compressed() const;

    private:
        Object* obj;
    };

    class Tip {
    public:
        Tip();
        ~Tip();
        void render() const;

    private:
        Object* obj;
    };

    class Reservoir {
    public:
        Reservoir();
        ~Reservoir();
        void render() const;

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

    float x = 0.0f;
    float y = 0.0f;
    bool clicked = false;
    bool drawing = false;
    Color line_color = { 0.0f, 0.0f, 0.0f };
};
