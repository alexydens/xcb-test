/* Base library */
#include <base.h>
#include <logging.h>

/* Project headers */
#include "window.h"
#include "graphics.h"

/* Entry point */
i32 main() {
  /* Create window */
  WindowState window = create_window("XCB Window", 1280, 720);
  /* Initialize graphics */
  GraphicsState graphics_state = init_graphics();
  /* Main loop */
  while (window.running) {
    /* Event loop */
    update_window(&window);
  }
  /* Quit graphics */
  quit_graphics(&graphics_state);
  /* Destroy window */
  destroy_window(&window);
  return 0;
}
