#include "types.hpp"
#include <SDL3/SDL.h>
#include <string_view>

struct window {
  // Window(std::string_view title, i32 width, i32 height);
  b8 init(std::string_view title, i32 width, i32 height);
  ~window();
  void process_events();
  void display_framebuffer(const struct framebuffer &fb);

private:
  SDL_Window *window_handle = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Texture *texture = nullptr;
};