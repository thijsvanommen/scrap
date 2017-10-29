#include "event.h"

Event::Event() {}

Event::Event(int delay, EventType newtype, int newbeing, int newitemno):
time(delay), type(newtype), being(newbeing), itemno(newitemno) {}

bool Event::operator==(const Event & rhs) {
    return (type == rhs.type
            && being == rhs.being
            && (type != EVENT_ITEMENERGY || itemno == rhs.itemno));
}

bool Event::operator<(const Event & rhs) {
    if (time != rhs.time)
        return (time - rhs.time > (1 << 15));
    else if (type != rhs.type)
        return (type < rhs.type);
    else if (being != rhs.being)
        return (being < rhs.being);
    else
        return (itemno < rhs.itemno);
}

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
    unsigned int child, parent;

    newevent.time += currenttime;
    child = num++;
    while (child > 0) {
        parent = (child - 1) / 2;
        if (elt[parent] < newevent)
             break;
        else {
            if (child < elt.size())
                elt[child] = elt[parent];
            else
                elt.push_back(elt[parent]);
            child = parent;
        }
    }
    if (child < elt.size())
        elt[child] = newevent;
    else
        elt.push_back(newevent);
}

Event EventQueue::pop() {
    Event ret = elt[0];
    currenttime = ret.time;
    pull(0);
    return ret;
}

void EventQueue::pull(Event event) {
    for (int i = 0; i < getsize(); i++)
        while (elt[i] == event && i < getsize())
            pull(i);
}

void EventQueue::pull(int parent) {
    int child;

    num--;
    while (true) {
        child = 2 * parent + 1;
        if (child + 1 < num && elt[child+1] < elt[child])
            child++;
        else if (child >= num)
            break;
        if (elt[num] < elt[child])
            break;
        else {
            elt[parent] = elt[child];
            parent = child;
        }
    }
    elt[parent] = elt[num];
}

Event EventQueue::peek(int i) {
    return elt[i];
}

void EventQueue::swapevents(Event event1, Event event2) {
    for (int i = 0; i < getsize(); i++) {
        if (elt[i] == event1)
            elt[i] = event2;
        else if (elt[i] == event2)
            elt[i] = event1;
    }    
}

void EventQueue::save(std::ostream & out) {
    out << num << ' ' << currenttime << '\n';
    for (int i = 0; i < num; i++) {
        out << elt[i].time << ' '
            << elt[i].type << ' '
            << elt[i].being;
        if (elt[i].type == EVENT_ITEMENERGY)
            out << ' ' << elt[i].itemno;
        out  << '\n';
    }
}

void EventQueue::load(std::istream & in) {
    Event tmp;
    int t;

    elt.clear();
    in >> num >> currenttime;
    for (int i = 0; i < num; i++) {
        in >> tmp.time
           >> t
           >> tmp.being;
        tmp.type = (EventType)t;
        if (tmp.type == EVENT_ITEMENERGY)
            in >> tmp.itemno;
        elt.push_back(tmp);
    }
}
