#include "video.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>

VideoModule* VideoModule_Create(const char* const title, Uint32 const w, Uint32 const h)
{
    VideoModule* video = NULL;
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL can't init the sub system video.");
        SDL_Log("%s\n", SDL_GetError());
        return NULL;
    }
    errno = 0;
    video = malloc(sizeof(*video));

    if(video == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OS can't allocated memory.");
        SDL_Log("%s\n", strerror(errno));
        VideoModule_Destroy(video);
        errno = 0;
        return NULL;
    } 

    if(SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_SHOWN, &video->window, &video->renderer) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL can't create window and renderer.");
        SDL_Log("%s\n", SDL_GetError());
        VideoModule_Destroy(video);
    }
    video->frameLimit = 60;
    video->screenState = SDL_TRUE;
    VideoModule_SetTitle(video, title);
    return video;
}
void VideoModule_Destroy(VideoModule* videoDataStructure)
{
    if(videoDataStructure->window != NULL)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "memory free %p\n", videoDataStructure->window);
        SDL_DestroyWindow(videoDataStructure->window);
    }

    if(videoDataStructure->renderer != NULL)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "memory free %p\n", videoDataStructure->renderer);
        SDL_DestroyRenderer(videoDataStructure->renderer);
    }
    
    if(videoDataStructure != NULL)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "memory free %p\n", videoDataStructure);
        free(videoDataStructure);
    }
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
void VideoModule_SetFrameLimit(VideoModule* video, Uint32 const frame)
{
    if(frame > 0)
        video->frameLimit = frame;
}
void VideoModule_SetTitle(VideoModule* video, const char* const title)
{
    SDL_SetWindowTitle(video->window, title);
}
void VideoModule_SetBackgroundColor(VideoModule* video, Uint8 const r, Uint8 const g, Uint8 const b, Uint8 const a)
{
    if(SDL_SetRenderDrawColor(video->renderer, r, g, b, a) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL can't set background color.");
        SDL_Log("%s\n", SDL_GetError());
        video->screenState = SDL_FALSE; 
    }
}
void VideoModule_Update(VideoModule* video)
{
    if(SDL_RenderClear(video->renderer) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL can't update screen.");
        SDL_Log("%s\n", SDL_GetError());
        video->screenState = SDL_FALSE;
    }
}
void VideoModule_Display(VideoModule* video)
{
    SDL_RenderPresent(video->renderer);
}