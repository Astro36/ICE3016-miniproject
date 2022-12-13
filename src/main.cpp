#include <GL/glut.h>
#include <Windows.h>
#include <iostream>
#include <mmsystem.h>
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

Camera camera{ 7.0f, 90.0f, 45.0f };
CubeMap* cube_map;
MyPen* mypen;
Paper* paper;

unsigned int sphere_selected = -1;

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // main viewport
    glViewport(0, 0, window_width, window_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double) window_width / window_height, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera.setup();

    float light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    cube_map->render();

    paper->render();
    draw_axis(5);

    mypen->render(animated);
    draw_axis(1);

    draw_text(hdc, L"안녕");

    glFlush();

    // mini viewport
    int viewport_width = 160;
    int viewport_height = 60;
    glViewport(window_width - viewport_width, window_height - viewport_height, viewport_width, viewport_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-viewport_width / 2, viewport_width / 2, -viewport_height / 2, viewport_height / 2, -50.0, 50.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float light_position2[] = { 0.0f, 0.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position2);

    // glDisable(GL_LIGHTING);
    draw_color_spheres();
    glEnable(GL_LIGHTING);

    glFlush();
    glutSwapBuffers();
}

void pick(int x, int y) {
    int viewport_width = 160;
    int viewport_height = 60;

    unsigned int select_buf[256];
    glSelectBuffer(256, select_buf);

    int viewport[4] = { window_width - viewport_width, 0, viewport_width, viewport_height }; // (x, y, width, height)

    glRenderMode(GL_SELECT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();
    gluPickMatrix(x, y, 0.1, 0.1, viewport);
    glOrtho(-viewport_width / 2, viewport_width / 2, -viewport_height / 2, viewport_height / 2, -50.0, 50.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    draw_color_spheres();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glFlush();

    int hits = glRenderMode(GL_RENDER);
    // std::cout << "hits: " << hits << '\n';
    if (hits >= 1) {
        unsigned int idx = 0;
        unsigned int selected_z = -1; // max value of unsigned int(overflow)
        sphere_selected = -1;
        for (int i = 1; i <= hits; i += 1) {
            unsigned int name_count = select_buf[idx]; // always 1
            unsigned int z_min = select_buf[idx + 1];
            unsigned int z_max = select_buf[idx + 2];
            unsigned int name = select_buf[idx + 3];
            idx += name_count + 3;
            if (selected_z > z_max) {
                selected_z = z_max;
                sphere_selected = name;
            }
        }
        if (sphere_selected == 0) {
            std::cout << "Line color: black\n";
            mypen->set_line_color({ 0.0f, 0.0f, 0.0f });
        } else if (sphere_selected == 1) {
            std::cout << "Line color: red\n";
            mypen->set_line_color({ 1.0f, 0.0f, 0.0f });
        } else if (sphere_selected == 2) {
            std::cout << "Line color: blue\n";
            mypen->set_line_color({ 0.0f, 0.0f, 1.0f });
        }
    }
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
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            pick(x, y);

        } else if (state == GLUT_UP) {
            prev_paper_x = -1;
            prev_paper_y = -1;
            sphere_selected = -1;
        }
    }
}

void motion_cb(int x, int y) {
    float pen_x = (x * paper->get_width() / window_width) - (paper->get_width() / 2);
    float pen_y = (y * paper->get_height() / window_height) - (paper->get_height() / 2);
    mypen->move(pen_x, pen_y);
    int paper_x = x * paper->get_image_width() / window_width;
    int paper_y = paper->get_image_height() - (y * paper->get_image_height() / window_height);
    if (prev_paper_x >= 0 && prev_paper_y >= 0 && mypen->is_clicked() && mypen->is_drawing_mode()) {
        mypen->perform_draw_line(paper, paper_x, paper_y, prev_paper_x, prev_paper_y);
        paper->update_texture();
    }
    prev_paper_x = paper_x;
    prev_paper_y = paper_y;
}

void keyboard_cb(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        preset_drawing(&camera);
        mypen->enable_drawing_mode();
        break;
    case '2':
        preset_observing(&camera);
        mypen->disable_drawing_mode();
        break;
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
        PlaySound(TEXT("res/click.wav"), NULL, SND_ASYNC);
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
