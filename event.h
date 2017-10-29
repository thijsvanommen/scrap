/*
 * Scrap, the Roguelike
 * Copyright 2005, 2006 Thijs van Ommen
 *
 * This file is part of Scrap, the Roguelike.
 *
 * Scrap is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Scrap is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Scrap.  If not, see <http://www.gnu.org/licenses/>.
 */

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
