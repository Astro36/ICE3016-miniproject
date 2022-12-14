#include "model.h"
#include <GL/glut.h>
#include <GL/glext.h>
#include <algorithm>
#include "bitmap.h"
#include "object.h"

CubeMap::CubeMap(float size)
: size(size) {
    glGenTextures(1, &texture);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    Bitmap image_px{ "res/cubemap/px.bmp" };
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, image_px.get_width(), image_px.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image_px.get_pixels());
    Bitmap image_nx{ "res/cubemap/nx.bmp" };
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, image_nx.get_width(), image_nx.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image_nx.get_pixels());
    Bitmap image_py{ "res/cubemap/py.bmp" };
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, image_py.get_width(), image_py.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image_py.get_pixels());
    Bitmap image_ny{ "res/cubemap/ny.bmp" };
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, image_ny.get_width(), image_ny.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image_ny.get_pixels());
    Bitmap image_pz{ "res/cubemap/pz.bmp" };
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, image_pz.get_width(), image_pz.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image_pz.get_pixels());
    Bitmap image_nz{ "res/cubemap/nz.bmp" };
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, image_nz.get_width(), image_nz.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image_nz.get_pixels());

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
}

void CubeMap::render() const {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    float half_size = size / 2;
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    // px
    glTexCoord3f(1, -1, -1);
    glVertex3f(half_size, -half_size, -half_size);
    glTexCoord3f(1, -1, 1);
    glVertex3f(half_size, -half_size, half_size);
    glTexCoord3f(1, 1, 1);
    glVertex3f(half_size, half_size, half_size);
    glTexCoord3f(1, 1, -1);
    glVertex3f(half_size, half_size, -half_size);

    // nx
    glTexCoord3f(-1, -1, 1);
    glVertex3f(-half_size, -half_size, half_size);
    glTexCoord3f(-1, -1, -1);
    glVertex3f(-half_size, -half_size, -half_size);
    glTexCoord3f(-1, 1, -1);
    glVertex3f(-half_size, half_size, -half_size);
    glTexCoord3f(-1, 1, 1);
    glVertex3f(-half_size, half_size, half_size);

    // py
    glTexCoord3f(1, 1, 1);
    glVertex3f(half_size, half_size, half_size);
    glTexCoord3f(-1, 1, 1);
    glVertex3f(-half_size, half_size, half_size);
    glTexCoord3f(-1, 1, -1);
    glVertex3f(-half_size, half_size, -half_size);
    glTexCoord3f(1, 1, -1);
    glVertex3f(half_size, half_size, -half_size);

    // ny
    glTexCoord3f(-1, -1, 1);
    glVertex3f(-half_size, -half_size, half_size);
    glTexCoord3f(1, -1, 1);
    glVertex3f(half_size, -half_size, half_size);
    glTexCoord3f(1, -1, -1);
    glVertex3f(half_size, -half_size, -half_size);
    glTexCoord3f(-1, -1, -1);
    glVertex3f(-half_size, -half_size, -half_size);

    // pz
    glTexCoord3f(1, -1, 1);
    glVertex3f(half_size, -half_size, half_size);
    glTexCoord3f(-1, -1, 1);
    glVertex3f(-half_size, -half_size, half_size);
    glTexCoord3f(-1, 1, 1);
    glVertex3f(-half_size, half_size, half_size);
    glTexCoord3f(1, 1, 1);
    glVertex3f(half_size, half_size, half_size);

    // nz
    glTexCoord3f(-1, -1, -1);
    glVertex3f(-half_size, -half_size, -half_size);
    glTexCoord3f(1, -1, -1);
    glVertex3f(half_size, -half_size, -half_size);
    glTexCoord3f(1, 1, -1);
    glVertex3f(half_size, half_size, -half_size);
    glTexCoord3f(-1, 1, -1);
    glVertex3f(-half_size, half_size, -half_size);
    glEnd();

    glDisable(GL_TEXTURE_CUBE_MAP);
    glEnable(GL_LIGHTING);
}

Paper::Paper(float width, float height, int image_width, int image_height)
: width(width), height(height) {
    image = new Bitmap{ image_width, image_height };
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image->get_width(), image->get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->get_pixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

Paper::~Paper() {
    delete image;
}

void Paper::fill_pixel(int x, int y, const Color& color) {
    if (x >= 0 && x < image->get_width() && y > 0 && y < image->get_height()) {
        image->fill_pixel(x, y, color);
    }
}

float Paper::get_width() const {
    return width;
}

float Paper::get_height() const {
    return height;
}

int Paper::get_image_width() const {
    return image->get_width();
}

int Paper::get_image_height() const {
    return image->get_height();
}

void Paper::render() const {
    float half_width = width / 2;
    float half_height = height / 2;
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-half_width, 0.0f, half_height);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(half_width, 0.0f, half_height);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(half_width, 0.0f, -half_height);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-half_width, 0.0f, -half_height);
    glEnd();
}

void Paper::save_as(const std::string& out_filename) const {
    image->save(out_filename, "res/image.bmp");
}

void Paper::update_texture() const {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image->get_width(), image->get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->get_pixels());
}

const Color& MyPen::get_line_color() const {
    return line_color;
}

bool MyPen::is_clicked() const {
    return clicked;
}

bool MyPen::is_drawing_mode() const {
    return drawing;
}

void MyPen::move(float x, float y) {
    this->x = x;
    this->y = y;
}

void MyPen::disable_drawing_mode() {
    drawing = false;
}

void MyPen::enable_drawing_mode() {
    drawing = true;
}

void MyPen::perform_click() {
    clicked = !clicked;
}

void MyPen::perform_draw_line(Paper* paper, int x1, int y1, int x2, int y2) {
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int p;
    int inc_2dy;
    int inc_2dydx;
    int inc;
    int ndx;

    if (dy <= dx) {
        inc_2dy = 2 * dy;
        inc_2dydx = 2 * (dy - dx);
        if (x2 < x1) {
            ndx = x1;
            x1 = x2;
            x2 = ndx;

            ndx = y1;
            y1 = y2;
            y2 = ndx;
        }
        inc = y1 < y2 ? 1 : -1;
        paper->fill_pixel(x1, y1, line_color);
        p = 2 * dy - dx;
        for (ndx = x1; ndx < x2; ndx++) {
            if (0 > p)
                p += inc_2dy;
            else {
                p += inc_2dydx;
                y1 += inc;
            }
            paper->fill_pixel(ndx, y1, line_color);
        }
    } else {
        inc_2dy = 2 * dx;
        inc_2dydx = 2 * (dx - dy);
        if (y2 < y1) {
            ndx = y1;
            y1 = y2;
            y2 = ndx;

            ndx = x1;
            x1 = x2;
            x2 = ndx;
        }
        inc = x1 < x2 ? 1 : -1;
        paper->fill_pixel(x1, y1, line_color);
        p = 2 * dx - dy;
        for (ndx = y1; ndx < y2; ndx++) {
            if (0 > p)
                p += inc_2dy;
            else {
                p += inc_2dydx;
                x1 += inc;
            }
            paper->fill_pixel(x1, ndx, line_color);
        }
    }
}

void MyPen::render(float animated) const {
    glTranslatef(x, 1.89f, y);
    if (drawing) {
        glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
    }

    glTranslatef(-1.5f * animated, 0.0f, 0.0f);
    pen_barrel.draw();
    pen_clip.draw();
    pen_grip.draw();

    if (!clicked) {
        glTranslatef(-0.1f, 0.0f, 0.0f);
    }
    glPushMatrix();
    glTranslatef(-0.5f * animated, 0.0f, 0.0f);
    pen_cap.draw();
    glPopMatrix();

    glTranslatef(1.0f * animated, 0.0f, 0.0f);
    pen_reservoir.draw();
    pen_socket.draw();

    glTranslatef(0.75f * animated, 0.0f, 0.0f);
    pen_spring.draw();

    glColor3f(line_color.red, line_color.green, line_color.blue);
    glPushMatrix();
    glTranslatef(1.88f, 0.0f, 0.0f);
    glutSolidSphere(0.004, 10, 10); // ball
    glPopMatrix();

    if (!clicked) {
        glTranslatef(0.1f, 0.0f, 0.0f);
    }
    glTranslatef(0.75f * animated, 0.0f, 0.0f);
    pen_tip.draw();
}

void MyPen::set_line_color(const Color& color) {
    line_color = color;
}

MyPen::Barrel::Barrel()
: obj(new Object("res/mypen-barrel.obj")) {}

MyPen::Barrel::~Barrel() {
    delete obj;
}

void MyPen::Barrel::draw() const {
    glColor4f(0.2f, 0.2f, 0.2f, 0.9f);
    obj->draw();
}

MyPen::Cap::Cap()
: obj(new Object("res/mypen-cap.obj")) {}

MyPen::Cap::~Cap() {
    delete obj;
}

void MyPen::Cap::draw() const {
    glColor4f(0.2f, 0.2f, 0.2f, 0.9f);
    obj->draw();
}

MyPen::Clip::Clip()
: obj(new Object("res/mypen-clip.obj")) {}

MyPen::Clip::~Clip() {
    delete obj;
}

void MyPen::Clip::draw() const {
    glColor3f(0.8f, 0.8f, 0.8f);
    obj->draw();
}

MyPen::Grip::Grip()
: obj(new Object("res/mypen-grip.obj")) {}

MyPen::Grip::~Grip() {
    delete obj;
}

void MyPen::Grip::draw() const {
    glColor3f(0.2f, 0.2f, 0.2f);
    obj->draw();
}

MyPen::Socket::Socket()
: obj(new Object("res/mypen-socket.obj")) {}

MyPen::Socket::~Socket() {
    delete obj;
}

void MyPen::Socket::draw() const {
    glColor3f(0.8f, 0.8f, 0.8f);
    obj->draw();
}

MyPen::Spring::Spring() {
    obj = new Object("res/mypen-spring.obj");
}

MyPen::Spring::~Spring() {
    delete obj;
}

void MyPen::Spring::draw() const {
    glColor3f(0.9f, 0.9f, 0.9f);
    obj->draw();
}

void MyPen::Spring::draw_compressed() const {
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glScalef(0.4f, 1.0f, 1.0f);
    glTranslatef(1.98f, 0.0f, 0.0f);
    obj->draw();
    glPopMatrix();
}

MyPen::Tip::Tip()
: obj(new Object("res/mypen-tip.obj")) {}

MyPen::Tip::~Tip() {
    delete obj;
}

void MyPen::Tip::draw() const {
    glColor3f(0.1f, 0.1f, 0.1f);
    obj->draw();
}

MyPen::Reservoir::Reservoir()
: obj(new Object("res/mypen-reservoir.obj")) {}

MyPen::Reservoir::~Reservoir() {
    delete obj;
}

void MyPen::Reservoir::draw() const {
    glColor4f(0.95f, 0.95f, 1.0f, 0.8f);
    obj->draw();
}
