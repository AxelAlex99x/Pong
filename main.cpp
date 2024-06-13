#include <iostream>
#include <C:\raylib\raylib\src\raylib.h>

using namespace std;

int pl_scr=0, cpu_scr=0;

Color Green= Color{20, 185, 140, 255};
Color DarkGreen= Color{25, 156, 120, 255};
Color Red= Color{213, 12, 12, 255};
Color Circle= Color{255, 255, 255, 75};

class Ball{
    public:
    float x, y;
    int speed_x, speed_y, radius;

    void Draw(){
        DrawCircle(x, y, radius, Red);
    }

    void Move(){
        x+= speed_x;
        y+= speed_y;

        if(y + radius >= GetScreenHeight() || y - radius <= 0){
            speed_y *= -1;
        }

    }
 
    void score(){

        if(x + radius >= GetScreenWidth()){
            pl_scr++;
            ResetBall();
        }

        if(x - radius <= 0){
            cpu_scr++;
            ResetBall();
        }

    }

    void ResetBall(){

        x= GetScreenWidth()/2;
        y= GetScreenHeight()/2;

        int speed_choices[2]={-1,1};
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];

    }

};

class Paddle{

    protected:

    void LimitMovement(){
        if(y<=0){
            y=0;
        }

        if(y + height >=GetScreenHeight()){
            y= GetScreenHeight() - height;
        }
    }

    public:

    float x, y, width, height;
    int speed;

    void Draw(){
        //DrawRectangle(x, y, width, height, WHITE); 
        DrawRectangleRounded(Rectangle{x, y, width, height}, 1, 0, WHITE);
    }

    void Move(){

        if(IsKeyDown(KEY_W)){
            y= y - speed;
        }

        if(IsKeyDown(KEY_S)){
            y= y + speed;
        }

        LimitMovement();
    }

};

class CPUpaddle:public Paddle{

    public:

    void Move(int ball_y){

        if(y + height/2 > ball_y){
            y= y-speed;
        }

        if(y + height/2 < ball_y){
            y= y+speed;
        }

        LimitMovement();
    }
};

Ball ball;
Paddle player;
CPUpaddle cpu;

int main () {

    cout<<"Starting\n";

    const int screen_width=1150;
    const int screen_height=725;

    InitWindow(screen_width,screen_height,"Pong Game");
    SetTargetFPS(60);

    ball.radius= 25;
    ball.x= screen_width/2;
    ball.y= screen_height/2;
    ball.speed_x= 7;
    ball.speed_y= 7;

    player.width= 20;
    player.height= 120;
    player.x= 10;
    player.y = screen_height/2 - player.height/2;
    player.speed= 6;

    cpu.width= 20;
    cpu.height= 120;
    cpu.x= screen_width - cpu.width - 10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed= 5;

    while (!WindowShouldClose()) {
        
        BeginDrawing();

        //Moving

        ball.Move();
        player.Move();
        cpu.Move(ball.y);

        //Score update

        ball.score();

        //Collision check

        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{player.x,player.y,player.width,player.height})) {
            ball.speed_x *= -1;
        }

         if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{cpu.x,cpu.y,cpu.width,cpu.height})) {
            ball.speed_x *= -1;
        }

        // Drawing

        ClearBackground(Green); // Clearing
        DrawRectangle(screen_width - screen_width/2, 0, screen_width, screen_height, DarkGreen);
        DrawCircle(screen_width/2, screen_height/2, 150, Circle);
        DrawLine(screen_width / 2 , 0, screen_width / 2, screen_height, WHITE); // Middle line

        ball.Draw();
        player.Draw();
        cpu.Draw();

        // Score display

        DrawText(TextFormat("%i",pl_scr), screen_width/2 - screen_width/4, 15, 50, WHITE);
        DrawText(TextFormat("%i",cpu_scr), screen_width/2 + screen_width/4, 15, 50, WHITE);

        EndDrawing();

    }
    

    CloseWindow();
    return 0;
}