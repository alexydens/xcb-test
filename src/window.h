/* Include guard */
#if !defined (WINDOW_H)
#define WINDOW_H

/* Base library */
#include <base.h>
#include <logging.h>

/* XCB (the X server C Bindings) */
#include <xcb/xcb.h>

/* Creating a window */
typedef struct {
  xcb_connection_t* _connection;  /* Connection to X server */
  xcb_window_t _id;               /* The X window ID */
  u16 width, height;              /* The window's width and height */
  bool running;                   /* If the window exists (not closed) */
  xcb_intern_atom_reply_t* reply2;/* For window destroy events */
} WindowState;

/* Create a window */
WindowState create_window(const char* title, u16 width, u16 height);
/* Update a window */
void update_window(WindowState* window);
/* Destroy a window */
void destroy_window(WindowState* window);

#endif /* WINDOW_H */
