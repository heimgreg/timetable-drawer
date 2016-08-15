#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"
#include "qcolordialog.h"
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
