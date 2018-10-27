#include <iostream>
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
printKeys(const YAML::Node& node, int indent)
{
	switch (node.Type()) {
		case YAML::NodeType::Sequence:
			std::cout << "-- Seq --" << std::endl;
			for (auto it = node.begin(); it != node.end(); ++it) {
				auto element = *it;
				printKeys(element, indent);
				// recurse on "element"
			}
			break;
		case YAML::NodeType::Map:
			for (auto it = node.begin(); it != node.end(); ++it) {
				auto key = it->first;
				auto value = it->second;
				for (auto i = 0; i < indent; ++i)
				{
					std::cout << "\t";
				}
				std::cout << key << std::endl;
				printKeys(value, indent + 1);
				// recurse on "key" and "value"
				// if you're sure that "key" is a string, just grab it here
			}
			break;
		default: break;
	}
}

void
AssetsManager::loadTiles()
{
	auto nodes = YAML::LoadFile("assets/tiles/tiles.yaml");
	printKeys(nodes, 0);
}

}
