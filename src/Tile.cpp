#include "Tile.h"
#include <iostream>

namespace city
{

void
Tile::spriteSheet(SpriteSheet sheet) noexcept
{
	m_sprite_sheet = std::move(sheet);
}


void
Tile::update(Game& game, const float dt)
{
}

constexpr std::size_t Xoffset = 0;
constexpr std::size_t Yoffset = 0;
constexpr float scale_factor = 5.0f;
constexpr std::size_t tile_size = 8 * scale_factor; // Smaller size of a 1x1 tile, isometry makes it a bit complex...

void
Tile::draw(Window& window, const float dt, std::size_t col, std::size_t row)
{
	auto sprite = m_sprite_sheet.next(dt);

	// Transform
	sprite.scale(scale_factor, scale_factor);

	// Then move with respect to the transormation
	auto rect = sprite.getGlobalBounds();

	sf::Vector2f pos;
	pos.x = (col - row) * tile_size + 10 * tile_size + Xoffset;
	pos.y = (col + row) * tile_size * 0.5 + Yoffset;
	
	sprite.setPosition(pos);

	window.draw(std::move(sprite));
}

/*******************************************************/
/*********  Properties utilities             ***********/
/*******************************************************/
namespace
{
static const std::unordered_map<Tile::Properties, std::string_view> properties_names
{
	{ Tile::Properties::name,           "name"},
	{ Tile::Properties::cost,           "cost"},
	{ Tile::Properties::population,     "population"},
	{ Tile::Properties::max_population, "max_population"},
	{ Tile::Properties::level,          "level"},
	{ Tile::Properties::max_levels,     "max_levels"},
	{ Tile::Properties::production,     "production"},
	{ Tile::Properties::storage,        "storage"},
	{ Tile::Properties::unknown,        "unknown"}
};

static const std::unordered_map<std::string_view, Tile::Properties> names_properties
= []{
	std::unordered_map<std::string_view, Tile::Properties> rev;
	for(const auto& p : properties_names)
	{
		rev.emplace(p.second, p.first);
	}
	return rev;
}();

} // namespace

Tile::Properties
Tile::propertyFromName(const std::string_view& name)
{
	auto it = names_properties.find(name);
	if (it != names_properties.cend())
	{
		return it->second;
	}
	else
	{
		return Properties::unknown;
	}
}

std::string_view
Tile::propertyToName(Tile::Properties prop)
{
	auto it = properties_names.find(prop);
	if (it != properties_names.cend())
	{
		return it->second;
	}
	else
	{
		return properties_names.at(Properties::unknown);
	}
}

void
Tile::property(Properties prop, Property val) noexcept
{
	m_properties[prop] = std::move(val);
}
} // namespace city


