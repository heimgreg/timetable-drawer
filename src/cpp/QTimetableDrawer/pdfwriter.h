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

  bool drawTimetableFromEvents(std::vector<Event> events);
  bool saveToFile(std::string filename);

private:
  int dotsPerInch;
  int dotsPerHour;
  int dotsDayWidth;
  HPDF_PageDirection pageOrientation;
  HPDF_PageSizes pageSize;
  HPDF_Doc doc;
};

#endif // PDFWRITER_H
