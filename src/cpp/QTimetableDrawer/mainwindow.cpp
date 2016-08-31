#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timetabledrawer.h"
#include "qfiledialog.h"
#include "qcolordialog.h"
#include "qthread.h"
#include "qmessagebox.h"
#include <string>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->colorTable->setColumnWidth(0,200);
  for(int i = 0; i < ui->colorTable->rowCount(); ++i)
  {
    ui->colorTable->setItem(i,0,new QTableWidgetItem());
    ui->colorTable->setItem(i,1,new QTableWidgetItem());
    ui->colorTable->item(i,1)->setFlags(Qt::ItemIsEnabled);
  }
}

MainWindow::~MainWindow()
{
  delete ui;
}

bool MainWindow::readyForTimetableCreation()
{
  if(ui->inputFileLineEdit->text() != ""
     && ui->outputFileLineEdit->text() != "")
  {
    if(ui->colorCheckBox->isChecked())
    {
      QTableWidgetItem* item = ui->colorTable->item(0,0);
      if(!item || item->text().isEmpty())
      {
        return false;
      }
    }
  }
  else
  {
    return false;
  }
  return true;
}

void MainWindow::enableCreationButtons()
{
  ui->createButton->setEnabled(true);
  ui->actionCreateTimetable->setEnabled(true);
  ui->statusBar->showMessage("Bereit zum Erstellen des Stundenplans");
}

void MainWindow::disableCreationButtons()
{
  ui->createButton->setEnabled(false);
  ui->actionCreateTimetable->setEnabled(false);
  ui->statusBar->clearMessage();
}

void MainWindow::chooseCellColor(int row, int column)
{
  QString dialogTitle = "Wähle eine Farbe";
  if(ui->colorTable->item(row,0) != NULL)
  {
    QString name = ui->colorTable->item(row,0)->text();
    dialogTitle = "Wähle eine Farbe für '" + name + "'";
  }
  QColor color = QColorDialog::getColor(Qt::white,this,dialogTitle,0);
  if(color.isValid())
  {
    if(ui->colorTable->item(row,column) == NULL)
    {
      ui->colorTable->setItem(row,column,new QTableWidgetItem());
    }
    ui->colorTable->item(row,column)->setBackgroundColor(color);
  }
}

void MainWindow::on_error(QString err)
{
  QMessageBox errDialog;
  errDialog.critical(this,"Fehler",err);
}

void MainWindow::on_creationSuccessful()
{
  ui->statusBar->showMessage("Stundenplan wurde erfolgreich erstellt.");
}

void MainWindow::on_colorCheckBox_toggled(bool checked)
{
  if(checked)
  {
    ui->colorTable->setEnabled(true);
  }
  else
  {
    ui->colorTable->setEnabled(false);
  }

  if(readyForTimetableCreation())
  {
    enableCreationButtons();
  }
  else
  {
    disableCreationButtons();
  }
}

void MainWindow::chooseInputFile()
{
  QString inputFileName = QFileDialog::getOpenFileName(this,tr("Eingabedatei auswählen"),"",tr("CSV Datei (*.csv)"));
  ui->inputFileLineEdit->setText(inputFileName);
}

void MainWindow::chooseOutputFile()
{
  QString outputFileName = QFileDialog::getSaveFileName(this,tr("Ausgabedatei auswählen"),"",tr("PDF Datei (*.pdf)"));
  ui->outputFileLineEdit->setText(outputFileName);
}

void MainWindow::on_inputFileSelectionButton_clicked()
{
  chooseInputFile();
}

void MainWindow::on_outputFileSelectionButton_clicked()
{
  chooseOutputFile();
}

void MainWindow::on_inputFileLineEdit_textChanged(const QString &arg1)
{
  if(readyForTimetableCreation())
  {
    enableCreationButtons();
  }
  else
  {
    disableCreationButtons();
  }
}

void MainWindow::on_outputFileLineEdit_textChanged(const QString &arg1)
{
  if(readyForTimetableCreation())
  {
    enableCreationButtons();
  }
  else
  {
    disableCreationButtons();
  }
}

void MainWindow::on_colorTable_cellChanged(int row, int column)
{
  if(readyForTimetableCreation())
  {
    enableCreationButtons();
  }
  else
  {
    disableCreationButtons();
  }
}

void MainWindow::on_colorTable_cellClicked(int row, int column)
{
  if(column == 1)
  {
    chooseCellColor(row,column);
  }
}

void MainWindow::on_colorTable_cellPressed(int row, int column)
{
  if(column == 1)
  {
    chooseCellColor(row,column);
  }
}

void MainWindow::on_actionNew_triggered()
{
  // Clear all user inputs
  ui->inputFileLineEdit->clear();
  ui->outputFileLineEdit->clear();
  ui->colorTable->clear();
}

void MainWindow::on_actionChooseInputFile_triggered()
{
  chooseInputFile();
}

void MainWindow::on_actionChooseOutputFile_triggered()
{
  chooseOutputFile();
}

void MainWindow::on_actionQuit_triggered()
{
  QApplication::quit();
}

void MainWindow::startCreation()
{
  ui->statusBar->clearMessage();

  QThread* thread = new QThread();
  TimetableDrawer* worker = new TimetableDrawer();

  worker->setInputfile(ui->inputFileLineEdit->text().toStdString());
  worker->setOutputfile(ui->outputFileLineEdit->text().toStdString());
  if(ui->colorCheckBox->isChecked())
  {
    std::vector<std::pair<std::string,QColor> > colors;
    for(int i = 0; i < ui->colorTable->rowCount(); ++i)
    {
      if(ui->colorTable->item(i,0)->text() != "")
      {
        std::pair<std::string,QColor> line(ui->colorTable->item(i,0)->text().toStdString(),ui->colorTable->item(i,1)->background().color());
        colors.push_back(line);
      }
    }
    worker->setColors(colors);
  }

  worker->moveToThread(thread);

  connect(thread, SIGNAL(started()), worker, SLOT(process()));

  connect(worker, SIGNAL(error(QString)), this, SLOT(on_error(QString)));
  connect(worker, SIGNAL(error(QString)), thread, SLOT(quit()));
  connect(worker, SIGNAL(error(QString)), worker, SLOT(deleteLater()));

  connect(worker, SIGNAL(finished()), this, SLOT(on_creationSuccessful()));
  connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
  connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));

  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
  thread->start();
}

void MainWindow::on_createButton_clicked()
{
  startCreation();
}

void MainWindow::on_actionCreateTimetable_triggered()
{
  startCreation();
}

void MainWindow::on_actionAbout_triggered()
{
  //QMessageBox::about(this,"Über QTimetableDrawer","Programm zur Erstellung eines ausdruckbaren Stundenplans aus einer online exportierten csv Datei\n\nCopyright (c) 2016 Gregor Heiming\n\n <a href=\"https://github.com/heimgreg/timetable-drawer\">QTimetableDrawer on GitHub</a>");
  QMessageBox aboutBox(this);
  aboutBox.setWindowTitle("Über QTimetableDrawer");
  aboutBox.setIconPixmap(this->windowIcon().pixmap(128));
  aboutBox.setTextFormat(Qt::RichText);
  aboutBox.setText("<h3>Über QTimetableDrawer</h3>Programm zur Erstellung eines ausdruckbaren Stundenplans aus einer online exportierten csv Datei<br><br>Copyright (c) 2016 Gregor Heiming<br><br><a href=\"https://github.com/heimgreg/timetable-drawer\">QTimetableDrawer bei GitHub</a>");
  aboutBox.exec();
}

void MainWindow::on_actionAboutQt_triggered()
{
  QMessageBox::aboutQt(this,"Über Qt");
}
