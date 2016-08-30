#include "timetabledrawer.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "hpdf.h"
#include "pdfwriter.h"

TimetableDrawer::TimetableDrawer()
{
  useColors = false;
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
  for(unsigned i = 0; i < c.size(); ++i)
  {
    ColorCode cc;
    int r,g,b;
    c[i].second.getRgb(&r,&g,&b);
    cc.keyword = c[i].first;
    cc.r = (float)r / 255.0;
    cc.g = (float)g / 255.0;
    cc.b = (float)b / 255.0;
    colors.push_back(cc);
  }
  useColors = true;
}

bool TimetableDrawer::readEventsFromCSVFile()
{
  if(inputfile == "")
  {
    errorMessage = "Keine Eingabedatei angegeben.";
    return false;
  }

  std::ifstream ifs(inputfile.c_str());
  if(!ifs.is_open())
  {
    errorMessage = "Fehler beim Lesen der Datei '" + QString::fromStdString(inputfile) + "'.";
    return false;
  }

  std::string line;

  bool first = true;
  while(std::getline(ifs,line))
  {
    if(first)
    {
      first = false;
      continue;
    }

    std::istringstream linestream(line);
    std::string content;

    std::string date;
    std::string time;
    std::string duration;
    std::string title;
    std::string room;

    int col = 0;
    while(std::getline(linestream,content,','))
    {
      switch (col) {
        case 1:
          // Date
          date = content;
          break;
        case 2:
          // Time
          time = content;
          break;
        case 4:
          // Duration
          duration = content;
          break;
        case 6:
          // Title
          title = content;
          break;
        case 9:
          // Room
          room = content;
          break;
        default:
          break;
      }
      col++;
    }
    std::string datetime = date + " " + time;
    datetime.erase(std::remove(datetime.begin(),datetime.end(),'\"'),datetime.end());
    duration.erase(std::remove(duration.begin(),duration.end(),'\"'),duration.end());
    title.erase(std::remove(title.begin(),title.end(),'\"'),title.end());
    room.erase(std::remove(room.begin(),room.end(),'\"'),room.end());
    Event ev;
    strptime(datetime.c_str(),"%d.%m.%Y %H:%M",&ev.datetime);
    try {
      ev.duration = std::stoi(duration);
    }
    catch(std::invalid_argument exc)
    {
      errorMessage = "Fehler beim Lesen der Datei '" + QString::fromStdString(inputfile) + "'.";
      return false;
    }

    ev.name = title;
    ev.room = room;
    events.push_back(ev);
  }

  //qDebug(std::to_string(events.size()).c_str());

  ifs.close();
  return true;
}

bool TimetableDrawer::writeEventsToPDFFile()
{
  if(outputfile == "")
  {
    errorMessage = "Keine Ausgabedatei angegeben.";
    return false;
  }

  PDFWriter writer;
  if(!writer.isValid())
  {
    errorMessage = "Konnte kein pdf Dokument erstellen.";
    return false;
  }

  if(!sortEventsByWeek())
  {
    errorMessage = "Fehler beim Sortieren der Veranstaltungen.";
    return false;
  }

  writer.setColorCodes(colors);

  for(unsigned i = 0; i < eventsSortedByWeek.size(); ++i)
  {
    writer.drawTimetableFromEvents(eventsSortedByWeek[i], eventsSortedByWeek[i][0].datetime.tm_year + 1900, getWeeknumberFromDatetime(eventsSortedByWeek[i][0].datetime));
  }

  //writer.drawTimetableFromEvents(events,42);
  if(!writer.saveToFile(outputfile))
  {
    errorMessage = "Konnte keine pdf Datei schreiben.";
    return false;
  }

  return true;
}

bool TimetableDrawer::sortEventsByWeek()
{
  if(events.size() == 0)
  {
    errorMessage = "Es sind keine Veranstaltungen vorhanden.";
    return false;
  }
  int lastWeeknumber = getWeeknumberFromDatetime(events[0].datetime);
  int lastYear = events[0].datetime.tm_year;

  std::vector<Event> weekEvents;

  Event ev;
  for(unsigned i = 0; i < events.size(); ++i)
  {
    ev = events[i];
    if((getWeeknumberFromDatetime(ev.datetime) > lastWeeknumber && ev.datetime.tm_year == lastYear) || ev.datetime.tm_year > lastYear)
    {
      lastWeeknumber = getWeeknumberFromDatetime(ev.datetime);
      lastYear = ev.datetime.tm_year;
      eventsSortedByWeek.push_back(weekEvents);
      weekEvents.clear();
    }
    weekEvents.push_back(ev);
  }
  eventsSortedByWeek.push_back(weekEvents);

  return true;
}

int TimetableDrawer::getWeeknumberFromDatetime(tm datetime)
{
  char weeknumberString[3];
  strftime(weeknumberString,3,"%W",&datetime);
  return std::atoi(weeknumberString);
}

void TimetableDrawer::process()
{
  bool success;
  success = readEventsFromCSVFile();
  if(!success)
  {
    emit error(errorMessage);
  }
  success = writeEventsToPDFFile();
  if(!success)
  {
    emit error(errorMessage);
  }
  emit finished();
}

