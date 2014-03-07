#pragma once

#include "Game.h"
#include <memory>

class Application
{
public:
    Application(int argc, char** argv); 

public:
    void CreateWindow(const char* name, unsigned width, unsigned height);

public:
    void BeginLoop();

public:
    unsigned Width();
    unsigned Height();

private:
    static void _DisplayHandler();
    static void _KeyboardHandlerASCII(unsigned char key, int x, int y);
    static void _KeyboardSpecialHandler(int specialKey, int x, int y);
    static void _ResizeHandler(int width, int height);
    static void _UpdateHandler(int value);
    static void _RedrawHandler(int value);
	static void _RedrawSnake(int value);

private:
    void        _BindHandlers();

private:
    int         _windowHandler;
    static GamePtr _game;
};

typedef std::unique_ptr<Application> ApplicationPtr;