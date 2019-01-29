#ifndef CITY_GAME_TILE_H
#define CITY_GAME_TILE_H

#include <functional>
#include <unordered_map>
#include <variant>

#include "Game.h"
#include "SpriteSheet.h"

namespace city
{
class Tile
{
public:
	/// Neighbor info gets a tile from the world at given coordinates (empty if doesn't exsit)
	using neighbor_info = std::function<Tile*(int, int)>;

	using valid_area_condition = std::function<bool(std::size_t, std::size_t)>;

	struct drawing_info {
		valid_area_condition is_valid_area;
		std::reference_wrapper<Tile> brush;
	};

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

	void update(Game& game, const float dt, const neighbor_info& neighbor);

	void draw(Window& window, const float dt, std::size_t col, std::size_t row, const drawing_info& info);

	bool empty() const noexcept;

	void addLevel();

private:
	std::size_t m_current_level;
	std::vector<std::unordered_map<Properties, Property>> m_properties;
	SpriteSheet m_sprite_sheet;

};
}

#endif
