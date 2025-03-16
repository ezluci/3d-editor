#include "clock.h"

Clock::Clock()
:delta(0.0){}
void Clock::updateclock() {
    delta = clock.restart().asSeconds();
}
float Clock::getdelta()  {
    return delta;
}