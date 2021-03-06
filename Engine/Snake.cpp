#include "Snake.h"

Snake::SnakeSegment::SnakeSegment(unsigned int x, unsigned int y, unsigned int padding)
{
	this->x = x;
	this->y = y;
	p = padding;
}

unsigned int Snake::SnakeSegment::GetX() const
{
	return x;
}

unsigned int Snake::SnakeSegment::GetY() const
{
	return y;
}

unsigned int Snake::SnakeSegment::GetP() const
{
	return p;
}

Snake::Snake()
	: BASE_SPEED(0.25f)
	, BOOST_SPEED(0.08f)
	, timeOut(BASE_SPEED)
	, timeToStarve(7.0f)
	, START_X(Graphics::ScreenWidth / 2)
	, START_Y(Graphics::ScreenHeight / 2)
	, START_LENGTH(3)
	, food(Graphics::ScreenWidth, Graphics::ScreenHeight, CELL_SIZE)
{
	cHead = Color(127, 127, 31);
	cFood = Color(127,63,63);
	Restart();
}

void Snake::Restart()
{
	direction = eDir(rand() % 4);
	health = 3;
	time = 0;
	starvingTime = timeToStarve;
	won = false;
	dead = false;
	directionOld = direction;
	segments.clear();
	for (unsigned int i = 0; i < START_LENGTH; i++)
	{
		AddSegment(START_X - i, START_Y);
	}
}

void Snake::Update(Keyboard & kbd, float dt)
{
	if (dead || won)
	{
		return;
	}

	UpdateControls(kbd);

	starvingTime -= dt;
	time += dt;
	if (time < timeOut)
	{
		return;
	}
	time -= timeOut;

	if (starvingTime <= 0)
	{
		health--;
		starvingTime += timeToStarve;
		food.Respawn();
	}

	MoveSnake();

	if (CheckForSelfCollision() || segments.size() == 2)
	{
		health--;
	}

	// You win if your snake takes up half a screen
	if (segments.size() == ((Graphics::ScreenWidth - 2 * CELL_SIZE) / CELL_SIZE) * ((Graphics::ScreenHeight - 2 * CELL_SIZE) / CELL_SIZE) / 2)
	{
		won = true;
	}

	if (health == 0)
	{
		dead = true;
		return;
	}

	foodEaten = false;
	if (CheckForFoodCollision())
	{
		foodEaten = true;
		food.Respawn();
		starvingTime = timeToStarve;
		for (unsigned int i = 0; i < segments.size(); i++)
		{
			while (food.GetX() == segments[i].GetX() && food.GetY() == segments[i].GetY())
			{
				food.Respawn();
				i = 0;
			}
		}
	}
	else
	{
		segments.pop_back();
	}

}

void Snake::Render(Graphics & gfx)
{
	RenderGrid(gfx);
	RenderBorder(gfx);

	if (dead)
	{
		return;
	}

	RenderHealth(gfx);
	RenderFood(gfx);
	RenderSnake(gfx);
	RenderStarvingTime(gfx);
}

bool Snake::IsDead() const
{
	return dead;
}

bool Snake::IsGameWon() const
{
	return won;
}

void Snake::UpdateControls(Keyboard& kbd)
{
	if (kbd.KeyIsPressed(VK_RIGHT) && directionOld != eDir::GO_LEFT)
	{
		if (!DO_RIGHT)
		{
			DO_RIGHT = true;
			direction = eDir::GO_RIGHT;
			if (direction == directionOld)
			{
				timeOut = BOOST_SPEED;
			}
			else
			{
				timeOut = BASE_SPEED;
			}
		}
	}
	else
	{
		DO_RIGHT = false;
	}
	if (kbd.KeyIsPressed(VK_LEFT) && directionOld != eDir::GO_RIGHT)
	{
		if (!DO_LEFT)
		{
			DO_LEFT = true;
			direction = eDir::GO_LEFT;
			if (direction == directionOld)
			{
				timeOut = BOOST_SPEED;
			}
			else
			{
				timeOut = BASE_SPEED;
			}
		}
	}
	else
	{
		DO_LEFT = false;
	}
	if (kbd.KeyIsPressed(VK_DOWN) && directionOld != eDir::GO_UP)
	{
		if (!DO_DOWN)
		{
			DO_DOWN = true;
			direction = eDir::GO_DOWN;
			if (direction == directionOld)
			{
				timeOut = BOOST_SPEED;
			}
			else
			{
				timeOut = BASE_SPEED;
			}
		}
	}
	else
	{
		DO_DOWN = false;
	} 
	if (kbd.KeyIsPressed(VK_UP) && directionOld != eDir::GO_DOWN)
	{
		if (!DO_UP)
		{
			DO_UP = true;
			direction = eDir::GO_UP;
			if (direction == directionOld)
			{
				timeOut = BOOST_SPEED;
			}
			else
			{
				timeOut = BASE_SPEED;
			}
		}
	}
	else
	{
		DO_UP = false;
	}


	if (kbd.KeyIsPressed(VK_UP)){}
	else if (kbd.KeyIsPressed(VK_DOWN)){}
	else if (kbd.KeyIsPressed(VK_RIGHT)){}
	else if (kbd.KeyIsPressed(VK_LEFT)){}
	else
	{
		timeOut = BASE_SPEED;
	}
}

void Snake::AddSegment(unsigned int x, unsigned int y)
{
	SnakeSegment segment(x, y, 2);
	segments.push_back(segment);
}

void Snake::MoveSnake()
{
	const float R = 31;
	const float G = 63;
	const float B = 31;

	static const int moveX[] = {1,-1, 0, 0};
	static const int moveY[] = {0, 0, 1,-1};

	unsigned int x = segments[0].GetX() + moveX[(int)direction] * CELL_SIZE;
	unsigned int y = segments[0].GetY() + moveY[(int)direction] * CELL_SIZE;

	if (x == 0)
	{
		x = Graphics::ScreenWidth - (2 * CELL_SIZE);
	}
	else if (y == 0)
	{
		y = Graphics::ScreenHeight - (2 * CELL_SIZE);
	}
	else if (x == Graphics::ScreenWidth - CELL_SIZE)
	{
		x = CELL_SIZE;
	}
	else if (y == Graphics::ScreenHeight - CELL_SIZE)
	{
		y = CELL_SIZE;
	}

	if (foodEaten)
	{
		SnakeSegment nextSegment(x, y, 1);
		segments.push_front(nextSegment);
	}
	else
	{
		SnakeSegment nextSegment(x, y, 2);
		segments.push_front(nextSegment);
	}

	directionOld = direction;
}

bool Snake::CheckForSelfCollision()
{
	const unsigned int HEAD_X = segments[0].GetX();
	const unsigned int HEAD_Y = segments[0].GetY();

	for (unsigned int i = 1; i < segments.size(); i++)
	{
		if (HEAD_X == segments[i].GetX() && HEAD_Y == segments[i].GetY())
		{
			while (segments.size() != i+1)
			{
				segments.pop_back();
			}
			return true;
		}
	}
	return false;
}

bool Snake::CheckForFoodCollision()
{
	return (segments[0].GetX() == food.GetX() && segments[0].GetY() == food.GetY());
}


void Snake::RenderSnake(Graphics & gfx)
{
	DrawSquareWPadding(gfx, segments[0].GetX(), segments[0].GetY(), cHead, 1);
	segmentShade = 2.0f;
	for (unsigned int i = 1; i < segments.size(); i++)
	{

		if (segmentShade >= 1.0f)
		{
			segmentShadeDarkening = true;
		}
		else if (segmentShade <= 0.7f)
		{
			segmentShadeDarkening = false;
		}

		if (segmentShadeDarkening)
		{
			segmentShade -= 0.2f;
		}
		else
		{
			segmentShade += 0.2f;
		}
		
		DrawSquareWPadding( gfx, segments[i].GetX(), segments[i].GetY(),
						   Color(static_cast<int>(31.0f * segmentShade),
								 static_cast<int>(63.0f * segmentShade),
								 static_cast<int>(31.0f * segmentShade)), segments[i].GetP());
	}
}

void Snake::RenderBorder(Graphics & gfx)
{
	for (unsigned int i = 0; i < Graphics::ScreenHeight; i += CELL_SIZE)
	{
		for (unsigned int j = 0; j < Graphics::ScreenWidth; j += CELL_SIZE)
		{
			if (j == 0 ||
			    i == 0 ||
			    j == Graphics::ScreenWidth - CELL_SIZE ||
			    i == Graphics::ScreenHeight - CELL_SIZE)
			{
				DrawSquare(gfx, j, i, Color(75, 75, 75));
			}
		}
	}
}

void Snake::RenderGrid(Graphics & gfx)
{
	for (unsigned int i = CELL_SIZE; i < Graphics::ScreenHeight - CELL_SIZE; i += CELL_SIZE)
	{
		for (unsigned int j = CELL_SIZE; j < Graphics::ScreenWidth - CELL_SIZE; j++)
		{
			gfx.PutPixel(j, i, 35, 35, 35);
		}
	}
	for (unsigned int i = CELL_SIZE; i < Graphics::ScreenHeight - CELL_SIZE; i++)
	{
		for (unsigned int j = CELL_SIZE; j < Graphics::ScreenWidth - CELL_SIZE; j += CELL_SIZE)
		{
			gfx.PutPixel(j, i, 35, 35, 35);
		}
	}
}

void Snake::RenderHealth(Graphics & gfx)
{
	for (unsigned int i = 0; i < health; i++)
	{
		if (i < gfx.ScreenWidth - 1)
		DrawSquare(gfx, i * 25 + 25, 0, Color(255, 63, 63));
	}
}

void Snake::RenderStarvingTime(Graphics & gfx)
{
	for (int i = CELL_SIZE; i < starvingTime * (gfx.ScreenWidth - CELL_SIZE) / timeToStarve; i++)
	{
		if (i > 0 && i < Graphics::ScreenWidth - 1 - CELL_SIZE)
		{
			gfx.PutPixel(i, CELL_SIZE + 1, Colors::White);
			gfx.PutPixel(i, CELL_SIZE + 2, Colors::White);
		}
	}
}

void Snake::RenderFood(Graphics & gfx)
{
	DrawSquareWPadding(gfx, food.GetX(), food.GetY(), cFood, 2);
}

void Snake::DrawSquare(Graphics & gfx, unsigned int x, unsigned int y, Color c)
{
	for (unsigned int i = 0; i < CELL_SIZE; i++)
	{
		for (unsigned int j = 0; j < CELL_SIZE; j++)
		{
			gfx.PutPixel(x + j, y + i, c);
		}
	}
}

void Snake::DrawSquareWPadding(Graphics & gfx, unsigned int x, unsigned int y, Color c, int padding)
{
	for (unsigned int i = padding; i < CELL_SIZE - padding; i++)
	{
		for (unsigned int j = padding; j < CELL_SIZE - padding; j++)
		{
			gfx.PutPixel(x + j, y + i, c);
		}
	}
}
