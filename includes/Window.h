#ifndef CITY_WINDOW_H
#define CITY_WINDOW_H

#include <functional>
#include <SFML/Graphics.hpp>
#include "EventListener.h"

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

namespace city
{

class Window
{
	sf::RenderWindow m_window;
	sf::Clock m_clock;
	float m_dt_s;

	sf::Sprite m_background;

	sfg::SFGUI m_ui;
	sfg::Desktop m_desktop;

public:
	Window();
	float dt_s() const;
	void display(const std::function<void()>& draw);
	operator bool() const { return m_window.isOpen(); }

	void poll_events(EventListener* listener);
	void resize_background(std::size_t width, std::size_t height);

	sfg::Desktop& desktop() { return m_desktop; }
};

}
#endif // CITY_WINDOW_H
