#include "raylib.h"

bool isMovementUp(){
    return IsKeyDown(KEY_W) || IsKeyDown(KEY_UP);
}

bool isMovementDown(){
    return IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);
}

bool isMovementLeft(){
    return IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
}

bool isMovementRight(){
    return IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
}

int addBallRadius(int position, int ballRadius){
    return position + ballRadius;
}

int getEdgeValue(int baseValue, int length){
    return baseValue + length;
}

int main(){
    //Window config
    int zero = 0;
    int width = 800;
    int length = 600;
    Color color = RED;

    //Ball config
    int ballX = width / 2;
    int ballY = length / 2;
    float ballRadius = 25.0;
    Color ballColor = BLUE;
    int movSpeed = 10;

    //Ball edges
    int leftBallX = getEdgeValue(ballX, -ballRadius);
    int rightBallX = getEdgeValue(ballX, ballRadius);
    int upBallY = getEdgeValue(ballY, -ballRadius);
    int downBallY = getEdgeValue(ballY, ballRadius);

    //Axe config
    int axeX = 400;
    int axeY = 400;
    int axeLength = 50;
    int direction = 10;
    Color axeColor = YELLOW;

    //Axe edges
    int leftAxeX = getEdgeValue(axeX, zero);
    int rightAxeX = getEdgeValue(axeX, axeLength);
    int upAxeY = getEdgeValue(axeY, zero);
    int downAxeY = getEdgeValue(axeY, axeLength);

    bool collisionWithAxe = (downAxeY >= upBallY) &&
                            (upAxeY <= downBallY) &&
                            (rightAxeX >= leftBallX) &&
                            (leftAxeX <= rightBallX);
 
    InitWindow(width, length, "Axe");

    SetTargetFPS(60);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(color);

        //Ball edges
        leftBallX = getEdgeValue(ballX, -ballRadius);
        rightBallX = getEdgeValue(ballX, ballRadius);
        upBallY = getEdgeValue(ballY, -ballRadius);
        downBallY = getEdgeValue(ballY, ballRadius);

        //Axe edges
        leftAxeX = getEdgeValue(axeX, zero);
        rightAxeX = getEdgeValue(axeX, axeLength);
        upAxeY = getEdgeValue(axeY, zero);
        downAxeY = getEdgeValue(axeY, axeLength);

        collisionWithAxe = (downAxeY >= upBallY) &&
                            (upAxeY <= downBallY) &&
                            (rightAxeX >= leftBallX) &&
                            (leftAxeX <= rightBallX);

        if(collisionWithAxe){
             DrawText("Game Over!", width / 2, length / 2, 20, RAYWHITE);
        } else { 
            //Game logic
            DrawCircle(ballX, ballY, ballRadius, ballColor);
            DrawRectangle(axeX, axeY, axeLength, axeLength, axeColor);

            //Ball movement
            if(isMovementUp() && (ballY - ballRadius) >= zero){
                ballY -= movSpeed;
            }

            if(isMovementDown() && ballY + ballRadius <= length){
                ballY += movSpeed;
            }

            if(isMovementLeft() && ballX - ballRadius >= zero){
                ballX -= movSpeed;
            }

            if(isMovementRight() && ballX + ballRadius <= width){
                ballX += movSpeed;
            }

            //Axe movement
            axeY += direction;
            if(axeY > length || axeY < zero){
                direction = -direction;
            }
        }

        EndDrawing();
    }

}

