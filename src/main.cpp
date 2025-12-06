#include <iostream>
#include <raylib.h>

using namespace std;

int playerScore = 0;
int cpuScore = 0;

Color Purple = Color{128, 0, 128, 255};
Color LightPurple = Color{200, 50, 200, 255};
Color DarkPurple = Color{50, 10, 50, 255};
Color VLightPurple = Color{255, 200, 255, 255};

class Paddle
{
protected:
    void limitMovement()
    {
        if (y < 0)
        {
            y = 0;
        }

        if (y + height > GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;
    Color color;

    void draw()
    {
        DrawRectangle(x, y, width, height, color);
    }

    void update()
    {
        if (IsKeyDown(KEY_UP))
        {
            y -= speed;
        }

        if (IsKeyDown(KEY_DOWN))
        {
            y += speed;
        }

        limitMovement();
    }
};

class CPUPaddle: public Paddle
{   
public:
    void update(int ballHeight)
    {
        if (y + height/2 < ballHeight)
        {
            y -= speed;
        }

        if (y + height/2 > ballHeight)
        {
            y += speed;
        }

        if (y < 0)
        {
            y = 0;
        }

        if (y + height > GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }

        limitMovement();
    }
};

class Ball
{
    public:
    float x, y;
    float radius;
    float speedX;
    float speedY;
    Color color;

    void draw()
    {
        DrawCircle(x, y, radius, color);
    }

    void update()
    {
        x += speedX;
        y += speedY;

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speedY *= -1;
        }

        if (x + radius >= GetScreenWidth())
        {
            resetPosition();
            playerScore++;
        }

        if (x - radius <= 0)
        {
            resetPosition();
            cpuScore++;
        }
    }

    void resetPosition()
    {
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int directions[2] = {-1, 1};
        speedX *= directions[GetRandomValue(0,1)];
        speedY *= directions[GetRandomValue(0,1)];
    }
};

Ball ball;
Paddle player;
CPUPaddle cpu;

int main()
{
    ////////// Loading variables //////////
    // Window Variables
    const int windowHeight = 500;
    const int windowWidth = 900;
    InitWindow(windowWidth, windowHeight, "Game");
    SetTargetFPS(60);

    // Player Variables
    int border = 10;

    player.width = 20;
    player.height = 70;
    player.x = border;
    player.y = windowHeight/2-(player.height/2);
    player.speed = 12;
    player.color = LightPurple;

    cpu.width = 20;
    cpu.height = 70;
    cpu.x = GetScreenWidth() - cpu.width - border;
    cpu.y = windowHeight/2-(cpu.height/2);
    cpu.speed = 12;
    cpu.color = LightPurple;

    // Ball variables
    ball.x = windowWidth/2;
    ball.y = windowHeight/2;
    ball.radius = 20;
    ball.speedX = 10;
    ball.speedY = 10;
    ball.color = VLightPurple;

    ////////// Game Loop //////////
    while (WindowShouldClose() == false)
    {
        // Updating positions
        player.update();
        cpu.update(ball.y);
        ball.update();

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
        {
            ball.speedX *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
        {
            ball.speedX *= -1;
        }

        // Drawing to screen
        BeginDrawing();
            ClearBackground(DarkPurple);
            DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), LightPurple);

            // Drawing players
            player.draw();
            cpu.draw();

            // Drawing ball
            ball.draw();

            // Drawing score
            DrawText(TextFormat("%i", cpuScore), GetScreenWidth()/4 * 3, 20, 40, LightPurple);
            DrawText(TextFormat("%i", playerScore), GetScreenWidth()/4, 20, 40, LightPurple);

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}