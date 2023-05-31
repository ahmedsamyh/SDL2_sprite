#ifndef _SPRITE_SDL2_H_
#define _SPRITE_SDL2_H_

#include <SDL2/SDL.h>

typedef struct {
  SDL_Texture *texture;
  unsigned char *pixels;
  int actual_width, actual_height, components;
  SDL_Rect frame_rect;
  int vframes, hframes, hframe, vframe;
  float x, y;
  int width, height;
  float scale_x, scale_y;
  SDL_FPoint origin;
  float rotation;
  SDL_Renderer *rend_ptr;
} Sprite;

Sprite *Sprite_load(SDL_Renderer *rend, const char *filepath, int hframes,
                    int vframes);
void Sprite_free(Sprite *spr);
void Sprite_draw(Sprite *spr);
void Sprite_update(Sprite *spr, float delta);
void Sprite__update_frame(Sprite *spr);

#endif /* _SPRITE_SDL2_H_ */

//////////////////////////////////////////////////
#ifdef SPRITE_IMPLEMENTATION

Sprite *Sprite_load(SDL_Renderer *rend, const char *filepath, int hframes,
                    int vframes) {
  // allocate sprite on the heap
  Sprite *spr = (Sprite *)SDL_calloc(1, sizeof(Sprite));
  spr->rend_ptr = rend;
  spr->hframes = hframes;
  spr->vframes = vframes;

  // load pixel data from filesystem
  spr->pixels = stbi_load(filepath, &spr->actual_width, &spr->actual_height,
                          &spr->components, 4);

  if (spr->pixels == NULL) {
    fprintf(stderr, "ERROR: could not load pixels from %s!\n", filepath);
    SDL_free(spr);
    return NULL;
  }

  // create texture
  spr->texture =
      SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC,
                        spr->actual_width, spr->actual_height);

  // update texture with pixel data
  if (SDL_UpdateTexture(spr->texture, NULL, spr->pixels,
                        sizeof(unsigned char) * spr->components *
                            spr->actual_width) < 0) {
    fprintf(stderr, "ERROR: %s\n", SDL_GetError());
    SDL_free(spr);
    return NULL;
  }

  // update frame width
  spr->width = (spr->actual_width / spr->hframes);
  spr->height = (spr->actual_height / spr->vframes);
  Sprite__update_frame(spr);

  return spr;
}
void Sprite_free(Sprite *spr) {
  stbi_image_free(spr->pixels);
  SDL_DestroyTexture(spr->texture);
  SDL_free(spr);
}
void Sprite_draw(Sprite *spr) {
  ///
  SDL_FRect dstrect = {.x = spr->x,
                       .y = spr->y,
                       .w = spr->width * spr->scale_x,
                       .h = spr->height * spr->scale_y};
  SDL_RenderCopyExF(spr->rend_ptr, spr->texture, &spr->frame_rect, &dstrect,
                    spr->rotation, &spr->origin, SDL_FLIP_NONE);
}
void Sprite_update(Sprite *spr, float delta) {}
void Sprite__update_frame(Sprite *spr) {
  // just to be safe ^^
  if (spr == NULL || spr->texture == NULL || spr->pixels == NULL)
    return;

  spr->frame_rect.x = (spr->hframe * spr->width);
  spr->frame_rect.y = (spr->vframe * spr->height);
  spr->frame_rect.w = spr->width;
  spr->frame_rect.h = spr->height;
}

#endif