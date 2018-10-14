#ifndef CITY_GAME_STATE_START_H
#define CITY_GAME_STATE_START_H

#include "GameState.h"

namespace city::state
{
class Start : public GameState
{
public:
	void update(Game& game, const float dt) override;
	void draw(Window& window, const float dt) override;
};
}

#endif // CITY_GAME_STATE_START_H
