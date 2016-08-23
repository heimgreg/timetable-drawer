#include "pdfwriter.h"
#include <algorithm>

PDFWriter::PDFWriter()
{
  startTime = 8;
  endTime = 20;

  dotsPerInch = 72;
  dotsPerHour = 36;
  dotsDayWidth = 122;
  dotsTimeColumnWidth = 50;
  dotsHeaderRowHeight = 33;
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

void PDFWriter::setColorCodes(std::vector<ColorCode> cc)
{
  colorCodes = cc;
}

void PDFWriter::getDateStringFromWeeknumberAndWeekday(int year, int weeknumber, int weekday, char* date)
{
  std::string knownDates = std::to_string(year) + " " + std::to_string(weeknumber) + " " + std::to_string(weekday);
  tm datetime;
  strptime(knownDates.c_str(),"%Y %W %w",&datetime);
  strftime(date,12,"%d.%m.%Y",&datetime);
}

bool PDFWriter::drawTimetableGrid(HPDF_Page &page, int year, int weeknumber)
{
  HPDF_REAL pageWidth = HPDF_Page_GetWidth(page);
  HPDF_REAL pageHeight = HPDF_Page_GetHeight(page);

  dotsPageMarginHorizontal = std::max((double)dotsMinPageMarginHorizontal, 0.5 * (pageWidth - dotsTimetableSizeX));
  dotsPageMarginVertical = std::max((double)dotsMinPageMarginVertical, 0.5 * (pageHeight - dotsTimetableSizeY));

  // Weeknumber in the top left corner
  if(weeknumber > 0)
  {
    HPDF_Page_BeginText(page);
    HPDF_REAL fontSize = 12;
    HPDF_Page_SetFontAndSize(page,documentFont,fontSize);
    std::string weekString = "KW " + std::to_string(weeknumber);
    HPDF_REAL textWidth = HPDF_Page_TextWidth(page,weekString.c_str());
    HPDF_Page_TextOut(page,dotsPageMarginHorizontal + 0.5*(dotsTimeColumnWidth - textWidth),
                      pageHeight - dotsPageMarginVertical - dotsHeaderRowHeight + 0.5*(dotsHeaderRowHeight - fontSize),
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

    // Weekday and date in the header line
    if(i < 5)
    {
      HPDF_Page_BeginText(page);
      HPDF_REAL textSize = 14;
      std::string headerText = weekdays[i+1];
      if(weeknumber > 0)
      {
        char dateString[12];
        getDateStringFromWeeknumberAndWeekday(year,weeknumber,i+1,dateString);
        headerText += "\n" + std::string(dateString);
      }
      HPDF_Page_SetFontAndSize(page,documentFont,textSize);
      HPDF_Page_TextRect(page,dotsPageMarginHorizontal + dotsTimeColumnWidth + i * dotsDayWidth,
                         pageHeight - dotsPageMarginVertical,
                         dotsPageMarginHorizontal + dotsTimeColumnWidth + (i+1) * dotsDayWidth,
                         pageHeight - dotsPageMarginVertical - dotsHeaderRowHeight,
                         headerText.c_str(),HPDF_TALIGN_CENTER,NULL);
      HPDF_Page_EndText(page);
    }
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
      HPDF_REAL textSize = 11;
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

bool PDFWriter::drawEvent(HPDF_Page& page, Event ev)
{
  HPDF_REAL x = dotsPageMarginHorizontal + dotsTimeColumnWidth + (ev.datetime.tm_wday-1) * dotsDayWidth;
  HPDF_REAL y = HPDF_Page_GetHeight(page) - dotsPageMarginVertical - dotsHeaderRowHeight - (ev.datetime.tm_hour + 1.0/60.0 * (ev.datetime.tm_min + ev.duration) - startTime) * dotsPerHour;

  HPDF_Page_SetRGBFill(page,0.9,0.9,0.9);
  for(unsigned i = 0; i < colorCodes.size(); ++i)
  {
    std::string lcTitle = ev.name;
    std::transform(lcTitle.begin(), lcTitle.end(), lcTitle.begin(), ::tolower);
    std::string lcKeyword = colorCodes[i].keyword;
    std::transform(lcKeyword.begin(), lcKeyword.end(), lcKeyword.begin(), ::tolower);

    if(lcTitle.find(lcKeyword) != std::string::npos)
    {
      HPDF_Page_SetRGBFill(page,colorCodes[i].r,colorCodes[i].g,colorCodes[i].b);
      break;
    }
  }

  HPDF_Page_Rectangle(page,x,y,dotsDayWidth,ev.duration*dotsPerHour/60.0);
  HPDF_Page_FillStroke(page);

  int hour1 = ev.datetime.tm_hour;
  int min1 = ev.datetime.tm_min;
  int hour2 = hour1 + (int)(ev.duration / 60.0);
  int min2 = (min1 + ev.duration % 60) % 60;
  std::string evStartTime = std::to_string(hour1) + ":";
  if(min1 < 10)
    evStartTime += "0";
  evStartTime += std::to_string(min1);

  std::string evEndTime = std::to_string(hour2) + ":";
  if(min2 < 10)
    evEndTime += "0";
  evEndTime += std::to_string(min2);

  std::string evStr = ev.name + "\n" + evStartTime + " - " + evEndTime + "\n" + ev.room;

  HPDF_Page_BeginText(page);
  HPDF_Page_SetFontAndSize(page,documentFont,10);
  HPDF_Page_SetTextLeading(page,11);
  HPDF_Page_SetRGBFill(page,0,0,0);
  HPDF_Page_TextRect(page,x,y+ev.duration*dotsPerHour/60.0,x+dotsDayWidth,y,evStr.c_str(),HPDF_TALIGN_LEFT,NULL);
  HPDF_Page_EndText(page);

  return true;
}

bool PDFWriter::drawTimetableFromEvents(std::vector<Event> events, int year, int weeknumber)
{
  HPDF_Page page = HPDF_AddPage(doc);
  HPDF_Page_SetSize(page,pageSize,pageOrientation);

  drawTimetableGrid(page,year,weeknumber);

  for(unsigned i = 0; i < events.size(); ++i)
  {
    if(!drawEvent(page,events[i]))
    {
      return false;
    }
  }

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
