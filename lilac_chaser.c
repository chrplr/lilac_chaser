/* Display the Lilac Chaser illusion (https://en.wikipedia.org/wiki/Lilac_chaser) */


/* Copyright © Christophe Pallier

   Time-stamp: <2021-04-21 21:10:52 christophe@pallier.org>

   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
   The Software is provided "as is", without warranty of any kind, express or implied, including but not limited to the warranties of merchantability, fitness for a particular purpose and noninfringement. In no event shall the authors or copyright holders be liable for any claim, damages or other liability, whether in an action of contract, tort or otherwise, arising from, out of or in connection with the software or the use or other dealings in the Software.
*/


#include <stdio.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 1024;
const int CENTER_X = SCREEN_WIDTH / 2;
const int CENTER_Y = SCREEN_HEIGHT / 2;

const int n_circles = 12;
const int radius = 40;
const int distance = 300;


void draw_lilac_circle(SDL_Texture *sprite, int n_circles, int radius, int distance, int mask_pos) {

  SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);
  SDL_RenderClear(sdlRenderer);
  SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);

  /* draw fixation cross */
  int length = 20;
  int width = 5;
  int x = CENTER_X;
  int y = CENTER_Y;

  int access, w, h;
  Uint32 format;
  SDL_QueryTexture(sprite, &format, &access, &w, &h);

  thickLineRGBA(sdlRenderer,  x, y - length, x, y + length, width, 0, 0, 0, 255);
  thickLineRGBA(sdlRenderer,  x - length, y, x + length, y, width, 0, 0, 0, 255);

  /* draw sprites */


  for (int k = 0; k < n_circles; k++) {
      x = CENTER_X + (int)(distance * cos(2.0 * M_PI * k / n_circles));
      y = CENTER_Y + (int)(distance * sin(2.0 * M_PI * k / n_circles));
      if (k != mask_pos) {
          SDL_Rect dstrect = { x - w / 2, y - h / 2, w, h };
          SDL_RenderCopy(sdlRenderer, sprite, NULL, &dstrect);
      }
  }

   SDL_RenderPresent(sdlRenderer);
}


int is_relevant_event(void *nada, SDL_Event* event)
{
  if (event->type == SDL_QUIT || event->type == SDL_KEYDOWN)
    return 1;
  return 0;
}


int main() {

    SDL_Init(SDL_INIT_VIDEO);

    sdlWindow = SDL_CreateWindow("Lilac Chaser", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Surface * image = SDL_LoadBMP("blurred_disk.bmp");
    SDL_Texture * sprite = SDL_CreateTextureFromSurface(sdlRenderer, image);

    printf("Use the left and right arrows keys to change the speed.\n\n");

    SDL_SetEventFilter(is_relevant_event, NULL);

    int quit = 0;
    int time_step = 100;
    Uint64 start, now;
    int empty_pos = 0;

    while (!quit) {
        start = SDL_GetPerformanceCounter();

        draw_lilac_circle(sprite, n_circles, radius, distance, empty_pos);
        empty_pos = (empty_pos + 1) % n_circles;

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT: {
                quit = 1;
                break;
            }
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: {
                    quit = 1;
                    break;
                };
                case SDLK_RIGHT: {
                    if (time_step > 16) time_step -= 16;
                    break;
                };
                case SDLK_LEFT: {
                    time_step += 16;
                    break;
                }
                }
            }
        }
        SDL_Delay(time_step);
        now = SDL_GetPerformanceCounter();
        printf("time step = %.1f\n", (double)((now - start) * 1000) / SDL_GetPerformanceFrequency());
    }

    SDL_DestroyTexture(sprite);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);

    SDL_Quit();

    return 0;
}
