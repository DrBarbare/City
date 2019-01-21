#include "game_states/Start.h"
#include "game_states/Editor.h"

namespace city::state
{

Start::Start() : m_startEditor{false}
{
	// Create the label.
	auto label  = sfg::Label::Create( "Welcome to City" );

	// Create a simple button and connect the click signal.
	auto button = sfg::Button::Create( "Editor" );
	button->GetSignal(sfg::Widget::OnLeftClick)
	       .Connect([label, this]
	{
		label->SetText("Leaving Start...");
		m_startEditor = true;
	});

	// Create a vertical box layouter with 5 pixels spacing and add the label
	// and button to it.
	auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
	box->Pack( label );
	box->Pack( button, false );

	// Create a window and add the box layouter to it. Also set the window's title.
	auto ui_window = sfg::Window::Create(sfg::Window::Style::NO_STYLE);
	ui_window->SetTitle( "Menu" );
	ui_window->Add( box );

	m_draw_ui = [ui_window = std::move(ui_window)](Window& window, const float dt) {
		auto center = window.shape() / 2.0f;
		auto allocation = ui_window->GetAllocation();
		if (allocation.left == 0.0f && allocation.top == 0.0f)
		{
			sf::Vector2f allocation_size{allocation.width * 3, allocation.height * 2};
			ui_window->SetAllocation(sf::FloatRect{center - (allocation_size / 2.0f), allocation_size});
		}

		window.desktop().Add( ui_window );
	};
}

void
Start::update(Game& game, const float dt)
{
	if (m_startEditor)
	{
		game.back_to_previous_state();
		game.switch_to_state<Editor>();
	}
}

void
Start::draw(Window& window, const float dt)
{
	if (m_draw_ui)
	{
		m_draw_ui(window, dt);
	}
}



}
