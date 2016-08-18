#ifndef TIMETABLEDRAWER_H
#define TIMETABLEDRAWER_H

#include <QObject>
#include <string>
#include <vector>
#include <utility>
#include <ctime>
#include "qcolor.h"

struct Event
{
  std::string name;
  tm datetime;
  int duration; // in minutes
  std::string room;
};

class TimetableDrawer : public QObject
{
  Q_OBJECT
public:
  TimetableDrawer();
  ~TimetableDrawer();

  void setInputfile(std::string filename);
  void setOutputfile(std::string filename);
  void setColors(std::vector<std::pair<std::string,QColor> > c);

signals:
  void finished();
  void error(QString err);

public slots:
  void process();

private:
  std::string inputfile;
  std::string outputfile;
  std::vector<std::pair<std::string,QColor> > colors;
};

#endif // TIMETABLEDRAWER_H
