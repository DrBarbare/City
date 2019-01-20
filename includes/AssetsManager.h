#ifndef CITY_ASSETS_MANAGER_H
#define CITY_ASSETS_MANAGER_H

#include <unordered_map>
#include <filesystem>

#include <SFML/Graphics.hpp>

#include "Tile.h"

namespace city
{

namespace assets
{
enum class Textures
{
	background
};

}

class AssetsManager
{

static std::unordered_map<assets::Textures, std::filesystem::path> m_textures_paths;
static std::unordered_map<assets::Textures, sf::Texture> m_textures;
static std::unordered_map<std::string, Tile> m_tiles;

public:
	AssetsManager() = delete;
	~AssetsManager() = delete;

	static const std::filesystem::path& themeFile();
	static std::filesystem::path fontTTFFile(const std::string& font_name, const std::string& type_name);
	static sf::Texture& load(assets::Textures texture);
	static void loadTiles();
	static const auto& tiles() { return m_tiles; };
};

}
#endif // CITY_ASSETS_MANAGER_H
