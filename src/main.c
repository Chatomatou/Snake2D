#include "video.h"
#include "snake.h"
#include <stdio.h>

#define SCREEN_TITLE_LENGTH 256

int main(int argc, char* argv[])
{   
    SDL_Event event;
    Uint32 startClock = 0;
    char screenTitle[SCREEN_TITLE_LENGTH] = {'\0'};

    VideoModule* graphics = NULL;
    Snake* snake = NULL;
    Uint32 length = 0;
    graphics = VideoModule_Create("Snake", 800, 600);
    if(graphics == NULL)
        return -1;
     
    snake = Snake_Create(graphics);
      
    if(snake == NULL)
        return -1;
     
    VideoModule_SetFrameLimit(graphics, 15);
    while(graphics->screenState)
    {
        startClock = SDL_GetTicks();
        VideoModule_SetTitle(graphics, screenTitle);
 
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    graphics->screenState = SDL_FALSE;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_UP:
                            if(snake->vector != DOWN)
                                snake->vector = UP;
                            break;
                        case SDLK_RIGHT:
                            if(snake->vector != LEFT)
                                snake->vector = RIGHT;
                            break;
                        case SDLK_DOWN:
                            if(snake->vector != UP)
                                snake->vector = DOWN;
                            break;
                        case SDLK_LEFT:
                            if(snake->vector != RIGHT)
                                snake->vector = LEFT;
                            break;
                    }
                    break;
            }
        }
         
        VideoModule_SetBackgroundColor(graphics, 66, 215, 244, 255);
        VideoModule_Update(graphics);
         
        Snake_Update(graphics, snake);

        VideoModule_Display(graphics);

        if(1000 / graphics->frameLimit > SDL_GetTicks() - startClock)
        {
            SDL_Delay((1000 / graphics->frameLimit) - (SDL_GetTicks() - startClock));
        }
        sprintf(screenTitle, "SDL 2.0 Snake by Guerrier Numérique : Length %d : %d FPS", snake->length, 1000/(SDL_GetTicks() - startClock));
     }

    Snake_DestroySnake(snake);
    VideoModule_Destroy(graphics);
    return 0;
}