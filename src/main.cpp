#include <GL/glut.h>
#include <Windows.h>
#include <iostream>
#include "camera.h"
#include "draw.h"
#include "model.h"

int window_width = 500;
int window_height = 500;
int prev_window_width = 500;
int prev_window_height = 500;
int prev_paper_x = -1;
int prev_paper_y = -1;

HDC hdc; // handle display context

Camera camera{ 20, 45, 45 };
CubeMap* cube_map;
MyPen* mypen;
Paper* paper;

float animated = 0.0f;

bool fullscreen = false;

void init_font(HDC& hdc) {
    hdc = wglGetCurrentDC();
    HFONT font = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, HANGUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("맑은 고딕"));
    SelectObject(hdc, font);
}

void init_model() {
    cube_map = new CubeMap;
    mypen = new MyPen;
    paper = new Paper;
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

void init() {
    init_font(hdc);
    init_model();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

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

    cube_map->render();

    paper->render();
    draw_axis(5);

    mypen->render(animated);
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

void resize_cb(int width, int height) {
    window_width = width;
    window_height = height;
}

void mouse_cb(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        prev_paper_x = -1;
        prev_paper_y = -1;
    }
}

void motion_cb(int x, int y) {
    float pen_x = (x * paper->get_width() / window_width) - (paper->get_width() / 2);
    float pen_y = (y * paper->get_height() / window_height) - (paper->get_height() / 2);
    int paper_x = x * paper->get_image_width() / window_width;
    int paper_y = paper->get_image_height() - (y * paper->get_image_height() / window_height);
    mypen->move(pen_x, pen_y);
    if (prev_paper_x >= 0 && prev_paper_y >= 0) {
        mypen->perform_draw_line(paper, paper_x, paper_y, prev_paper_x, prev_paper_y);
        paper->update_texture();
    }
    prev_paper_x = paper_x;
    prev_paper_y = paper_y;
}

void keyboard_cb(unsigned char key, int x, int y) {
    switch (key) {
    case ',':
        animated = std::max(animated - 0.1f, 0.0f);
        break;
    case '.':
        animated += 0.1;
        break;
    case 'w':
        camera.rotate_up();
        break;
    case 'a':
        camera.rotate_left();
        break;
    case 's':
        camera.rotate_down();
        break;
    case 'd':
        camera.rotate_right();
        break;
    case 'z':
        camera.zoom_in();
        break;
    case 'x':
        camera.zoom_out();
        break;
    case ' ': // spacebar
        mypen->perform_click();
        if (mypen->is_drawing_mode()) {
            mypen->disable_drawing_mode();
        } else {
            mypen->enable_drawing_mode();
        }
        break;
    }
}

void special_keyboard_cb(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_F2:
        paper->save_as("paper.bmp");
        break;
    case GLUT_KEY_F11:
        toggle_fullscreen();
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
    glutCreateWindow("12191765 박승재");
    init();

    glutDisplayFunc(draw);
    glutReshapeFunc(resize_cb);
    glutMouseFunc(mouse_cb);
    glutMotionFunc(motion_cb);
    glutKeyboardFunc(keyboard_cb);
    glutSpecialFunc(special_keyboard_cb);
    glutIdleFunc(background_cb);

    glutMainLoop();
    return 0;
}
