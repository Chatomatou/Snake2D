#include "snake.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

Snake* Snake_Create(VideoModule* video)
{
    Snake* snake = NULL;
    Uint32 length = 0;

    errno = 0;
    snake = malloc(sizeof(*snake));
    if(snake == NULL)
    {
        VideoModule_Destroy(video);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OS can't allocated memory.");
        SDL_Log("%s\n", strerror(errno));
        errno = 0;
        return NULL;
    }
 
    for(length = 0; length < SNAKE_LENGTH; length++)
    {
        snake->texture[length] = SDL_CreateTexture(video->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SNAKE_W, SNAKE_H);

        if(snake->texture[length] == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL can't create texture");
            VideoModule_Destroy(video);
            Snake_DestroySnake(snake);
            return NULL;
        }
        
        SDL_SetRenderTarget(video->renderer, snake->texture[length]);

        if(length == 0)
        {
            VideoModule_SetBackgroundColor(video, 191, 14, 1, 255);
            VideoModule_Update(video);
        }
        else 
        {
            VideoModule_SetBackgroundColor(video, 252, 67, 0, 255);
            VideoModule_Update(video); 
        }
        SDL_SetRenderTarget(video->renderer, NULL);
    }
    SDL_SetRenderTarget(video->renderer, NULL);
    snake->texture[SNAKE_LENGTH] = SDL_CreateTexture(video->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SNAKE_W, SNAKE_H);
    if(snake->texture[SNAKE_LENGTH] == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL can't create texture");
        VideoModule_Destroy(video);
        Snake_DestroySnake(snake);
        return NULL;
    }
    SDL_SetRenderTarget(video->renderer, snake->texture[SNAKE_LENGTH]);
    VideoModule_SetBackgroundColor(video, 0, 255, 0, 255);
    VideoModule_Update(video);
    SDL_SetRenderTarget(video->renderer, NULL);

    srand(time(NULL));
    snake->position[SNAKE_LENGTH].x = SNAKE_W * rand() % SCREEN_WIDTH; 
    snake->position[SNAKE_LENGTH].y = SNAKE_H * rand() % SCREEN_HEIGHT;
    snake->position[SNAKE_LENGTH].w = SNAKE_W;
    snake->position[SNAKE_LENGTH].h = SNAKE_H;
    
    snake->position[0].x = SNAKE_W * 3;
    snake->position[0].y = SNAKE_H * 3;
    snake->position[0].w = SNAKE_W;
    snake->position[0].h = SNAKE_H;
    snake->length = 1;
    snake->vector = RIGHT;

    for(length = 1; length < SNAKE_LENGTH; length++)
    {
        snake->position[length].x = 0;
        snake->position[length].y = 0;
        snake->position[length].w = SNAKE_W;
        snake->position[length].h = SNAKE_H;

        snake->prevPosition[length].x = 0;
        snake->prevPosition[length].y = 0;
        snake->prevPosition[length].w = SNAKE_W;
        snake->prevPosition[length].h = SNAKE_H;    
    }
    return snake;
}
void Snake_DestroySnake(Snake* snakeDataStructure)
{
    Uint32 length = 0;
    for(length = 0; length < SNAKE_LENGTH; length++)
    {
        if(snakeDataStructure->texture[length] != NULL)
        {
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "memory free %p\n", snakeDataStructure->texture[length]);
            SDL_DestroyTexture(snakeDataStructure->texture[length]);
        }
    }

    if(snakeDataStructure != NULL)
    {   
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "memory free %p\n", snakeDataStructure);
        free(snakeDataStructure);
    }
 
}

void Snake_Update(VideoModule* video, Snake* snake)
{
    Uint32 collumns = 0;
    Uint32 rows = 0;
    Uint32 length = 0;

    switch(snake->vector)
    {
        case UP:
            snake->position[0].y -= VECTOR_COMPONENT;
            break;
        case RIGHT:
            snake->position[0].x += VECTOR_COMPONENT;
            break;
        case DOWN:
            snake->position[0].y += VECTOR_COMPONENT;
            break;
        case LEFT:
            snake->position[0].x -= VECTOR_COMPONENT;
            break;
    }

    
    for(length = 0; length < snake->length; length++)
    {
        snake->prevPosition[length].x = snake->position[length].x; 
        snake->prevPosition[length].y = snake->position[length].y; 
    }

    if(Snake_Collision(&snake->position[0], &snake->position[SNAKE_LENGTH]))
    {
        snake->position[SNAKE_LENGTH].x = SNAKE_W * rand() % SCREEN_WIDTH; 
        snake->position[SNAKE_LENGTH].y = SNAKE_H * rand() % SCREEN_HEIGHT;
        snake->length++;
    }

     
    if(Snake_ScreenCollision(&snake->position[0], SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        Snake_DestroySnake(snake);
        snake = Snake_Create(video);
        SDL_Delay(1500);
        return;
    }
 
    for(length = 0; length < snake->length; length++)   
    {
        if(length >= 1)
        {
            if(Snake_Collision(&snake->position[0], &snake->position[length]))
            {
                Snake_DestroySnake(snake);
                snake = Snake_Create(video);
                SDL_Delay(1500);
                return;
            }
        }
    }
    
    for(length = 1; length < snake->length; length++)
    {
        if(length == 1)
        {
            switch(snake->vector)
            {
                case UP:
                    snake->position[length].x = snake->position[length-1].x;
                    snake->position[length].y = snake->position[length-1].y + VECTOR_COMPONENT;
                    break;
                case RIGHT:
                    snake->position[length].x = snake->position[length-1].x - VECTOR_COMPONENT;
                    snake->position[length].y = snake->position[length-1].y;
                    break;
                case DOWN:
                    snake->position[length].x = snake->position[length-1].x;
                    snake->position[length].y = snake->position[length-1].y - VECTOR_COMPONENT;
                    break;
                case LEFT:
                    snake->position[length].x = snake->position[length-1].x + VECTOR_COMPONENT;
                    snake->position[length].y = snake->position[length-1].y;
                    break;
            }
        }
        else 
        {
            snake->position[length].x = snake->prevPosition[length - 1].x; 
            snake->position[length].y = snake->prevPosition[length - 1].y; 
        }
    }

   
    

    VideoModule_SetBackgroundColor(video, 198, 201, 206, 255);

    for(collumns = 0; collumns < SCREEN_WIDTH; collumns += SNAKE_W)
    {
        SDL_RenderDrawLine(video->renderer, collumns, 0, collumns, SCREEN_HEIGHT);
    }

    for(rows = 0; rows < SCREEN_HEIGHT; rows += SNAKE_H)
    {
        SDL_RenderDrawLine(video->renderer, 0, rows, SCREEN_WIDTH, rows);
    }     

    SDL_RenderCopy(video->renderer, snake->texture[SNAKE_LENGTH], NULL, &snake->position[SNAKE_LENGTH]);
    for(length = 0; length < snake->length; length ++)
    {
        SDL_RenderCopy(video->renderer, snake->texture[length], NULL, &snake->position[length]);
    }
}
SDL_bool Snake_Collision(const SDL_Rect* const a, const SDL_Rect* const b)
{
    if( (a->x >= b->x) && (a->x + a->w) <= (b->x + b->w) && (a->y >= b->y) && (a->y + a->h) <= (b->y + b->h)) 
        return SDL_TRUE;
    return SDL_FALSE;
}
SDL_bool Snake_ScreenCollision(const SDL_Rect* const a, Uint32 const w, Uint32 const h)
{
    if(a->x < 0 || a->x + a->w > w || a->y < 0 || a->y + a->h > h)
        return SDL_TRUE;
    return SDL_FALSE;
}