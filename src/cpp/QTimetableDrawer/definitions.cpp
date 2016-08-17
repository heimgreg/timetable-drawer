#ifndef DEFINITIONS_CPP
#define DEFINITIONS_CPP

#include <string>
#include <ctime>

struct Event
{
  std::string name;
  tm datetime;
  int duration; // in minutes
  std::string room;
};

#endif // DEFINITIONS_CPP
