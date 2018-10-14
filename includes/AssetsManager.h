#ifndef CITY_ASSETS_MANAGER_H
#define CITY_ASSETS_MANAGER_H

#include <unordered_map>
#include <filesystem>

#include <SFML/Graphics.hpp>

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

public:
	AssetsManager() = delete;
	~AssetsManager() = delete;

	static sf::Texture& load(assets::Textures texture);
};

}
#endif // CITY_ASSETS_MANAGER_H
