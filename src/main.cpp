#define _USE_MATH_DEFINES
#include <GL/glut.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include "draw.h"
#include "model.h"

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define EPSILON 1e-7F
#define ROTATION_SPEED 10.0
#define ZOOM_SPEED 1.0
#define TEXTURES 11

void init_texture(int texture, const Bitmap& image);
void update_texture(int texture, const Bitmap& image);

void init_texture(int texture, const Bitmap& image) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image.get_width(), image.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.get_pixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void update_texture(int texture, const Bitmap& image) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image.get_width(), image.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.get_pixels());
}

typedef struct spherical {
    double r;
    double phi;
    double theta;
} spherical_t;

int window_width = WINDOW_WIDTH;
int window_height = WINDOW_HEIGHT;

GLUquadric* cylinder_quad = gluNewQuadric();
GLUquadric* sphere_quad = gluNewQuadric();
GLuint textures[TEXTURES];
// spherical_t camera = { 5, 45, 45 };
spherical_t camera = { 5, 0, 0 };
GLfloat light_position[] = { 10, 10, 10, 1 };
HDC hdc; // handle display context

MyPen* pen_obj;
float pen_x, pen_y;
Bitmap paper{ 512, 512 };
GLuint paper_texture;

float animated = 0.0f;

double radian(double degree) {
    return degree * M_PI / 180;
}

void init() {
    pen_obj = new MyPen();

    hdc = wglGetCurrentDC();
    HFONT font = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, HANGUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"맑은 고딕");
    SelectObject(hdc, font);

    glClearColor(0, 0, 0, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);

    // char filenames[TEXTURES][20] = {
    //     "img/TexImage0.bmp", "img/TexImage1.bmp", "img/TexImage2.bmp", "img/TexImage3.bmp", "img/TexImage4.bmp", "img/TexImage5.bmp",
    //     "img/CIDER_T.bmp", "img/CIDER_S.bmp", "img/CIDER_B.bmp", "img/coke.bmp",
    //     "img/EARTH.bmp"
    // };
    // glEnable(GL_TEXTURE_2D);
    // glGenTextures(TEXTURES, textures);
    // for (int i = 0; i < TEXTURES; i += 1) {
    //     //Bitmap image{ filenames[i] };
    //     //init_texture(textures[i], image);
    // }
    // glGenTextures(1, &paper_texture);
    // init_texture(paper_texture, paper);

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

void init_cube_map_texture() {
    // glGenTextures(1, &texture_cube_map);
    // glEnable(GL_TEXTURE_CUBE_MAP);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cube_map);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // int image_width, image_height, channels;
    // unsigned char* image;

    // image = readImageData("img/1024px.bmp", &image_width, &image_height, &channels);
    // glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // delete[] image;

    // image = readImageData("img/1024nx.bmp", &image_width, &image_height, &channels);
    // glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // delete[] image;

    // image = readImageData("img/1024py.bmp", &image_width, &image_height, &channels);
    // glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // delete[] image;

    // image = readImageData("img/1024ny.bmp", &image_width, &image_height, &channels);
    // glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // delete[] image;

    // image = readImageData("img/1024pz.bmp", &image_width, &image_height, &channels);
    // glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // delete[] image;

    // image = readImageData("img/1024nz.bmp", &image_width, &image_height, &channels);
    // glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // delete[] image;

    // glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    // glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    // glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
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
    init_cube_map_texture();
}

void draw_string() {
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluOrtho2D(0.0, 10.0, 10.0, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glRasterPos3f(0.5f, 1.0f, 0.0f);
    std::wstring text = L"1234";
    for (int i = 0; i < text.size(); i += 1) {
        int list = glGenLists(1);
        wglUseFontBitmapsW(hdc, text[i], 1, list);
        glCallList(list);
        glDeleteLists(list, 1);
    }
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopAttrib();
}

void draw_paper(float size = 1.0f) {
    glBindTexture(GL_TEXTURE_2D, paper_texture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-size / 2, 0.0f, size / 2);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(size / 2, 0.0f, size / 2);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(size / 2, 0.0f, -size / 2);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-size / 2, 0.0f, -size / 2);
    glEnd();
}

void draw(void) {
    // main viewport
    glViewport(0, 0, window_width, window_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float) window_width / window_height, 1, 50);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (camera.theta == 0 || camera.theta == 180) {
        camera.theta += EPSILON;
    }
    double x = camera.r * sin(radian(camera.theta)) * cos(radian(camera.phi));
    double y = camera.r * sin(radian(camera.theta)) * sin(radian(camera.phi));
    double z = camera.r * cos(radian(camera.theta));
    gluLookAt(y, z, x, 0, 0, 0, 0, (0 <= camera.theta && camera.theta < 180 ? 1 : -1), 0);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    draw_paper(10.0f);

    draw_axis(5);
    glTranslatef(pen_x, 1.89f, pen_y);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
    pen_obj->draw(animated);

    // text
    draw_string();

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

void motion_cb(int x, int y) {
    int paper_x = x * paper.get_width() / window_width;
    int paper_y = paper.get_height() - (y * paper.get_height() / window_height);
    paper.fill_pixel(paper_x, paper_y, pen_obj->get_line_color());
    update_texture(paper_texture, paper);
    pen_x = (x * 10.0f / window_width) - 5.0f; // paper size = 10.f
    pen_y = (y * 10.0f / window_height) - 5.0f;
}

void resize_cb(int width, int height) {
    window_width = width;
    window_height = height;
}

void keyboard_cb(unsigned char key, int x, int y) {
    switch (key) {
    case '0': // camera
        light_position[0] = 10;
        light_position[1] = 10;
        light_position[2] = 10;
        break;
    case 'z': // zoom in
        camera.r -= ZOOM_SPEED;
        break;
    case 'x': // zoom out
        camera.r += ZOOM_SPEED;
        break;
    case 's': // save
        paper.save("img/paper.bmp", "img/TexImage0.bmp");
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
    case GLUT_KEY_LEFT:
        camera.phi -= ROTATION_SPEED;
        if (camera.phi < 0) {
            camera.phi += 360;
        }
        break;
    case GLUT_KEY_UP:
        camera.theta -= ROTATION_SPEED;
        if (camera.theta < 0) {
            camera.theta += 360;
        }
        break;
    case GLUT_KEY_RIGHT:
        camera.phi += ROTATION_SPEED;
        if (camera.phi >= 360) {
            camera.phi -= 360;
        }
        break;
    case GLUT_KEY_DOWN:
        camera.theta += ROTATION_SPEED;
        if (camera.theta >= 360) {
            camera.theta -= 360;
        }
        break;
    }
}

void background(void) {
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(300, 300);
    glutCreateWindow("12191765");
    init();

    glutDisplayFunc(draw);
    glutReshapeFunc(resize_cb);
    glutMotionFunc(motion_cb); // main
    glutKeyboardFunc(keyboard_cb);
    glutSpecialFunc(special_keyboard_cb);
    glutIdleFunc(background);

    glutMainLoop();
    return 0;
}