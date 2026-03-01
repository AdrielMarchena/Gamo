#include "engine/platform/window.h"

int main(void) {
  EngineWindow *window = engine_window_create();
  if (!window) {
    return -1;
  }

  while (!engine_window_should_close(window)) {
    engine_gl_basic_clear_for_test();

    engine_window_swap_buffers(window);
    engine_window_poll_events(window);
  }

  engine_window_destroy(window);
  return 0;
}
