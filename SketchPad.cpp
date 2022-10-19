#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include <GL/glut.h>

#define NULL 0

void mouse(int, int, int, int);
void key(unsigned char, int, int);
void display(void);
void myReshape(GLsizei, GLsizei);
void myinit(void);
void box(int, int, int);
void right_menu(int);
void middle_menu(int);
void icons();
void click();
void pixel_menu(int);
void fill_menu(int);
int draw(int, int);
int color(int, int);


GLsizei wh = 500, ww = 800; //initial window width and window height

float size = 10.0, size1 = 10.0, e = 3.0, f = 3.0, g = 0.1, lwd = 1.0;

int draw_type = 1, pick, l = 0, snake = 0, h = 0, s = 0, fill = 0, count = 0, i, j;

int rx, ry, a = 6, k, ct, b = 7, c = 0, t, first = 0, m = 0, v = 1, d, n = 1, o = 2, p = 4, q = 5, hy;

GLfloat xm, ym, xmm, ymm, r = 25;

FILE* myimage[1024][1024];


static float xp[3], yp[3], theta; //array to store mouse cooordintes and rotation angle

GLfloat colors[][3] = { {0.0,0.0,0.0},{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},{0.0,1.0,1.0},
{1.0,0.0,0.7},{1.0,1.0,0.0},{1.0,1.0,1.0},{0.7,0.8,0.2},{1,0.8,0.5},{0.2,0.7,0.8},{0.7,0.2,0.8},
{0.6,0.5,0.5} };

void spray(int a, int b, int e)
{
	glDisable(GL_POINT_SMOOTH);
	glPointSize(g);
	glBegin(GL_POINTS);
	glVertex2f(a, b);
	glVertex2f(a + o, b + n);
	glVertex2f(a + n, b + o);
	glVertex2f(a + p, b + o);
	glVertex2f(a + o, b + p);
	glVertex2f(a + o, b + p);
	glVertex2f(a + o, b + o);
	glVertex2f(a + p, b + p);
	glVertex2f(a + q, b);


	glVertex2f(a - o, b - n);
	glVertex2f(a - n, b - o);
	glVertex2f(a - p, b - o);
	glVertex2f(a - o, b - p);
	glVertex2f(a - o, b - p);
	glVertex2f(a - o, b - o);
	glVertex2f(a - p, b - p);
	glVertex2f(a, b + q);

	glVertex2f(a + o, b - n);
	glVertex2f(a + n, b - o);
	glVertex2f(a + p, b - o);
	glVertex2f(a + o, b - p);
	glVertex2f(a + o, b - p);
	glVertex2f(a + o, b - o);
	glVertex2f(a + p, b - p);
	glVertex2f(a - q, b);

	glVertex2f(a - o, b + n);
	glVertex2f(a - n, b + o);
	glVertex2f(a - p, b + o);
	glVertex2f(a - o, b + p);
	glVertex2f(a - o, b + p);
	glVertex2f(a - o, b + o);
	glVertex2f(a - p, b + p);
	glVertex2f(a, b - q);

	if (snake == 1)
	{
		glVertex2f(a + q + 5, b + q + 5);
		glVertex2f(a + q + 5, b - (q + 5));
		glVertex2f(a - (q + 5), b - q + 5);
		glVertex2f(a - (q + 5), b + q + 5);
	}
	glEnd();
}

void colorwin(int x, int y)
{
	t = color(x, y);
	b = t;
	c = -1;

	glColor3fv(colors[b]);
	l = 1;//fill on ,'l' variable  is used to avoid collision with 'fill'
	box(0, wh / 10, 0.039 * ww);/*background color indicator*/

	glBegin(GL_POLYGON);//to change background color
	glVertex2f(ww, wh / 15.06);
	glVertex2f(0.0725 * ww, wh / 15.06);
	glVertex2f(0.0725 * ww, wh);
	glVertex2f(ww, wh);
	glEnd();

	glBegin(GL_LINES); /*redraw border*/
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(0.072 * ww, wh / 15);
	glVertex2f(ww, wh / 15);
	glVertex2f(0.072 * ww, wh / 15.022);
	glVertex2f(0.072 * ww, wh);
	glEnd();
}

void draw_pixel(GLint cx, GLint cy)
{
	glPointSize(3.0);
	glBegin(GL_POINTS);
	if (draw_type == 6 && cy > 0.07 * wh && cx > 0.0909 * ww)
		glVertex2i(cx, cy);
	else if (ct == 1)
	{
		glBegin(GL_POINTS);
		glVertex2f(cx, cy);
	}
	glEnd();
}

void plotpixels(GLint h, GLint k, GLint x, GLint y)
{
	draw_pixel(x + h, y + k);
	draw_pixel(-x + h, y + k);
	draw_pixel(x + h, -y + k);
	draw_pixel(-x + h, -y + k);
	draw_pixel(y + h, x + k);
	draw_pixel(-y + h, x + k);
	draw_pixel(y + h, -x + k);
	draw_pixel(-y + h, -x + k);
}

void Circle_draw(GLint h, GLint k, GLint r)
{
	GLint d = 1 - r, x = 0, y = r;
	while (y > x)					// midpoint formula to draw circle
	{
		plotpixels(h, k, x, y);
		if (d < 0)
			d += 2 * x + 3;			
		else
		{
			d += 2 * (x - y) + 5;
			--y;
		}
		++x;
	}
	plotpixels(h, k, x, y);
}

void myReshape(GLsizei w, GLsizei h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(0, 0, w, h);

	ww = w;
	wh = h;
}

void init()
{
	glClearColor(1, 1, 1, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, ww, 0, wh);
}

void rect(int a, int b, int c, int d)//to draw square/rectangle 
{
	glLineWidth(lwd);
	if (fill)
		glBegin(GL_POLYGON);
	else 
		glBegin(GL_LINE_LOOP);
	glVertex2f(xm, ym);
	glVertex2f(xmm, ym);
	glVertex2f(xmm, ymm);
	glVertex2f(xm, ymm);
	glEnd();
	glFlush();
}

void mov(int x, int y) /* motion function for pen,lines,rectangle,brush and eraser*/
{
	if (wh - y > 0.07 * wh && x > 0.0909 * ww)
	{
		if (draw_type == 2)/*rectangle*/
		{
			glEnable(GL_COLOR_LOGIC_OP);
			glLogicOp(GL_XOR);

			if (first == 1)
			{

				rect(xm, ym, xmm, ymm);
				glFlush();
			}
			xmm = x;
			ymm = wh - y;
			rect(xm, ym, xmm, ymm);
			glFlush();
			first = 1;
			m = 1;
		}
	}

	if (draw_type == 1)/*line*/
	{
		if (wh - y > 0.07 * wh && x > 0.0909 * ww)
		{
			glEnable(GL_COLOR_LOGIC_OP);
			glLogicOp(GL_XOR);
			if (first == 1)
			{
				glBegin(GL_LINES);
				glVertex2f(xm, ym);
				glVertex2f(xmm, ymm);
				glEnd();
				glFlush();
			}

			xmm = x;
			ymm = (wh - y);
			glBegin(GL_LINES);
			glVertex2f(xm, ym);
			glVertex2f(xmm, ymm);
			glEnd();
			glFlush();
			first = 1;
			m = 1;
		}
	}

	glDisable(GL_COLOR_LOGIC_OP);

	if (draw_type == 9)/*pencil*/
	{
		glFlush();
		y = wh - y;
		glColor3fv(colors[a]);
		if (y > 0.07 * wh && x > 0.0909 * ww)
		{
			glPointSize(e);
			glBegin(GL_POINTS);
			glVertex2f(x, y);

			glBegin(GL_LINES);
			glVertex2f(xp[0], yp[0]);
			glVertex2f(x, y);
			glEnd();

			glFlush();
			glEnd();
		}
	}
	else if (draw_type == 7 || draw_type == 10)//eraser or brush
	{
		glFlush();
		y = wh - y;

		if (draw_type == 7)//eraser
		{
			l = 1;
			glColor3fv(colors[b]);
			if (y > 0.07 * wh && x > 0.0909 * ww)
				box(x, y, size);
		}
		else //brush
		{
			l = 1;
			glColor3fv(colors[a]);
			if (y > 0.07 * wh && x > 0.0909 * ww)
				box(x, y, size1);
		}
		glEnd();
	}
	else if (draw_type == 11)//airbrush/spray
	{
		glFlush();
		y = wh - y;
		glColor3fv(colors[a]);
		if (y > 0.07 * wh && x > 0.0909 * ww)
		{
			spray(x, y, g);
			glFlush();
			glEnd();
		}
	}
}

int draw(int x, int y)
{
	x = wh - x;
	int j = h;

	if (x < wh - ww / 18)
		return 0;
	if (y > 0.87 * wh && y < 0.90 * wh)/*'change background color' with selected color*/
	{
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		return 8;

	}
	else if (y > 0.80 * wh)//copy
	{
		h = 14;
		return 14;
	}
	else if (y > 0.77 * wh)//paste
	{
		h = 13;
		return 13;
	}
	else if (y > 0.71 * wh)/*Air Brush*/
	{
		h = 11;
		click();
		glutSetCursor(GLUT_CURSOR_SPRAY);
		return 11;
	}
	else if (y > 0.62 * wh)/*Brush*/
	{
		h = 10;
		click();
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		return 10;
	}
	else if (y > 0.53 * wh)/*Pen*/
	{
		h = 9;
		click();
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		return 9;
	}
	else if (y > 0.47 * wh)/*Text*/
	{
		h = 5;
		click();
		glutSetCursor(GLUT_CURSOR_TEXT);
		return 5;
	}
	else if (y > 0.39 * wh)/*Triangle*/
	{
		h = 3;
		click();
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		return 3;
	}
	else if (y > 0.32 * wh)/*Square*/
	{
		h = 2;
		click();
		glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);
		return 2;
	}
	else if (y > 0.24 * wh)/*Circle*/
	{
		h = 6;
		click();
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		return 6;

	}
	else if (y > 0.17 * wh)/*points*/
	{
		h = 4;
		click();
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		return 4;
	}
	else if (y > 0.11 * wh)/*Line*/
	{
		h = 1;
		click();
		glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);
		return 1;
	}
	else return 1;
}

void lines(int a, int b, int c, int d)
{

	glLineWidth(lwd);
	glBegin(GL_LINES);
	glVertex2f(a, b);
	glVertex2f(c, d);
	glEnd();
}

void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{


		glPushAttrib(GL_ALL_ATTRIB_BITS);
		c = 0;
		glPopAttrib();
		glFlush();
	}

	if (c >= 0)
	{
		a = color(x, y);
		c++;
	}

	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		glColor3fv(colors[a]);
		l = 1;
		/*draw color indicator*/
		box(0, wh / 10.2, ww / 50);

		c--;
		if (c < 1)
			c = 0;
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		pick = draw(x, y);/*To slect draw mode*/

		glColor3fv(colors[a]);
		if (pick != 0)
		{
			count = 0;
			draw_type = pick;
		}

		else switch (draw_type)
		{

			c = 0; //counter to check if draw_type is not 8/'change background color'
		case 1:glDisable(GL_LINE_SMOOTH);

		case 2:
			glutMotionFunc(mov);//call motionn func for lines and retangle/square
			break;

		case (3):/*TRIANGLE*/
			switch (count)
			{
			case(0):
				count++;
				xp[0] = x;
				yp[0] = y;
				break;

			case(1):
				count++;
				xp[1] = x;
				yp[1] = y;
				glLineWidth(lwd);
				if (y > 0.07 * wh && x > 0.0909 * ww)
					glBegin(GL_LINES);//to display line/edge b/w first two points
				glVertex2f(xp[0], wh - yp[0]);
				glVertex2f(xp[1], wh - yp[1]);
				glEnd();
				break;

			case(2):
				if (fill) glBegin(GL_POLYGON);//to display triangle
				else
					glLineWidth(lwd);
				glBegin(GL_LINE_LOOP);
				if (y > 0.07 * wh && x > 0.0909 * ww)
					glVertex2f(xp[0], wh - yp[0]);
				glVertex2f(xp[1], wh - yp[1]);
				glVertex2f(x, wh - y);
				glEnd();
				count = 0;
			}
			break;

		case(4):/*Points*/
		{
			y = wh - y;
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			glPointSize(f);
			glBegin(GL_POINTS);
			if (y > 35 && x > 0.0909 * ww)
				glVertex2f(x, y);
			glEnd();
			count++;
		}
		break;

		case(5):/*TEXT*/
		{
			rx = x;
			//select loacation to type
			ry = wh - y;
			glRasterPos2i(rx, ry);
			count = 0;
		}
		break;

		case 6:/*Circle*/
			y = wh - y;
			switch (count)
			{
			case 0:count++;
				xp[0] = x;
				yp[0] = y;
				if (y > 0.07 * wh && x > 0.0909 * ww)
					glBegin(GL_POINTS);//to display centere of circle
				glVertex2f(x, y);
				glEnd();
				break;

			case 1: count++;
				xp[1] = x;
				yp[1] = y;
				count = 0;
				r = sqrt((x - xp[0]) * (x - xp[0]) + (y - yp[0]) * (y - yp[0]));//Apply Distance formula to calculate given radius
				Circle_draw(xp[0], yp[0], r);
				break;
			}
			break;

		case 7:/*Eraser*/
		{
			c = 0;
			mov(x, y);//call motion fum\nc for eraser

		}
		break;

		case 8:/*Screen/background color*/


		{    c++;//value of c is changed to indicate this draw_type else where
		colorwin(x, y);
		count++;
		}
		case 9:
		case 10:
			y = wh - y;
			xp[0] = x;
			yp[0] = y;
			glutMotionFunc(mov);//call motion func for pencil and brush
			break;

		case 11:
			y = wh - y;
			if (y > 0.07 * wh && x > 0.0909 * ww)
			{

				spray(x, y, e); //draw spray on click
				glutMotionFunc(mov);//call motion func for spray/airbrush
			}
			break;

		case(12)://line strips
			switch (count)
			{
			case(0):
				count++;
				if (s > 0)
				{
					if (y > 0.07 * wh && x > 0.0909 * ww)
					{
						glBegin(GL_LINE_STRIP);
						glVertex2f(xp[1], wh - yp[1]);
						glVertex2f(x, wh - y);
						glEnd();
					}
					else s = count = 0;;
				}
				xp[0] = x;
				yp[0] = y;
				break;
			case(1):
				count++;
				xp[1] = x;
				yp[1] = y;
				s++;
				if (y > 0.07 * wh && x > 0.0909 * ww)
				{
					glBegin(GL_LINE_STRIP);
					glVertex2f(xp[0], wh - yp[0]);
					glVertex2f(x, wh - y);
					glEnd();
				}
				s++;
				count = 0;
			}
			break;
		case 13://copy
			glReadPixels(0.075 * ww, (wh - y) / 0.8, ww, wh, GL_RGB, GL_UNSIGNED_BYTE, myimage);//copy when clicked within the drawing area.
			break;

		case 14://paste
			rx = x;
			ry = wh - y;
			if (!fill)
			{
				glRasterPos2i(rx / 0.89, ry / 0.89); //select position to paste
				glDrawPixels(ww, wh, GL_RGB, GL_UNSIGNED_BYTE, myimage);
				break;
			}
		}
	}
	/*for erasable line and erasable rectangle*/
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (draw_type == 1)//lines
		{
			xm = x;
			ym = wh - y;

			glEnable(GL_COLOR_LOGIC_OP);
			glLogicOp(GL_XOR);
			first = 0;
			m = 0;
		}
		if (draw_type == 2)//rectangle/square
		{

			xm = x;
			ym = wh - y;
			glPointSize(1.5);
			if (y > 0.07 * wh && x > 0.0909 * ww && a == 0)
				glBegin(GL_POINTS);
			glVertex2f(xm, ym);
			glEnd();
			glEnable(GL_COLOR_LOGIC_OP);
			glLogicOp(GL_XOR);
			first = 0;
			m = 0;
		}
	}
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		l = 0;
		icons();
		glColor3fv(colors[a]);

		if (draw_type == 1)//line
		{
			if (m == 1)
			{
				if (ymm > 0.07 * wh && xmm > 0.0909 * ww)
					glBegin(GL_LINES);

				glVertex2f(xm, ym);
				glVertex2f(xmm, ymm);
				glEnd();
				glFlush();

				glLogicOp(GL_COPY);

				xmm = x;
				ymm = wh - y;

				glLogicOp(GL_COPY);
				if (ymm > 0.07 * wh && xmm > 0.0909 * ww)
					lines(xm, ym, xmm, ymm);
				glFlush();
				m = 1;
			}
		}
		else if (draw_type == 2)//square
		{
			if (m == 1)
			{
				if (ymm > 35 && xm > 0.0909 * ww)
					rect(xm, ym, xmm, ymm);

				glLogicOp(GL_COPY);

				xmm = x;
				ymm = wh - y;

				glLogicOp(GL_COPY);
				if (ymm > 0.07 * wh && xmm > 0.0909 * ww)
					rect(xm, ym, xmm, ymm);
				m = 1;
			}
		}
	}
	glPopAttrib();
	glFlush();
}

/*to select draw mode*/
/*to draw box for icons*/
void box(int x, int y, int s)
{

	if (l == 0)                        // draw_square function to create icons/boxes
		glBegin(GL_LINE_LOOP);
	else if (l > 0)
		glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x + s, y);
	glVertex2i(x + s, y + s);
	glVertex2i(x, y + s);
	glEnd();
}

void right_menu(int id)
{
	if (id == 3) 
		display();
	if (id == 4)
	{
		if (snake != 1)
		{
			snake = 1;
			glutSetCursor(GLUT_CURSOR_SPRAY);
			draw_type = 11;
			h = 11;
		}
		else snake = 0;
	}

	if (id == 5)
	{
		h = 12;
		icons();
		draw_type = 12;
	}
	if (id == 6)
		exit(0);
}

/*to select draw color and other options*/
int color(int x, int y)
{
	y = wh - y;
	if (y < wh / 15)
	{
		count = 0;
		icons();
		glVertex3fv(colors[a]);
		if (x < 1.52 * ww / 10)
			return 1;  ///red
		else if (x < 2.02 * ww / 10)
			return 2;  //green
		else if (x < 2.52 * ww / 10)
			return 3;//blue
		else if (x < 3.02 * ww / 10)
			return 4;//cyan
		else if (x < 3.52 * ww / 10)
			return 5;//magenta
		else if (x < 4.02 * ww / 10)
			return 6;//yellow
		else if (x < 4.52 * ww / 10)
			return 0;//black
		else if (x < 5.02 * ww / 10)
			return 8;
		else if (x < 5.52 * ww / 10)
			return 9;
		else if (x < 6.02 * ww / 10)
			return 10;
		else if (x < 6.52 * ww / 10)
			return 12;
		else if (x < 7.02 * ww / 10)
			return 11;
		else if (x < 7.52 * ww / 10)
			return 7;//white  
		else if (x < 7.9 * ww / 10)//eraser
		{
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
			h = 7;
			icons();
			draw_type = 7;
		}
		else if (x < 8.4 * ww / 10)
		{
			if (draw_type == 7 && size < 50)//eraser
				size += 4;
			else if (draw_type == 10 && size1 < 30)//brush
				size1 += 1.5;
			else if (draw_type == 9) //pencil
			{
				if (e < 10)
					e += 1.0;
			}
			else if (draw_type == 4)//points
			{
				if (f < 4.8)
					f += 0.4;
			}
			else if (draw_type == 1 || draw_type == 2 || draw_type == 3)//lines
			{
				if (lwd < 6)
					lwd += 1;
			}
			else if (draw_type == 11)//spray/airbrush
			{
				if (g < 1.6)
					g += 0.3;
				if (n < 4)
					n++, o++, q++, p++;
			}
		}
		else if (x < 0.89 * ww)//decrease respective size
		{
			{
				if (draw_type == 7)//eraser
				{
					if (size > 2)
						size -= 3;
				}
				else if (draw_type == 10 && size1 > 6)//brush
					size1 -= 1.5;
				else if (draw_type == 4)//points
				{
					if (f > 2.5)
						f -= 0.4;
				}
				else if (draw_type == 1 || draw_type == 2 || draw_type == 3)//'lines/square/Triangle' line/edge thickness
				{
					if (lwd > 0)
						lwd -= 1;

				}
				else if (draw_type == 9) //pencil
				{
					if (e > 0.5)
						e -= 0.5;
				}
				else if (draw_type == 11)//spray/airbrush
				{
					if (g > 0.4)
						g -= 0.3;
					if (n > 0)
						n--, o--, p--, q--;
				}
			}
		}
		else if (x < 9.4 * ww / 10)
		{

			fill = 0;
			return a;
		}
		else	if (x < ww - 10)
		{
			fill = 1;
			return a;
		}
	}
	return a;
}

void fill_menu(int id)
{
	if (id == 1) 
		fill = 1;
	else 
		fill = 0;
}

void key(unsigned char k, int xx, int yy)
{
	if (draw_type != 5) return;
	glColor3fv(colors[a]);
	glRasterPos2i(rx, ry);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, k);
	rx += glutBitmapWidth(GLUT_BITMAP_9_BY_15, k);
}

void text()
{
	if (h == 0 || h == 5)
	{
		if (h == 0)
		{
			glColor3fv(colors[a]);
			glRasterPos2i(0, 0.49 * wh);
		}
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'T');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'E');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'X');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'T');
		glEnd();
	}

	if (h == 0 || h == 13)
	{

		if (h == 0)
		{
			glColor3f(0.0, 0.0, 0.0);
			glRasterPos2i(2, 0.207 * wh);
		}
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'C');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'O');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'P');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'Y');
	}

	if (h == 0 || h == 14)
	{
		if (h == 0)
			glRasterPos2i(2, 0.173 * wh);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'P');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'A');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'S');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'T');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'E');
	}
}

void icons()
{
	int shift = 0;
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(3);
	l = 1;
	glColor3f(0.7, 0.7, 0.7);//draw-mode indicator
	box(6, wh / 100, ww / 20);

	if (h == 0)//common display for all initially or after clearing
	{
		l = 1;
		glColor3fv(colors[b]);
		box(0, wh / 10.2, 0.039 * ww);
		l = 0;
		glColor3f(0.0, 0.0, 0.0);
		box(0, wh / 10.2, ww / 25);
		l = 1;
		glColor3fv(colors[a]);
		/*draw color indicator*/
		box(0, wh / 10, ww / 50);
	}
	/*icons for draw modes*/

	if (h == 0 || h == 13)//copy
	{
		if (h == 0)
		{
			glColor3f(0.97, 0.97, 0.97);
			glRectf(1, 0.195 * wh, 0.043 * ww, 0.219 * wh);
			text();
		}
		glColor3f(0.0, 0.0, 0.0);

		if (h != 0)
			glRasterPos2i(4, 0.029 * wh);
		text();
	}

	if (h == 0 || h == 14)//paste
	{
		if (h == 0)
		{
			glColor3f(0.91, 0.91, 0.91);
			glRectf(1, 0.168 * wh, 0.047 * ww, 0.192 * wh);
			text();
		}
		glColor3f(0.0, 0.0, 0.0);

		if (h != 0)
			glRasterPos2i(4, 0.029 * wh);
		text();
	}

	if (h == 0 || h == 11)//spray
	{
		glColor3f(0.45, 0.45, 0.53);/*Airbrush/spray*/
		box(0, 0.228 * wh, ww / 20);
		glColor3fv(colors[a]);
		/*for airbrush*/
		spray(8, 0.285 * wh, g);
		spray(9, 0.275 * wh, g);
	}

	if (h == 11)
	{
		spray(15, 0.034 * wh, g);
		spray(18, 0.04 * wh, g);
		glRasterPos2i(6, 0.050 * wh);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'S');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'P');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'R');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'A');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'Y');
	}

	if (h == 0 || h == 10)//brush
	{
		glColor3f(0.52, 0.57, 0.53);/*brush*/
		box(0, 0.305 * wh, ww / 20);
		/* display brush symbol*/

		glBegin(GL_POLYGON);
		glColor3fv(colors[a]);
		glVertex2f(0.0080 * ww, 0.348 * wh);
		glVertex2f(0.043 * ww, 0.348 * wh);
		glVertex2f(0.043 * ww, 0.369 * wh);
		glVertex2f(0.0080 * ww, 0.369 * wh);
		glEnd();

		glBegin(GL_LINES);
		glVertex2f(0.0228 * ww, 0.350 * wh);
		glVertex2f(0.0228 * ww, 0.323 * wh);
		glVertex2f(0.0228 * ww, 0.323 * wh);
		glVertex2f(0.0309 * ww, 0.323 * wh);
		glVertex2f(0.0309 * ww, 0.323 * wh);
		glVertex2f(0.0309 * ww, 0.350 * wh);
		glEnd();
	}

	if (h == 10)
	{
		glBegin(GL_POLYGON);
		glColor3fv(colors[a]);
		glVertex2f(0.019 * ww, wh / 18);
		glVertex2f(0.053 * ww, wh / 18);
		glVertex2f(0.053 * ww, 0.029 * wh);
		glVertex2f(0.019 * ww, 0.029 * wh);
		glEnd();

		glBegin(GL_LINES);
		glVertex2f(0.032 * ww, 0.032 * wh);
		glVertex2f(0.032 * ww, 0.015 * wh);
		glVertex2f(0.032 * ww, 0.015 * wh);
		glVertex2f(0.042 * ww, 0.015 * wh);
		glVertex2f(0.042 * ww, 0.015 * wh);
		glVertex2f(0.042 * ww, 0.032 * wh);
		glEnd();

		glRasterPos2i(6, 0.055 * wh);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'B');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'R');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'U');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'S');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'H');

	}//end brush draw

	if (h == 0 || h == 9)//pencil
	{
		glPointSize(e);
		glBegin(GL_POINTS);
		glVertex2f(0.043 * ww, 0.43 * wh);
		glEnd();
		glColor3f(0.93, 0.93, 0.93);/*pen*/
		box(0, 0.380 * wh, ww / 20);

		/*pen symbol draw*/

		glBegin(GL_LINES);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(0.010 * ww, 0.422 * wh);
		glVertex2f(0.044 * ww, 0.452 * wh);
		glVertex2f(0.018 * ww, 0.418 * wh);
		glVertex2f(0.048 * ww, 0.448 * wh);
		glEnd();

		glColor3fv(colors[a]);
		glBegin(GL_TRIANGLES);
		glVertex2f(0.010 * ww, 0.422 * wh);
		glVertex2f(0.018 * ww, 0.418 * wh);
		glVertex2f(0.002 * ww, 0.410 * wh);
		glEnd();

		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_POINTS);
		glPointSize(e + 5.0);
		glVertex2f(0.044 * ww, 0.448 * wh);
		glEnd();
		glFlush();
	}

	if (h != 0 && h == 9)
	{
		glBegin(GL_LINES);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(0.019 * ww, 0.029 * wh);
		glVertex2f(0.053 * ww, 0.059 * wh);
		glVertex2f(0.025 * ww, 0.025 * wh);
		glVertex2f(0.057 * ww, 0.055 * wh);
		glEnd();
		glColor3fv(colors[a]);
		glBegin(GL_TRIANGLES);
		glVertex2f(0.019 * ww, 0.029 * wh);
		glVertex2f(0.025 * ww, 0.025 * wh);
		glVertex2f(0.0135 * ww, 0.018 * wh);
		glEnd();
		glColor3f(0.0, 0.0, 0.0);
		ct = 1;
		Circle_draw(0.052 * ww, 0.054 * wh, 1.85);
		glEnd();
		ct = 2;
	}
	//end pen symbol draw

	if (h == 0 || h == 5)//text
	{
		glColor3f(0.4, 0.4, 0.4);
		box(0, 0.457 * wh, ww / 19.5);
		glColor3fv(colors[a]);
		glRasterPos2i(0, 0.49 * wh);
		text();
		if (h != 0)
		{

			glRasterPos2i(5, 0.049 * wh);
			text();
		}
		glFlush();

	}

	if (h == 0 || h == 3)//triangle
	{
		glColor3f(0.1, 0.4, 0.7);/*triangle*/
		box(0, 0.531 * wh, ww / 20);


		/*triangle icon*/
		glColor3fv(colors[a]);
		glBegin(GL_TRIANGLES);
		glVertex2i(0.008 * ww, 0.547 * wh);
		glVertex2i(0.043 * ww, 0.547 * wh);
		glVertex2i(0.023 * ww, 0.589 * wh);
		glEnd();
		if (h != 0)
		{
			if (fill)
				glBegin(GL_TRIANGLES);
			else glBegin(GL_LINE_LOOP);
			glVertex2i(0.019 * ww, 0.018 * wh);
			glVertex2i(0.051 * ww, 0.018 * wh);
			glVertex2i(0.035 * ww, 0.058 * wh);
			glEnd();
		}
		glFlush();
	}
	if (h == 0 || h == 2)//square
	{
		glColor3f(0.4, 0.5, 0.3);/*rectangle*/
		box(0, 0.605 * wh, ww / 20);
		/*rectangle icon*/
		glColor3fv(colors[a]);
		l = 1;
		box(5, 0.614 * wh, ww / 29);
		if (h != 0)
		{
			if (fill) l = 1;
			else l = 0;
			box(ww / 70, 0.0215 * wh, ww / 29);
		}
	}
	if (h == 0 || h == 4)
	{
		glColor3f(0.6, 0.5, 0.5);/*points*/
		box(0, 0.75 * wh, ww / 20);

		/*point icon*/
		if (a != 12)
			glColor3fv(colors[a]);
		else glColor3fv(colors[0]);
		glPointSize(5.0);
		glBegin(GL_POINTS);
		glVertex2i(0.020 * ww, 0.78 * wh);
		if (h != 0)
			glVertex2f(0.034 * ww, 0.0432 * wh);
		glEnd();
	}

	if (h == 0 || h == 1)//lines
	{
		glColor3f(0.0, 0.0, 0.0);/*lines*/
		box(0, 0.826 * wh, ww / 20);/* line icon*/
		if (a == 0)
			d = 7;
		else d = a;
		glColor3fv(colors[d]);
		glBegin(GL_LINES);
		glVertex2i(0.008 * ww, 0.865 * wh);
		glVertex2i(0.044 * ww, 0.865 * wh);
		if (h != 0)
		{
			glColor3fv(colors[a]);
			glVertex2f(0.015 * ww, 0.044 * wh);
			glVertex2f(0.052 * ww, 0.044 * wh);
		}
		glEnd();
	}

	if (h == 0 || h == 6)//circle
	{
		ct = 1;
		/*Cirlce icon box*/
		glColor3f(0.44, 0.44, 0.44);
		box(0, 0.68 * wh, ww / 21);

		/*circle icon*/
		glColor3fv(colors[a]);
		Circle_draw(ww / 40.95, 0.716 * wh, ww / 58);
		if (h != 0)
			Circle_draw(0.032 * ww, 0.038 * wh, ww / 58);
		ct = 0;
	}

	if (h == 12)//line_strip
	{
		glColor3fv(colors[a]);
		glVertex2f(0.015 * ww, 0.044 * wh);
		glVertex2f(0.052 * ww, 0.044 * wh);
	}

	if (h == 0 || h == 7)//eraser
	{
		/* text for eraser */
		glColor3f(0.0, 0.0, 0.0);
		glRasterPos2i(.759 * ww, 0.0335 * wh);
		glColor3f(0.94, 0.94, 0.94);
		glRectf(0.75 * ww, 0.0295 * wh, 0.78 * ww, 0.0085 * wh);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'E');
		/*eraser symbol*/

		if (h != 0)
		{
			glRasterPos2i(.0110 * ww, 0.038 * wh);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 69);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 82);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 65);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 83);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 69);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 82);
		}	//end eraser
	}

	if (h == 0)
	{
		/*icons for colors and other options*/
		glColor3f(0.0, 0.0, 0.0);//black
		box(4.02 * ww / 10, 0.0075 * wh, ww / 28);
		glColor3f(1.0, 0.0, 0.0);//red
		box(1.02 * ww / 10, 0.0075 * wh, ww / 28);
		glColor3f(0.0, 1.0, 0.0);//green
		box(1.52 * ww / 10, 0.0075 * wh, ww / 28);
		glColor3f(0.0, 0.0, 1.0);//blue
		box(2.02 * ww / 10, 0.0075 * wh, ww / 28);
		glColor3f(0.0, 1.0, 1.0);//cyan
		box(2.52 * ww / 10, 0.0075 * wh, ww / 28);
		glColor3f(1.0, 0.0, 0.7);//magenta
		box(3.02 * ww / 10, 0.0075 * wh, ww / 28);
		glColor3f(1.0, 1.0, 0.0);//yellow
		box(3.52 * ww / 10, 0.0075 * wh, ww / 28);
		glColor3f(0.97, 0.97, 0.97);//white
		box(7.02 * ww / 10, 0.0075 * wh, ww / 28);

		glColor3f(0.7, 0.8, 0.2);
		box(4.52 * ww / 10, 0.0075 * wh, ww / 28);
		glColor3f(0.2, 0.7, 0.8);
		box(5.52 * ww / 10, 0.0075 * wh, ww / 28);
		glColor3f(1, 0.8, 0.5);//orange
		box(5.02 * ww / 10, 0.0075 * wh, ww / 28);
		glColor3f(0.7, 0.2, 0.8);
		box(6.52 * ww / 10, 0.0075 * wh, ww / 28);
		glColor3f(0.6, 0.5, 0.5);//brown
		box(6.02 * ww / 10, 0.0075 * wh, ww / 28);

		l = 0;

		/*object for fill icon*/
		glColor3fv(colors[a]);
		glBegin(GL_TRIANGLES);
		glVertex2f(0.955 * ww, 0.01 * wh);
		glVertex2f(0.985 * ww, 0.01 * wh);
		glVertex2f(0.970 * ww, 0.04 * wh);
		glEnd();

		/*object for no fill icon*/
		glColor3fv(colors[a]);
		glBegin(GL_LINE_LOOP);
		glVertex2f(0.904 * ww, 0.01 * wh);
		glVertex2f(0.934 * ww, 0.01 * wh);
		glVertex2f(0.919 * ww, 0.04 * wh);
		glEnd();

		glColor3f(0.5, 0.8, 0.8);
		box(0.80 * ww, 0.0075 * wh, ww / 25);
		glColor3f(0.6, 0.7, 0.6);
		box(0.85 * ww, 0.0075 * wh, ww / 25);

		/*text for draw modes*/

	/*text for increase and decrease icon*/
		glColor3f(0.0, 0.0, 0.0);
		glRasterPos2i(8.07 * ww / 10, 0.0075 * wh + 7);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, '+');

		glRasterPos2i(8.52 * ww / 10 + 6, 0.02 * wh);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, '-');

		/* boundary for color icons*/
		glBegin(GL_LINE_LOOP);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(0.099 * ww, 0);
		glVertex2f(0.099 * ww, wh / 16.79);
		glVertex2f(0.747 * ww, wh / 16.79);
		glVertex2f(0.747 * ww, 0);
		glEnd();
		// end color icon boundary
		/*Drawing boundaries*/
		glBegin(GL_LINES);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(0.072 * ww, wh / 15);
		glVertex2f(ww, wh / 15);
		glVertex2f(0.072 * ww, wh / 15 - 0.011);
		glVertex2f(0.072 * ww, wh);
		glEnd();//end boundary draw
	}
	glDisable(GL_LINE_SMOOTH);
	glLineWidth(lwd);
}

void click()//to animate/depress when an icon/draw_object is clicked 
{
	if (l == 1)//fill=1 to show click effect of an icon
	{
		glColor3f(0.0, 0.0, 0.0);
		if (h == 1)
			box(0, 0.826 * wh, ww / 20);
		else  if (h == 2)
			box(0, 0.605 * wh, ww / 20);
		else if (h == 3)
			box(0, 0.534 * wh, ww / 20.8);
		else if (h == 4)
			box(0, 0.75 * wh, ww / 20);
		else if (h == 5)
			box(0, 0.457 * wh, ww / 19.5);
		else if (h == 6)
			box(0, 0.68 * wh, ww / 21);
		else if (h == 9)
			box(0, 0.380 * wh, ww / 20);
		else if (h == 10)
			box(0, 0.305 * wh, ww / 20.8);
		else if (draw_type == 11)
			box(0, 0.228 * wh, ww / 20);
	}
}

void display(void)
{

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	
	b = 7;
	h = 0;

	glBegin(GL_QUADS);
	glColor3f(0.6, 0.6, 0.6);
	glVertex2f(0.0684 * ww, 0);
	glVertex2f(0, 0);
	glVertex2f(0, wh);
	glVertex2f(0.0684 * ww, wh);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.7, 0.7, 0.7);
	glVertex2f(0.0688 * ww, 0);
	glVertex2f(ww, 0);
	glVertex2f(ww, wh / 16);
	glVertex2f(0.0688 * ww, wh / 16);
	glEnd();

	icons();

	glColor3f(0.0, 0.0, 0.0);
	glFlush();
}

void main(int argc, char** argv)
{
	int  f_menu;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(ww, wh);

	glutCreateWindow("Sketch Pad");

	glutInitWindowPosition(800, 500);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	f_menu = glutCreateMenu(fill_menu);
	glutAddMenuEntry("fill on", 1);
	glutAddMenuEntry("fill off", 2);
	glutCreateMenu(right_menu);
	glutAddSubMenu("Fill", f_menu);
	glutAddMenuEntry("Clear", 3);
	glutAddMenuEntry("Snake", 4);
	glutAddMenuEntry("Line-loop", 5);
	glutAddMenuEntry("Quit", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	init();
	glutReshapeFunc(myReshape);
	glutKeyboardFunc(key);
	glutMainLoop();
}
