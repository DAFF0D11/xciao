/* gcc -Wall -Wextra -o xoi xoi.c -lX11 -lXtst */
/* sxhkd ~/.config/bspwm/bspwmsxhkd */
/* change name? xciao XCIAO */
#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

/* #include <X11/extensions/XTest.h> */

void fakepresskey(Display *display, Window win, char *typemask, char *strkey);
char *getwindowclass(Display *display, Window w, XClassHint ch);

int main(int argc, char **argv) {
  Display *display;
  Window focus;
  int revert;
  XClassHint ch;
  // Ensure we have an argument
  if (argc != 6)
    return 1;

  /* xgetfocus.c - link against libX11 */
  display = XOpenDisplay(NULL);
  XGetInputFocus(display, &focus, &revert);
  XGetClassHint(display, focus, &ch);

  if (strcmp(getwindowclass(display, focus, ch), argv[1]) == 0) {
    fakepresskey(display, focus, argv[4],argv[5]);
  } else {
    fakepresskey(display, focus, argv[2],argv[3]);
  }

  return 0;
}

char *getwindowclass(Display *display, Window w, XClassHint ch) {
  XGetClassHint(display, w, &ch);
  return ch.res_class;
}

void fakepresskey(Display *display, Window win, char *typemask, char *strkey) {
  XEvent ev;

  memset(&ev, 0x00, sizeof(ev));
  ev.xkey.display = display;
  ev.xkey.window = win;
  ev.xkey.subwindow = None;

  if (strcmp(typemask, "ControlMask") == 0) {
    ev.xkey.state = ControlMask;
  } else if (strcmp(typemask, "ShiftMask") == 0) {
    ev.xkey.state = ShiftMask;
  } else if (strcmp(typemask, "Mod1Mask") == 0) {
    ev.xkey.state = Mod1Mask;
  } else if (strcmp(typemask, "Mod4Mask") == 0) {
    ev.xkey.state = Mod4Mask;
  }

  ev.xkey.keycode = XKeysymToKeycode(display, XStringToKeysym(strkey));
  ev.xkey.same_screen = True;

  /* press */
  ev.xkey.type = KeyPress;
  XSendEvent(display, win, True, KeyPressMask, &ev);
  XFlush(display);
  /* usleep(100000); */

  /* release */
  ev.xkey.type = KeyRelease;
  XSendEvent(display, win, True, KeyReleaseMask, &ev);
  XFlush(display);
  /* usleep(100000); */
}
