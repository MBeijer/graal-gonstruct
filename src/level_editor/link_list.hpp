#ifndef GRAAL_LEVEL_EDITOR_LINK_LIST_HPP_
#define GRAAL_LEVEL_EDITOR_LINK_LIST_HPP_

#include <gtkmm.h>
#include "level.hpp"

namespace Graal {
  namespace level_editor {
    class window;
    class edit_link: public Gtk::Dialog {
    public:
      explicit edit_link(window& _window);
      ~edit_link() override;

      void get(const link& _link);
      link get_link();
    protected:
      window& m_window;

      Gtk::Entry m_edit_x;
      Gtk::Entry m_edit_y;
      Gtk::Entry m_edit_width;
      Gtk::Entry m_edit_height;

      Gtk::Entry m_edit_destination;
      Gtk::Entry m_edit_new_x;
      Gtk::Entry m_edit_new_y;
    };

    class link_list: public Gtk::Dialog {
    public:
      explicit link_list(window& _window);

      void get();
    protected:
      void on_show() override;
      void on_response(int response_id) override;
      void on_edit_clicked();
      void on_delete_clicked();

      window& m_window;
      Glib::RefPtr<Gtk::ListStore> m_list_store;
      Gtk::TreeView m_tree_view;

      class link_columns: public Gtk::TreeModelColumnRecord {
      public:
        link_columns() {
          add(iter); add(destination); add(new_x); add(new_y);
        }
        ~link_columns() override;

        Gtk::TreeModelColumn<level::link_list_type::iterator> iter;
        Gtk::TreeModelColumn<Glib::ustring> destination;
        Gtk::TreeModelColumn<Glib::ustring> new_x;
        Gtk::TreeModelColumn<Glib::ustring> new_y;
      };

    public:
      link_columns columns;
    };
  }
}

#endif

