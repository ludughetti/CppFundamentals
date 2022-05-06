#include <raylib.h>

struct AnimData {
    Texture2D texture;
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
    Color color;
    int maxFrame;
};

bool isOnGround(AnimData data, int ground){
    return data.pos.y >= ground;
}

AnimData updateAnimData(AnimData data, float dT){
    if(data.runningTime >= data.updateTime){
        data.runningTime = 0.0;
        data.rec.x = data.frame * data.rec.width;
        data.frame++;

        if(data.frame > data.maxFrame){
            data.frame = 0;
        }   
    }

    return data;
}

float updateBackgrounds(float posX, Texture2D texture, float dT, float scale, 
        float textureY, int textureSpeed, bool collision){
    
    if(!collision){
        posX -= textureSpeed * dT;
    }

    if(posX <= -texture.width * scale){
        posX = 0.0;
    }

    Vector2 background1Pos{posX, textureY};
    DrawTextureEx(texture, background1Pos, 0.0, scale, WHITE);
    
    Vector2 background2Pos{posX + texture.width * scale, textureY};
    DrawTextureEx(texture, background2Pos, 0.0, scale, WHITE);

    return posX;
}

int main() {
    //Window dimensions
    int windowDimensions[2]{768, 550};

    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher!");
    Texture2D victory = LoadTexture("textures/victory-logo.png");
    Texture2D gameOver = LoadTexture("textures/game-over-logo.png");

    //Gravity (pixels/s)/s
    const int gravity = 1000;
    int velocity = 0;

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float backgroundX{};

    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float midgroundX{};

    Texture2D foreground = LoadTexture("textures/foreground.png");
    float foregroundX{};
    
    bool isInAir = false;

    //Jump velocity (pixels * s)
    const int jumpVel = -600;

    //Player config
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    const float ground = windowDimensions[1] - scarfy.height;
    const float updateTime = 1.0 / 12.0;
    const float nebulaDistance = 800.0;

    AnimData scarfyData = {
        scarfy,
        {0.0, 0.0, scarfy.width / 6, scarfy.height}, 
        {scarfy.width / 2 - scarfyData.rec.width / 2, ground},
        0, 
        updateTime, 
        0.0,
        WHITE,
        5
    };

    //Nebula config
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    const int nebulaeSize{1};
    AnimData nebulae[nebulaeSize]{};

    for(int i = 0; i < nebulaeSize; i++){
        AnimData nebulaData = {
            nebula,
            {0.0, 0.0, nebula.width / 8, nebula.height / 8},
            {windowDimensions[0] + i * nebulaDistance, windowDimensions[1] - nebulaData.rec.height},
            0,
            updateTime,
            0.0, 
            WHITE,
            7
        };
        nebulae[i] = nebulaData;
    }

    bool collision{};
    float finishLine{nebulae[nebulaeSize - 1].pos.x + nebulaDistance};

    //Nebula x velocity (pixels/second)
    int nebulaVelocity{-(nebulaDistance / 2)};

    SetTargetFPS(60);
    while(!WindowShouldClose()){
        const float dT = GetFrameTime();

        BeginDrawing();
        ClearBackground(WHITE);
        
        backgroundX = updateBackgrounds(backgroundX, background, dT, 3.0, 0.0, 20, collision);
        midgroundX = updateBackgrounds(midgroundX, midground, dT, 3.0, 0.0, 20, collision);
        foregroundX = updateBackgrounds(foregroundX, foreground, dT, 2.5, foreground.width / 3, 40, collision);
        
        if(!collision && scarfyData.pos.x < finishLine){
            if(scarfyData.pos.y >= ground){
                velocity = 0;
                isInAir = false;
            } else {
                //Apply gravity
                velocity += gravity * dT;
                isInAir = true;
            }

            if(IsKeyPressed(KEY_SPACE) && !isInAir){
                velocity += jumpVel;
            } 
            
            //Update nebula position and running time
            for(int i = 0; i < nebulaeSize; i++){
                nebulae[i].pos.x += nebulaVelocity * dT;
                nebulae[i].runningTime += dT;
            }

            //Update player position
            scarfyData.pos.y += velocity * dT;

            //Update running time
            scarfyData.runningTime += dT;

            //Update animation frame
            if(isOnGround(scarfyData, ground)){
                scarfyData = updateAnimData(scarfyData, dT);
            }

            for(AnimData neb : nebulae){
                float pad{20};
                Rectangle nebRec{ neb.pos.x + pad, neb.pos.y + pad,
                    neb.rec.width - 2 * pad, neb.rec.height - 2 * pad
                };

                Rectangle scarfyRec{ scarfyData.pos.x, scarfyData.pos.y,
                    scarfyData.rec.width, scarfyData.rec.height
                };

                if(CheckCollisionRecs(nebRec, scarfyRec)){
                    collision = true;
                }

                neb = updateAnimData(neb, dT);
                if(!collision){
                    DrawTextureRec(nebula, neb.rec, neb.pos, neb.color);
                }
            }

            //Draw player
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

            //Update finish line
            finishLine += nebulaVelocity * dT;
        } else if(scarfyData.pos.x >= finishLine){
                //Win the game
                Vector2 victoryPos = {0.0, 0.0};
                DrawTextureEx(victory, victoryPos, 0.0, 0.8, WHITE);
        } else {
            //Lose the game
            Vector2 gameOverPos = {-gameOver.width / 4.0, -gameOver.height / 16.0};
            DrawTextureEx(gameOver, gameOverPos, 0.0, 2.0, WHITE);
        }

        EndDrawing();
    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    UnloadTexture(gameOver);

    CloseWindow();
}