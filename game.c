#include <stdio.h>
#include <raylib.h>

int main(int argc, char *argv[])
{
  InitWindow(1280, 720, "First window");

  while (!WindowShouldClose()) {
   BeginDrawing();

   ClearBackground(SKYBLUE);

   EndDrawing(); 
  }

  CloseWindow();
  
  printf("Hello there, mate!"); 
  return 0;
}
