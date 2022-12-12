#include "model.h"
#include <GL/glut.h>
#include <GL/glext.h>
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

    Bitmap image_px{ "res/1024px.bmp" };
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, image_px.get_width(), image_px.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image_px.get_pixels());
    Bitmap image_nx{ "res/1024nx.bmp" };
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, image_nx.get_width(), image_nx.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image_nx.get_pixels());
    Bitmap image_py{ "res/1024py.bmp" };
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, image_py.get_width(), image_py.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image_py.get_pixels());
    Bitmap image_ny{ "res/1024ny.bmp" };
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, image_ny.get_width(), image_ny.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image_ny.get_pixels());
    Bitmap image_pz{ "res/1024pz.bmp" };
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, image_pz.get_width(), image_pz.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image_pz.get_pixels());
    Bitmap image_nz{ "res/1024nz.bmp" };
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, image_nz.get_width(), image_nz.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image_nz.get_pixels());

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
}

void CubeMap::draw() const {
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

void MyPen::draw(float disassembled) const {
    glTranslatef(-1.5f * disassembled, 0.0f, 0.0f);
    pen_barrel.draw();
    pen_clip.draw();
    pen_grip.draw();

    if (!clicked) {
        glTranslatef(-0.1f, 0.0f, 0.0f);
    }
    glPushMatrix();
    glTranslatef(-0.5f * disassembled, 0.0f, 0.0f);
    pen_cap.draw();
    glPopMatrix();

    glTranslatef(1.0f * disassembled, 0.0f, 0.0f);
    pen_reservoir.draw();
    pen_socket.draw();

    glTranslatef(0.75f * disassembled, 0.0f, 0.0f);
    pen_spring.draw();

    glColor3f(line_color.red, line_color.green, line_color.blue);
    glPushMatrix();
    glTranslatef(1.88f, 0.0f, 0.0f);
    glutSolidSphere(0.004, 10, 10); // ball
    glPopMatrix();

    if (!clicked) {
        glTranslatef(0.1f, 0.0f, 0.0f);
    }
    glTranslatef(0.75f * disassembled, 0.0f, 0.0f);
    pen_tip.draw();
}

void MyPen::perform_click() {
    clicked = !clicked;
}

const Color& MyPen::get_line_color() const {
    return line_color;
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
    glTranslatef(1.98f, 0, 0);
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