/* Implements window.h */
#include "window.h"

/* Create a window */
WindowState create_window(const char* title, u16 width, u16 height) {
  /* The strust to fill */
  WindowState window;
  log_info(
      "Creating window with title: %s, width: %d, and height: %d",
      title, width, height
  );

  /* Establish connection */
  log("\tConnecting to X server...\n");
  window._connection = xcb_connect(NULL, NULL);
  /* Get first screen */
  log("\tGetting first screen...\n");
  const xcb_setup_t* setup = xcb_get_setup(window._connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
  xcb_screen_t* screen = iter.data;
  /* Create window */
  log("\tActual window creation...\n");
  u32 mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  u32 values[2] = {
    screen->black_pixel,
    XCB_EVENT_MASK_EXPOSURE
    | XCB_EVENT_MASK_KEY_PRESS
  };
  window._id = xcb_generate_id(window._connection);
  xcb_create_window(
      window._connection,                 /* The connection to the X server */
      XCB_COPY_FROM_PARENT,               /* The depth */
      window._id,                         /* The ID */
      screen->root,                       /* The parent window */
      0, 0, width, height,                /* x, y, width, height */
      5,                                  /* Border width */
      XCB_WINDOW_CLASS_INPUT_OUTPUT,      /* Class (some flags?) */
      screen->root_visual,                /* Visual */
      mask, values                        /* Masks */
  );
  /* Set title */
  xcb_change_property(
      window._connection,                 /* Connection */
      XCB_PROP_MODE_REPLACE,              /* Mode */
      window._id,                         /* Window */
      XCB_ATOM_WM_NAME, XCB_ATOM_STRING,  /* Property, type */
      8,                                  /* Format */
      strlen(title),                      /* Size */
      title                               /* Data */
  );
  /* Make sure to detect delete event */
  xcb_intern_atom_cookie_t cookie = xcb_intern_atom(
      window._connection, 1, 12, "WM_PROTOCOLS"
  );
  xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(
      window._connection, cookie, 0
  );
  xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(
      window._connection, 0, 16, "WM_DELETE_WINDOW"
  );
  window.reply2 = xcb_intern_atom_reply(
      window._connection, cookie2, 0
  );
  xcb_change_property(
      window._connection,                 /* Connection */
      XCB_PROP_MODE_REPLACE,              /* Mode */
      window._id,                         /* Window */
      reply->atom, 4,                     /* Property, type */
      32,                                 /* Format */
      1,                                  /* Size */
      &window.reply2->atom                       /* Data */
  );
  /* Map Window to screen */
  xcb_map_window(window._connection, window._id);
  /* Flush commands */
  xcb_flush(window._connection);

  /* Set fields, return */
  window.width = width;
  window.height = height;
  window.running = true;
  return window;
}
/* Update a window */
void update_window(WindowState* window) {
  /* Poll through events */
  xcb_generic_event_t* event;
  while ((event = xcb_poll_for_event(window->_connection))) {
    switch (event->response_type & ~0x80) {
      case XCB_EXPOSE:
        {
          xcb_expose_event_t* e = (xcb_expose_event_t*)event;
          log_info("Resize: %u, %u", e->width, e->height);
          window->width = e->width;
          window->height = e->height;
        } break;
      case XCB_KEY_PRESS:
        log_info("Key down: %p", ((xcb_key_press_event_t*)event)->detail);
        break;
      case XCB_CLIENT_MESSAGE:
        {
          if (
              ((xcb_client_message_event_t*)event)->data.data32[0]
              == window->reply2->atom
          )
            window->running = false;
        } break;
      default: break;
    }
  }
}
/* Destroy a window */
void destroy_window(WindowState* window) {
  log_info("Destroying window...");
  log("\tActual window destruction...\n");
  xcb_destroy_window(window->_connection, window->_id);
  log("\tClosing connection...\n");
  xcb_disconnect(window->_connection);
  /* Close connection */
  window->_connection = NULL;
  window->_id = 0;
  window->width = 0;
  window->height = 0;
  window->running = false;
}
