#include "Tile.h"

namespace city
{

Tile::Tile() : m_current_level{0u} {}

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
constexpr float scale_factor = 1.0f;
constexpr std::size_t tile_size = 8 * scale_factor; // Smaller size of a 1x1 tile, isometry makes it a bit complex...

float
Tile::tileSize() noexcept
{
	return tile_size;
}

void
Tile::draw(Window& window, const float dt, std::size_t col, std::size_t row, bool highlight)
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

	if (highlight)
	{
		sprite.setColor(sf::Color(0x7d, 0x7d, 0x7d));
	}
	else
	{
		sprite.setColor(sf::Color(0xff, 0xff, 0xff));
	}

	window.renderer().draw(std::move(sprite));
}

void
Tile::addLevel()
{
	m_properties.emplace_back();
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
	if (m_properties.empty()) addLevel();
	m_properties[m_current_level][prop] = std::move(val);
}

Tile::Property
Tile::get_property(Properties prop) const
{
	Property property;
	// Look for a property across all previous levels, properties are hereditary
	for (auto level = m_current_level; level >= 0 && level < m_properties.size(); --level)
	{
		auto&& level_prop = m_properties[level];
		auto it = level_prop.find(prop);
		if (it != level_prop.cend())
		{
			property = it->second;
			break;
		}
	}
	return property;
}

bool
Tile::empty() const noexcept
{
	return m_properties.empty();
}

} // namespace city


