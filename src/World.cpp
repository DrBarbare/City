#include <algorithm>
#include <iterator>

#include "geometry/Size.h"

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
	m_tiles{geometry::Size{10, 10}},
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
	return m_tiles.area();
}

auto
World::neighbor_info_getter(geometry::Point pt)
{
	return [pt=std::move(pt), this](int qx, int qy) -> Tile* {
				std::size_t coordx = int(pt.x()) + qx;
				std::size_t coordy = int(pt.y()) + qy;
				if (coordx < width() && coordy < height()) return &m_tiles.at(coordx, coordy);
				else                                       return nullptr;
			};
}

void
World::setRegion(geometry::Point tl, geometry::Point br, const Tile& tile)
{
	if (tile.empty()) return;

	for (std::size_t row = tl.y(); row <= br.y(); row++)
	{
		for (std::size_t col = tl.x(); col <= br.x(); col++)
		{
			m_tiles.at(col, row) = tile;
		}
	}

	// Need a second pass to trigger the tile region changes as needed
	for (std::size_t row = tl.y(); row <= br.y(); row++)
	{
		for (std::size_t col = tl.x(); col <= br.x(); col++)
		{
			m_tiles.at(col, row).update_region(neighbor_info_getter({col, row}));
		}
	}
}

void
World::draw(Window& window, float dt, const Tile::drawing_info& info)
{
	m_tiles.for_each([&](auto x, auto y, auto& val)
			{
				val.draw(window, dt, x, y, info);
			});
}

void
World::update(Game& game, float dt)
{
	m_tiles.for_each([&game, dt, this](auto x, auto y, auto& val)
		{
			val.update(game, dt, neighbor_info_getter({x, y}));
		});
}

sf::Vector2f
World::gameDimension() const
{
	return sf::Vector2f(m_tiles.width(), m_tiles.height() * 0.5) * Tile::tileSize();
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
