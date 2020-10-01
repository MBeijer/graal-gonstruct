#ifndef GRAAL_LEVEL_EDITOR_PREFERENCES_EDITOR_HPP_
#define GRAAL_LEVEL_EDITOR_PREFERENCES_EDITOR_HPP_

#include <gtkmm.h>
#include "preferences.hpp"

namespace Graal {
  namespace level_editor {
    class preferences_display : public Gtk::Dialog {
    public:
      static const int NOTHING_CHANGED                       = 0;
      static const int GRAAL_DIR_CHANGED                     = 1 << 0;
      static const int SHOW_SELECTION_WHILE_DRAGGING_CHANGED = 1 << 1;
      static const int REMEMBER_DEFAULT_TILE_CHANGED         = 1 << 2;
      static const int STICKY_TILE_SELECTION_CHANGED         = 1 << 3;
      static const int SELECTION_BACKGROUND_CHANGED          = 1 << 4;
      static const int USE_GRAAL_CACHE_CHANGED               = 1 << 5;

      typedef int preference_changes;

      typedef sigc::signal<void, preference_changes>
          signal_preferences_changed_type;
      explicit preferences_display(preferences& _prefs);

      void update_controls();

      signal_preferences_changed_type& signal_preferences_changed();

    private:
      void on_response(int response_id) override;

      void apply();

      preferences& m_prefs;
      signal_preferences_changed_type m_signal_preferences_changed;

      Gtk::FileChooserButton m_pref_graaldir;
      Gtk::CheckButton       m_pref_selection_border_while_dragging;
      Gtk::CheckButton       m_pref_selection_background;
      Gtk::CheckButton       m_pref_remember_default_tile;
      Gtk::CheckButton       m_pref_sticky_tile_selection;
      Gtk::CheckButton       m_pref_use_graal_cache;
    };
  }
}

#endif
