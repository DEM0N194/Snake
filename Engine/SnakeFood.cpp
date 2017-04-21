#include "SnakeFood.h"

SnakeFood::SnakeFood(const unsigned int ScreenWidth, const unsigned int ScreenHeight, const unsigned int CELL_SIZE)
	: rng(rd())
	, xDist(1, ScreenWidth / CELL_SIZE - 2)
	, yDist(1, ScreenHeight / CELL_SIZE - 2)
{
	this->CELL_SIZE = CELL_SIZE;
	Respawn();
}

void SnakeFood::Respawn()
{
	x = CELL_SIZE * xDist(rng);
	y = CELL_SIZE * yDist(rng);
}

unsigned int SnakeFood::GetX() const
{
	return x;
}

unsigned int SnakeFood::GetY() const
{
	return y;
}
