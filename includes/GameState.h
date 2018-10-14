#ifndef CITY_GAME_STATE_H
#define CITY_GAME_STATE_H

#include "Game.h"
#include "EventListener.h"

namespace city
{

class GameState : public EventListener
{
public:
	virtual ~GameState() = default;
	virtual void update(Game& game, const float dt) =0;
	virtual void draw(Window& window, const float dt) =0;
};

}

#endif // CITY_GAME_STATE_H
