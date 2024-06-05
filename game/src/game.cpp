#include <raylib.h>
#include <vector>
#include <anticheat/CAnticheat.hpp>
#include <rlImGui.h>
#include <imgui.h>

int rangeRandomAlg2 (int min, int max){
    int n = max - min + 1;
    int remainder = RAND_MAX % n;
    int x;
    do{
        x = rand();
    }while (x >= RAND_MAX - remainder);
    return min + x % n;
}
class CCoin
{
public:
    CCoin() : position{0, 0} {}
    void Draw()
    {
        DrawCircle(position.x, position.y, 20, BLUE);
    }

    bool &IsPickup()
    {
        return disabled;
    }

    Vector2 &Position()
    {
        return position;
    }

public:
    int value = 1; // a coin = 1 score
    bool disabled = false;

    Vector2 position; // position
};

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
    int &Score() { return this->score; }

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
    CGame() : v2ScreenSize({800,600}) {}
    void Init()
    {
        cplayer = new CPlayer();
        for (int i=0; i<15; i++)
        {
            CCoin coin;
            coin.Position() = {(float)rangeRandomAlg2(20, 700), (float)rangeRandomAlg2(20, 600)};
            vCoins.push_back(coin);
        }
        CAnti::StartAntiCheat();
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
            cplayer->Velocity() = {0, 0};
            cplayer->Position() = {100, 100};
        }
    }

    void ProcessCollision()
    {

    }

    void Update()
    {
        ProcessKeys();
        cplayer->Update();
        for (auto& coin  : vCoins)
        {
            if (!coin.IsPickup())
            {
                if (CheckCollisionCircleRec(coin.Position(), 20, {cplayer->Position().x, cplayer->Position().y, 50, 100}))
                {
                    cplayer->Score()++;
                    coin.IsPickup() = true;

                    // respawn
                    CCoin coin;
                    coin.Position() = {(float)rangeRandomAlg2(20, 700), (float)rangeRandomAlg2(20, 600)};
                    vCoins.push_back(coin);
                }
            }
        }
    }

    void Render()
    {
        cplayer->Draw();
        for (auto& coin : vCoins)
        {
            if (!coin.IsPickup())
                coin.Draw();
        }
        DrawText(TextFormat("Score %d", cplayer->Score()), 500, 30, 30, RED);
    }

    void DrawUI()
    {
        if (ImGui::Begin("AntiCheat Debug Menu"))
        {
            if (ImGui::BeginTabBar("Menu"))
            {
                if (ImGui::BeginTabItem("Handles"))
                {
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::End();
        }
    }

    void Run()
    {
        InitWindow(v2ScreenSize.x, v2ScreenSize.y, "The Game");
        SetTargetFPS(60);

        this->Init();
        rlImGuiSetup(true);
        while (!WindowShouldClose())
        {
            this->Update();
            BeginDrawing();
            ClearBackground(RAYWHITE);
            this->Render();
            EndDrawing();
            rlImGuiBegin();
            this->DrawUI();
            rlImGuiEnd();
        }
        rlImGuiShutdown();
        this->Shutdown();
        CloseWindow();
    }

    void Shutdown()
    {
    }

private:
    CPlayer *cplayer;
    std::vector<CCoin> vCoins;
    Vector2 v2ScreenSize;
};

int main()
{
    CGame *game = new CGame();
    game->Run();
    return 0;
}
