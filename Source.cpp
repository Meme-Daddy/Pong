#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>

using std::string;

bool atStartMenu = true,
	atGamemodeMenu = false,
	atEndScreen = false,
	paused = false,
	playing = false,
	backToStart = false,
	exitGame = false;
int roundsPlayed = 0;
int gameMode = 0; // 0 = PVP 1 = PVCPU
int theme = 0; // 0 == Regular 1 = Inverted


 string GetCurrentDirectory(int input) // 1 = menu music 2 = pong paddle hit 3 = pong wall hit 4 = score
{
	string currentDirectory(GetApplicationDirectory());
	string musicFilename("\Assets\\Audio\\HopesAndDreams.mp3");
	string pongPaddleHit("\Assets\\Audio\\PongPaddleHit.wav");
	string pongWallHit("\Assets\\Audio\\PongWallHit.wav");
	string pongScore("\Assets\\Audio\\PongScore.wav");
	if (input == 1)
		return currentDirectory.append(musicFilename);
	else if (input == 2)
		return currentDirectory.append(pongPaddleHit);
	else if (input == 3)
		return currentDirectory.append(pongWallHit);
	else if (input == 4)
		return currentDirectory.append(pongScore);
}


class Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> m_StartTime;

public:
	void Start()
	{
		m_StartTime = std::chrono::high_resolution_clock::now();
	}
	
	float GetDuration()
	{
		std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - m_StartTime;
		return duration.count();
	}
};


struct UI
{
	int btnState = 0,
		pongBtnState = 0,
		startBtnState = 0,
		settingsBtnState = 0,
		exitBtnState = 0; // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED

	int pvpBtnState = 0,
		pvcBtnState = 0,
		backBtnState = 0,
		mainMenuBtnState = 0;

	int counter = 0;

	bool btnAction = false,
		pongBtnAction = false,
		startBtnAction = false,
		settingsBtnAction = false,
		exitBtnAction = false;
	bool pvpBtnAction = false,
		pvcBtnAction = false,
		backBtnAction = false;

	Vector2 mousePoint = { 400, 400 };
	// Start Menu collision rects
	Rectangle pongTxtBounds = { (float)GetScreenWidth() / 3 + 40, (float)GetScreenHeight() / 5 - 30, 360, 100 };
	Rectangle startTxtBounds = { (float)GetScreenWidth() / 3 + 70, (float)GetScreenHeight() / 2, 295, 50 };
	Rectangle settingsTxtBounds = { (float)GetScreenWidth() / 3 + 100, (float)GetScreenHeight() / 2 + 80, 220, 50 };
	Rectangle exitTxtBounds = { (float)GetScreenWidth() / 3 + 155, (float)GetScreenHeight() / 2 + 160, 100, 50 };

	// Gamemode Menu collision rects
	Rectangle pvpTxtBounds = { (float)GetScreenWidth() / 3 - 30, (float)GetScreenHeight() / 2, 460, 50 };
	Rectangle pvcTxtBounds = { (float)GetScreenWidth() / 3 - 10, (float)GetScreenHeight() / 2 + 90, 400, 50 };
	Rectangle backTxtBounds = { (float)GetScreenWidth() / 2 - 85, (float)GetScreenHeight() - 180 , 135, 50 };

	//End screen collision rects
	Rectangle mainMenuTxtBounds = { (float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() - 160 , 275, 50 };

	//Pause screen collisions
	Rectangle pauseMainMenuTxtBounds = { (float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 50 , 275, 50 };

	Color color = YELLOW,
		pong = color,
		start = color,
		settings = color,
		exit = color,
		pvp = color,
		pvc = color,
		back = color,
		mainMenu = color;

	Music music;

	Timer t;
	Timer t2;

	
	void StartMenuDraw()
	{	// Pong text collision
		float timer = t2.GetDuration();

		if (CheckCollisionPointRec(mousePoint, pongTxtBounds))
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				pongBtnState = 2;
			else pongBtnState = 1;

			if (pongBtnState == 1)
				pong = RED;

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
			{
				counter++;
				theme = (counter % 2) ? 1 : 0;
			}
				
		}

		else pongBtnState = 0;

		if (pongBtnState == 0)
			if (theme == 0)
				pong = YELLOW;
			else if (theme == 1)
				pong = BLACK;

		// Start Game text collision
		if (CheckCollisionPointRec(mousePoint, startTxtBounds))
		{
			
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				startBtnState = 2;
			else 
			{
				startBtnState = 1; 
				startBtnAction = false;
			}

			if (startBtnState == 1)
				start = RED;

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
			{
				startBtnAction = true;
				atGamemodeMenu = true;
				t.Start();
			}
		}
		else startBtnState = 0;

		if (startBtnState == 0)
			if (theme == 0)
				start = YELLOW;
			else if (theme == 1)
				start = BLACK;

		// Settings text collision
		if (CheckCollisionPointRec(mousePoint, settingsTxtBounds))
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				settingsBtnState = 2;
			else settingsBtnState = 1;

			if (settingsBtnState == 1)
				settings = RED;

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
				settingsBtnState = true;
		}
		else settingsBtnState = 0;

		if (settingsBtnState == 0) 
			if (theme == 0)
				settings = YELLOW;
			else if (theme == 1)
				settings = BLACK;

		// Exit text collision
		if (CheckCollisionPointRec(mousePoint, exitTxtBounds) && timer > 0.3)
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				exitBtnState = 2;
			else exitBtnState = 1;

			if (exitBtnState == 1)
					exit = RED;
			
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
				exitGame = true;
		}
		else exitBtnState = 0;

		if (exitBtnState == 0)
			if (theme == 0)
				exit = YELLOW;
			else if (theme == 1)
				exit = BLACK;

		/*if (theme == 1)
		{
			pong = BLACK;
			start = BLACK;
			settings = BLACK;
			exit = BLACK;
		}*/

		DrawText("PONG!", GetScreenWidth() / 3 + 35, GetScreenHeight() / 5 - 40, 125, pong);
		DrawText("Start Game", GetScreenWidth() / 3 + 75, GetScreenHeight() / 2, 50, start);
		DrawText("Settings", GetScreenWidth() / 3 + 110, GetScreenHeight() / 2 + 80, 50, settings);
		DrawText("Exit", GetScreenWidth() / 3 + 160, GetScreenHeight() / 2 + 160, 50, exit);
		
		//DrawRectangleLinesEx(pongTxtBounds, 2, WHITE);
	}


	void GameModeMenuDraw()
	{
		float timer = t.GetDuration();

		if (CheckCollisionPointRec(mousePoint, pvpTxtBounds))
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				pvpBtnState = 2;
			else pvpBtnState = 1;
			if (pvpBtnState == 1)
				pvp = RED;

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && timer > 0.3) // Timer prevents auto clicking into next menu
			{
				atGamemodeMenu = false;
				atStartMenu = false;
				playing = true;
				gameMode = 0;
			}
		}
		else pvpBtnState = 0;

		if (pvpBtnState == 0)
			if (theme == 0)
				pvp = YELLOW;
			else if (theme == 1)
				pvp = BLACK;



		if (CheckCollisionPointRec(mousePoint, pvcTxtBounds))
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				pvcBtnState = 2;
			else pvcBtnState = 1;

			if (pvcBtnState == 1)
				pvc = RED;

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && timer > 0.3)
			{
				atGamemodeMenu = false;
				atStartMenu = false;
				playing = true;
				gameMode = 1;
			}
		}
		else pvcBtnState = 0;

		if (pvcBtnState == 0)
			if (theme == 0)
				pvc = YELLOW;
			else if (theme == 1)
				pvc = BLACK;



		if (CheckCollisionPointRec(mousePoint, backTxtBounds))
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				backBtnState = 2;
			else backBtnState = 1;

			if (backBtnState == 1)
				back = RED;

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
			{
				atGamemodeMenu = false;
				atStartMenu = true;
				backBtnAction = true;
				t2.Start();
			}
		}
		else backBtnState = 0;

		if (backBtnState == 0)
			if (theme == 0)
				back = YELLOW;
			else if (theme == 1)
				back = BLACK;

		if (theme == 0)
			color = YELLOW;
		else if (theme == 1)
			color = BLACK;
		//DrawRectangleLinesEx(backTxtBounds, 2, WHITE);
		
		DrawText("Gamemode Select", (float)GetScreenWidth() / 4 - 25, (float)GetScreenHeight() / 3 - 40, 80, color);
		DrawText("Player VS Player", (float)GetScreenWidth() / 3 - 25, (float)GetScreenHeight() / 2, 50, pvp);
		DrawText("Player VS CPU", (float)GetScreenWidth() / 3, (float)GetScreenHeight() / 2 + 90, 50, pvc);
		DrawText("Back", (float)GetScreenWidth() / 2 - 80, (float)GetScreenHeight() - 180, 50, back);
	}


	void EndScreenDraw(int leftPoints, int rightPoints)
	{
		if (CheckCollisionPointRec(mousePoint, mainMenuTxtBounds))
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				mainMenuBtnState = 2;
			else mainMenuBtnState = 1;

			if (mainMenuBtnState == 1)
				mainMenu = RED;

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
			{
				playing = false;
				atEndScreen = false;
				atGamemodeMenu = false;
				backToStart = true;
				atStartMenu = true;
				gameMode = 0;
			}
		}
		else mainMenuBtnState = 0;

		if (mainMenuBtnState == 0)
			if (theme == 0)
				mainMenu = YELLOW;
			else if (theme == 1)
				mainMenu = BLACK;

		if (theme == 0)
			color = YELLOW;
		else if (theme == 1)
			color = BLACK;

		if (leftPoints > rightPoints)
		{
			DrawText("LEFT PLAYER WINS!", GetScreenWidth() / 5 - 35, GetScreenHeight() / 2 - 100, 80, color);
			DrawText("Press SPACE to play again...", GetScreenWidth() / 5 + 20, GetScreenHeight() / 2, 50, color);
			DrawText("Main Menu", GetScreenWidth() / 3 + 75, GetScreenHeight() / 2 + 200, 50, mainMenu);
		}
		if (rightPoints > leftPoints)
		{
			DrawText("RIGHT PLAYER WINS!", GetScreenWidth() / 5 - 35, GetScreenHeight() / 2 - 100, 80, color);
			DrawText("Press SPACE to play again...", GetScreenWidth() / 5 + 20, GetScreenHeight() / 2, 50, color);
			DrawText("Main Menu", GetScreenWidth() / 3 + 75, GetScreenHeight() / 2 + 200, 50, mainMenu);
		}
	}

	void PauseScreenDraw()
	{
		if (CheckCollisionPointRec(mousePoint, pauseMainMenuTxtBounds))
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				mainMenuBtnState = 2;
			else mainMenuBtnState = 1;
			if (mainMenuBtnState = 1)
				mainMenu = RED;
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
			{
				playing = false;
				paused = false;
				atEndScreen = false;
				atGamemodeMenu = false;
				backToStart = true;
				atStartMenu = true;
				gameMode = 0;
			}
		}
		else mainMenuBtnState = 0;
		if (mainMenuBtnState == 0)
			if (theme == 0)
				mainMenu = YELLOW;
			else if (theme == 1)
				mainMenu = BLACK;

		if (theme == 0)
			color = YELLOW;
		else if (theme == 1)
			color = BLACK;

		//DrawRectangleLinesEx(pauseMainMenuTxtBounds, 2, WHITE);
		DrawText("PAUSED", GetScreenWidth() / 3 + 20, GetScreenHeight() / 3 - 50, 90, color);
		DrawText("Main Menu", GetScreenWidth() / 3 + 75, GetScreenHeight() / 2 + 50, 50, mainMenu);
	}
};

struct Ball
{
	float x, y;
	float speedX, speedY, baseSpeedX, baseSpeedY;
	float radius;
	Color color = WHITE;
	
	//Drawing circle object
	void Draw()
	{
		if (theme == 1 && !atEndScreen)
			color = BLACK;
		else if (theme == 0 && !atEndScreen)
			color = WHITE;
		DrawCircle(x, y, radius, color);
	}

};

struct Paddle
{
	float x = 0, y = 0;
	float width = 0, height = 0;
	float speed = 0;
	int counter = 0;
	Color color = WHITE;
		  

	// Using a struct to get helps with making multiple rectangle obj for collision etc
	Rectangle GetRectangle()
	{
		return Rectangle{ x - width / 2.0f, y - height / 2.0f, width, height }; // Changes the offset from corner of rect to center
	}
	// Drawing Paddle object
	void Draw()
	{
		if (theme == 1)
			color = BLACK;
		else if (theme == 0)
			color = WHITE;
		DrawRectangleRec(GetRectangle(), color);
	}

	// Paddle Movements

	void LeftPaddleMovement(int ballY, int ballX)
	{
		// AI controls

		if (atStartMenu)
			if (ballY - 25 > y) // If ball is below paddle
			{
				if (ballX < GetScreenWidth() / 4)
					y += 600 * GetFrameTime();
				if (ballX > GetScreenWidth() / 4 && ballX < GetScreenWidth() / 2)
					y += 350 * GetFrameTime();
				if (ballX > GetScreenWidth() / 2 && ballX < GetScreenWidth() - (GetScreenWidth() / 4))
					y += 100 * GetFrameTime();
				if (ballX > GetScreenWidth() - (GetScreenWidth() / 4))
					y += 50 * GetFrameTime();
			}

		if (atStartMenu)
			if (ballY + 35 < y) // If ball is above paddle
			{
				if (ballX < GetScreenWidth() / 4)
					y -= 600 * GetFrameTime();
				if (ballX > GetScreenWidth() / 4 && ballX < GetScreenWidth() / 2)
					y -= 250 * GetFrameTime();
				if (ballX > GetScreenWidth() / 2 && ballX < GetScreenWidth() - (GetScreenWidth() / 4))
					y -= 100 * GetFrameTime();
				if (ballX > GetScreenWidth() - (GetScreenWidth() / 4))
					y -= 50 * GetFrameTime();
			}

		// Player controls
		if (atStartMenu == false && IsKeyDown(KEY_W))
			y -= speed * GetFrameTime();
		if (atStartMenu == false && IsKeyDown(KEY_S))
			y += speed * GetFrameTime();

	}

	void RightPaddleMovement(int ballY, int ballX)
	{


		if (atStartMenu || gameMode == 1)
			if (ballY - 25 > y) // If ball is below paddle
			{
				if (ballX < GetScreenWidth() / 4)
					y += 100 * GetFrameTime();
				if (ballX > GetScreenWidth() / 4 && ballX < GetScreenWidth() / 2)
					y += 150 * GetFrameTime();
				if (ballX > GetScreenWidth() / 2 && ballX < GetScreenWidth() - (GetScreenWidth() / 4))
					y += 300 * GetFrameTime();
				if (ballX > GetScreenWidth() - (GetScreenWidth() / 4))
					y += speed * GetFrameTime();
			}

		if (atStartMenu || gameMode == 1)
			if (ballY + 35 < y) // If ball is above paddle
			{
				if (ballX < GetScreenWidth() / 4)
					y -= 100 * GetFrameTime();
				if (ballX > GetScreenWidth() / 4 && ballX < GetScreenWidth() / 2)
					y -= 150 * GetFrameTime();
				if (ballX > GetScreenWidth() / 2 && ballX < GetScreenWidth() - (GetScreenWidth() / 4))
					y -= 300 * GetFrameTime();
				if (ballX > GetScreenWidth() - (GetScreenWidth() / 4))
					y -= speed * GetFrameTime();
			}


		if (!atStartMenu && gameMode == 0 && IsKeyDown(KEY_UP))
			y -= speed * GetFrameTime();
		if (!atStartMenu && gameMode == 0 && IsKeyDown(KEY_DOWN))
			y += speed * GetFrameTime();
	}
};




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




int main()
{
	const int screenWidth = 1280;
	const int screenHeight = 720;
	//SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Pong!");
	SetTargetFPS(60);
	SetExitKey(KEY_F10);

	InitAudioDevice();
	Music menuMusic = LoadMusicStream(GetCurrentDirectory(1).c_str());
	Sound pongPaddleHit = LoadSound(GetCurrentDirectory(2).c_str());
	Sound pongWallHit = LoadSound(GetCurrentDirectory(3).c_str());
	Sound pongScore = LoadSound(GetCurrentDirectory(4).c_str());
	SetMusicVolume(menuMusic, 0.08f);
	SetSoundVolume(pongPaddleHit, 0.2f);
	SetSoundVolume(pongWallHit, 0.3f);
	SetSoundVolume(pongScore, 0.1f);

	Color backroundColor = BLACK;

	int leftPoints = 0;
	int rightPoints = 0;
	int pointsTowin = 5;

	UI menu;

	Ball ball;
	int ballDirection = 0;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.baseSpeedX = 300;
	ball.baseSpeedY = 373;
	ball.speedX = ball.baseSpeedX;
	ball.speedY = ball.baseSpeedY;
	ball.radius = 5;
	

	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2.0f;
	leftPaddle.width = 10;
	leftPaddle.height = 100;
	leftPaddle.speed = 500;
	leftPaddle.color;

	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 50;
	rightPaddle.y = GetScreenHeight() / 2.0f;
	rightPaddle.width = 10;
	rightPaddle.height = 100;
	rightPaddle.speed = 500;
	rightPaddle.color;

	Color pointsColor = LIGHTGRAY;
	
	int counter = 0;

	// While window is not closed
	while (!WindowShouldClose() && !exitGame) // Game loop
	{
		UpdateMusicStream(menuMusic);

		if (IsKeyPressed(KEY_ESCAPE)) // Setting pause key
			counter++;
		if (backToStart)
			counter = 0;
		paused = (counter % 2) ? true : false;
				
		if (theme == 1)
			backroundColor = WHITE;
		else if (theme == 0)
			backroundColor = BLACK;


		if (!paused) // Ball and paddle motion
		{
		ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();
		leftPaddle.LeftPaddleMovement(ball.y, ball.x);
		rightPaddle.RightPaddleMovement(ball.y, ball.x);
		}

		menu.mousePoint = GetMousePosition(); // Sending mouse xy to startmenu struct


		///////////////////////////
		//   Collision checks   ///
		///////////////////////////

		// Ball constraints
		if (ball.y < 0) // If ball y goes above the top of screen. 0 is the top
		{
			PlaySound(pongWallHit);
			ball.y = 0;
			ball.speedY *= -1;
		}
		if (ball.y > GetScreenHeight()) // If ball y goes below the bottom of screen
		{
			PlaySound(pongWallHit);
			ball.y = GetScreenHeight();
			ball.speedY *= -1;
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRectangle()))
		{
			if (ball.speedX < 0) // If ball is going to the left
			{
				PlaySound(pongPaddleHit);
				ball.x = leftPaddle.x + leftPaddle.width / 2;
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
			}
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRectangle())) // If ball is going to the right
		{
			if (ball.speedX > 0) // If ball is going to the right
			{
				PlaySound(pongPaddleHit);
				ball.x = rightPaddle.x - rightPaddle.width / 2;
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
			}
		}
		// If paddle hits top of screen
		if (CheckCollisionRecs(leftPaddle.GetRectangle(), Rectangle{ 0, -1, (float)GetScreenWidth(), 1 }))
		{
			leftPaddle.y = leftPaddle.height / 2;
		}
		// If paddle hits bottom of screen
		if (CheckCollisionRecs(leftPaddle.GetRectangle(), Rectangle{ 0, (float)GetScreenHeight() + 1, (float)GetScreenWidth(), 1 }))
		{
			leftPaddle.y = GetScreenHeight() - (leftPaddle.height / 2);
		}
		// If top of screen
		if (CheckCollisionRecs(rightPaddle.GetRectangle(), Rectangle{ 0, -1, (float)GetScreenWidth(), 1 }))
		{
			rightPaddle.y = rightPaddle.height / 2;
		}
		// If bottom of screen
		if (CheckCollisionRecs(rightPaddle.GetRectangle(), Rectangle{ 0, (float)GetScreenHeight() + 1, (float)GetScreenWidth(), 1 }))
		{
			rightPaddle.y = GetScreenHeight() - (rightPaddle.height / 2);
		}

		// If ball goes past right of screen,left scores
		if (ball.x > GetScreenWidth())
		{
			PlaySound(pongScore);
			leftPoints++;
			ballDirection++;
			ball.x = GetScreenWidth() / 2.0f;
			ball.y = GetScreenHeight() / 2.0f;
			ball.speedX = (leftPoints > rightPoints) ? ball.baseSpeedX : -ball.baseSpeedX;
			ball.speedY = (ballDirection % 2 == 1) ? -ball.baseSpeedY : ball.baseSpeedY;
			leftPaddle.x = 50;
			leftPaddle.y = GetScreenHeight() / 2.0f;
			rightPaddle.x = GetScreenWidth() - 50;
			rightPaddle.y = GetScreenHeight() / 2.0f;
		}
		// If ball goes past left of screen, right scores
		if (ball.x < 0)
		{
			PlaySound(pongScore);
			rightPoints++;
			ballDirection++;
			ball.x = GetScreenWidth() / 2.0f;
			ball.y = GetScreenHeight() / 2.0f;
			ball.speedX = (leftPoints > rightPoints) ? ball.baseSpeedX : -ball.baseSpeedX;
			ball.speedY = (ballDirection % 2 == 1) ? -ball.baseSpeedY : ball.baseSpeedY;
			leftPaddle.x = 50;
			leftPaddle.y = GetScreenHeight() / 2.0f;
			rightPaddle.x = GetScreenWidth() - 50;
			rightPaddle.y = GetScreenHeight() / 2.0f;
		}
		
		// Restart game if press SPACE or ai wins at main menu
		if ((IsKeyReleased(KEY_SPACE) && (leftPoints >= pointsTowin || rightPoints >= pointsTowin))
			|| ((leftPoints >= pointsTowin || rightPoints >= pointsTowin) && atStartMenu == true))
		{
			backToStart = true;
			atEndScreen = false;
		}


		///////////////////////
		///		DRAWING		///
		///////////////////////

		BeginDrawing();
		ClearBackground(backroundColor);

		if (atStartMenu) // Start menu
		{
			PlayMusicStream(menuMusic);
			
			if (backToStart)
			{
				leftPoints = 0;
				rightPoints = 0;
				ball.x = GetScreenWidth() / 2.0f;
				ball.y = GetScreenHeight() / 2.0f;
				ball.speedX = (ballDirection % 2 == 1) ? -ball.baseSpeedX : ball.baseSpeedX;
				ball.speedY = ball.baseSpeedY;
				ball.x += ball.speedX * GetFrameTime();
				ball.y += ball.speedY * GetFrameTime();
				leftPaddle.x = 50;
				leftPaddle.y = GetScreenHeight() / 2.0f;
				rightPaddle.x = GetScreenWidth() - 50;
				rightPaddle.y = GetScreenHeight() / 2.0f;
				backToStart = false;
			}
			//Drawing objects
			if (atGamemodeMenu && !playing) // If/After player clicks start game
				menu.GameModeMenuDraw();
			
			if(!atGamemodeMenu && !playing)
				menu.StartMenuDraw(); // Reinitialize start menu after leaving other menus

			ball.Draw();
			leftPaddle.Draw();
			rightPaddle.Draw();

			// Showing score
			if (theme == 0)
				pointsColor = LIGHTGRAY;
			else if (theme == 1)
				pointsColor = BLACK;
			DrawText(TextFormat("%i", leftPoints), 250, 100, 80, pointsColor);
			DrawText(TextFormat("%i", rightPoints), 975, 100, 80, pointsColor);

			if (leftPoints >= pointsTowin)
			{
				ball.speedX = 0;
				ball.speedY = 0;
				leftPaddle.x = 50;
				leftPaddle.y = GetScreenHeight() / 2.0f;
				rightPaddle.x = GetScreenWidth() - 50;
				rightPaddle.y = GetScreenHeight() / 2.0f;
				ball.color = backroundColor;
			}
			if (rightPoints >= pointsTowin)
			{
				ball.speedX = 0;
				ball.speedY = 0;
				leftPaddle.x = 50;
				leftPaddle.y = GetScreenHeight() / 2.0f;
				rightPaddle.x = GetScreenWidth() - 50;
				rightPaddle.y = GetScreenHeight() / 2.0f;
				ball.color = backroundColor;
			}

		}

		if (!atStartMenu) // Playing in game
		{
			if (paused)
				menu.PauseScreenDraw();

			if (playing)
			{
				leftPoints = 0;
				rightPoints = 0;
				ball.x = GetScreenWidth() / 2.0f;
				ball.y = GetScreenHeight() / 2.0f;
				ball.speedX = (ballDirection % 2 == 1) ? -ball.baseSpeedX : ball.baseSpeedX;
				ball.speedY = ball.baseSpeedY;
				ball.x += ball.speedX * GetFrameTime();
				ball.y += ball.speedY * GetFrameTime();
				leftPaddle.x = 50;
				leftPaddle.y = GetScreenHeight() / 2.0f;
				rightPaddle.x = GetScreenWidth() - 50;
				rightPaddle.y = GetScreenHeight() / 2.0f;
				playing = false;
			} 

			StopMusicStream(menuMusic);

			ball.Draw();
			leftPaddle.Draw();
			rightPaddle.Draw();

			// Showing score
			if (theme == 0)
				pointsColor = LIGHTGRAY;
			else if (theme == 1)
				pointsColor = BLACK;
			DrawText(TextFormat("%i", leftPoints), 250, 100, 80, pointsColor);
			DrawText(TextFormat("%i", rightPoints), 980, 100, 80, pointsColor);

			if (leftPoints >= pointsTowin)
			{
				atEndScreen = true;
				ball.color = backroundColor;
				ball.speedX = 0;
				ball.speedY = 0;
				leftPaddle.x = 50;
				leftPaddle.y = GetScreenHeight() / 2.0f;
				rightPaddle.x = GetScreenWidth() - 50;
				rightPaddle.y = GetScreenHeight() / 2.0f;
				
				menu.EndScreenDraw(leftPoints, rightPoints);
			}
			if (rightPoints >= pointsTowin)
			{
				atEndScreen = true;
				ball.color = backroundColor;
				ball.speedX = 0;
				ball.speedY = 0;
				leftPaddle.x = 50;
				leftPaddle.y = GetScreenHeight() / 2.0f;
				rightPaddle.x = GetScreenWidth() - 50;
				rightPaddle.y = GetScreenHeight() / 2.0f;

				menu.EndScreenDraw(leftPoints, rightPoints);
			}
		}

		EndDrawing();
	}
	UnloadMusicStream(menuMusic);
	CloseAudioDevice(); // Always close audio device is using audio

	CloseWindow(); // Always close window
	return 0;
}