#pragma once

#include <gtkmm.h>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include "level.hpp"
#include "tileset.hpp"
#include "preferences.hpp"
#include "undo_buffer.hpp"
#include "image_cache.hpp"
#include "ogl_tiles_display.hpp"
#include "ogl_texture_cache.hpp"

#include "level_map.hpp"

namespace Graal {
namespace level_editor {

class level_display: public ogl_tiles_display {
public:
  typedef std::vector<bool> layer_visibility_list_type;
  level_display(preferences& _prefs, image_cache& cache, int default_tile_index = 0);
  virtual ~level_display() {}

  void set_default_tile(int tile_index);

  void update_selection();

  // writes the selection to the level
  void save_selection();
  tile_buf selection;
  level::npc_list_type::iterator selected_npc;
  void clear_selection();
  void delete_selection();
  void lift_selection();
  void grab_selection();

  // Level loading/etc.
  void new_level(int fill_tile);
  const boost::filesystem::path& get_level_path() const {
    return ""; // TODO: NYI
  }
  void set_level_path(const boost::filesystem::path& new_path);
  void save_level();
  void save_level(const boost::filesystem::path& path);

  void set_level(Graal::level* _level);
  // Sets the used level map
  void set_level_map(level_map* _level_map);
  void load_level(const boost::filesystem::path& file_path);

  void set_selection(const Graal::npc& npc);
  bool in_selection(int x, int y);

  // Return the currently active level
  const boost::shared_ptr<level>& get_level();

  // Create new npc at mouse
  Graal::npc& drag_new_npc();
  // TODO: use c++0x move semantics, tiles will be swapped with the
  // current selection and then hopefully discarded
  void drag_selection(tile_buf& tiles, int drag_x, int drag_y);
  void drag_selection(level::npc_list_type::iterator npc_iter);

  inline int to_tiles_x(int x);
  inline int to_tiles_y(int y);

  // TODO: no
  bool npc_selected() { return (selected_npc != get_level()->npcs.end()); }
  bool is_selecting() { return m_selecting; }
  bool has_selection() { return !selection.empty() || m_select_width > 0 || npc_selected(); }
  int select_x() { return m_select_x; }
  int select_y() { return m_select_y; }
  int select_tile_x() { return m_select_x / m_tile_width; }
  int select_tile_y() { return m_select_y / m_tile_height; }
  int select_width() { return m_select_width; }
  int select_height() { return m_select_height; }

  level_editor::undo_buffer undo_buffer;
  level_editor::undo_buffer redo_buffer;

  void undo();
  void redo();

  typedef sigc::signal<void, int> signal_default_tile_changed_type;
  signal_default_tile_changed_type& signal_default_tile_changed();

  typedef sigc::signal<void, const Glib::ustring&>
    signal_title_changed_type;
  signal_title_changed_type& signal_title_changed();

  typedef sigc::signal<void, bool> signal_unsaved_status_changed_type;
  signal_unsaved_status_changed_type& signal_unsaved_status_changed();

  typedef sigc::signal<void, const std::string&> signal_status_update_type;
  signal_status_update_type& signal_status_update();

  void flood_fill(int tx, int ty, int fill_with_index);

  // Create a link from the current selection (but don't add it)
  link create_link();

  void add_undo_diff(basic_diff* diff);

  void set_unsaved(bool new_unsaved);
  bool get_unsaved();

  void set_active_layer(int layer);
  int get_active_layer() { return m_active_layer; };

  virtual tile_buf& get_tile_buf();
  Cairo::RefPtr<Cairo::ImageSurface> render_level();

  void set_layer_visibility(std::size_t layer, bool visible);
  bool get_layer_visibility(std::size_t layer);
protected:
  void draw_tiles();
  void draw_selection();
  void draw_misc();
  virtual void draw_all();
  
  void setup_buffers();

  void draw_rectangle(float x, float y, float width, float height, float r, float g, float b, float a = 1.0, bool fill = false);
  //virtual bool on_expose_event(GdkEventExpose* event);
  void on_button_pressed(GdkEventButton* event);
  void on_button_released(GdkEventButton* event);
  void on_button_motion(GdkEventMotion* event);
  void on_mouse_leave(GdkEventCrossing* event);

  int m_current_level_x, m_current_level_y;
  boost::shared_ptr<level_map> m_level_map;

  preferences& m_preferences;

  // selection stuff
  bool m_selecting;
  int m_select_x, m_select_y, m_select_width, m_select_height;
  
  // currently creating a new npc
  bool m_new_npc;
  // drag stuff
  bool m_dragging;
  // in pixel:
  int m_drag_start_x, m_drag_start_y;
  // mouse offset from the selection origin
  int m_drag_mouse_x, m_drag_mouse_y;

  layer_visibility_list_type m_layer_visibility;
private:
  virtual void set_surface_buffers();

  int m_active_layer;
  bool m_unsaved;

  int m_default_tile_index;

  image_cache& m_image_cache;
  ogl_texture_cache m_texture_cache;

  signal_default_tile_changed_type m_signal_default_tile_changed;
  signal_title_changed_type m_signal_title_changed;
  signal_unsaved_status_changed_type m_signal_unsaved_status_changed;
  signal_status_update_type m_signal_status_update;

  unsigned int m_position_buffer;
  unsigned int m_texcoord_buffer;

  // Vertex data structures
  struct vertex_position {
    vertex_position(int _x, int _y): x(_x), y(_y) {}
    int x, y;
  };

  struct vertex_texcoord {
    vertex_texcoord(float _u, float _v): u(_u), v(_v) {}
    float u, v;
  };

  // Store vertices here in case of no VBO support
  std::vector<vertex_position> m_positions;
  bool m_use_vbo;
};

}
}
