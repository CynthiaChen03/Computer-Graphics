#define FREEGLUT_STATIC
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
const double PI = 3.14159265358979323846;
int width = 800;
int height = 600;

bool flag;

using namespace std;
float r = 0;
float s = 0;
float t = 0;

GLfloat step = 1; // declare step
int time_interval = 16; // declare refresh interval in ms

//The shining point rectanglar area
int rectLeft = 370;   // x coordinates in the lower left corner of the shining point rectanglar area
int rectBottom = 240;  // Shining points area's bottom left y coordinates
int rectRight = 780;   // Area's top-right x-coordinate
int rectTop = 380;     // Area's top-right y coordinate

struct Point {
	int x;
	int y;
	float r;
	float g;
	float b;
	int lifespan; // The existence time of the/point
};
std::vector<Point> points; // An array of memory points
void timer(int value ) {
	// The lifetime of the update point
	for (auto& point : points) {
		point.lifespan--;
	}

	// Deletes a point with a lifetime of 0
	points.erase(std::remove_if(points.begin(), points.end(), [](const Point& point) {
		return point.lifespan <= 0;
		}), points.end());


	// Add new points to generate 5 points
	for (int i = 0; i < 5; ++i) {
		Point newPoint;
		newPoint.x = rectLeft + rand() % (rectRight - rectLeft + 1);
		newPoint.y = rectBottom + rand() % (rectTop - rectBottom + 1);
		newPoint.r = (rand() % 256) / 255.0f;
		newPoint.g = (rand() % 256) / 255.0f;
		newPoint.b = (rand() % 256) / 255.0f;
		newPoint.lifespan = 7; // The existence time of a point is 7 frames

		points.push_back(newPoint);
	}


	// 重绘窗口
	glutPostRedisplay();

	// 设置下一次定时器
	glutTimerFunc(1000 / 30, timer, 0); // 30帧每秒，即每秒闪烁30次
}

void when_in_mainloop() { // idle callback function
	glutPostRedisplay(); // force OpenGL to redraw the current window
}

void OnTimer(int value) {
	r += step;
	if (r >= 360)
		r = 0;
	else if (r <= 0)
		r = 359;
	glutTimerFunc(time_interval, OnTimer, 1);
}

void keyboard_input(unsigned char key, int x, int y) {// keyboard interaction
	if (key == 'q' || key == 'Q')
		exit(0);
	else if (key == 't' || key == 'T')
		t += 20;
	else if (key == 'b' || key == 'B')
		t -= 20;
	else if (key == 'i' || key == 'I')
		s += 20;
	else if (key == 'd' || key == 'D')
		s -= 20; 
	else if (key == 'f' || key == 'F')
	{ //press ‘f’ to toggle polygons drawn in between ‘line’ and ‘fill’ modes
		if (flag == false)
			flag = true;
		else
			flag = false;
	}
}

void mouse_input(int button, int state, int x, int y) { // mouse interaction
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON && step >= -15) {
	step -= 1; // decrement step
	}
	else if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON && step <= 15)
		step += 1; // increment step
}

void onMouseWheel(int button, int dir, int x, int y) {
	if (dir > 0) {
		// Scroll the mouse wheel up
		s-=20;
	}
	else if (dir < 0) {
		// Scroll down with the mouse wheel
		s+=20;
	}
}

void drawCircle(float x, float y, float radius) {
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);  // Center of the circle
	for (int i = 0; i <= 360; i++) {
		float angle = 2.0f * PI * float(i) / float(360);  // Convert degrees to radians
		float dx = radius * cosf(angle);
		float dy = radius * sinf(angle);
		glVertex2f(x + dx, y + dy);
	}
	glEnd();
}

void drawCloud(float x, float y) {
	// Draw individual circles to form a cloud shape
	glColor3f(1.0, 1.0, 1.0);  // Set color to white
	drawCircle(x, y, 30);
	drawCircle(x + 20, y + 20, 30);
	drawCircle(x + 40, y, 30);
	drawCircle(x + 60, y + 20, 30);
}

void drawTree() {
	glScalef(1.3, 1.3, 0);
	//stump
	glColor3f(0.7,0.52,0.38);
	glBegin(GL_POLYGON);
	glVertex2f(5,0);
	glVertex2f(25, 0);
	glVertex2f(25, 60);
	glVertex2f(5, 60);
	glEnd();

	//canopy
	glColor3f(0.23, 0.7, 0.44);
	glBegin(GL_TRIANGLES);
	glVertex2f(-45, 60);
	glVertex2f(75, 60);
	glVertex2f(15, 60 + 40);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2f(-25, 100 -15);
	glVertex2f(55, 100- 15);
	glVertex2f(15, 100 -15 + 20);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2f(-5, 105 - 5);
	glVertex2f(35, 105 - 5);
	glVertex2f(15, 105 -5 + 10);
	glEnd();
}

void drawFlowerLeaf() {
	glColor3f(0.15, 0.5, 0.13); 
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i += 30) {
		float theta = i * 3.1415926 / 180;
		float x = 7 * cos(theta);
		float y = 14 * sin(theta);
		glVertex2f(x, y);
	}
	glEnd();
}

void drawFlower(float x, float y ,float radius ) {
	if (flag == false)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//Draw stem
	glPushMatrix();
	glColor3f(0.5, 0.7, 0.1);
	glBegin(GL_POLYGON);
	glVertex2f(x + 3, y - 8);
	glVertex2f(x - 2, y - 8);
	glVertex2f(x + 2, y - 28);
	glVertex2f(x + 5, y - 48);
	glVertex2f(x + 2, y - 68);
	glVertex2f(x - 2, y - 88);

	glVertex2f(x + 3, y - 88);
	glVertex2f(x + 7, y - 68);
	glVertex2f(x + 9, y - 48);
	glVertex2f(x + 7, y - 28);
	
	glEnd();

	glPushMatrix();
	glTranslatef(x+19, y-40, 0); // Move the leaf onto the stem
	glRotatef(-50, 0, 0, 1);
	drawFlowerLeaf(); // Draw the leaf
	glPopMatrix();


	//Draw petal
	glPushMatrix();
	glColor3f(0.92, 0.45, 0.4);
		// Draw several circles to form the petal
		int numCircles = 5;
		for (int i = 0; i < numCircles; i++) {
			float angle = i * (360.0 / numCircles);
			float xOffset = radius * cos(angle * 3.14159265 / 180);
			float yOffset = radius * sin(angle * 3.14159265 / 180);

			glBegin(GL_POLYGON);
			for (int j = 0; j < 360; j += 10) {
				float theta = j * 3.14159265 / 180;
				glVertex2f(x + xOffset + radius * cos(theta), y + yOffset + radius * sin(theta));
			}
			glEnd();
		}


	//Draw stamen
	glColor3f(1, 1, 0);
	drawCircle(x, y,8);
	glPopMatrix();


	glPopMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

void drawSun(){
	//Draw a sun
	glPushMatrix();
	glColor3f(1, 1, 0.3);
	drawCircle(700, 500, 40);
	glPopMatrix();
	//Draw the light of sun
	glPushMatrix();
	glTranslatef(700, 500, 0);
	glRotatef(r, 0, 0, 1);
	glColor3f(1, 0.5, 0.1);
	glBegin(GL_TRIANGLES);
	glVertex2f(40, 15);
	glVertex2f(40, -15);
	glVertex2f(60, 0);
	glEnd();
	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(40, 15);
	glVertex2f(40, -15);
	glVertex2f(60, 0);
	glEnd();
	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(40, 15);
	glVertex2f(40, -15);
	glVertex2f(60, 0);
	glEnd();	
	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(40, 15);
	glVertex2f(40, -15);
	glVertex2f(60, 0);
	glEnd();	
	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(40, 15);
	glVertex2f(40, -15);
	glVertex2f(60, 0);
	glEnd();	
	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(40, 15);
	glVertex2f(40, -15);
	glVertex2f(60, 0);
	glEnd();	
	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(40, 15);
	glVertex2f(40, -15);
	glVertex2f(60, 0);
	glEnd();	
	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(40, 15);
	glVertex2f(40, -15);
	glVertex2f(60, 0);
	glEnd();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//Draw a smile face
	glPushMatrix();
	//Draw the black eyes of the face
	glColor3f(0, 0, 0);
	drawCircle(685, 506, 3);
	glColor3f(0, 0, 0);
	drawCircle(715, 506, 3);
	// Draw the smiling mouth of the face
	glColor3f(1, 0.5, 0);
	glBegin(GL_LINE_LOOP);
	for (int i = 180; i <= 360; i++) {
		float angle = i * 3.1415926 / 180.0;  // 角度转弧度
		float x = 700 + 15 * cos(angle);    // 半径为200的半圆，圆心在(400, 300)
		float y = 495 + 15 * sin(angle);
		glVertex2f(x, y);
	}
	glEnd();

	glPopMatrix();
}

void drawBalloon(float x, float y) {
	//Draw ballon
	glColor3f(1, 0, 0); 
	glLineWidth(3); // 
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		float theta = i * 3.1415 / 180.0;
		float x1 = 24.0 * cos(theta); 
		float y1 = 32.0 * sin(theta); 
		glVertex2f(x + x1, y + y1);
	}
	glEnd();

	// Draw sin curve as the line of a balloon
	glPushMatrix();
	glTranslatef(x, y-97, 0);
	glScalef(0.25, 0.18, 0);
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(1);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 361; i = i + 1)
	{
		float x2 = (float)i;
		float y2 = 40.0 * sin(i * (6.284 / 360.0));
		glVertex2f(y2, x2);
	}
	glEnd();
	glPopMatrix();
	glLineWidth(2.0); // Restore default lineweight

}

void drawString(float x, float y, std::string text) {
	glColor3f(0,0,0);
	glRasterPos2f(x, y);
	for (char c : text) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
}

void drawShinePoints() {
	// Set the size of the point
	glPointSize(5.0f);

	// Draw flashing dots
	glBegin(GL_POINTS);
	for (const auto& point : points) {
		glColor3f(point.r, point.g, point.b);
		glVertex2f(point.x, point.y);
	}
	glEnd();
}

void stripe1() {
	// Get the vertex coordinates of the polygon
	GLfloat polygonVertices[][2] = {
	{0, 0},
	{250, 0},
	{250, 40},
	{80, 100},
	{0, 100}
	};

	// Calculate the number and spacing of horizontal lines
	int numLines = 10; // The number of horizontal lines
	float lineHeight = 100 / (float)numLines; // Spacing of horizontal lines
	// Allneigntoooa/ Draw horizontal lines inside the polygon
	glColor3f(0.9, 0.8, 0.79);
	glBegin(GL_LINES);
	for (int i = 1; i < numLines; ++i) {
		GLfloat y = i * lineHeight; // The y coordinate of the line
		glVertex2f(0, y); // / Starting point of the line (left)

		// Calculate the x-coordinate of the end point of the line, making sure it is inside the polygon
		GLfloat maxX = 250;
		for (int j = 0; j < 4; ++j) {
			GLfloat xIntersect = (y - polygonVertices[j][1]) * (polygonVertices[j + 1][0] - polygonVertices[j][0]) /
				(polygonVertices[j + 1][1] - polygonVertices[j][1]) + polygonVertices[j][0];
			if (xIntersect < maxX) {
				maxX = xIntersect;
			}
		}
		glVertex2f(maxX, y); // / End of the line (right)
	}
	glEnd();
}
void stripe2() {
	GLfloat polygonVertices[][2] = {
	{0, 150},
	{100, 150},
	{100, 250},
	{0, 250}
	};

	int numLines = 10;
	float lineHeight = 100 / (float)numLines; 

	glColor3f(0.9, 0.8, 0.79);
	glBegin(GL_LINES);
	for (int i = 1; i < numLines; ++i) {
		GLfloat y = 150 + i * lineHeight; 
		glVertex2f(0, y); 

		GLfloat maxX = 100;
		for (int j = 0; j < 3; ++j) {
			GLfloat xIntersect = (y - polygonVertices[j][1]) * (polygonVertices[j + 1][0] - polygonVertices[j][0]) /
				(polygonVertices[j + 1][1] - polygonVertices[j][1]) + polygonVertices[j][0];
			if (xIntersect < maxX) {
				maxX = xIntersect;
			}
		}
		glVertex2f(maxX, y); 
	}
	glEnd();
}
void stripe3() {
	// Get the vertex coordinates of the polygon
	float vertices[][2] = { {150, 250}, {250, 250}, {250, 100}, {170, 85} };

	// Scan line algorithm
	glColor3f(0.9, 0.8, 0.79);
	glBegin(GL_LINES);
	float minY = 85;  // 多边形最低点的y坐标
	float maxY = 250;  // 多边形最高点的y坐标
	float lineHeight = 10;  // 设置水平直线的高度
	for (float y = minY; y <= maxY; y += lineHeight) {
		for (int i = 0; i < 4; ++i) {
			float x1 = vertices[i][0];
			float y1 = vertices[i][1];
			float x2 = vertices[(i + 1) % 4][0];
			float y2 = vertices[(i + 1) % 4][1];
			if ((y >= y1 && y <= y2) || (y >= y2 && y <= y1)) {
				float intersectionX = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
				glVertex2f(intersectionX, y);
			}
		}
	}
	glEnd();
}

void drawCentralBuilding() {
	glPushMatrix();
	//Background
	glColor3f(0.89, 0.88, 0.89);
	glBegin(GL_POLYGON);
	glVertex2f(30, 0);
	glVertex2f(230, 0);
	glVertex2f(230, 250);
	glVertex2f(30, 250);
	glEnd;

	glColor3f(0.71, 0.9, 0.95);
	glBegin(GL_POLYGON);  //Window below
	glVertex2f(10, 0);
	glVertex2f(235, 0);
	glVertex2f(235, 100);
	glVertex2f(10, 100);
	glEnd();

	glColor3f(0.35, 0.35, 0.35);
	glBegin(GL_LINE_LOOP); //Outline of right area
	glVertex2f(0, 0);
	glVertex2f(235, 0);
	glVertex2f(235, 100);
	glVertex2f(0, 100);
	glEnd();

	glColor3f(0.72, 0.28, 0.28);
	glBegin(GL_POLYGON); //Wall below
	glVertex2f(0, 0);
	glVertex2f(250, 0);
	glVertex2f(250, 40);
	glVertex2f(80, 100);
	glVertex2f(0, 100);
	glEnd();
	

	glColor3f(0.35, 0.35, 0.35);
	glBegin(GL_LINE_LOOP); //Wall below outline
	glVertex2f(0, 0);
	glVertex2f(250, 0);
	glVertex2f(250, 40);
	glVertex2f(80, 100);
	glVertex2f(0, 100);
	glEnd();

	glColor3f(0.71, 0.9, 0.95);
	glBegin(GL_POLYGON); //Left window
	glVertex2f(7, 150);
	glVertex2f(65, 150);
	glVertex2f(65, 135);
	glVertex2f(7, 135);
	glEnd();
	glColor3f(0.35, 0.35, 0.35);
	glBegin(GL_LINE_LOOP); //Left window outline
	glVertex2f(7, 150);
	glVertex2f(65, 150);
	glVertex2f(65, 135);
	glVertex2f(7, 135);
	glEnd();

	glColor3f(0.65, 0.7, 0.7);
	glBegin(GL_POLYGON);//First pillar
	glVertex2f(15, 100);
	glVertex2f(30, 100);
	glVertex2f(30, 135);
	glVertex2f(15, 135);
	glEnd();
	glBegin(GL_POLYGON); //Second pillar
	glVertex2f(42, 100);
	glVertex2f(57, 100);
	glVertex2f(57, 135);
	glVertex2f(42, 135);
	glEnd();
	glBegin(GL_POLYGON); //Third pillar
	glVertex2f(95, 150);
	glVertex2f(80, 150);
	glVertex2f(80, 100);
	glVertex2f(95, 100);
	glEnd();

	glColor3f(0.35, 0.35, 0.35);
	glBegin(GL_LINE_LOOP); //First pillar outline
	glVertex2f(15, 100);
	glVertex2f(30, 100);
	glVertex2f(30, 135);
	glVertex2f(15, 135);
	glEnd();
	glColor3f(0.35, 0.35, 0.35);
	glBegin(GL_LINE_LOOP); //Second pillar outline
	glVertex2f(42, 100);
	glVertex2f(57, 100);
	glVertex2f(57, 135);
	glVertex2f(42, 135);
	glEnd();
	glColor3f(0.35, 0.35, 0.35);
	glBegin(GL_LINE_LOOP); //Third pillar outline
	glVertex2f(95, 150);
	glVertex2f(80, 150);
	glVertex2f(80, 100);
	glVertex2f(95, 100);
	glEnd();


	glColor3f(0.72, 0.28, 0.28);
	glBegin(GL_POLYGON); //Top-left wall
	glVertex2f(0, 150);
	glVertex2f(100, 150);
	glVertex2f(100, 250);
	glVertex2f(0, 250);
	glEnd();

	glColor3f(0.35, 0.35, 0.35);
	glBegin(GL_LINE_LOOP); //Top-left wall outline
	glVertex2f(0, 150);
	glVertex2f(100, 150);
	glVertex2f(100, 250);
	glVertex2f(0, 250);
	glEnd();

	glColor3f(0.71, 0.9, 0.95);
	glBegin(GL_POLYGON);  //Window above
	glVertex2f(100, 250);
	glVertex2f(150, 250);
	glVertex2f(162, 180);
	glVertex2f(100, 180);
	glEnd();
	glColor3f(0.35, 0.35, 0.35);
	glBegin(GL_LINE_LOOP);  //Window above outline
	glVertex2f(100, 250);
	glVertex2f(150, 250);
	glVertex2f(162, 180);
	glVertex2f(100, 180);
	glEnd();

	glColor3f(0.72, 0.28, 0.28);
	glBegin(GL_POLYGON); //Top-right wall
	glVertex2f(150, 250);
	glVertex2f(250, 250);
	glVertex2f(250, 100);
	glVertex2f(170, 85);
	glEnd();
	glColor3f(0.35, 0.35, 0.35);
	glBegin(GL_LINE_LOOP);  //Top--right wall outline
	glVertex2f(150, 250);
	glVertex2f(250, 250);
	glVertex2f(250, 100);
	glVertex2f(170, 85);
	glEnd();

	//Draw stripes on the three walls
	glPushMatrix();
	stripe1(); //Wall below
	stripe2(); //Top-left wall 
	stripe3(); //Top-right wall
	glPopMatrix();
	
	
	glPopMatrix();
}

void display() {
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	// Set the projection matrix
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);

	//Draw background
	//Draw sky
	glBegin(GL_QUADS);
	glColor3f(0.7, 0.88, 0.96);
	glVertex2f(0,380); // Bottom-left vertex
	glVertex2f(800, 380);  // Bottom-right vertex
	glColor3f(0.3, 0.57, 0.96);
	glVertex2f(800, 800);   // Top-right vertex
	glVertex2f(0,800 );  // Top-left vertex
	glEnd();
	//Draw land
	glBegin(GL_QUADS);
	glColor3f(0.73, 0.9, 0.9);
	glVertex2f(0,0);
	glVertex2f(800, 0);
	glColor3f(0.99, 0.89, 0.8);
	glVertex2f(800, 380);
	glVertex2f(0, 380);
	glEnd();

	//Draw sun
	drawSun();

	//Draw Central Building
	glPushMatrix();
	glTranslatef(100, 150, 0);
	drawCentralBuilding();
	glPopMatrix();

    // Draw multiple clouds at different positions
	glPushMatrix(); 
	glRotated(-5, 0, 0, 1);// 绕Z轴旋转-5度
	glScalef(0.9, 0.9, 0);
	drawCloud(20, 500);      // 绘制第一朵云
	glPushMatrix();
	drawCloud(200, 600);     // 绘制第二朵云
	glPushMatrix();
	drawCloud(450, 550);     // 绘制第三朵云
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//Draw flowers
	glPushMatrix();
	glScalef(0.75, 0.75, 0);
	drawFlower(200, 140, 14);
	drawFlower(350, 200, 14);
	drawFlower(500, 110, 14);
	drawFlower(700, 170, 14);
	drawFlower(880, 210, 14);
	glPopMatrix();

	//draw trees
	glPushMatrix();
	glTranslatef(50, 60, 0);
	drawTree();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(700, 30, 0);
	drawTree();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(420, 100, 0);
	drawTree();
	glPopMatrix();
	
	//draw balloons
	glPushMatrix();
	glTranslatef(t, s, 0);
	//glScalef(0.8, 0.8, 0);
	drawBalloon(500, 500);
	drawBalloon(280, 450);
	drawBalloon(80, 400);
	drawBalloon(730, 400);
	glPopMatrix();

	//draw string
	glPushMatrix();
	drawShinePoints();
	glPopMatrix();

	glPushMatrix();
	drawString(400, 330, "Cordially invites you to the");
	drawString(500, 330 - 30, "XJTLU  ");
	drawString(400, 330 - 60, "Graduation CeremonyClass of 2024!");
	glPopMatrix();


	glutSwapBuffers();

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("A Invitation Card to 2024 XJTLU Graduation Ceremony");

	flag = false;

	glutDisplayFunc(display);
	glutIdleFunc(when_in_mainloop);
	glutTimerFunc(time_interval, OnTimer, 1);
	glutTimerFunc(1000 / 30, timer, 0); // 30 frames per second, or 30 flashes per second
	glutMouseFunc(mouse_input); // mouse callback function
	glutKeyboardFunc(keyboard_input);
	glutMouseWheelFunc(onMouseWheel);
	glutMainLoop();
	return 0;
}