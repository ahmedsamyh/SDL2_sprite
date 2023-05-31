#ifndef _SPRITE_SDL2_H_
#define _SPRITE_SDL2_H_

#include <SDL2/SDL.h>

typedef struct {
  SDL_Texture *texture;
  unsigned char *pixels;
  int actual_width, actual_height, components;
  SDL_Rect frame_rect;
  int vframes, hframes, hframe, vframe;
  float hspeed;
  float x, y;
  int width, height;
  float scale_x, scale_y;
  SDL_FPoint origin;
  float rotation;
  float time_per_each_frame;
  float accumulated_time;
  SDL_Renderer *rend_ptr;
} Sprite;

Sprite *Sprite_load(SDL_Renderer *rend, const char *filepath, int hframes,
                    int vframes);
void Sprite_free(Sprite *spr);
void Sprite_draw(Sprite *spr);
void Sprite_animate(Sprite *spr, const float delta);
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
  spr->hspeed = 1.f;
  spr->scale_x = 1.f;
  spr->scale_y = 1.f;
  spr->rotation = 0.f;
  spr->origin.x = 0.5f;
  spr->origin.y = 0.5f;
  spr->time_per_each_frame = 0.25f;
  spr->accumulated_time = 0.f;

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
  SDL_FRect dstrect = {
      .x = spr->x - (spr->origin.x * (spr->width * spr->scale_x)),
      .y = spr->y - (spr->origin.y * (spr->height * spr->scale_y)),
      .w = spr->width * spr->scale_x,
      .h = spr->height * spr->scale_y};
  SDL_FPoint actual_origin = {.x = spr->origin.x * dstrect.w,
                              .y = spr->origin.y * dstrect.h};
  SDL_RenderCopyExF(spr->rend_ptr, spr->texture, &spr->frame_rect, &dstrect,
                    spr->rotation, &actual_origin, SDL_FLIP_NONE);
}
void Sprite_animate(Sprite *spr, const float delta) {
  spr->accumulated_time += delta * spr->hspeed;
  if (spr->accumulated_time >= spr->time_per_each_frame) {
    spr->accumulated_time -= spr->time_per_each_frame;

    spr->hframe++;
    if (spr->hframe >= spr->hframes)
      spr->hframe = 0;

    Sprite__update_frame(spr);
  }
}
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
