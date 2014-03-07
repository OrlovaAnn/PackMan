#pragma once

#include <memory>
#include <deque>
#include <fstream>
#include <iostream>
#include <math.h>


struct Creature
{
	Creature():x(1), y(1), direction(1){}
	Creature(int xx, int yy, int d):x(xx), y(yy), direction(d){}
	int x;
	int y;
	int direction;
};

struct GameField
{
	int _width;
	int _height;
	int** field;
	GameField(){};
	void setGameField(int w, int h, std::string file);
};

class Game
{
public:
    Game();

public:
    void Draw(); // NVI
    void KeyPressed(int specialKey, unsigned char key, int x, int y);
    void Update();
    unsigned UpdateInterval();

private:
	void movePackMan();
	void straightMove(Creature &c);
	void Game::moveToTarget(Creature &c, int x, int y);
	void moveBlinky();
	void movePinky();
	void moveInky();
	void moveClyde();

private:
    virtual void DoDraw();

private:
    void _Pause();
    void _Restart();
    void _Stop();

private:
    void _SpeedUp();
    void _SpeedDown();

private:
    void _DrawStatus(unsigned x0, unsigned y0);

private:
    void _InGameKey(int specialKey, unsigned char key);

private:
    void print(int& value);
    void print(int value);

private:
//game state
    unsigned      _score;
    unsigned      _speed;
    bool          _lose;
    bool          _paused;
	bool          _win;
	bool          _packMan_stop;
	bool		  _scared;
    unsigned      x0;
    unsigned      y0;
    unsigned      cell_x, cell_y;
	unsigned      _eatenTargets;
	unsigned direction;
	Creature _packMan;
	Creature _blinky;
	Creature _pinky;
	Creature _inky;
	Creature _clyde;
	GameField gameField;
};

typedef std::unique_ptr<Game> GamePtr;