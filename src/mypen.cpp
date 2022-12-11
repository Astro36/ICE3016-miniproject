#include "mypen.h"
#include <GL/glut.h>
#include "object.h"

void MyPen::click()
{
	clicked = !clicked;
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

	glColor3f(color.red, color.green, color.blue);
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

const Color& MyPen::get_color() const
{
	return color;
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

void MyPen::Spring::draw_compress() const {
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glScalef(0.4f, 1.0f, 1.0f);
	glTranslatef(1.98f, 0, 0);
	obj->draw();
	glPopMatrix();
}
