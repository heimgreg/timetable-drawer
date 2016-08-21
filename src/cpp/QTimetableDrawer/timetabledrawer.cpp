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
  colors = c;
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
  writer.drawTimetableFromEvents(events,0);
  writer.saveToFile(outputfile);

  return true;
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

