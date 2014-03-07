#include "GL.h"
#include "Text.h"
#include "Game.h"
#include <iostream>
#include <stdlib.h>    
#include <time.h>    

const unsigned char ESC   = 27;
const unsigned char SPACE = 32;

const unsigned MIN_SPEED  = 1;
const unsigned MAX_SPEED  = 9;

const unsigned WIN_NUNBER = 244;

const unsigned CELL_SIZE  = 15;
const unsigned WIDTH      = 28;
const unsigned HEIGHT     = 31;

const unsigned X0          = 50;
const unsigned Y0          = 50;

const unsigned CREATURES_SPEED = 9;
const unsigned PACKMAN_SPEED = 7;

static int counter;
static int counterPinky;
static int counterCreatures;
static int counterScared;
inline int X(int x) {return  X0 + x * CELL_SIZE;}
inline int Y(int y) {return  Y0 + y * CELL_SIZE;}
inline int distance(int x1, int y1, int  x2, int y2 ){ return (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);}

void Game::print(int value)
{
    printf("VALUE\n");
}

void Game::print(int& value)
{
    printf("REFERENCE\n");
}

Game::Game() 
: _score(0)
, _speed(MIN_SPEED)
, _lose(false)
, _paused(false)
, _win(false)
, _packMan_stop(false)
, _scared(false)
, x0(X0)
, y0(Y0)
, cell_x(x0)
, cell_y(y0)
{	
	_packMan.x = 14;
	_packMan.y = 7;
	_packMan.direction = 1;
	_blinky.x = 14;
	_blinky.y = 19;
	_blinky.direction = 3; 
	_pinky.x = 13;
	_pinky.y = 17;
	_pinky.direction = 3; 
	_inky.x = 14;
	_inky.y = 17;
	_inky.direction = 3; 
	_clyde.x = 12;
	_clyde.y = 17;
	_clyde.direction = 3; 
	_eatenTargets = 0;

	gameField.setGameField(WIDTH, HEIGHT,"GameField.txt");
	
    static_assert(offsetof(Game,cell_x) > offsetof(Game,x0), "cell_x < x0");
    static_assert(offsetof(Game,cell_y) > offsetof(Game,y0), "cell_y < y0");
}

void Game::straightMove(Creature &c)
{
	switch (c.direction)
    {
	case 0 :if ( gameField.field[c.x][c.y + 1] != 0 ) 
			{
				++c.y;
			} 
			break;
	case 2 :if ( gameField.field[c.x][c.y - 1] != 0 ) 
			{
				--c.y; 
			} 
			break;
	case 3 :
			if(c.x - 1 < 0)
			{
				c.x = gameField._width - 1;
			}
			if ( gameField.field[c.x - 1][c.y] != 0 )
			{
				--c.x;
			} 
		    break;
	case 1 :
			if(c.x + 1 == gameField._width)
			{
				c.x = 0;
			}
			if ( gameField.field[c.x + 1][c.y] != 0 )
			{
				++c.x;
			} 
			break;
    }
}

void Game::moveToTarget(Creature &c, int x, int y)
{
	int minDist = 100000000;
	int currDir = -1;
	if(std::abs(gameField.field[c.x][c.y]) == 2)
	{
		if(gameField.field[c.x + 1][c.y] != 0)
		{
			if(c.direction != 3) 
			{
				if( minDist > distance(c.x + 1, c.y, x, y))
				{
					 minDist = distance(c.x + 1, c.y, x, y);
					 currDir = 1;
				}
			}
		}
		if(gameField.field[c.x - 1][c.y] != 0)
		{
			if(c.direction != 1) 
			{
				if( minDist > distance(c.x - 1, c.y, x, y))
				{
					 minDist = distance(c.x - 1, c.y, x, y);
					 currDir = 3;
				}				
			}
		}
		if(gameField.field[c.x ][c.y + 1] != 0)
		{
			if(c.direction != 2) 
			{
				if( minDist > distance(c.x, c.y + 1, x, y))
				{
					 minDist = distance(c.x, c.y + 1, x, y);
					 currDir = 0;
				}
			}
		}
		if(gameField.field[c.x][c.y - 1] != 0)
		{
			if(c.direction != 0) 
			{
				if( minDist > distance(c.x, c.y - 1, x, y))
				{
					 minDist = distance(c.x, c.y - 1, x, y);
					 currDir = 2;
				}
			}
		}
		c.direction = currDir;
	}
}

void GameField::setGameField(int w, int h, std::string file)
{
	_width = w;
	_height = h ;
	field = new int*[_height];
	for (int i = 0; i < _height; ++i)
	{
		field[i] = new int[_width];
	}
	int x;
	std::ifstream myInFile;
	myInFile.open(file, std::ifstream::in);
	for(int j = _height - 1; j >= 0; --j)
	{
		for ( int i = _width - 1; i >= 0; --i )
		{
			if(myInFile.eof())
			{
				std::cout << "Unexpected end of file!"<< std::endl;
			}
			myInFile >> x;
			field[i][j] = x;
		}
	}
	myInFile.close();
}


#pragma warning (disable : 4100) //to suppress unused warning int x, int y needed by GLUT API
void Game::KeyPressed(int specialKey, unsigned char key, int x, int y)
{
    switch (specialKey)
    {
        case GLUT_KEY_UP    :    
        case GLUT_KEY_DOWN  :    
        case GLUT_KEY_LEFT  :    
        case GLUT_KEY_RIGHT : _InGameKey(specialKey, 0);  break;
    }

    switch (key)
    { 
        case SPACE          : _InGameKey(0, key);         break;
        case '+'            : _SpeedUp();                 break;
        case '-'            : _SpeedDown();               break;
        case 'p'            :           
        case 'P'            : _Pause();                   break;
        case 'r'            :   
        case 'R'            : _Restart();                 break;
        case ESC            : _Stop();                    break;
    }
}
#pragma warning (default : 4100)

void Game::_InGameKey(int specialKey, unsigned char key)
{
    if (!_paused && !_lose)
    {
        switch (specialKey)
        {
			case GLUT_KEY_UP    : if(gameField.field[_packMan.x][_packMan.y + 1] != 0)
								  {									 
									  _packMan.direction = 0;
								  } 
								  break;
            case GLUT_KEY_DOWN  : if(gameField.field[_packMan.x][_packMan.y - 1] != 0)
								  {
									  _packMan.direction = 2; 
								  }
								  break;
            case GLUT_KEY_LEFT  : if(gameField.field[_packMan.x - 1][_packMan.y] != 0)
								  {
									  _packMan.direction = 3;
								  }  
								  break;
            case GLUT_KEY_RIGHT : if(gameField.field[_packMan.x + 1][_packMan.y] != 0)
								  {
									  _packMan.direction = 1;
								  } 
								  break;
        }

        switch (key)
        {
            case SPACE          : break;
        }
    }
}

void Game::Draw()
{
    GLClearBuffer();
    DoDraw();
    GLSwapBuffers();
}

void Game::DoDraw()
{
    GLDrawGrid(x0, y0, WIDTH, HEIGHT, CELL_SIZE);
    GLDrawBorder(x0, y0, WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE);
	for(int j = 0; j < gameField._height; ++j)
	{
		for ( int i = 0; i < gameField._width; ++i )
		{
			if(gameField.field[i][j] == 0)
			{
				GLDrawRect( X(i), Y(j), X(i) + CELL_SIZE, Y(j) + CELL_SIZE, 0x00ffff88);
			}
			if(gameField.field[i][j] > 0)
			{
				unsigned int color = 0x0088ff88;
				if(gameField.field[i][j] == 3) color = 0x001100dd;
				GLDrawRect( X(i) + CELL_SIZE/3, Y(j) + CELL_SIZE/3, X(i) + 2*CELL_SIZE/3, Y(j) + 2*CELL_SIZE/3, color);
			}
		}
	}
	GLDrawRect(X(_packMan.x), Y(_packMan.y), X(_packMan.x) + CELL_SIZE, Y(_packMan.y) + CELL_SIZE, 0x00ffff00);
	unsigned int creaturesScaredColor = 0x0000ff00;
	if(!_scared)
	{
		GLDrawRect(X(_blinky.x), Y(_blinky.y), X(_blinky.x) + CELL_SIZE, Y(_blinky.y) + CELL_SIZE, 0x00ff0000);
		GLDrawRect(X(_pinky.x), Y(_pinky.y), X(_pinky.x) + CELL_SIZE, Y(_pinky.y) + CELL_SIZE, 0x00ff0fc0);
		GLDrawRect(X(_inky.x), Y(_inky.y), X(_inky.x) + CELL_SIZE, Y(_inky.y) + CELL_SIZE, 0x000000ff);
		GLDrawRect(X(_clyde.x), Y(_clyde.y), X(_clyde.x) + CELL_SIZE, Y(_clyde.y) + CELL_SIZE, 0x00ff8800);
	}
	else
	{
		GLDrawRect(X(_blinky.x), Y(_blinky.y), X(_blinky.x) + CELL_SIZE, Y(_blinky.y) + CELL_SIZE, creaturesScaredColor);
		GLDrawRect(X(_pinky.x), Y(_pinky.y), X(_pinky.x) + CELL_SIZE, Y(_pinky.y) + CELL_SIZE, creaturesScaredColor);
		GLDrawRect(X(_inky.x), Y(_inky.y), X(_inky.x) + CELL_SIZE, Y(_inky.y) + CELL_SIZE, creaturesScaredColor);
		GLDrawRect(X(_clyde.x), Y(_clyde.y), X(_clyde.x) + CELL_SIZE, Y(_clyde.y) + CELL_SIZE, creaturesScaredColor);
	}
	
	counter++;
	counterPinky++;
	counterCreatures++;
	if(_scared)
	{
		++counterScared;
		if(counterScared == 300)
		{
			_scared = false;
			counterScared = 0;
		}
	}
	if(counterPinky == 30)
	{
		_pinky.x = 14;
		_pinky.y = 19;
		_pinky.direction = 3;
	}
	if(_eatenTargets == 30)
	{
		_inky.x = 14;
		_inky.y = 19;
		_inky.direction = 3;
	}
	
	if(_eatenTargets == 80)
	{
		_clyde.x = 14;
		_clyde.y = 19;
		_clyde.direction = 3;
	}
	if(!_lose && !_win && counterCreatures == CREATURES_SPEED)
	{				
		moveBlinky();
		movePinky();
		moveInky();
		moveClyde();
		counterCreatures = 0;
	}
	if(!_lose && !_win && counter == PACKMAN_SPEED)
	{				
		movePackMan();
		counter = 0;
	}
    _DrawStatus(x0 + (WIDTH + 1) * CELL_SIZE, y0);
}

void Game::_DrawStatus(unsigned x0, unsigned y0)
{
    GLDrawText(x0, y0 + 40, GLUT_BITMAP_HELVETICA_12, FormatText("Score: %i", _score).c_str());
    GLDrawText(x0, y0 + 60, GLUT_BITMAP_HELVETICA_12, FormatText("Speed: %i", _speed).c_str());

    if (_lose)
    {
        GLDrawText(x0, y0 + 240, GLUT_BITMAP_HELVETICA_18, "    Game over."     );
        GLDrawText(x0, y0 + 220, GLUT_BITMAP_HELVETICA_18, "Press R to restart.");
    }
    else if (_paused)
    {
        GLDrawText(x0, y0 + 240, GLUT_BITMAP_HELVETICA_18, "   Game paused."     );
        GLDrawText(x0, y0 + 220, GLUT_BITMAP_HELVETICA_18, "Press P to continue.");
    }
	else if (_win)
    {
        GLDrawText(x0, y0 + 240, GLUT_BITMAP_HELVETICA_18, "    You've won!!  "     );
        GLDrawText(x0, y0 + 220, GLUT_BITMAP_HELVETICA_18, "Press R to restart.");
    }
}

void Game::Update()
{
    if (!_paused)
    {
		if(!_scared)
		{
			if((_packMan.x == _blinky.x && _packMan.y == _blinky.y) ||
				(_packMan.x == _pinky.x && _packMan.y == _pinky.y)||
				(_packMan.x == _inky.x && _packMan.y == _inky.y)||
				(_packMan.x == _clyde.x && _packMan.y == _clyde.y))
			{
			 _lose = true;
			}
		}
		else
		{
			if(_packMan.x == _blinky.x && _packMan.y == _blinky.y)
			{
				_blinky.x = 14;
				_blinky.y = 19;
				_blinky.direction = 3; 
				_score += 10;
			}
			if(_packMan.x == _pinky.x && _packMan.y == _pinky.y)
			{
				_pinky.x = 14;
				_pinky.y = 19;
				_pinky.direction = 3; 
				_score += 10;
			}
			if(_packMan.x == _inky.x && _packMan.y == _inky.y)
			{
				_inky.x = 14;
				_inky.y = 19;
				_inky.direction = 3; 
				_score += 10;
			}
			if(_packMan.x == _clyde.x && _packMan.y == _clyde.y)
			{
				_clyde.x = 14;
				_clyde.y = 19;
				_clyde.direction = 3; 
				_score += 10;
			}
		}
		if(_eatenTargets == WIN_NUNBER)
		{
			_win = true;
			_SpeedUp();
			//_Pause();
			//_Restart();
		}
    }

}

void Game::_Restart()
{
    _lose   = false;
    _paused = false;
    _score  = 0;
	_win = false;
	_packMan.x = 14;
	_packMan.y = 7;
	_packMan.direction = 1;
	_blinky.x = 14;
	_blinky.y = 19;
	_blinky.direction = 3; 
	_pinky.x = 13;
	_pinky.y = 17;
	_pinky.direction = 3; 
	_inky.x = 14;
	_inky.y = 17;
	_inky.direction = 3; 
	_clyde.x = 12;
	_clyde.y = 17;
	_clyde.direction = 3; 
	_eatenTargets = 0;
	counterCreatures = 0;
	counter = 0;
	counterPinky = 0;
	counterScared = 0;

	gameField.setGameField(WIDTH, HEIGHT,"GameField.txt");
}

void Game::_Pause()
{
    _paused = !_paused;
}

void Game::_Stop()
{
    exit(0);
}

unsigned Game::UpdateInterval()
{
    return (MAX_SPEED - _speed + 1)*100;
}

void Game::_SpeedUp()
{
    _speed = std::min(_speed + 1, MAX_SPEED);
}

void Game::_SpeedDown()
{
    _speed = std::max(_speed - 1, MIN_SPEED);
}


void Game::movePackMan()
{	
	straightMove(_packMan);

	if( gameField.field[_packMan.x][_packMan.y] > 0)
	{
		if(gameField.field[_packMan.x][_packMan.y] == 3)
		{
			_scared = true;
		}
		else
		{
			++_score;
			++_eatenTargets;
		}
		gameField.field[_packMan.x][_packMan.y] = -gameField.field[_packMan.x][_packMan.y];
	}
	Update();
}

void Game::moveBlinky()
{
	if(!_scared)
	{
		straightMove(_blinky);
		moveToTarget(_blinky, _packMan.x, _packMan.y);
	}
	else
	{
		straightMove(_blinky);
		moveToTarget(_blinky, 1, HEIGHT - 1);	
	}
	Update();
}

void Game::movePinky()
{
	if(!_scared)
	{
		straightMove(_pinky);
		int dx;
		int dy;
		if(_packMan.direction == 1) { dx = 1; dy = 0;}
		if(_packMan.direction == 3) { dx = -1; dy = 0;}
		if(_packMan.direction == 0) { dx = 0; dy = 1;}
		if(_packMan.direction == 2) { dx = 0; dy = -1;}	
		moveToTarget(_pinky, _packMan.x + 4*dx, _packMan.y + 4*dy);
	}
	else
	{
		straightMove(_pinky);
		moveToTarget(_pinky, WIDTH - 1, HEIGHT - 1);	
	}
	Update();
}

void Game::moveInky()
{
	if(!_scared)
	{
		straightMove(_inky);
		int dx;
		int dy;
		if(_packMan.direction == 1) { dx = 1; dy = 0;}
		if(_packMan.direction == 3) { dx = -1; dy = 0;}
		if(_packMan.direction == 0) { dx = 0; dy = 1;}
		if(_packMan.direction == 2) { dx = 0; dy = -1;}
		moveToTarget(_inky, 2*(_packMan.x + 2*dx) - _blinky.x, 2*(_packMan.y + 2*dy) - _blinky.y);
	}
	else
	{
		straightMove(_inky);
		moveToTarget(_inky, WIDTH - 1, 1);
	}
	Update();
}

void Game::moveClyde()
{
	if(!_scared)
	{
		if(distance(_clyde.x, _clyde.y, _packMan.x, _packMan.y) > 64)
		{
			straightMove(_clyde);
			moveToTarget(_clyde, _packMan.x, _packMan.y);
		}
		else
		{	
			straightMove(_clyde);
			moveToTarget(_clyde, 1, 1);
		}
	}
	else
	{
		straightMove(_clyde);
		moveToTarget(_clyde, 1, 1);
	}
	Update();
}