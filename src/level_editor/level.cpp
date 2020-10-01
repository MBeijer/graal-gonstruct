#include "level.hpp"
#include "helper.hpp"
#include <fstream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <string>
#include <iostream>

using namespace Graal::helper;

Graal::level::level(int fill_tile): m_unique_npc_id_counter(0) {
  // Always create one layer
  create_tiles(0, fill_tile);

  // Assert a few things
  g_assert(Graal::tile_transparent.index == Graal::tile::transparent_index);
  g_assert(Graal::tile_invalid.index == Graal::tile::invalid_index);
}

int Graal::level::get_width() {
  return 64;
}

int Graal::level::get_height() {
  return 64;
}

Graal::npc& Graal::level::add_npc() {
  Graal::npc npc;
  npc.id = ++m_unique_npc_id_counter;
  npcs.push_back(npc);
  return npcs.back();
}

Graal::npc& Graal::level::add_npc(Graal::npc npc) {
  npc.id = ++m_unique_npc_id_counter;
  npcs.push_back(npc);
  return npcs.back();
}

Graal::level::npc_list_type::iterator Graal::level::get_npc(int id) {
  npc_list_type::iterator it, end = npcs.end();
  for (it = npcs.begin(); it != end; ++it) {
    if (it->id == id)
      return it;
  }

  return end;
}

void Graal::level::delete_npc(int id) {
  npcs.erase(get_npc(id));
}

Graal::tile_buf& Graal::level::create_tiles(int layer, int fill_tile, bool overwrite) {
  int old_size = get_layer_count();
  if (old_size < layer + 1) {
    /* When skipping several layers, fill the layers below with the same fill_tile */
    layers.resize(layer + 1);
    while (old_size < layer + 1) {
      create_tiles(old_size++, fill_tile, true);
    }
  } else if (!overwrite) {
    return layers[layer];
  }

  tile_buf& tiles = layers[layer];
  tiles.resize(get_width(), get_height());

  for (int x = 0; x < get_width(); x ++) {
    for (int y = 0; y < get_height(); y ++) {
      tiles.get_tile(x, y).index = fill_tile;
    }
  }

  return tiles;
}

bool Graal::level::tiles_exist(int layer) {
  if (get_layer_count() < layer + 1)
    return false;

  Graal::tile_buf& tiles = layers[layer];
  return tiles.get_width() == get_width() && tiles.get_height() == get_height();
}

Graal::tile_buf& Graal::level::get_tiles(int layer) {
  return layers[layer];
}

const Graal::tile_buf& Graal::level::get_tiles(int layer) const {
  return layers[layer];
}

int Graal::level::get_layer_count() const {
  return static_cast<int>(layers.size());
}

void Graal::level::insert_layer(int index, int fill_tile) {
  layers.insert(layers.begin() + index, Graal::tile_buf());
  create_tiles(index, fill_tile, true);
}

void Graal::level::delete_layer(int index) {
  layers.erase(layers.begin() + index);
}

Graal::level* Graal::load_nw_level(const boost::filesystem::path& path) {
  if (!boost::filesystem::exists(path))
    throw std::runtime_error("load_nw_level("+path.string()+") failed: File not found");

  std::ifstream file(path.string().c_str());

  std::string version = read_line(file);

  if (version.find(NW_LEVEL_VERSION) != 0) {
    throw std::runtime_error("load_nw_level() failed: Version mismatch (" + version + " != " + NW_LEVEL_VERSION + ")");
  }

  auto* level = new Graal::level();
  while(!file.eof()) {
    std::string type = read<std::string>(file);

    // read tiles
    if (type == "BOARD") {
      int start_x = read<int>(file);
      int start_y = read<int>(file);
      int width = read<int>(file);
      int layer = read<int>(file);
      std::string data = read<std::string>(file);

      // Fill lowest layer with tile 0 by default, otherwise use transparent tile
      int fill_tile = layer ? tile::transparent_index : 0;
      Graal::tile_buf& tiles = level->create_tiles(layer, fill_tile);

      for (int i = 0; i < width * 2; i +=2) {
        int tile_index = static_cast<int>(helper::parse_base64(data.substr(i, 2)));
        int x = start_x + i/2;

        tiles.get_tile(x, start_y) = Graal::tile(tile_index);
      }
    // read links
    } else if (type == "LINK") {
      Graal::link link;
      link.destination = read<std::string>(file);
      link.x = read<int>(file);
      link.y = read<int>(file);
      link.width = read<int>(file);
      link.height = read<int>(file);

      link.new_x = read<std::string>(file);
      link.new_y = read<std::string>(file);

      level->links.push_back(link);
    // read signs
    } else if (type == "SIGN") {
      Graal::sign sign;
      sign.x = read<int>(file);
      sign.y = read<int>(file);

      read_line(file); // finish the current line
      std::string line;
      while (true) {
        line = read_line(file);

        // Protect against infinite loop in malformed levels
        if (line == "SIGNEND" || file.eof())
          break;

        sign.text += line;
        sign.text += "\n";
      }

      level->signs.push_back(sign);
    // read npcs
    } else if (type == "NPC") {
      Graal::npc& npc = level->add_npc();
      npc.image = read<std::string>(file);
      if (npc.image == "-")
        npc.image.clear();
      float rx, ry;
      rx = read<float>(file);
      ry = read<float>(file);
      npc.set_level_x(rx);
      npc.set_level_y(ry);

      read_line(file); // finish the current line
      std::string line;
      while (true) {
        line = read_line(file);

        // Protect against infinite loop in malformed levels
        if (line == "NPCEND" || file.eof())
          break;

        npc.script += line;
        npc.script += "\n";
      }
    // else skip the line
    } else {
      read_line(file);
    }
  }

  file.close();
  return level;

}

void Graal::save_nw_level(const Graal::level* level, const boost::filesystem::path& path) {
  std::ofstream stream(path.string().c_str());

  stream << NW_LEVEL_VERSION << std::endl;

  // white space separator
  std::string s = " ";
  // write tiles
  for (int layer = 0; layer < level->get_layer_count(); layer ++) {
    const Graal::tile_buf& tiles = level->get_tiles(layer);
    for (int y = 0; y < tiles.get_height(); y ++) {
      std::string data;
      // chunk start, chunk data pairs
      std::list<std::pair<int, std::string> > chunks;
      /* Separate each row into chunks of actually non-transparent tiles.
       * Every time we encounter a transparent tile, flush the current data
       * into the chunk list and clear it. If we never encounter a transparent
       * tile, flush the entire data after the loop */
      int current_start = 0;
      for (int x = 0; x < tiles.get_width(); x ++) {
        Graal::tile tile = tiles.get_tile(x, y);
        if (tile == Graal::tile_transparent) {
          if (!data.empty()) {
            chunks.emplace_back(current_start, data);
            current_start = x;
            data.clear();
          }

          // Skip transparent tile
          current_start ++;
          continue;
        }

        data += helper::format_base64(tile.index);
      }
      if (!data.empty())
        chunks.emplace_back(current_start, data);

      /* Draw one BOARD entry for each chunk so transparent tile-data is culled */
      std::list<std::pair<int, std::string> >::iterator iter, end = chunks.end();
      for (iter = chunks.begin(); iter != end; ++iter) {
        stream << "BOARD" << s << iter->first << s << y << s << iter->second.length() / 2 << s << layer // x, y, width, layer
               << s << iter->second << std::endl;
      }
    }
  }

  // write links
  Graal::level::link_list_type::const_iterator link_iter, link_end;
  link_end = level->links.end();
  for (link_iter = level->links.begin();
       link_iter != link_end;
       link_iter ++) {
    stream << "LINK" << s << link_iter->destination << s << link_iter->x << s << link_iter->y
           << s << link_iter->width << s << link_iter->height << s << link_iter->new_x
           << s << link_iter->new_y << std::endl;
  }

  // write signs
  Graal::level::sign_list_type::const_iterator sign_iter, sign_end;
  sign_end = level->signs.end();
  for (sign_iter = level->signs.begin();
       sign_iter != sign_end;
       sign_iter ++) {
    stream << "SIGN" << s << sign_iter->x << s << sign_iter->y << std::endl;
    stream << sign_iter->text << std::endl;
    stream << "SIGNEND" << std::endl;
  }

  // write npcs
  Graal::level::npc_list_type::const_iterator npc_iter, npc_end;
  npc_end = level->npcs.end();
  for (npc_iter = level->npcs.begin();
       npc_iter != npc_end;
       npc_iter ++) {
    std::string image = npc_iter->image;
    // No image is represented by "-"
    if (image.empty())
      image = "-";
    stream << "NPC" << s << image << s << npc_iter->get_level_x() << s << npc_iter->get_level_y() << std::endl;
    stream << npc_iter->script << std::endl;
    stream << "NPCEND" << std::endl;
  }
}
