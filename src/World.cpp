#include <algorithm>
#include <iterator>

#include "World.h"
#include "AssetsManager.h"

#include <iostream>

namespace
{

constexpr int mock[] = { 
	0, 1, 1, 0, 0, 0, 2, 2, 2, 0,
	0, 1, 1, 0, 0, 0, 2, 2, 2, 0,
	0, 1, 1, 0, 0, 0, 2, 2, 2, 0,
	0, 1, 1, 1, 0, 0, 2, 2, 0, 0,
	0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0
};

}

namespace city
{

World::World() :
	m_width{10},
	m_height{10},
	m_tiles{m_width, m_height},
	m_floodFill(m_tiles,
	            [](const Tile& tileA, const Tile& tileB)
	            {
	            	return tileA.name() == tileB.name();
	            })
{
	const auto& all_tiles = AssetsManager::tiles();
	Tile store[3] =
	{
		all_tiles.at("grass"),
		all_tiles.at("water"),
		all_tiles.at("forest")
	};

	std::transform(&mock[0], (&mock[size() -1])+1, m_tiles.begin(),
	[&store](int val) {
		return store[val];
	});
}

std::size_t
World::size() const
{
	return m_width * m_height;
}

void
World::draw(Window& window, float dt, const point_condition& highlight)
{
	m_tiles.for_each([&](auto x, auto y, auto& val)
			{
				val.draw(window, dt, x, y, highlight(x, y));
			});
}

sf::Vector2f
World::gameDimension() const
{
	return sf::Vector2f(m_width, m_height * 0.5) * Tile::tileSize();
}

sf::Vector2f
World::viewCoordsToWorld(const sf::Vector2f& pos) const
{
	float x = pos.y / Tile::tileSize() + pos.x / (2.0f * Tile::tileSize()) - m_tiles.width() * 0.5 - 0.5;
	float y = pos.y / Tile::tileSize() - pos.x / (2.0f * Tile::tileSize()) + m_tiles.width() * 0.5 + 0.5;
	return {x, y};
}

void
World::regionInfo(sf::Vector2f coords) const
{
	const std::size_t x = coords.x;
	const std::size_t y = coords.y;
	if (m_tiles.check_coordinates(x, y))
	{
		auto points = m_floodFill(x, y);
		std::cerr << "Region contains: " << points.size() << " tiles.\n";
	}
	else
	{
		std::cerr << "Click not on map";
	}
}

}
