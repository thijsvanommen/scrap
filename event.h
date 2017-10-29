#ifndef EVENT_H
#define EVENT_H

const int EVENTQUEUESIZE = 72;

enum EventType {EVENT_ACTION, EVENT_ENERGY, EVENT_SELFENERGY};

#include <fstream>

class Event {
public:
	Event();
	Event(int delay, EventType newtype, int newbeing = -1);
	
	unsigned int time;
	EventType type;
	int being;
};

class EventQueue {
public:
	EventQueue();

	void empty();
	int getsize();
	void push(Event newevent);
	Event pop(int parent = 0);
	Event peek(int i = 0);

    void save(std::ostream & out);
    void load(std::istream & in);

private:
	int num;
	int currenttime;
	Event elt[EVENTQUEUESIZE];
};

extern EventQueue * QUEUE;

#endif
