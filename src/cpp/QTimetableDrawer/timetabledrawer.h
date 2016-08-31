#ifndef TIMETABLEDRAWER_H
#define TIMETABLEDRAWER_H

#include <QObject>
#include <string>
#include <vector>
#include <utility>
#include "event.h"
#include "qcolor.h"
#include "hpdf_types.h"

class TimetableDrawer : public QObject
{
  Q_OBJECT
public:
  TimetableDrawer();
  ~TimetableDrawer();

  void setInputfile(std::string filename);
  void setOutputfile(std::string filename);
  void setColors(std::vector<std::pair<std::string,QColor> > c);
  void setWeeklyTimetable(bool weekly);

signals:
  void finished();
  void error(QString err);

public slots:
  void process();

private:
  std::string inputfile;
  std::string outputfile;
  std::vector<ColorCode> colors;
  std::vector<Event> events;
  std::vector<std::vector<Event> > eventsSortedByWeek;
  bool useColors;
  bool weeklyTimetable;
  QString errorMessage;

  bool readEventsFromCSVFile();
  bool writeEventsToPDFFile();
  bool sortEventsByWeek();
  int getWeeknumberFromDatetime(tm datetime);
};

#endif // TIMETABLEDRAWER_H
