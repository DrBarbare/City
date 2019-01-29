#ifndef CITY_GAME_WORLD_H
#define CITY_GAME_WORLD_H

#include "algorithms/Matrix.h"
#include "geometry/Point.h"
#include "algorithms/FloodFill.h"

#include "Game.h"
#include "Tile.h"

namespace city
{

class World
{
public:
	World();

	void draw(Window& window, float dt, const Tile::drawing_info& highlight);
	void update(Game& game, float dt);

	sf::Vector2f gameDimension() const;

	sf::Vector2f viewCoordsToWorld(const sf::Vector2f& pos) const;
	void regionInfo(sf::Vector2f pos) const;

	void setRegion(geometry::Point tl, geometry::Point br, const Tile& tile);
	
	std::size_t width() const noexcept
	{
		return m_tiles.width();
	}

	std::size_t height() const noexcept
	{
		return m_tiles.height();
	}

private:
	std::size_t size() const;

	algorithms::Matrix<Tile> m_tiles;
	mutable algorithms::FloodFill<Tile> m_floodFill;

	auto neighbor_info_getter(geometry::Point pt);
};

} // namespace city
#endif // CITY_GAME_WORLD_H
