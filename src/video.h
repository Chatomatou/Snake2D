#ifndef __VIDEO__H__  
#define __VIDEO__H__

#include <SDL2/SDL.h>

typedef struct VideoModule
{
    SDL_Window* window; /* La fenetre */
    SDL_Renderer* renderer; /*Le rendu de la fenetre*/
    Uint32 frameLimit; /*Limitation fps*/
    SDL_bool screenState; /*Etat de la fenetre*/

}VideoModule;

VideoModule* VideoModule_Create(const char* const title, Uint32 const w, Uint32 const h);
void VideoModule_Destroy(VideoModule* videoDataStructure);

void VideoModule_SetFrameLimit(VideoModule* video, Uint32 const frame);
void VideoModule_SetTitle(VideoModule* video, const char* const title);
void VideoModule_SetBackgroundColor(VideoModule* video, Uint8 const r, Uint8 const g, Uint8 const b, Uint8 const a);
void VideoModule_Update(VideoModule* video);
void VideoModule_Display(VideoModule* video);

#endif