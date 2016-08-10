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


import sys
from subprocess import call
import os

# Own modules
from ttd_svgoutput import *
from ttd_csvinput import *


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


  
main()

