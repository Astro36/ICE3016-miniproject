#include "draw.h"
#include <GL/freeglut.h>
#include <string>

void draw_axis(float length) {
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f); // X
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(length * 2, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f); // Y
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, length * 2, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f); // Z
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, length * 2);
    glEnd();
    glEnable(GL_LIGHTING);
}

void draw_color_spheres() {
    glInitNames();
    glPushName(-1); // no empty stack

    glLoadName(0);
    glPushMatrix();
    glTranslatef(-50.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glutSolidSphere(20.0f, 20, 20);
    glPopMatrix();

    glLoadName(1);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidSphere(20.0f, 20, 20);
    glPopMatrix();

    glLoadName(2);
    glPushMatrix();
    glTranslatef(50.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glutSolidSphere(20.0f, 20, 20);
    glPopMatrix();
}

void draw_text(const HDC& hdc, const std::wstring& text) {
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 10.0, 10.0, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos3f(0.5f, 1.0f, 0.0f);
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
    glEnable(GL_LIGHTING);
}
