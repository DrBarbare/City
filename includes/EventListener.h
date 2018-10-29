#ifndef CITY_EVENT_LISTENER_H
#define CITY_EVENT_LISTENER_H

namespace city
{

class EventListener
{

public:

	virtual void on_resize() {}
	virtual void on_closed() {}
	virtual void on_mouse_button_pressed(int /* button */, float /* x */, float /* y */) {}
	virtual void on_mouse_button_released(int /* button */, float /* x */, float /* y */) {}
	virtual void on_mouse_moved(float /* x */, float /* y */) {}
	virtual void on_mouse_wheel(float /* delta */) {}
};

}

#endif // CITY_EVENT_LISTENER_H
