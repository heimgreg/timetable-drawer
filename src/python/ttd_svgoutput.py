#!/usr/bin/python

from ttd_definitions import *
from ttd_colors import *
import datetime

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

