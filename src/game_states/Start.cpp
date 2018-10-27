#include "game_states/Start.h"
#include "game_states/Editor.h"

namespace city::state
{

Start::Start() : m_startEditor{false}
{
	// Create the label.
	m_label = sfg::Label::Create( "Start State." );

	// Create a simple button and connect the click signal.
	m_button = sfg::Button::Create( "Editor" );
	m_button->GetSignal(sfg::Widget::OnLeftClick)
	       .Connect([&]
	{
		m_label->SetText("Leaving Start...");
		m_startEditor = true;
	});

	// Create a vertical box layouter with 5 pixels spacing and add the label
	// and button to it.
	m_box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
	m_box->Pack( m_label );
	m_box->Pack( m_button, false );

	// Create a window and add the box layouter to it. Also set the window's title.
	m_ui_window = sfg::Window::Create();
	m_ui_window->SetTitle( "Menu" );
	m_ui_window->Add( m_box );
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
	// Create a desktop and add the window to it.
	window.desktop().Add( m_ui_window );
}



}
