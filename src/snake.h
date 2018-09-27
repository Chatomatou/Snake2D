#ifndef __SNAKE__H__ 
#define __SNAKE__H__

#include "video.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SNAKE_LENGTH 20
#define SNAKE_W 20
#define SNAKE_H 20
#define VECTOR_COMPONENT 20 
#define BOARD_COLLUMNS (SCREEN_WIDTH / SNAKE_W)
#define BOARD_ROWS (SCREEN_HEIGH / SNAKE_H)

typedef enum Movement 
{   
    UP = 0,
    DOWN = 1,
    RIGHT = 2,
    LEFT = 3
}Movement;

typedef struct Snake 
{
    SDL_Texture* texture[SNAKE_LENGTH + 1];
    SDL_Rect position[SNAKE_LENGTH + 1];
    SDL_Rect prevPosition[SNAKE_LENGTH];

    Uint32 length;
    Movement vector;
}Snake;

Snake* Snake_Create(VideoModule* video);
void Snake_DestroySnake(Snake* snakeDataStructure);

void Snake_Update(VideoModule* video, Snake* snake);
SDL_bool Snake_Collision(const SDL_Rect* const a, const SDL_Rect* const b);
SDL_bool Snake_ScreenCollision(const SDL_Rect* const a, Uint32 const w, Uint32 const h);
#endif 