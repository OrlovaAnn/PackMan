#include "GL.h"

void GLInit(int argc, char** argv)
{
    glutInit(&argc, argv);
}

int GLCreateWindow(const char* name, unsigned width, unsigned height, unsigned x_offset, unsigned y_offset)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(x_offset, y_offset);
    glutInitWindowSize(width, height);

    int handler = glutCreateWindow(name);

    if (handler)
        glutSetWindow(handler);

    return handler; 
}

void GLMainLoop()
{
    glutMainLoop();
}

void GLResizeWindow(unsigned width, unsigned height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height);
}

int GLWindowHeight()
{

    return glutGet(GLUT_WINDOW_HEIGHT);
}

int GLWindowWidth()
{
    return glutGet(GLUT_WINDOW_WIDTH);
}

void GLResetTimer(int millisec, void (*callback)(int value))
{
    glutTimerFunc(millisec, callback, 0);
}

void GLRedraw()
{
    glutPostRedisplay();
}

void GLRegisterDisplayCallback(void (*callback)())
{
    glutDisplayFunc(callback);
}

void GLRegisterKeyboardCallback(void (*ascii)(unsigned char key, int, int), void (*special)(int key, int, int))
{
    glutKeyboardFunc(ascii);
    glutSpecialFunc(special);
}

void GLRegisterResizeCallback(void (*callback)(int width, int height))
{
    glutReshapeFunc(callback);
}

void GLClearBuffer()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLSwapBuffers()
{
    glutSwapBuffers();
}

void GLDrawBorder(unsigned x0, unsigned y0, unsigned width, unsigned height)
{
    glColor3ub(255, 255, 255);

    glBegin(GL_LINE_STRIP);

    glVertex2f(x0, y0 + height);
    glVertex2f(x0, y0);
    glVertex2f(x0 + width, y0);
    glVertex2f(x0 + width, y0 + height);
    glVertex2f(x0, y0 + height);

    glEnd();
}

void GLDrawGrid(unsigned x0, unsigned y0, unsigned vLines, unsigned hLines, unsigned step)
{
    glColor3ub(50, 50, 50);

    glLineStipple(3, 0xAAAA);
    glBegin(GL_LINES);

    unsigned width  = x0 + vLines * step;
    unsigned height = y0 + hLines * step;

    for (unsigned x = x0; x <= width; x += step)
    {
        glVertex2f(x, y0);
        glVertex2f(x, height);
    }

    for (unsigned y = y0; y <= height; y += step)
    {
        glVertex2f(x0   , y);
        glVertex2f(width, y);    
    }

    glEnd();
}

void GLDrawRect(int x1, int y1, int x2, int y2, unsigned color)
{
    if (color)
    {
        unsigned char red   = (color & 0xff0000) >> 16;
        unsigned char green = (color & 0x00ff00) >> 8;
        unsigned char blue  = (color & 0x0000ff);

        glColor3ub(red, green, blue);
        glRecti(x1 + 1, y1 + 1, x2 - 1, y2 - 1);
        glColor3ub(255, 255, 255);
    }
}


void GLDrawText( unsigned x, unsigned y, void* font, const char* text)
{
    glRasterPos2i(x, y);
    while(*text)
        glutBitmapCharacter(font, *text++);
}