#ifndef CITY_GAME_H
#define CITY_GAME_H

#include <stack>
#include <memory>
#include "Window.h"

namespace city
{
class GameState;

class Game
{
	// Options
	Window m_window;
	// TODO Switch to queue
	std::stack<std::unique_ptr<GameState> > m_states;

public:
	Game();
	~Game();

	void loop();

	void back_to_previous_state();

	decltype(auto) current_state() const;
	template<typename T, typename... Args>
	void switch_to_state(Args&&... args)
	{
		m_states.emplace(new T(std::forward<Args>(args)...));
	}
};

}

#endif // CITY_GAME_H
