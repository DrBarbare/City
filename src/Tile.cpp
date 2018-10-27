#include "Tile.h"

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

void
Tile::draw(Window& window, const float dt)
{
	auto sprite = m_sprite_sheet.next(dt);
	sprite.setPosition(300, 100);
	sprite.scale(10.0f, 10.0f);
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


