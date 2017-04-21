#pragma once
#include <deque>
#include "Graphics.h"
#include "Keyboard.h"
#include "Colors.h"
#include "SnakeFood.h"
class Snake
{
private:
	class SnakeSegment
	{
	public:
		SnakeSegment(unsigned int x, unsigned int y, unsigned int padding);
		unsigned int GetX() const;
		unsigned int GetY() const;
		unsigned int GetP() const;
	private:
		unsigned int x;
		unsigned int y;
		unsigned int p;
	};
private:
	enum class eDir
	{					// [xv;yv] 
		GO_RIGHT,		// [ 1; 0]
		GO_LEFT,		// [-1; 0]
		GO_DOWN,		// [ 0; 1]
		GO_UP			// [ 0;-1]
	};
public:
	Snake();
	void Restart();
	void Update(Keyboard& kbd, float dt);
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
	void DrawSquareWPadding(Graphics & gfx, unsigned int x, unsigned int y, Color c, int padding);
private:
	static constexpr unsigned int CELL_SIZE = 20;
	std::deque<SnakeSegment> segments;
	SnakeFood food;
	eDir direction;
	eDir directionOld;
	bool dead;
	bool won;
	bool foodEaten;
	float starvingTime;
	const float timeToStarve;
	unsigned int health;
	float time;
	float timeOut;
	const float BASE_SPEED;
	const float BOOST_SPEED;
	const unsigned int START_X;
	const unsigned int START_Y;
	const unsigned int START_LENGTH;
	float segmentShade;
	bool segmentShadeDarkening;
	Color cHead;
	Color cFood;
	bool DO_RIGHT;
	bool DO_LEFT;
	bool DO_DOWN;
	bool DO_UP;
};