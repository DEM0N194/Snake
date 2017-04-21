#pragma once
#include <random>

class SnakeFood
{
public:
	SnakeFood(const unsigned int & ScreenWidth, const unsigned int & ScreenHeight, const unsigned int & CELL_SIZE);
	void Respawn();
	unsigned int GetX() const;
	unsigned int GetY() const;
private:
	unsigned int x;
	unsigned int y;
private:
	unsigned int CELL_SIZE;
	std::random_device rd;
	std::mt19937 rng;
	std::uniform_int_distribution<int> xDist;
	std::uniform_int_distribution<int> yDist;
};