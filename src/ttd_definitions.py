#!/usr/bin/python

from collections import namedtuple
from math import sqrt

Event = namedtuple("Event","name date time duration room")

imgMargin = 10
x0 = 80 # Linker Rand des Montags
y0 = 50 # t0 (frueheste Zeit) bei y = 100px
t0 = 8 # frueheste Zeit
tn = 20 # spaeteste Zeit
pxPerHour = 60
daywidth = 220
pagebounds = 70 # Abstand zum Seitenrand
maxNumCharsInLine = 34

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
