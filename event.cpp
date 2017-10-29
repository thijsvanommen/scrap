#include "event.h"

Event::Event() {}

Event::Event(int delay, EventType newtype, int newbeing):
time(delay), type(newtype), being(newbeing) {}

EventQueue::EventQueue() {
	empty();
}

void EventQueue::empty() {
	num = 0;
	currenttime = 0;
}

int EventQueue::getsize() {
	return num;
}

void EventQueue::push(Event newevent) {
	int child, parent;

	if (num > 0)
		newevent.time += currenttime;
	child = num++;
	while (child > 0) {
		parent = (child - 1) / 2;
		if (elt[parent].time < newevent.time)
		 	break;
		else {
			elt[child] = elt[parent];
			child = parent;
		}
	}
	elt[child] = newevent;
}

Event EventQueue::pop(int parent) {
	Event ret = elt[parent];
	int child;
	num--;
	currenttime = ret.time;
	while (true) {
		child = 2 * parent + 1;
		if (child + 1 < num && elt[child+1].time < elt[child].time)
			child++;
		else if (child >= num)
			break;
		if (elt[num].time < elt[child].time)
			break;
		else {
			elt[parent] = elt[child];
			parent = child;
		}
	}
	elt[parent] = elt[num];

	if (currenttime >= 16384) { // arbitrary constant
		for (int i = 0; i < num; i++)
			elt[i].time -= currenttime;
		currenttime = 0;
	}
	return ret;
}

Event EventQueue::peek(int i) {
	return elt[i];
}

void EventQueue::save(std::ostream & out) {
    out << num << ' ' << currenttime << '\n';
    for (int i = 0; i < num; i++)
        out << elt[i].time << ' '
            << elt[i].type << ' '
            << elt[i].being << '\n';
}

void EventQueue::load(std::istream & in) {
    int t;

    in >> num >> currenttime;
    for (int i = 0; i < num; i++) {
        in >> elt[i].time
           >> t
           >> elt[i].being;
        elt[i].type = (EventType)t;
    }
}
