#include "Game.h"
#include "game_states/Start.h"


namespace city
{

Game::Game()
{
	// TODO: Add initial state
	switch_to_state<state::Start>();
	
}
Game::~Game() = default;

void
Game::loop()
{
	const float dt_s = m_window.dt_s();

	while(!m_states.empty()
	      && m_window)
	{
		// Cache current state since update may affect stack
		const auto& current_state = m_states.top();
		m_window.poll_events(current_state.get());
		current_state->update(*this, dt_s);
		m_window.display([&]{
				current_state->draw(m_window, dt_s);
		});
	}
}


} // namespace city
