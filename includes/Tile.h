#ifndef CITY_GAME_TILE_H
#define CITY_GAME_TILE_H

#include <unordered_map>
#include <variant>

#include "Game.h"
#include "SpriteSheet.h"

namespace city
{
class Tile
{
public:
	using string_property = std::string;
	using uint_property = std::size_t;
	using float_property = float;
	using Property = std::variant<string_property, uint_property, float_property>;
	enum class Properties {
		name,
		cost,
		population,
		max_population,
		production,
		storage,
		unknown // Leave at the end to remember the size of the enum
	};

	static Properties propertyFromName(const std::string_view& name);
	static std::string_view propertyToName(Properties prop);

	static float tileSize() noexcept;

	Tile();

	Property get_property(Properties prop) const;
	template<typename T>
	auto property(Properties prop) const { return std::get<T>(get_property(prop)); }

	const auto& name() const { return property<string_property>(Properties::name); }
	const auto& level() const noexcept { return m_current_level; }

	void property(Properties prop, Property val) noexcept;
	void spriteSheet(SpriteSheet sheet) noexcept;

	void update(Game& game, const float dt);
	void draw(Window& window, const float dt, std::size_t col, std::size_t row, bool highlight);

	bool empty() const noexcept;

	void addLevel();

private:
	std::size_t m_current_level;
	std::vector<std::unordered_map<Properties, Property>> m_properties;
	SpriteSheet m_sprite_sheet;

};
}

#endif
