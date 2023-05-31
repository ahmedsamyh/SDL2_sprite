#include <SDL2/SDL.h>
#include <stdio.h>
#define SPRITE_IMPLEMENTATION
#include "sprite.h"

static unsigned int width = 1280;
static unsigned int height = 720;

int main(int argc, char *argv[]) {
  SDL_Window *win;
  SDL_Renderer *rend;
  float dt;
  Uint64 tp1 = 0, tp2 = 0;
  const char *title = "SDL2_sprite";
  Sprite *spr;

  // create window
  win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         (int)width, (int)height,
                         SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS);
  if (win == NULL) {
    fprintf(stderr, "ERROR: %s\n", SDL_GetError());
    exit(1);
  }

  // create renderer
  rend = SDL_CreateRenderer(
      win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // load sprite
  spr = Sprite_load(rend, "player.png", 2, 4);

  if (spr == NULL) {
    fprintf(stderr, "ERROR: failed to load sprite!\n");
    exit(1);
  }

  spr->scale.x = 8;
  spr->scale.y = 8;

  int speed = 500;

  // game loop
  while (1) {
    // calculate delta time
    tp2 = SDL_GetTicks64();
    dt = (float)(tp2 - tp1) / 1000.f;
    tp1 = tp2;
    int fps = (int)(1 / dt);

    char full_title[256];
    sprintf(full_title, "%s | %.3fs | %dfps", title, dt, fps);
    SDL_SetWindowTitle(win, full_title);

    SDL_Event e;
    // event loop
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        goto quit;
      }

      if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_SPACE) {
          Sprite_change_vframe(spr, spr->vframe + 1);
        }
      }
    }

    // clear
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);

    // get key input
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_DOWN]) {
      spr->pos.y += speed * dt;
    }
    if (keys[SDL_SCANCODE_UP]) {
      spr->pos.y -= speed * dt;
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
      spr->pos.x += speed * dt;
    }
    if (keys[SDL_SCANCODE_LEFT]) {
      spr->pos.x -= speed * dt;
    }

    Sprite_animate(spr, dt);

    Sprite_draw(spr);

    // display
    SDL_RenderPresent(rend);
  }

quit:
  Sprite_free(spr);
  SDL_DestroyWindow(win);
  SDL_DestroyRenderer(rend);
  SDL_Quit();

  return 0;
}
