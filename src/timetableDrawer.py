#!/usr/bin/python
"""Create a pdf timetable for each week of the semester

As the university administration tool KLIPS 2.0 (Uni Koeln) does not have
an option to export the personal timetables for the whole semester,
this script was created to transform the data that can be exported from
KLIPS 2.0 into a printable format.

Steps for creating the required input file:
  Visitenkarte
   -> Terminkalender
       -> Export
           -> Datei-Format: Download als CSV
              Zeichensatz:  UTF-8

Usage:
  If the script is runnable:
    $ ./timetableDrawer.py -c exportedfromklips.csv [-o output.pdf]
  Else:
    $ python timetableDrawer.py -c exportedfromklips.csv [-o output.pdf]
"""

__author__ = "Gregor Heiming"
__copyright__ = "Copyright 2016, Gregor Heiming"
__credits__ = ["Gregor Heiming"]
__version__ = "0.1"
__maintainer__ = "Gregor Heiming"
__email__ = "gregor.heiming@rwth-aachen.de"



import datetime
import csv
import sys
from collections import namedtuple
from subprocess import call
from math import sqrt
import os


imgMargin = 10
x0 = 80 # Linker Rand des Montags
y0 = 50 # t0 (frueheste Zeit) bei y = 100px
t0 = 8 # frueheste Zeit
tn = 20 # spaeteste Zeit
pxPerHour = 60
daywidth = 220
pagebounds = 70 # Abstand zum Seitenrand
maxNumCharsInLine = 34

# Use colors from http://www.discoveryplayground.com/computer-programming-for-kids/rgb-colors/
colors = {
  "Mathematik" : "orange"
}

imgSizeX = x0 + 5*daywidth + imgMargin
imgSizeY = y0 + (tn-t0+1)*pxPerHour + imgMargin
pageSizeX = 0;
pageSizeY = 0;
if imgSizeX > imgSizeY:
  # landscape format
  if imgSizeX < sqrt(2)*imgSizeY:
    # y direction bounds page size
    pageSizeY = imgSizeY + 2*pagebounds
    pageSizeX = sqrt(2)*pageSizeY
  else:
    # x direction bounds page size
    pageSizeX = imgSizeX + 2*pagebounds
    pageSizeY = pageSizeX / sqrt(2)
else:
  # portrait format
  if imgSizeY < sqrt(2)*imgSizeX:
    # x direction bounds page size
    pageSizeX = imgSizeX + 2*pagebounds
    pageSizeY = pageSizeX * sqrt(2)
  else:
    # y direction bounds page size
    pageSizeY = imgSizeY + 2*pagebounds
    pageSizeX = pageSizeY / sqrt(2)

Event = namedtuple("Event","name date time duration room")

def printHelpMessage():
  print("Usage: " + sys.argv[0] + " -c <in.csv> -o <out.svg> [options]")
  print("Options:")
  print("  -h [--help]  Print this message")
  print("  -c           Provide input csv file <in.csv> (export from Klips)")
  print("  -o           Provide output file <out.svg>")

def main():
  csvfile = ""
  outputfile = "semesterplan.pdf"

  if len(sys.argv) == 1:
    printHelpMessage()

  for i in range(len(sys.argv)):
    if sys.argv[i] == "-h" or sys.argv[i] == "--help":
      printHelpMessage()
      return
    elif sys.argv[i] == "-c" and len(sys.argv) > i+1:
      i+=1
      csvfile = sys.argv[i]
      print("Using csv file '" + csvfile + "'")
    elif sys.argv[i] == "-o" and len(sys.argv) > i+1:
      i+=1
      outputfile = sys.argv[i]
  
  print("Writing output to file '" + outputfile + "'")
  
  if csvfile == "":
    print "No inputfile provided!"
    return
  
  events = readCSVFile(csvfile)
  sortedevents = splitEventsByWeeks(events)
  weekofsemester = 0
  writtenfiles = []
  os.system("mkdir tmp")
  for weekevents in sortedevents:
    weekofsemester += 1
    filename = "tmp/semesterwoche" + str(weekofsemester)
    writeEventsToTimeTableSVG(filename + ".svg",weekevents)
    writtenfiles.append(filename)
  
  pdffiles = []
  
  for filename in writtenfiles:
    call(["rsvg-convert","-f","pdf","-o",filename + ".pdf",filename + ".svg"])
    pdffiles.append(filename + ".pdf")
  
  call(["pdftk"] + pdffiles + ["cat","output",outputfile])
  call(["rm","-r","tmp"])
  
def readCSVFile(filename):
  events = []

  with open(filename,'r') as csvfile:
    reader = csv.reader(csvfile)
    first = True
    for row in reader:
      if not first:
        datelist = row[1].split('.')
        evDate = datetime.date(int(datelist[2]),int(datelist[1]),int(datelist[0]))
        timelist = row[2].split(':')
        evTime = datetime.time(int(timelist[0]),int(timelist[1]))
        ev = Event(name=row[6],date=evDate,time=evTime,duration=int(row[4]),room=row[9])
        events.append(ev)
      else:
        first = False
    
  return events

def splitEventsByWeeks(events):
  lastcalendarweek = events[0].date.isocalendar()[1]
  lastyear = events[0].date.year
  
  weeklist = []
  sortedevents = []
  
  for event in events:
    if (event.date.isocalendar()[1] > lastcalendarweek and event.date.year == lastyear) or event.date.year > lastyear:
      lastcalendarweek = event.date.isocalendar()[1]
      lastyear = event.date.year
      sortedevents.append(weeklist)
      weeklist = []
    weeklist.append(event)
  sortedevents.append(weeklist)
  
  return sortedevents

def writeEventsToTimeTableSVG(filename,events):
  f = open(filename, 'w')
  viewboxminX = (pageSizeX - imgSizeX) / 2
  viewboxminY = (pageSizeY - imgSizeY) / 2
  f.write("<svg baseProfile=\"full\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns:ev=\"http://www.w3.org/2001/xml-events\" width=\"" + str(pageSizeX) + "\" height=\"" + str(pageSizeY) + "\" viewBox=\"-" + str(viewboxminX) + " -" + str(viewboxminY) + " " + str(pageSizeX) + " " + str(pageSizeY) + "\">\n")
  week = events[0].date.isocalendar()[1]
  f.write(getTimeTableGridAsString(week,events[0].date.year))
  for event in events:
    time = float(event.time.hour) + float(event.time.minute) / 60.0
    endTime = (datetime.datetime.combine(datetime.date.today(), event.time) + datetime.timedelta(minutes=event.duration)).time()
    timeStr = event.time.strftime("%H:%M") + " - " + endTime.strftime("%H:%M")
    f.write(writeBoxString(event.name,event.date.isoweekday(),time,timeStr,event.duration,event.room))
  f.write("</svg>")
  f.close()
  
def writeBoxString(name,day,time,timeStr,duration,room):
  box = ""
  
  color = "linen"
  for subject in colors.keys():
    if subject in name:
      color = colors[subject]
  box += "  <rect x=\"" + str(getXPositionFromDay(day)) + "\" y=\"" + str(getYPositionFromTime(time)) + "\" rx=\"3\" ry=\"3\" width=\"" + str(daywidth) + "\" height=\"" + str(duration * pxPerHour / 60) + "\" fill=\"" + color + "\" stroke=\"black\" />\n"
  
  ystep = 14
  currentY = getYPositionFromTime(time)+12
  
  if duration >= 60:
    while len(name) > maxNumCharsInLine:
      index = len(name)
      while index > maxNumCharsInLine:
        index = name[:index].rfind(' ')
      box += "  <text x=\"" + str(getXPositionFromDay(day)+3) + "\" y=\"" + str(currentY) + "\" font-size=\"14\" text-anchor=\"start\" fill=\"black\">" + name[:index] + "</text>\n"
      currentY += ystep
      name = name[index+1:]
  else:
    if len(name) > maxNumCharsInLine:
      name = name[:maxNumCharsInLine-3] + "..."
  box += "  <text x=\"" + str(getXPositionFromDay(day)+3) + "\" y=\"" + str(currentY) + "\" font-size=\"14\" text-anchor=\"start\" fill=\"black\">" + name + "</text>\n"
  currentY += ystep
  
  box += "  <text x=\"" + str(getXPositionFromDay(day)+3) + "\" y=\"" + str(currentY) + "\" font-size=\"14\" text-anchor=\"start\" fill=\"black\">" + timeStr + "</text>\n"
  currentY += ystep
  
  if duration >= 60:
    while len(room) > maxNumCharsInLine:
      index = len(room)
      while index > maxNumCharsInLine:
        index = room[:index].rfind('(')
      box += "  <text x=\"" + str(getXPositionFromDay(day)+3) + "\" y=\"" + str(currentY) + "\" font-size=\"14\" text-anchor=\"start\" fill=\"black\">" + room[:index] + "</text>\n"
      currentY += ystep
      room = room[index:]
  else:
    if len(room) > maxNumCharsInLine:
      if '(' in room:
        index = room.rfind('(')
        room = room[:index]
      else:
        room = room[:maxNumCharsInLine-3] + "..."
  box += "  <text x=\"" + str(getXPositionFromDay(day)+3) + "\" y=\"" + str(currentY) + "\" font-size=\"14\" text-anchor=\"start\" fill=\"black\">" + room + "</text>\n"
  currentY += ystep
  
  return box

def getYPositionFromTime(t):
  y = y0 + (t-t0) * pxPerHour
  return y

def getXPositionFromDay(d):
  x = x0 + (d-1)*daywidth
  return x

def getTimeTableGridAsString(week,year):
  grid = ""
  
  for t in range(t0,tn+1):
    y = getYPositionFromTime(t)
    grid += "  <line x1=\"" + str(imgMargin) + "\" x2=\"" + str(imgSizeX-imgMargin) + "\" y1=\"" + str(y) + "\" y2=\""+ str(y) + "\" stroke=\"black\" />\n"
    grid += "  <text x=\"15\" y=\"" + str(y+15) + "\" font-size=\"14\" text-anchor=\"start\" fill=\"black\">" + str(t) + ":00</text>\n"
  
  for d in range(1,7):
    x = getXPositionFromDay(d)
    date = datetime.datetime.strptime(str(year) + "-W" + str(week) + "-" + str(d), "%Y-W%W-%w")
    dayname = ""
    if(d == 1):   dayname = "Montag"
    elif(d == 2): dayname = "Dienstag"
    elif(d == 3): dayname = "Mittwoch"
    elif(d == 4): dayname = "Donnerstag"
    elif(d == 5): dayname = "Freitag"
    grid += "  <line x1=\"" + str(x) + "\" x2=\"" + str(x) + "\" y1=\"" + str(imgMargin) + "\" y2=\"" + str(imgSizeY-imgMargin) + "\" stroke=\"black\" />\n"
    grid += "  <text x=\"" + str(x+daywidth/2) + "\" y=\"" + str(y0-25) + "\" font-size=\"20\" text-anchor=\"middle\" fill=\"black\">" + str(dayname) + "</text>\n"
    if d >= 1 and d <= 5:
      grid += "  <text x=\"" + str(x+daywidth/2) + "\" y=\"" + str(y0-5) + "\" font-size=\"18\" text-anchor=\"middle\" fill=\"black\">" + date.strftime("%d.%m.%Y") + "</text>\n"
  
  # outer grid lines
  grid += "  <line x1=\"" + str(imgMargin) + "\" x2=\"" + str(imgSizeX-imgMargin) + "\" y1=\"" + str(imgMargin) + "\" y2=\""+ str(imgMargin) + "\" stroke=\"black\" />\n"
  grid += "  <line x1=\"" + str(imgMargin) + "\" x2=\"" + str(imgMargin) + "\" y1=\"" + str(imgMargin) + "\" y2=\"" + str(imgSizeY-imgMargin) + "\" stroke=\"black\" />\n"
  grid += "  <line x1=\"" + str(imgMargin) + "\" x2=\"" + str(imgSizeX-imgMargin) + "\" y1=\"" + str(imgSizeY-imgMargin) + "\" y2=\"" + str(imgSizeY-imgMargin) + "\" stroke=\"black\" />\n"
  
  grid += "<text x=\"" + str(imgMargin + (x0-imgMargin)/2) + "\" y=\"" + str(y0-15) + "\" font-size=\"16\" text-anchor=\"middle\" fill=\"black\">KW " + str(week) + "</text>\n"
  
  
  return grid
  
  
main()

