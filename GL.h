#pragma once

#pragma warning (disable:4505) //warning produced by GLUT API
#include <GL/glut.h>
#pragma warning (default:4505)

void GLInit(int argc, char** argv);
int GLCreateWindow(const char* name, unsigned width, unsigned height, unsigned x_offset = 0, unsigned y_offset = 0);
int GLWindowHeight();
int GLWindowWidth();
void GLResizeWindow(unsigned width, unsigned height);

void GLMainLoop();

void GLResetTimer(int millisec, void (*callback)(int value));

void GLRedraw();
void GLRegisterDisplayCallback(void (*callback)());
void GLRegisterKeyboardCallback(void (*ascii)(unsigned char key, int, int), void (*special)(int key, int, int));
void GLRegisterResizeCallback(void (*callback)(int width, int height));

void GLClearBuffer();
void GLSwapBuffers();

void GLDrawBorder(unsigned x0, unsigned y0, unsigned width, unsigned height);
void GLDrawGrid(unsigned x0, unsigned y0, unsigned vLines, unsigned hLines, unsigned step);
void GLDrawRect(int x1, int y1, int x2, int y2, unsigned color);
void GLDrawText( unsigned x, unsigned y, void* font, const char* text);

