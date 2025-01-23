#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <raylib.h>

const int window_width = 1280;
const int window_height = 720;

const int rectangle_width = 50;
const int rectangle_height = 150;

int player_score = 0;
int cpu_score = 0;

typedef struct Ball{
  float x_pos, y_pos;
  float x_speed, y_speed;
  int radius;

  void (*draw_ball)(struct Ball* ball);
  void (*update_ball)(struct Ball* ball);
} Ball;

void drawBall(Ball* ball) {
  DrawCircle(ball->x_pos, ball->y_pos, ball->radius, RED);
}

void updateBall(Ball* ball) {
  ball->x_pos = ball->x_pos + ball->x_speed * GetFrameTime();
  ball->y_pos = ball->y_pos + ball->y_speed * GetFrameTime();

  if (ball->y_pos + ball->radius >= window_height || ball->y_pos - ball->radius <= 0) {
    ball->y_speed *= -1;
  }
  if (ball->x_pos + ball->radius >= window_width) {
    ball->x_speed *= -1;
    player_score++;
    ball->x_pos = window_width / 2;
    ball->y_pos = window_height / 2;
  }
  if (ball->x_pos - ball->radius <= 0) {
    ball->x_speed *= -1;
    cpu_score++;
    ball->x_pos = window_width / 2;
    ball->y_pos = window_height / 2;
  }
}

typedef struct Paddle {

  float x_pos, y_pos;
  float width, height;
  float speed;

  void (*draw_paddle)(struct Paddle* paddle);
  void (*update_paddle)(struct Paddle* paddle, Ball* ball);

  bool is_cpu;
} Paddle;

void drawPaddle(Paddle* paddle) {
  DrawRectangle(paddle->x_pos, paddle->y_pos, paddle->width, paddle->height, YELLOW);
}

void updatePaddle(Paddle* paddle, Ball* ball) {
  assert(paddle);

  if (paddle->is_cpu == false) {
    if (IsKeyDown(KEY_UP)) {
      paddle->y_pos = paddle->y_pos - paddle->speed * GetFrameTime();
    }
    if (IsKeyDown(KEY_DOWN)) {
      paddle->y_pos = paddle->y_pos + paddle->speed * GetFrameTime();
    }
  } else {
    assert(ball);

    if (paddle->y_pos + paddle->height/2 > ball->y_pos) {
      paddle->y_pos = paddle->y_pos - paddle->speed * GetFrameTime();
    } else {
      paddle->y_pos = paddle->y_pos + paddle->speed * GetFrameTime();
    }
  }
  
  if (paddle->y_pos <= 0) {
    paddle->y_pos = 0;
  }
  if (paddle->y_pos + paddle->height >= window_height) {
    paddle->y_pos = window_height - paddle->height;
  }
  
}

int main(int argc, char *argv[]) {
  InitWindow(window_width, window_height, "First window");
  SetTargetFPS(60);

  Ball ball;

  ball.x_pos = window_width/2; 
  ball.y_pos = window_height/2;
  ball.radius = 25;
  ball.x_speed = 400;
  ball.y_speed = 400;
  ball.draw_ball = drawBall;
  ball.update_ball = updateBall;

  Paddle paddle_left;
  paddle_left.width = rectangle_width;
  paddle_left.height = rectangle_height;
  paddle_left.x_pos = 10;
  paddle_left.y_pos = (window_height/2 - rectangle_height/2);
  paddle_left.speed = 250;
  paddle_left.draw_paddle = drawPaddle;
  paddle_left.update_paddle = updatePaddle;
  paddle_left.is_cpu = false;

  Paddle paddle_right;
  paddle_right.width = rectangle_width;
  paddle_right.height = rectangle_height;
  paddle_right.x_pos = (window_width - rectangle_width) - 10;
  paddle_right.y_pos = (window_height/2 - rectangle_height/2);
  paddle_right.speed = 250;
  paddle_right.draw_paddle = drawPaddle;
  paddle_right.update_paddle = updatePaddle;
  paddle_right.is_cpu = true;

  while (!WindowShouldClose()) {
    BeginDrawing();

    ball.update_ball(&ball);
    paddle_left.update_paddle(&paddle_left, NULL);
    paddle_right.update_paddle(&paddle_right, &ball);

    if (CheckCollisionCircleRec((Vector2){ball.x_pos, ball.y_pos}, ball.radius, (Rectangle){paddle_left.x_pos, paddle_left.y_pos, paddle_left.width, paddle_left.height})) {
      ball.x_speed *= -1;
    }
    if (CheckCollisionCircleRec((Vector2){ball.x_pos, ball.y_pos}, ball.radius, (Rectangle){paddle_right.x_pos, paddle_right.y_pos, paddle_right.width, paddle_right.height})) {
      ball.x_speed *= -1;
    }

    ClearBackground(BLACK);

    ball.draw_ball(&ball);
    paddle_left.draw_paddle(&paddle_left);
    paddle_right.draw_paddle(&paddle_right);

    DrawLine(window_width/2, 0, window_width/2, window_height, BLUE);

    DrawText(TextFormat("%i", player_score), window_width * 3/4, 10, 40, SKYBLUE);
    DrawText(TextFormat("%i", cpu_score), window_width / 4, 10, 40, SKYBLUE);
    EndDrawing(); 
  }

  CloseWindow();
  
  printf("Hello there, mate!"); 
  return 0;
}
