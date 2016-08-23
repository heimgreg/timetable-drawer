#ifndef EVENT_H
#define EVENT_H

#include <ctime>
#include <string>

struct Event
{
  std::string name;
  tm datetime;
  int duration; // in minutes
  std::string room;
};

struct ColorCode
{
  std::string keyword;
  float r;
  float g;
  float b;
};

#endif // EVENT_H

