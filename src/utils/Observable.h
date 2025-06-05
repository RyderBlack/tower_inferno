#ifndef TOWER_INFERNO_OBSERVABLE_H
#define TOWER_INFERNO_OBSERVABLE_H

#include <vector>
#include <functional>

// Forward declaration
class Observer;

class Observable {
private:
    std::vector<Observer*> observers;
    
public:
    virtual ~Observable() = default;
    
    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    
protected:
    void notify(int eventType, void* subject);
};

// Observer interface
class Observer {
public:
    virtual ~Observer() = default;
    virtual void onNotify(int eventType, void* subject) = 0;
};

// Event types
enum class GameEvent {
    ENEMY_SPAWNED,
    ENEMY_DIED,
    TOWER_PLACED,
    TOWER_SOLD,
    TOWER_UPGRADED,
    WAVE_STARTED,
    WAVE_COMPLETED,
    GAME_OVER,
    GAME_WON,
    PLAYER_DAMAGED,
    GOLD_CHANGED,
    SCORE_CHANGED
};

#endif //TOWER_INFERNO_OBSERVABLE_H
