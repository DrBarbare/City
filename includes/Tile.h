#ifndef CITY_GAME_TILE_H
#define CITY_GAME_TILE_H

#include <unordered_map>
#include <variant>

#include "SpriteSheet.h"

namespace city
{
class Tile
{
public:
	using Property = std::variant<std::size_t, float>;
	enum class Properties {
		cost,
		population,
		max_population,
		level,
		max_levels,
		production,
		storage,
		unknown // Leave at the end to remember the size of the enum
	};

	static Properties propertyFromName(const std::string_view& name);
	static std::string_view propertyToName(Properties prop);

	const Property& property(Properties prop) const { return m_properties.at(prop); }
	void property(Properties prop, Property val) { m_properties[prop] = val; }

private:
	std::unordered_map<Properties, Property> m_properties;
	SpriteSheet sprite_sheet;

};
}

#endif
