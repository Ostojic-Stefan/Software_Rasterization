#include "window.hpp"

#include "event.hpp"
#include "framebuffer.hpp"

b8 window::init(std::string_view title, i32 width, i32 height) {
  i32 success = SDL_Init(SDL_INIT_VIDEO);
  if (!success) {
    return false;
  }

  window_handle = SDL_CreateWindow(title.data(), width, height, 0);

  if (!window_handle) {
    // log failure
    return false;
  }

  // if (!SDL_SetWindowRelativeMouseMode(window_handle, true)) {
  //   // Log failure
  //   return false;
  // }

  // create renderer
  renderer = SDL_CreateRenderer(window_handle, NULL);
  if (!renderer) {
    // Log failure
    return false;
  }

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                              SDL_TEXTUREACCESS_STREAMING, width, height);

  if (!texture) {
    // Log failure
    return false;
  }

  return true;
}

window::~window() {
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window_handle);
}

void window::process_events() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT: {
      event::Event close_event = {
          .type = event::EventType::AppQuit,
      };
      event::trigger_event(close_event);
    } break;
    case SDL_EVENT_MOUSE_MOTION: {
      math::vec2 pos = {(f32)event.motion.x, (f32)event.motion.y};

      event::MouseMoveData mouse_data{.pos = pos};
      event::Event mouse_move_event = {};
      mouse_move_event.type = event::EventType::MouseMove;
      mouse_move_event.data = {.mouse_move = mouse_data};

      event::trigger_event(mouse_move_event);
      input::process_mouse_move(pos);

      math::vec2 rel = {(f32)event.motion.xrel, (f32)event.motion.yrel};
      input::process_mouse_move_rel(rel);
      break;
    }
    case SDL_EVENT_MOUSE_WHEEL:
      input::process_mouse_wheel((f32)event.wheel.y);
      break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP: {
      b8 is_pressed = event.type == SDL_EVENT_MOUSE_BUTTON_DOWN;

      switch (event.button.button) {
      case SDL_BUTTON_LEFT:
        input::process_mouse(input::mouse_btn::left, is_pressed);
        break;
      case SDL_BUTTON_RIGHT:
        input::process_mouse(input::mouse_btn::right, is_pressed);
        break;
      case SDL_BUTTON_MIDDLE:
        input::process_mouse(input::mouse_btn::middle, is_pressed);
        break;
      }
    } break;
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP: {
      input::key_code code = (input::key_code)event.key.scancode;
      input::process_key(code, event.type == SDL_EVENT_KEY_DOWN);
    } break;
    }
  }
}

void window::display_framebuffer(const framebuffer &fb) {
  SDL_UpdateTexture(texture, NULL, fb.color_buffer.get(),
                    fb.width * sizeof(color));
  SDL_RenderClear(renderer);
  SDL_RenderTexture(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}
