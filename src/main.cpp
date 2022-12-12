﻿#include <GL/glut.h>
#include <Windows.h>
#include <algorithm>
#include <iostream>
#include <math.h>
#include "camera.h"
#include "draw.h"
#include "model.h"

#define TEXTURES 11

int window_width = 500;
int window_height = 500;
int prev_window_width = 500;
int prev_window_height = 500;

HDC hdc; // handle display context

Camera camera{ 20, 45, 45 };
CubeMap* cube_map;
MyPen* pen_obj;
Paper* paper;

float pen_x, pen_y;

float animated = 0.0f;

bool fullscreen = false;

void toggle_fullscreen() {
    fullscreen = !fullscreen;
    if (fullscreen) {
        prev_window_width = glutGet(GLUT_WINDOW_WIDTH);
        prev_window_height = glutGet(GLUT_WINDOW_HEIGHT);
        glutFullScreen();
    } else {
        glutReshapeWindow(prev_window_width, prev_window_height);
    }
}

void init_font(HDC& hdc) {
    hdc = wglGetCurrentDC();
    HFONT font = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, HANGUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("맑은 고딕"));
    SelectObject(hdc, font);
}

void init() {
    pen_obj = new MyPen;
    paper = new Paper;

    init_font(hdc);

    glClearColor(0, 0, 0, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LIGHTING);
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT0);

    GLfloat specular_sun[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_sun);
    glMaterialf(GL_FRONT, GL_SHININESS, 16);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_CULL_FACE);
    cube_map = new CubeMap();
}

void init_light() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    float light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);
}

void init2() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);

    init_light();
}

void draw(void) {
    // main viewport
    glViewport(0, 0, window_width, window_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double) window_width / window_height, 0.1, 100.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera.setup();

    cube_map->draw();

    paper->draw();

    draw_axis(5);
    glTranslatef(pen_x, 1.89f, pen_y);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
    pen_obj->draw(animated);
    draw_axis(1);

    // text
    draw_text(hdc, L"안녕");

    glFlush();

    // mini viewport
    /*glViewport(0, window_height * 3 / 4, window_width / 4, window_height / 4);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)window_width / window_height, 1, 50);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(3, 3, 3, 0, 0, 0, 0, 1, 0);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glColor3f(1, 1, 0);
    glutSolidTorus(0.5, 1, 10, 10);
    glPopMatrix();
    draw_axis(2);
    glFlush();*/

    glutSwapBuffers();
}

int prev_x, prev_y;

void motion_cb(int x, int y) {
    int paper_x = x * paper->get_image_width() / window_width;
    int paper_y = paper->get_image_height() - (y * paper->get_image_height() / window_height);
    std::cout << paper_x << ' ' << paper_y << '\n';
    paper->fill_pixel(paper_x, paper_y, pen_obj->get_line_color());
    paper->update_texture();
    prev_x = x;
    prev_y = y;
    pen_x = (x * 10.0f / window_width) - 5.0f; // paper size = 10.f
    pen_y = (y * 10.0f / window_height) - 5.0f;
}

void resize_cb(int width, int height) {
    window_width = width;
    window_height = height;
}

void keyboard_cb(unsigned char key, int x, int y) {
    switch (key) {
    case 'z':
        camera.zoom_in();
        break;
    case 'x':
        camera.zoom_out();
        break;
    case 's':
        paper->save_as("paper.bmp");
        break;
    case ',':
        animated = std::max(animated - 0.1f, 0.0f);
        break;
    case '.':
        animated += 0.1;
        break;
    case ' ':
        pen_obj->perform_click();
        break;
    }
}

void special_keyboard_cb(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_F11:
        toggle_fullscreen();
        break;
    case GLUT_KEY_LEFT:
        camera.rotate_left();
        break;
    case GLUT_KEY_UP:
        camera.rotate_up();
        break;
    case GLUT_KEY_RIGHT:
        camera.rotate_right();
        break;
    case GLUT_KEY_DOWN:
        camera.rotate_down();
        break;
    }
}

void background_cb(void) {
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(300, 300);
    glutCreateWindow("12191765 박승재");
    init();

    glutDisplayFunc(draw);
    glutReshapeFunc(resize_cb);
    glutMotionFunc(motion_cb);
    glutKeyboardFunc(keyboard_cb);
    glutSpecialFunc(special_keyboard_cb);
    glutIdleFunc(background_cb);

    glutMainLoop();
    return 0;
}