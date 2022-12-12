#include "draw.h"
#include <GL/glut.h>

void draw_axis(float length) {
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
}
