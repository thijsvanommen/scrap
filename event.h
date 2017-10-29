#ifndef EVENT_H
#define EVENT_H

class Event;
class EventQueue;

const int EVENTQUEUESIZE = 72;

enum EventType {EVENT_ACTION, EVENT_ENERGY, EVENT_SELFENERGY};

class Being;

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

private:
	int num;
	int currenttime;
	Event elt[EVENTQUEUESIZE];
};

extern EventQueue * QUEUE;

#endif
