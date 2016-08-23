#ifndef PDFWRITER_H
#define PDFWRITER_H

#include "hpdf.h"
#include "event.h"
#include <cstdio>
#include <exception>
#include <string>
#include <vector>

class PDFWriter
{
public:
  PDFWriter();
  virtual ~PDFWriter();

  // Setter
  // ...

  void setColorCodes(std::vector<ColorCode> cc);
  bool drawTimetableFromEvents(std::vector<Event> events, int year, int weeknumber);
  bool saveToFile(std::string filename);

private:
  HPDF_REAL dotsPerInch;
  HPDF_REAL dotsPerHour;
  HPDF_REAL dotsDayWidth;
  HPDF_REAL dotsTimeColumnWidth;
  HPDF_REAL dotsHeaderRowHeight;
  HPDF_REAL dotsMinPageMarginHorizontal;
  HPDF_REAL dotsMinPageMarginVertical;
  HPDF_REAL dotsPageMarginHorizontal;
  HPDF_REAL dotsPageMarginVertical;

  HPDF_REAL dotsTimetableSizeX;
  HPDF_REAL dotsTimetableSizeY;

  int startTime;
  int endTime;

  HPDF_Doc doc;
  HPDF_PageSizes pageSize;
  HPDF_PageDirection pageOrientation;
  HPDF_Font documentFont;

  std::vector<ColorCode> colorCodes;

  std::vector<std::string> weekdays = {"Sonntag","Montag","Dienstag","Mittwoch","Donnerstag","Freitag","Samstag"};

  bool drawTimetableGrid(HPDF_Page &page, int year, int weeknumber);
  bool drawEvent(HPDF_Page &page, Event ev);
  void getDateStringFromWeeknumberAndWeekday(int year, int weeknumber, int weekday, char* date);
};

#endif // PDFWRITER_H
