#ifndef CLOCK_H
#define CLOCK_H
#include <SFML/System.hpp>
class Clock{
private:
sf::Clock clock;
float delta;
public:
Clock();
void updateclock();
float getdelta();
};
#endif