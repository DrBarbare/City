#include <iostream>
#include <sstream>
#include <yaml-cpp/yaml.h>
#include "AssetsManager.h"


namespace city
{

std::unordered_map<assets::Textures, std::filesystem::path>
AssetsManager::m_textures_paths{{assets::Textures::background, {"assets/background.png"}}};

std::unordered_map<assets::Textures, sf::Texture>
AssetsManager::m_textures;

sf::Texture&
AssetsManager::load(assets::Textures texture)
{
	auto it = m_textures.find(texture);
	if (it != m_textures.cend())
	{
		return it->second;
	}
	else
	{
		auto& t = m_textures[texture];
		t.loadFromFile(m_textures_paths.at(texture).string());
		return t;
	}
}

void
fillTile(const YAML::Node& node, Tile& tile)
{
	for (const auto& config_prop : node)
	{
		auto prop = Tile::propertyFromName(config_prop.first.as<std::string>());
		auto val  = config_prop.second.as<std::size_t>();
		tile.property(prop, val);
	}
}

std::unordered_map<std::string_view, Tile>
createTiles(const YAML::Node& node)
{
	std::unordered_map<std::string_view, Tile> tiles;
	for (const auto& element : node)
	{
		auto name = element.first.as<std::string>();
		auto p = tiles.emplace(std::move(name), Tile{});
		if (p.second)
		{
			auto props = element.second;
			if (props.IsSequence())
			{
				// TODO: Handle levels
				std::cerr << name << " has levels\n";
				// Tile has levels
			}
			else
			{
				fillTile(props, p.first->second);
			}
		}
		else
		{
			std::cerr << "Already in\n";
		}
	}
	return tiles;
}

void
AssetsManager::loadTiles()
{
	auto nodes = YAML::LoadFile("assets/tiles/tiles.yaml");
	auto tiles = createTiles(nodes);
	std::cerr << "7 tiles Loaded: " << tiles.size() << std::endl;
}

}
