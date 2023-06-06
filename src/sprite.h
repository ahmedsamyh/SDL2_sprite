#ifndef _SPRITE_SDL2_H_
#define _SPRITE_SDL2_H_

#include <SDL2/SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdio.h>
#ifndef VEC_IMPLEMENTATION
#define VEC_IMPLEMENTATION
#include <vec.h>
#endif

typedef struct {
  SDL_Texture *texture;
  unsigned char *pixels;
  int actual_width, actual_height, components;
  SDL_Rect frame_rect;
  int vframes, hframes, hframe, vframe;
  float hspeed;
  Vec2f pos;
  int width, height;
  Vec2f scale;
  SDL_FPoint origin;
  float rotation;
  float time_per_each_frame;
  float accumulated_time;
  SDL_Renderer *rend_ptr;
} Sprite;

Sprite *Sprite_load(SDL_Renderer *rend, const char *filepath, int hframes,
                    int vframes);

/**
 * Loads texture data from a file and initializes a sprite.
 *
 * \param rend the SDL_Renderer that will be used to render the sprite
 * \param filepath the path of the image to load
 * \param hframes total number of horizontal frames
 * \param vframes total number of vertical frames
 * \returns a pointer to a Sprite that is allocated on the heap (which is freed
 * using Sprite_free)
 *
 * \sa Sprite_free
 */
void Sprite_free(Sprite *spr);
/**
 * Frees the pixel data, texture and the sprite itself.
 *
 * \param spr the Sprite to be freed
 */
void Sprite_draw(Sprite *spr);
/**
 * Draw the sprite on the SDL_Renderer that is passed to Sprite_load
 *
 * \param spr the Sprite to draw
 *
 * \sa Sprite_load
 */
void Sprite_animate(Sprite *spr, const float delta);
/**
 * Animate the sprite.
 *
 * \param spr the Sprite to animate
 * \param delta the delta time that is used to animate (for smoothness in
 * varying fps)
 *
 * This function only animates the hframe of the Sprite. The default time for
 * each frame is 0.25s, to change the speed, change the hspeed variable of the
 * Sprite object. vframe is only to change the 'state' of a Sprite. Use
 * Sprite_change_vframe() to change it. **DO NOT CHANGE THE vframe OF THE SPRITE
 * MANUALLY**.
 *
 * \sa Sprite_change_vframe
 */
void Sprite_change_hframe(Sprite *spr, int hframe);
/**
 * Change the hframe of the Sprite.
 *
 * \param spr the Sprite to change the hframe of
 * \param hframe the hframe to change to
 *
 * Do not change the hframe manually, use this function instead. Because the
 * frame_rect also needs to be updated.
 *
 * \sa Sprite_change_vframe
 */
void Sprite_change_vframe(Sprite *spr, int vframe);
/**
 * Change the vframe of the Sprite.
 *
 * \param spr the Sprite to change the vframe of
 * \param vframe the vframe to change to
 *
 * Do not change the vframe manually, use this function instead. Because the
 * frame_rect also needs to be updated.
 *
 * \sa Sprite_change_hframe
 */

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
  spr->scale.x = 1.f;
  spr->scale.y = 1.f;
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
  Sprite_change_hframe(spr, spr->hframe);

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
      .x = spr->pos.x - (spr->origin.x * (spr->width * spr->scale.x)),
      .y = spr->pos.y - (spr->origin.y * (spr->height * spr->scale.y)),
      .w = spr->width * spr->scale.x,
      .h = spr->height * spr->scale.y};
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

    Sprite_change_hframe(spr, spr->hframe);
  }
}
void Sprite_change_hframe(Sprite *spr, int hframe) {
  // just to be safe ^^
  if (spr == NULL || spr->texture == NULL || spr->pixels == NULL)
    return;

  if (hframe < 0)
    hframe = spr->hframes - 1;
  if (hframe > spr->hframes - 1)
    hframe = 0;
  spr->hframe = hframe;
  spr->frame_rect.x = (spr->hframe * spr->width);
  spr->frame_rect.y = (spr->vframe * spr->height);
  spr->frame_rect.w = spr->width;
  spr->frame_rect.h = spr->height;
}
void Sprite_change_vframe(Sprite *spr, int vframe) {
  // just to be safe ^^
  if (spr == NULL || spr->texture == NULL || spr->pixels == NULL)
    return;

  if (vframe < 0)
    vframe = spr->vframes - 1;
  if (vframe > spr->vframes - 1)
    vframe = 0;
  spr->vframe = vframe;
  spr->frame_rect.x = (spr->hframe * spr->width);
  spr->frame_rect.y = (spr->vframe * spr->height);
  spr->frame_rect.w = spr->width;
  spr->frame_rect.h = spr->height;
}
#endif
