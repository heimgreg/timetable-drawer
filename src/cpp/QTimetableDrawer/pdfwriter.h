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

  bool drawTimetableGrid(HPDF_Page &page, int weeknumber);
  bool drawTimetableFromEvents(std::vector<Event> events, int weeknumber);
  bool saveToFile(std::string filename);

private:
  HPDF_REAL dotsPerInch;
  HPDF_REAL dotsPerHour;
  HPDF_REAL dotsDayWidth;
  HPDF_REAL dotsTimeColumnWidth;
  HPDF_REAL dotsHeaderRowHeight;
  HPDF_REAL dotsMinPageMarginHorizontal;
  HPDF_REAL dotsMinPageMarginVertical;

  HPDF_REAL dotsTimetableSizeX;
  HPDF_REAL dotsTimetableSizeY;

  int startTime;
  int endTime;

  HPDF_Doc doc;
  HPDF_PageSizes pageSize;
  HPDF_PageDirection pageOrientation;
  HPDF_Font documentFont;
};

#endif // PDFWRITER_H
