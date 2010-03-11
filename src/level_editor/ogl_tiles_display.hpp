#pragma once

#include <GL/glew.h>

#ifdef WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
#endif

#include <gtkmm.h>
#include "GLArea.hpp"
#include "level.hpp"

namespace Graal {
namespace level_editor {

unsigned int load_texture_from_surface(Cairo::RefPtr<Cairo::ImageSurface>& surface, unsigned int id = 0);

class ogl_tiles_display: public GLArea {
public:
  ogl_tiles_display();
  virtual ~ogl_tiles_display() {}

  void set_tile_size(int tile_width, int tile_height);
  void set_tileset_surface(const Cairo::RefPtr<Cairo::ImageSurface>& surface);
  virtual void set_surface_buffers();

  void clear();

  virtual tile_buf& get_tile_buf() { return m_tile_buf; }
  void set_tile_buf(tile_buf& buf);

  void invalidate();
protected:
  void on_gl_realize();
  bool on_gl_expose_event(GdkEventExpose* event);
  bool on_gl_configure_event(GdkEventConfigure* event);

  virtual void draw_tile(tile& _tile, int x, int y);
  virtual void draw_all();

  void load_tileset(Cairo::RefPtr<Cairo::ImageSurface>& surface);

  sigc::connection m_connection_idle;

  unsigned int m_tileset;
  int m_tile_width, m_tile_height;
  int m_tileset_width, m_tileset_height;

  tile_buf m_tile_buf;

  Cairo::RefPtr<Cairo::ImageSurface> m_new_tileset;
};

}
}
