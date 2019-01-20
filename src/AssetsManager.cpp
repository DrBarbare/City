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

namespace
{
std::filesystem::path assets_dir = "assets";
std::filesystem::path tiles_dir = assets_dir / "tiles";
std::filesystem::path fonts_dir = assets_dir / "fonts";
}

namespace city
{

std::unordered_map<assets::Textures, std::filesystem::path>
AssetsManager::m_textures_paths{{assets::Textures::background, {"assets/background.png"}}};

std::unordered_map<assets::Textures, sf::Texture>
AssetsManager::m_textures;

const std::filesystem::path&
AssetsManager::themeFile()
{
	static std::filesystem::path file = assets_dir / "city.theme";
	return file;
}

std::filesystem::path
AssetsManager::fontTTFFile(const std::string& font_name, const std::string& type_name)
{
	return fonts_dir / font_name / (font_name + "-" + type_name + ".ttf");
}

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

std::unordered_map<std::string, Tile>
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
loadTileAnimation(const YAML::Node& node, Tile& tile)
{
	if (node && node.IsMap())
	{
		const auto& name = tile.property<Tile::string_property>(Tile::Properties::name);
		auto file = (tiles_dir / name).replace_extension(".png");

		std::cout << "Getting image: " << file << std::endl;

		SpriteSheet sheet(file, node["frame_shape"].as<sf::IntRect>());

		for (const auto row : node["rows"])
		{
			sheet.add_animation(row["offset"].as<std::size_t>(),
			                    row["frames"].as<std::size_t>(),
			                    row["duration"].as<double>());
		}

		tile.spriteSheet(std::move(sheet));
	}
}

std::unordered_map<std::string, Tile>
createTiles(const YAML::Node& node)
{
	std::unordered_map<std::string, Tile> tiles;
	for (const auto& element : node)
	{
		auto name = element.first.as<std::string>();
		auto p = tiles.emplace(name, Tile{});
		if (p.second)
		{
			auto& tile = p.first->second;
			tile.property(Tile::Properties::name, name);
			fillTileProperties(element.second["properties"], tile);
			loadTileAnimation(element.second["animations"], tile);
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
	for (const auto& tile : m_tiles)
	{
		std::cout << "Tiles:" << tile.first << std::endl;
	}
	std::cerr << m_tiles.size() << " tiles Loaded." << std::endl;
}

}
