#include "game_states/Start.h"

#include <iostream>

namespace city::state
{
void
Start::update(Game& game, const float dt)
{

}

void
Start::draw(Window& window, const float dt)
{
	static auto firstCall = true;
	if (firstCall)
	{
		// Create the label.
		static auto label = sfg::Label::Create( "Hello world!" );

		// Create a simple button and connect the click signal.
		static auto button = sfg::Button::Create( "Greet SFGUI!" );
		button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&] { label->SetText( "Hello SFGUI, pleased to meet you!" ); } );

		// Create a vertical box layouter with 5 pixels spacing and add the label
		// and button to it.
		static auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
		box->Pack( label );
		box->Pack( button, false );

		// Create a window and add the box layouter to it. Also set the window's title.
		static auto ui_window = sfg::Window::Create();
		ui_window->SetTitle( "Hello world!" );
		ui_window->Add( box );

		// Create a desktop and add the window to it.
		window.desktop().Add( ui_window );
		firstCall = false;
	}
}



}
