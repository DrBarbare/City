#ifndef CITY_GAME_WORLD_H
#define CITY_GAME_WORLD_H

#include <vector>

#include "Game.h"
#include "Tile.h"

namespace city
{

class World
{
public:
	World();

	void draw(Window& window, float dt);
	void update(Game& window, float dt);


private:
	std::size_t size() const;


	std::size_t m_width;
	std::size_t m_height;
	std::vector<Tile> m_tiles;
};

} // namespace city
#endif // CITY_GAME_WORLD_H
