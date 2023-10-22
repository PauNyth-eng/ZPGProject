//
// Created by Pavel Janu(Skola) on 10/20/2023.
//

#pragma once

#include "Enums.h"

#include <vector>
class Observer
{
public:
    virtual void notify(EventType eventType, void* object) = 0;
};


class Observable {

    mutable std::vector<Observer*> observers;

public:

    void registerObserver(Observer& obs);
    void removeObserver(Observer& obs);
    void notifyObservers(EventType type, void* obj) const;

};
