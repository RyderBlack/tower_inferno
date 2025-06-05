#include "Observable.h"

void Observable::addObserver(Observer* observer) {
    if (observer) {
        observers.push_back(observer);
    }
}

void Observable::removeObserver(Observer* observer) {
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it != observers.end()) {
        observers.erase(it);
    }
}

void Observable::notify(int eventType, void* subject) {
    for (auto observer : observers) {
        observer->onNotify(eventType, subject);
    }
}
