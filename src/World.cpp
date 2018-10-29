#include <algorithm>
#include <iterator>

#include "World.h"
#include "AssetsManager.h"

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
	m_height{10}
{
	const auto& all_tiles = AssetsManager::tiles();
	Tile store[3] =
	{
		all_tiles.at("grass"),
		all_tiles.at("water"),
		all_tiles.at("forest")
	};

	m_tiles.reserve(size());
	std::transform(&mock[0], (&mock[size() -1])+1, std::back_inserter(m_tiles),
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
World::draw(Window& window, float dt)
{
	for (std::size_t i = 0; i < m_tiles.size(); ++i)
	{
		m_tiles[i].draw(window, dt, i % m_width, i / m_width);
	}
}

sf::Vector2f
World::gameDimension() const
{
	return sf::Vector2f(m_width, m_height * 0.5) * Tile::tileSize();
}

}
