#include "GL.h"
#include "Game.h"
#include "Application.h"

const int WINDOW_OFFSET_X = 100;
const int WINDOW_OFFSET_Y = 100;

const unsigned FRAME_INTERVAL = 25; //msec. 40 fps
const unsigned SNAKE_INTERVAL = 150;

GamePtr Application::_game = GamePtr(new Game());

Application::Application(int argc, char** argv) 
{
    GLInit(argc, argv);
}

void Application::CreateWindow(const char* name, unsigned width, unsigned height)
{
    _windowHandler = GLCreateWindow(name, width, height, WINDOW_OFFSET_X, WINDOW_OFFSET_Y);
    
    if (_windowHandler)
        _BindHandlers();
    else
        printf("%s: Can't create window", __FUNCTION__);
}

void Application::_BindHandlers()
{
    GLRegisterDisplayCallback(_DisplayHandler);
    GLRegisterKeyboardCallback(_KeyboardHandlerASCII, _KeyboardSpecialHandler);
    GLRegisterResizeCallback(_ResizeHandler);
    
    GLResetTimer(FRAME_INTERVAL, _RedrawHandler);
	//GLResetTimer(SNAKE_INTERVAL, _RedrawSnake);
    GLResetTimer(_game->UpdateInterval(), _UpdateHandler);
}

#pragma warning(disable:4100) // actually useless formal parameter resulted by GLUT API

void Application::_UpdateHandler(int value)
{
    _game->Update();
    GLResetTimer(_game->UpdateInterval(), _UpdateHandler);
}

void Application::_RedrawHandler(int value)
{
	GLRedraw();
    _game->Update();
    GLResetTimer(FRAME_INTERVAL, _RedrawHandler);
}

void Application::_RedrawSnake(int value)
{
    GLRedraw();
	_game->Update();
    GLResetTimer(SNAKE_INTERVAL, _RedrawSnake);
}

#pragma warning(default:4100)

void Application::_KeyboardSpecialHandler(int key, int x, int y)
{
    _game->KeyPressed(key, 0, x, y);
}

void Application::_KeyboardHandlerASCII(unsigned char key, int x, int y)
{
    _game->KeyPressed(0, key, x, y);
}

void Application::_ResizeHandler(int width, int height)
{
    GLResizeWindow(width, height);
}

void Application::BeginLoop()
{
    if (_windowHandler)
        GLMainLoop();
}

void Application::_DisplayHandler()
{
    _game->Draw();
}

unsigned Application::Width()
{
    return GLWindowWidth(); 
}

unsigned Application::Height()
{
    return GLWindowHeight();
}