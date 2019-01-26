#include "Game.h"
#include "game_states/Start.h"
#include "AssetsManager.h"

namespace city
{

Game::Game()
{
	AssetsManager::loadTiles();
	switch_to_state<state::Start>();
}
Game::~Game() = default;

void
Game::back_to_previous_state()
{
	m_states.pop();	
}

decltype(auto)
Game::current_state() const
{
	return m_states.top();
}

void
Game::loop()
{
	while(true)
	{
		const float dt_s = m_window.dt_s();

		if (!m_window.poll_events(current_state().get())) break;
		
		current_state()->update(*this, dt_s);
		if (m_states.empty() || !m_window) break; 

		m_window.display([dt_s, this] {
			current_state()->draw(m_window, dt_s);
		});
	}
}


} // namespace city
