#include "AdditionalFunc.h"

using namespace std;


bool eventTrigger(const double& interval, double& lastUpdateTime) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

bool ElementOnDeque(Vector2 position, deque <Vector2> Deque) {
    for (int i = 0; i < Deque.size(); i++) {
        if (Vector2Equals(position, Deque[i])) return true;
    }
    return false;
}



