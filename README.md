# Timetable Drawer
Create a pdf timetable for each week of the semester

## About

As the university administration tool KLIPS 2.0 (Uni Koeln) does not have
an option to export the personal timetables for the whole semester,
this script was created to transform the data that can be exported from
KLIPS 2.0 into a printable format.

## System requirements
The code is developed and tested on an Ubuntu system with Python 2.7.6.
It might also work on other Linux systems and/or with other Python versions.

For the pdf file creation two external programs are called:
- `rsvg-convert` and
- `pdftk`,

i.e. these programs should be installed on your system.

## Usage
1. Export of the csv file from [KLIPS 2.0](https://klips2.uni-koeln.de/co/webnav.ini)
   Visitenkarte -> Terminkalender -> Export
   - Datei-Format: Download als CSV
   - Zeichensatz:  UTF-8
2. Run the script by typing the following command in a terminal

   ```
   python timetableDrawer.py -c exportedfromklips.csv [-o output.pdf]
   ```
   
   where `exportedfromklips.csv` needs to be replaced by the filename of the exported csv file.
   `output.pdf` or any other filename can be provided to specify the name (and path) of the output pdf.
