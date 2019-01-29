#include "Tile.h"
#include <array>

namespace
{
template<typename... Flags>
constexpr bool any_flag(uint8_t val, Flags... flags)
{
	uint8_t mask = (flags | ...);
	return (val | mask) == mask;
}

template<typename... Flags>
constexpr bool all_flags(uint8_t val, Flags... flags)
{
	uint8_t mask = (flags | ...);
	return val == mask;
}

constexpr std::size_t
get_road_sprite_index(uint8_t neighbors)
{
	std::size_t ans = 0u;
	constexpr uint8_t sw_ne = (1 << 0);
	constexpr uint8_t nw_se = (1 << 1);
	constexpr uint8_t se_nw = (1 << 2);
	constexpr uint8_t ne_sw = (1 << 3);

	if     (all_flags(neighbors, nw_se, se_nw, ne_sw, sw_ne)) ans = 2;
	else if (any_flag(neighbors, nw_se, se_nw))               ans = 0;
	else if (any_flag(neighbors, ne_sw, sw_ne))               ans = 1;
	else if (any_flag(neighbors, nw_se, ne_sw))               ans = 3;
	else if (any_flag(neighbors, se_nw, sw_ne))               ans = 4;
	else if (any_flag(neighbors, nw_se, sw_ne))               ans = 5;
	else if (any_flag(neighbors, se_nw, ne_sw))               ans = 6;
	else if (any_flag(neighbors, nw_se, se_nw, sw_ne))        ans = 7;
	else if (any_flag(neighbors, nw_se, se_nw, ne_sw))        ans = 8;
	else if (any_flag(neighbors, nw_se, sw_ne, ne_sw))        ans = 9;
	else if (any_flag(neighbors, se_nw, sw_ne, ne_sw))        ans = 10;

	return ans;
}

uint8_t
get_neighbor_mask(const city::Tile::neighbor_info& get_neighbor, const char* tile_name)
{
	static const std::array<std::pair<int, int>, 4> neighbor_positions
	{{             {  0, -1},
		{ -1,  0},            {  1,  0},
		           {  0,  1}             }};
	uint8_t neighbors_byteset = 0x00;
	auto neighbor_index = 0u;
	for (auto [x, y] : neighbor_positions)
	{
		auto neighbor = get_neighbor(x, y);
		if (neighbor && neighbor->name() == tile_name)
		{
			neighbors_byteset |= (1 << neighbor_index);
		}
		neighbor_index++;
	}
	return neighbors_byteset;
}

} // namespace

namespace city
{

Tile::Tile() : m_current_level{0u} {}

void
Tile::spriteSheet(SpriteSheet sheet) noexcept
{
	m_sprite_sheet = std::move(sheet);
}

void
Tile::update_region(const neighbor_info& get_neighbor)
{
	auto neighbors = get_neighbor_mask(get_neighbor, name().c_str());
	auto animation = get_road_sprite_index(neighbors);
	m_sprite_sheet.set_animation(animation);
}

void
Tile::update(Game& game, const float dt, const neighbor_info& get_neighbor)
{
	// TODO: Upgrade tiles
}

constexpr std::size_t tile_size = 8; // Smaller size of a 1x1 tile, isometry makes it a bit complex...

float
Tile::tileSize() noexcept
{
	return tile_size;
}

void
Tile::draw(Window& window, const float dt, std::size_t col, std::size_t row, const drawing_info& info)
{
	bool is_drawing = !info.brush.get().empty() && info.is_valid_area(col, row);

	// Grab the bush tile to overlay
	auto sprite = is_drawing ? info.brush.get().m_sprite_sheet.next(dt) : m_sprite_sheet.next(dt);

	// Compute the number of tiles occupied by this one in height.
	auto distance_from_base = (sprite.getLocalBounds().height / tile_size) - 1;

	// Transform: move into isometric space.
	sf::Vector2f pos;
	pos.x = (col - row) * tile_size + 10 * tile_size;
	pos.y = (col + row) * tile_size * 0.5
            - distance_from_base * tile_size; // Shift tiles that are greater than the base tile, think skyscrapers.

	sprite.setPosition(pos);

	if (info.is_valid_area(col, row))
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
	for (auto level = m_current_level; 0u <= level && level < m_properties.size(); --level)
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


