#include "timetabledrawer.h"

TimetableDrawer::TimetableDrawer()
{

}

TimetableDrawer::~TimetableDrawer()
{

}

void TimetableDrawer::setInputfile(std::string filename)
{
  inputfile = filename;
}

void TimetableDrawer::setOutputfile(std::string filename)
{
  outputfile = filename;
}

void TimetableDrawer::setColors(std::vector<std::pair<std::string, QColor> > c)
{
  colors = c;
}

void TimetableDrawer::process()
{
  //emit error("Fehler!");
  emit finished();
}

