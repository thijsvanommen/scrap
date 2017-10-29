#ifndef EVENT_H
#define EVENT_H

enum EventType {EVENT_ACTION, EVENT_ENERGY, EVENT_ITEMENERGY};

#include <fstream>
#include <vector>

class Event {
public:
    Event();
    Event(int delay, EventType newtype, int newbeing = -1, int newitemno = -1);

    bool operator==(const Event & rhs);
    bool operator<(const Event & rhs);
    
    unsigned int time;
    EventType type;
    int being;
    int itemno;
};

class EventQueue {
public:
    EventQueue();

    void empty();
    int getsize();
    void push(Event newevent);
    Event pop();
    void pull(Event event);
    void pull(int parent);
    Event peek(int i = 0);
    void swapevents(Event event1, Event event2);

    void save(std::ostream & out);
    void load(std::istream & in);

private:
    int num;
    int currenttime;
    std::vector<Event> elt;
};

#endif
