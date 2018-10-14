#ifndef CITY_EVENT_LISTENER_H
#define CITY_EVENT_LISTENER_H

namespace city
{

class EventListener
{

public:
	virtual void on_resize() {}
	virtual void on_closed() {}
};

}

#endif // CITY_EVENT_LISTENER_H
