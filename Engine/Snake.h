#pragma once
#include <deque>
#include <random>
#include "Graphics.h"
#include "Keyboard.h"
#include "Colors.h"
class Snake
{
private:
	class SnakeSegment
	{
	public:
		SnakeSegment(unsigned int x, unsigned int y);
		unsigned int GetX() const;
		unsigned int GetY() const;
	private:
		unsigned int x;
		unsigned int y;
	};
private:
	class SnakeFood
	{
	public:
		SnakeFood();
		void Respawn();
		unsigned int GetX() const;
		unsigned int GetY() const;
	private:
		unsigned int x;
		unsigned int y;
	private:
		std::random_device rd;
		std::mt19937 rng;
		std::uniform_int_distribution<int> xDist;
		std::uniform_int_distribution<int> yDist;
	};
private:
	enum class eDir
	{				// [xv;yv] 
		GO_RIGHT,		// [ 1; 0]
		GO_LEFT,		// [-1; 0]
		GO_DOWN,		// [ 0; 1]
		GO_UP			// [ 0;-1]
	};
public:
	Snake();
	void Restart();
	void Update(Keyboard& kbd);
	void Render(Graphics& gfx);
	bool IsDead() const;
	bool IsGameWon() const;
private:
	void UpdateControls(Keyboard& kbd);
	void AddSegment(unsigned int x, unsigned int y);
	void MoveSnake();
	bool CheckForSelfCollision();
	bool CheckForFoodCollision();
private:
	void RenderFood(Graphics& gfx);
	void RenderSnake(Graphics& gfx);
	void RenderBorder(Graphics& gfx);
	void RenderGrid(Graphics& gfx);
	void RenderHealth(Graphics& gfx);
	void RenderStarvingTime(Graphics& gfx);
	void DrawSquare(Graphics& gfx, unsigned int x, unsigned int y, Color c);
private:
	static constexpr unsigned int CELL_SIZE = 20;
	std::deque<SnakeSegment> segments;
	SnakeFood food;
	eDir direction;
	eDir directionOld;
	bool dead;
	bool won;
	int starvingTime;
	const int timeToStarve;
	unsigned int health;
	unsigned int time;
	unsigned int timeOut;
	const unsigned int BASE_SPEED;
	const unsigned int BOOST_SPEED;
	const unsigned int START_X;
	const unsigned int START_Y;
	const unsigned int START_LENGTH;
	Color cHead;
	Color cSegment;
	Color cFood;
	bool DO_RIGHT;
	bool DO_LEFT;
	bool DO_DOWN;
	bool DO_UP;
};