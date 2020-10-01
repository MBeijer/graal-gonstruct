#ifndef GRAAL_LEVEL_EDITOR_WINDOW_HPP_
#define GRAAL_LEVEL_EDITOR_WINDOW_HPP_

#include <gtkmm.h>
#include "level.hpp"
#include "level_display.hpp"
#include "tileset_display.hpp"
#include "filesystem.hpp"
#include "default_tile_display.hpp"
#include "preferences_display.hpp"
#include "copy_cache.hpp"
#include "tile_objects_display.hpp"

#include "window/header.hpp"
#include "window/file_commands.hpp"
#include "window/edit_commands.hpp"
#include "window/level_commands.hpp"

namespace Graal {
  namespace level_editor {
    class toolbar_tools_display;
    class layers_control;

    class window: public Gtk::Window {
    public:
      typedef std::map<std::string, tileset> tileset_map_type;

      void load_level(const boost::filesystem::path& file_path, bool activate = true);
      level_display* get_nth_level_display(int n);
      level_display* get_current_level_display();
      void set_current_level(level_display& display, int level_x, int level_y);
      void create_new_page(level_display& display, bool activate = true);

      void update_matching_level_displays(const std::string& prefix);
      void on_tileset_update(const Cairo::RefPtr<Cairo::ImageSurface>& surface);

      const boost::shared_ptr<level>& get_current_level();
      void display_error(const Glib::ustring& message);

      void set_status(const std::string& text);
      void set_level_buttons(bool enabled = true);
      void tiles_selected(tile_buf& selection, int x, int y);
      void set_default_tile(int tile_index);

      tile_buf get_current_tile_selection();

      Cairo::RefPtr<Cairo::ImageSurface> get_image(const std::string& file_name);

      explicit window(preferences& _prefs);

      ~window() override;

      default_tile_display default_tile;
      tileset_map_type tilesets;
      filesystem fs;

      preferences_display prefs_display;
      boost::shared_ptr<basic_cache> copy_cache;

      typedef sigc::signal<void, level_display&> signal_switch_level_display_type;
      signal_switch_level_display_type& signal_switch_level_display();

      bool close_all_levels();
      // TODO: dirty hack
      // True if we're in the middle of opening level[s] and don't want update_all spam
      bool opening_levels;

      bool save_current_page();
      bool save_current_page_as();

      std::unique_ptr<level_display> create_level_display();
    protected:
      class tab_label: public Gtk::HBox {
      public:
        explicit tab_label(const Glib::ustring& label);
        ~tab_label() override;
        typedef Glib::SignalProxy0<void> signal_proxy_close;

        signal_proxy_close close_event();
        void set_label(const Glib::ustring& label);
        void set_unsaved_status(bool status);
      protected:
        Gtk::Label m_label;
        Gtk::Label m_unsaved;
        Gtk::Button m_button;
      };

      image_cache m_image_cache;

      header m_header;
      file_commands m_file_commands;
      edit_commands m_edit_commands;
      level_commands m_level_commands;

      tileset_display display_tileset;
      preferences& m_preferences;

      toolbar_tools_display* m_tools;
      layers_control* m_layers_control;
      tile_objects_display m_tile_objects;

      Gtk::Statusbar m_status_bar;
      Gtk::Label m_status;

      int m_tile_width;
      int m_tile_height;

      bool on_delete_event(GdkEventAny* event) override;

      void on_action_about();
      void on_close_level_clicked(Gtk::ScrolledWindow& scrolled, level_display& display);
      void on_switch_page(GtkNotebookPage* page, guint page_num);
      void on_preferences_changed(preferences_display::preference_changes c);
      void on_tileset_expose_event(GdkEventExpose* event);

      void update_cache();

      boost::shared_ptr<level> m_level;

      Gtk::Notebook m_nb_levels;
      Gtk::Notebook m_nb_toolset;

      Gtk::FileChooserDialog m_fc_save;
    private:
      signal_switch_level_display_type m_signal_switch_level_display;
    };
  }
}

#endif
