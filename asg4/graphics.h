#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <memory>
#include <vector>
using namespace std;

#include <GL/freeglut.h>

#include "rgbcolor.h"
#include "shape.h"

class object {
   private:
      shared_ptr<shape> pshape;
      vertex center;
      rgbcolor color;
      rgbcolor selected_color;
   public:
      bool selected;
      object(const shared_ptr<shape> s, vertex& v, rgbcolor& c);
      // Default copiers, movers, dtor all OK.
      void draw() { 
          if (selected) {
              pshape->draw (center, selected_color); 
          } else {
              pshape->draw (center, color); 
          }
      }
      void move (GLfloat delta_x, GLfloat delta_y) {
         center.xpos += delta_x;
         center.ypos += delta_y;
      }
      void maybe_wrap_around();
};

class mouse {
      friend class window;
   private:
      int xpos {0};
      int ypos {0};
      int entered {GLUT_LEFT};
      int left_state {GLUT_UP};
      int middle_state {GLUT_UP};
      int right_state {GLUT_UP};
   private:
      void set (int x, int y) { xpos = x; ypos = y; }
      void state (int button, int state);
      void draw();
};

class window {
      friend class mouse;
   private:
      static vector<object> objects;
      static size_t selected_object;
      static mouse mus;
   private:
      static void close();
      static void entry (int mouse_entered);
      static void display();
      static void reshape (int width, int height);
      static void keyboard (GLubyte key, int, int);
      static void special (int key, int, int);
      static void motion (int x, int y);
      static void passivemotion (int x, int y);
      static void mousefn (int button, int state, int x, int y);
      static void move_selected_object(GLfloat xpos, GLfloat ypos);
      static void select_object (size_t obj);
   public:
      static int width;         // in pixels
      static int height;        // in pixels
      static int moveby;
      static GLfloat border_thickness;
      static GLfloat selected_border_thickness;
      static rgbcolor border_color;
      static void push_back (const object& obj) {
                  objects.push_back (obj); }
      static void setwidth (int width_) { width = width_; }
      static void setheight (int height_) { height = height_; }
      static void main();
};

#endif

