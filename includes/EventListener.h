#ifndef CITY_EVENT_LISTENER_H
#define CITY_EVENT_LISTENER_H

#include <SFML/Graphics.hpp>

namespace city
{

class EventListener
{

public:
	using MousePixelToCoords = std::function<sf::Vector2f(const sf::View&)>;

	virtual void on_resize() {}
	virtual void on_closed() {}
	virtual void on_mouse_wheel(float /* delta */) {}
	virtual void on_mouse_button_pressed(int /* button */, float /* x */, float /* y */) {}
	virtual void on_mouse_button_released(int /* button */, float /* x */, float /* y */) {}
	virtual void on_mouse_moved(float /* x */, float /* y */) {}
	virtual void on_mouse_button_pressed(int /* button */, const MousePixelToCoords&) {}
	virtual void on_mouse_button_released(int /* button */, const MousePixelToCoords&) {}
	virtual void on_mouse_moved(const MousePixelToCoords&) {}
};

}

#endif // CITY_EVENT_LISTENER_H
