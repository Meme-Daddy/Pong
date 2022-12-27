#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <iostream>

using namespace std;

struct Ball
{
	float x, y;
	float speedX, speedY;
	float radius;
	Color color = WHITE;
	
	//Drawing circle object
	void Draw()
	{
		DrawCircle(x, y, radius, color);
	}

};

struct Paddle
{
	float x, y;
	float width, height;
	float speed;
	Color color = WHITE;

	Rectangle GetRectangle()
	{
		return Rectangle{ x - width / 2.0f, y - height / 2.0f, width, height }; // Changes the offset from corner of rect to center
	}
	// Drawing Paddle object
	void Draw()
	{
		DrawRectangleRec(GetRectangle(), color);
	}

	void LeftPaddleMovement()
	{
		if (IsKeyDown(KEY_W))
		{
			y -= speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_S))
		{
			y += speed * GetFrameTime();
		}
	}

	void RightPaddleMovement()
	{
		if (IsKeyDown(KEY_UP))
		{
			y -= speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_DOWN))
		{
			y += speed * GetFrameTime();
		}
	}
};

int main()
{
	const int screenWidth = 1280;
	const int screenHeight = 720;
	//SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Pong");
	SetTargetFPS(60);

	Color backroundColor = BLACK;

	int leftPoints = 0;
	int rightPoints = 0;
	int pointsTowin = 5;

	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.speedX = 300;
	ball.speedY = 300;
	ball.radius = 5;
	ball.color;

	int ballDirection = 0;

	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2.0f;
	leftPaddle.width = 10;
	leftPaddle.height = 100;
	leftPaddle.speed = 300;
	leftPaddle.color;

	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 50;
	rightPaddle.y = GetScreenHeight() / 2.0f;
	rightPaddle.width = 10;
	rightPaddle.height = 100;
	rightPaddle.speed = 300;
	rightPaddle.color;

	
	// While window is not closed
	while (!WindowShouldClose())
	{
		// Moving ball relative to fps
		ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();
		

		// Ball constraints
		if (ball.y < 0) // If ball y goes above the top of screen. 0 is the top
		{
			ball.y = 0;
			ball.speedY *= -1;
		}
		if (ball.y > GetScreenHeight()) // If ball y goes below the bottom of screen
		{
			ball.y = GetScreenHeight();
			ball.speedY *= -1;
		}
		
		leftPaddle.LeftPaddleMovement();
		rightPaddle.RightPaddleMovement();

		// Collision checks
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRectangle()))
		{
			if (ball.speedX < 0)
			{
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
			}
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRectangle()))
		{
			if (ball.speedX > 0)
			{
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
			}
		}
		// If top of screen
		if (CheckCollisionRecs(leftPaddle.GetRectangle(), Rectangle{0, -1, (float)GetScreenWidth(), 1}))
		{
			leftPaddle.y = leftPaddle.height / 2;
		}
		// If bottom of screen
		if (CheckCollisionRecs(leftPaddle.GetRectangle(), Rectangle{ 0, (float)GetScreenHeight() + 1, (float)GetScreenWidth(), 1})) 
		{
			leftPaddle.y = GetScreenHeight() - (leftPaddle.height / 2);
		}

		if (CheckCollisionRecs(rightPaddle.GetRectangle(), Rectangle{ 0, -1, (float)GetScreenWidth(), 1 }))
		{
			rightPaddle.y = rightPaddle.height / 2;
		}
		if (CheckCollisionRecs(rightPaddle.GetRectangle(), Rectangle{ 0, (float)GetScreenHeight() + 1, (float)GetScreenWidth(), 1 }))
		{
			rightPaddle.y = GetScreenHeight() - (rightPaddle.height / 2);
		}

		// If ball goes past right of screen
		if (ball.x > GetScreenWidth())
		{
			leftPoints++;
			ball.x = GetScreenWidth() / 2.0f;
			ball.y = GetScreenHeight() / 2.0f;
			ball.speedX = -300;
		}
		// If ball goes past left of screen
		if (ball.x < 0)
		{
			rightPoints++;
			ball.x = GetScreenWidth() / 2.0f;
			ball.y = GetScreenHeight() / 2.0f;
			ball.speedX = 300;
		}

		if (leftPoints >= pointsTowin)
		{
			ball.speedX = 0;
			ball.speedY = 0;
			ball.color = backroundColor;

			DrawText("LEFT PLAYER WINS!", GetScreenWidth() / 5 - 35, GetScreenHeight() / 2 - 100, 80, YELLOW);
			DrawText("Press SPACE to play again...", GetScreenWidth() / 5 + 20, GetScreenHeight() / 2, 50, YELLOW); 
		}
		if (rightPoints >= pointsTowin)
		{
			ball.speedX = 0;
			ball.speedY = 0;
			ball.color = backroundColor;

			DrawText("RIGHT PLAYER WINS!", GetScreenWidth() / 5 - 35, GetScreenHeight() / 2 - 100, 80, YELLOW);
			DrawText("Press SPACE to play again...", GetScreenWidth() / 5 + 20, GetScreenHeight() / 2, 50, YELLOW);
		}
		if (IsKeyPressed(KEY_SPACE) && (leftPoints >= pointsTowin || rightPoints >= pointsTowin))
		{
			ballDirection++;
			ball.color = WHITE;
			leftPoints = 0;
			rightPoints = 0;
			ball.speedX = (ballDirection%2 == 1) ? -300 : 300;
			ball.speedY = (ballDirection % 2 == 1) ? -300 : 300;
			ball.x += ball.speedX * GetFrameTime();
			ball.y += ball.speedY * GetFrameTime();
		}
		
		BeginDrawing();
		DrawFPS(0, 0);
		ClearBackground(BLACK);

		ball.Draw();
		leftPaddle.Draw();
		rightPaddle.Draw();

		DrawRectangle(0, -1, GetScreenWidth(), 1, WHITE);

		DrawText(TextFormat("%i", leftPoints), 250, 100, 80, LIGHTGRAY);
		DrawText(TextFormat("%i", rightPoints), 975, 100, 80, LIGHTGRAY);
		
		EndDrawing();
	}
	CloseWindow();
	return 0;
}