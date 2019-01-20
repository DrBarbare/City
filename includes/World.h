#ifndef CITY_GAME_WORLD_H
#define CITY_GAME_WORLD_H

#include "algorithms/Matrix.h"
#include "algorithms/FloodFill.h"

#include "Game.h"
#include "Tile.h"

namespace city
{

class World
{
public:
	using point_condition = std::function<bool(std::size_t, std::size_t)>;
	World();

	void draw(Window& window, float dt, const point_condition& highlight);
	void update(Game& window, float dt);

	sf::Vector2f gameDimension() const;

	sf::Vector2f viewCoordsToWorld(const sf::Vector2f& pos) const;
	void regionInfo(sf::Vector2f pos) const;

	
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
};

} // namespace city
#endif // CITY_GAME_WORLD_H
