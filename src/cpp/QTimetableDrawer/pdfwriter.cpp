#include "pdfwriter.h"

PDFWriter::PDFWriter()
{
  dotsPerInch = 72;
  dotsPerHour = 60;
  dotsDayWidth = 220;
  pageOrientation = HPDF_PAGE_LANDSCAPE;
  pageSize = HPDF_PAGE_SIZE_A4;

  doc = HPDF_New(NULL,NULL);
}

PDFWriter::~PDFWriter()
{
  HPDF_Free(doc);
}

bool PDFWriter::drawTimetableFromEvents(std::vector<Event> events)
{
  HPDF_Page page1 = HPDF_AddPage(doc);
  HPDF_Page_SetSize(page1,pageSize,pageOrientation);

  HPDF_Page_Rectangle(page1,50,50,HPDF_Page_GetWidth(page1)-100,HPDF_Page_GetHeight(page1)-110);
  HPDF_Page_Stroke(page1);

  return true;
}

bool PDFWriter::saveToFile(std::string filename)
{
  try
  {
    HPDF_SaveToFile(doc,filename.c_str());
  }
  catch(std::exception e)
  {
    return false;
  }
  return true;
}
