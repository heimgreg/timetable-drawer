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

signals:
  void finished();
  void error(QString err);

public slots:
  void process();

private:
  std::string inputfile;
  std::string outputfile;
  std::vector<std::pair<std::string,QColor> > colors;
  std::vector<Event> events;
  bool useColors;
  QString errorMessage;

  bool readEventsFromCSVFile();
  bool writeEventsToPDFFile();
};

#endif // TIMETABLEDRAWER_H
