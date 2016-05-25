#!/usr/bin/python

import csv
import datetime
from ttd_definitions import *

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

