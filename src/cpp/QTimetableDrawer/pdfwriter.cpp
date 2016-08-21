#include "pdfwriter.h"
#include <algorithm>

PDFWriter::PDFWriter()
{
  startTime = 8;
  endTime = 20;

  dotsPerInch = 72;
  dotsPerHour = 36;
  dotsDayWidth = 120;
  dotsTimeColumnWidth = 70;
  dotsHeaderRowHeight = 30;
  dotsMinPageMarginHorizontal = 50;
  dotsMinPageMarginVertical = 50;

  dotsTimetableSizeX = dotsTimeColumnWidth + 5 * dotsDayWidth;
  dotsTimetableSizeY = dotsHeaderRowHeight + (endTime - startTime) * dotsPerHour;

  if(dotsTimetableSizeX > dotsTimetableSizeY)
    pageOrientation = HPDF_PAGE_LANDSCAPE;
  else
    pageOrientation = HPDF_PAGE_PORTRAIT;

  pageSize = HPDF_PAGE_SIZE_A4;

  doc = HPDF_New(NULL,NULL);

  documentFont = HPDF_GetFont(doc,"Times-Roman",NULL);
}

PDFWriter::~PDFWriter()
{
  HPDF_Free(doc);
}

bool PDFWriter::drawTimetableGrid(HPDF_Page &page, int weeknumber)
{
  HPDF_REAL pageWidth = HPDF_Page_GetWidth(page);
  HPDF_REAL pageHeight = HPDF_Page_GetHeight(page);

  HPDF_REAL dotsPageMarginHorizontal = std::max((double)dotsMinPageMarginHorizontal, 0.5 * (pageWidth - dotsTimetableSizeX));
  HPDF_REAL dotsPageMarginVertical = std::max((double)dotsMinPageMarginVertical, 0.5 * (pageHeight - dotsTimetableSizeY));

  // Weeknumber in the top left corner
  if(weeknumber > 0)
  {
    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page,documentFont,14);
    std::string weekString = "KW " + std::to_string(weeknumber);
    HPDF_REAL textWidth = HPDF_Page_TextWidth(page,weekString.c_str());
    HPDF_Page_TextOut(page,dotsPageMarginHorizontal + 0.5*(dotsTimeColumnWidth - textWidth),
                      pageHeight - dotsPageMarginVertical - dotsHeaderRowHeight + 0.5*(dotsHeaderRowHeight - 14),
                      weekString.c_str());
    HPDF_Page_EndText(page);
  }

  // Vertical lines
  HPDF_Page_MoveTo(page,dotsPageMarginHorizontal,pageHeight - dotsPageMarginVertical);
  HPDF_Page_LineTo(page,dotsPageMarginHorizontal,dotsPageMarginVertical);
  HPDF_Page_Stroke(page);
  for(int i = 0; i < 6; ++i)
  {
    HPDF_Page_MoveTo(page,dotsPageMarginHorizontal + dotsTimeColumnWidth + i * dotsDayWidth,pageHeight - dotsPageMarginVertical);
    HPDF_Page_LineTo(page,dotsPageMarginHorizontal + dotsTimeColumnWidth + i * dotsDayWidth,dotsPageMarginVertical);
    HPDF_Page_Stroke(page);
  }

  // Horizontal lines
  HPDF_Page_MoveTo(page,dotsPageMarginHorizontal,pageHeight - dotsPageMarginVertical);
  HPDF_Page_LineTo(page,pageWidth - dotsPageMarginHorizontal,pageHeight - dotsPageMarginVertical);
  HPDF_Page_Stroke(page);
  for(int i = 0; i < (endTime - startTime + 1); ++i)
  {
    HPDF_Page_MoveTo(page,dotsPageMarginHorizontal,
                     pageHeight - dotsPageMarginVertical - dotsHeaderRowHeight - i * dotsPerHour);
    HPDF_Page_LineTo(page,pageWidth - dotsPageMarginHorizontal,
                     pageHeight - dotsPageMarginVertical - dotsHeaderRowHeight - i * dotsPerHour);
    HPDF_Page_Stroke(page);

    // Times in the left column
    if(i < (endTime - startTime))
    {
      HPDF_Page_BeginText(page);
      HPDF_REAL textSize = 12;
      HPDF_Page_SetFontAndSize(page,documentFont,textSize);
      std::string timeString = "";
      if(startTime + i < 10)
        timeString += "0";
      timeString += std::to_string(startTime + i) + ":00";
      HPDF_REAL textWidth = HPDF_Page_TextWidth(page,timeString.c_str());
      HPDF_Page_TextOut(page,dotsPageMarginHorizontal + 0.5 * (dotsTimeColumnWidth - textWidth),
                        pageHeight - dotsPageMarginVertical - dotsHeaderRowHeight - i * dotsPerHour - textSize,
                        timeString.c_str());
      HPDF_Page_EndText(page);
    }
  }

  return true;
}

bool PDFWriter::drawTimetableFromEvents(std::vector<Event> events, int weeknumber)
{
  HPDF_Page page = HPDF_AddPage(doc);
  HPDF_Page_SetSize(page,pageSize,pageOrientation);

  drawTimetableGrid(page,0);

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