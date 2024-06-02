#include <raylib.h>

// the player lmao
class CPlayer
{
public:
    CPlayer() : score(0), position({100, 100}), size({50, 100}), velocity({0, 0}) {}

    void Draw()
    {
        DrawRectangle(position.x, position.y, size.x, size.y, RED); // our player is a square for now, too lazy to use a sprite let's fix it one day
    }

    void Update()
    {
        position.x += velocity.x;
        position.y += velocity.y;
    }

    Vector2 &Velocity() { return this->velocity; }
    Vector2 &Position() { return this->position; }

private:
    int score;        // amount of collected coins
    Vector2 position; // position on screen
    Vector2 size;     // player size
    Vector2 velocity; // player speed
};

/*Write the fucking game here  ez*/
class CGame
{
public:
    void Init()
    {
        cplayer = new CPlayer();
    }
    void ProcessKeys()
    {
        // we dont use IsKeyDown cuz it drops all other keys
        if (IsKeyDown(KEY_D))
        {
            cplayer->Velocity().x += 100 * GetFrameTime();
        }
        else if (IsKeyDown(KEY_A))
        {
            cplayer->Velocity().x -= 100 * GetFrameTime();
        }
        else if (IsKeyDown(KEY_W))
        {
            cplayer->Velocity().y -= 100 * GetFrameTime();
        }
        else if (IsKeyDown(KEY_S))
        {
            cplayer->Velocity().y += 100 * GetFrameTime();
        }
        else
        {
            cplayer->Velocity() = {0, 0};
        }

        if (IsKeyPressed(KEY_R))
        {
            cplayer->Velocity() = {0,0};
            cplayer->Position() = {100,100};
        }
    }

    void Update()
    {
        ProcessKeys();
        cplayer->Update();
    }

    void Render()
    {
        cplayer->Draw();
    }

    void Run()
    {
        InitWindow(800, 600, "The Game");
        SetTargetFPS(60);

        this->Init();

        while (!WindowShouldClose())
        {
            this->Update();
            BeginDrawing();
            ClearBackground(RAYWHITE);
            this->Render();
            EndDrawing();
        }
        this->Shutdown();
        CloseWindow();
    }

    void Shutdown()
    {
    }

private:
    CPlayer *cplayer;
};

int main()
{
    CGame *game = new CGame();
    game->Run();
    return 0;
}