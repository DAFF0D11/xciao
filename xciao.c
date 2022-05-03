#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define LENGTH(X) (sizeof X / sizeof X[0])

typedef struct {
  const char *class;
  char *inputMask;
  char *inputKey;
  char *outputMask;
  char *outputKey;
} Remap;

static Remap remaps[] = {
 /*       Class      | Input mask  | input key    | Output mask  | Output key */
    {"Chromium", "ControlMask", "j",           "ControlMask", "Prior"},/* Control-j -> Previous tab */
    {"Chromium", "ControlMask", "k",           "ControlMask", "Next"}, /* Control-k -> Next tab */
    {"Chromium", "ControlMask", "h",           "Mod1Mask",    "Home"}, /* Control-h -> Home page */
    {"Chromium", "ControlMask", "d",           "",            "Next"}, /* Control-d -> Page down */
    {"Chromium", "ControlMask", "u",           "",            "Prior"},/* Contorl-u -> Page up */
    {"Chromium", "ControlMask", "bracketleft", "Mod1Mask",    "Left"}, /* Control-[ -> Backwards history */
    {"Chromium", "ControlMask", "bracketright","Mod1Mask",    "Right"},/* Control-] -> Forwards history */
    {"Chromium", "Mod1Mask",    "h",           "",            "Left"}, /* Alt-h -> Left arrow key */
    {"Chromium", "Mod1Mask",    "j",           "",            "Down"}, /* Alt-j -> Down arrow key */
    {"Chromium", "Mod1Mask",    "k",           "",            "Up"},   /* Alt-k -> Up arrow key */
    {"Chromium", "Mod1Mask",    "l",           "",            "Right"},/* Alt-l -> Right arrow key */

    {"Brave-browser", "ControlMask", "j",           "ControlMask", "Prior"},/* Control-j -> Previous tab */
    {"Brave-browser", "ControlMask", "k",           "ControlMask", "Next"}, /* Control-k -> Next tab */
    {"Brave-browser", "ControlMask", "h",           "Mod1Mask",    "Home"}, /* Control-h -> Home page */
    {"Brave-browser", "ControlMask", "d",           "",            "Next"}, /* Control-d -> Page down */
    {"Brave-browser", "ControlMask", "u",           "",            "Prior"},/* Contorl-u -> Page up */
    {"Brave-browser", "ControlMask", "bracketleft", "Mod1Mask",    "Left"}, /* Control-[ -> Backwards history */
    {"Brave-browser", "ControlMask", "bracketright","Mod1Mask",    "Right"},/* Control-] -> Forwards history */
    {"Brave-browser", "Mod1Mask",    "h",           "",            "Left"}, /* Alt-h -> Left arrow key */
    {"Brave-browser", "Mod1Mask",    "j",           "",            "Down"}, /* Alt-j -> Down arrow key */
    {"Brave-browser", "Mod1Mask",    "k",           "",            "Up"},   /* Alt-k -> Up arrow key */
    {"Brave-browser", "Mod1Mask",    "l",           "",            "Right"},/* Alt-l -> Right arrow key */

    {"firefox-nightly", "ControlMask", "j",           "ControlMask", "Prior"},/* Control-j -> Previous tab */
    {"firefox-nightly", "ControlMask", "k",           "ControlMask", "Next"}, /* Control-k -> Next tab */
    {"firefox-nightly", "ControlMask", "h",           "Mod1Mask",    "Home"}, /* Control-h -> Home page */
    {"firefox-nightly", "ControlMask", "d",           "",            "Next"}, /* Control-d -> Page down */
    {"firefox-nightly", "ControlMask", "u",           "",            "Prior"},/* Contorl-u -> Page up */
    {"firefox-nightly", "ControlMask", "bracketleft", "Mod1Mask",    "Left"}, /* Control-[ -> Backwards history */
    {"firefox-nightly", "ControlMask", "bracketright","Mod1Mask",    "Right"},/* Control-] -> Forwards history */
    {"firefox-nightly", "Mod1Mask",    "h",           "",            "Left"}, /* Alt-h -> Left arrow key */
    {"firefox-nightly", "Mod1Mask",    "j",           "",            "Down"}, /* Alt-j -> Down arrow key */
    {"firefox-nightly", "Mod1Mask",    "k",           "",            "Up"},   /* Alt-k -> Up arrow key */
    {"firefox-nightly", "Mod1Mask",    "l",           "",            "Right"},/* Alt-l -> Right arrow key */
};

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

  ev.xkey.type = KeyPress;
  XSendEvent(display, win, True, KeyPressMask, &ev);
  XFlush(display);

  ev.xkey.type = KeyRelease;
  XSendEvent(display, win, True, KeyReleaseMask, &ev);
  XFlush(display);
}

int main(int argc, char **argv) {
  Display *display;
  Window focus;
  int revert;
  XClassHint ch;
  unsigned int i;
  const Remap *r;

  if (argc != 3)
    return 1;

  display = XOpenDisplay(NULL);
  XGetInputFocus(display, &focus, &revert);
  XGetClassHint(display, focus, &ch);

  for (i = 0; i < LENGTH(remaps); i++) {
    r = &remaps[i];
    if (strcmp(getwindowclass(display, focus, ch), r->class) == 0) {
      if ((strcmp(r->inputMask, argv[1]) == 0) &&
          (strcmp(r->inputKey, argv[2]) == 0)) {
        fakepresskey(display, focus, r->outputMask, r->outputKey);
        return 0;
      }
    }
  }

  fakepresskey(display, focus, argv[1], argv[2]);
  return 0;
}
