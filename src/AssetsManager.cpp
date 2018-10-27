#include <iostream>

#include <yaml-cpp/yaml.h>

#include "AssetsManager.h"
#include "SpriteSheet.h"

namespace YAML
{
template<>
struct convert<sf::IntRect>
{
static bool decode(const Node& node, sf::IntRect& rhs)
{
	if(!node.IsSequence() || node.size() != 4) {
		return false;
	}
	else
	{
		rhs = sf::Rect{node[0].as<int>(),
		               node[1].as<int>(),
		               node[2].as<int>(),
		               node[3].as<int>()};
		return true;
	}
}
};
}

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

std::unordered_map<std::string_view, Tile>
AssetsManager::m_tiles;

void
fillTileProperties(const YAML::Node& node, Tile& tile)
{
	if (node.IsSequence())
	{
		// TODO: Handle levels
	}
	else
	{
		for (const auto& config_prop : node)
		{
			auto prop = Tile::propertyFromName(config_prop.first.as<std::string>());
			auto val  = config_prop.second.as<std::size_t>();
			tile.property(prop, val);
		}
	}
}

void
loadTileAnimation(const std::string_view& name, const YAML::Node& node, Tile& tile)
{
	if (node && node.IsMap())
	{
	std::cout << "Getting animations" << std::endl;
	auto file = (std::filesystem::path("assets/tiles") / name).replace_extension(".png");
	SpriteSheet sheet(file, node["frame_shape"].as<sf::IntRect>());
	
	for (const auto row : node["rows"])
	{
		sheet.add_animation(row["offset"].as<std::size_t>(),
		                    row["frames"].as<std::size_t>(),
		                    row["duration"].as<double>());
	}
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
			auto& tile = p.first->second;
			fillTileProperties(element.second["properties"], p.first->second);
			loadTileAnimation(name, element.second["animations"], p.first->second);
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
	m_tiles = createTiles(nodes);
	std::cerr << m_tiles.size() << " tiles Loaded." << std::endl;
}

}
