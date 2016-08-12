#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"
#include <string>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->colorTable->setColumnWidth(0,200);
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

void MainWindow::on_inputFileSelectionButton_clicked()
{
  QString inputFileName = QFileDialog::getOpenFileName(this,tr("Eingabedatei auswählen"),"",tr("CSV Datei (*.csv)"));
  ui->inputFileLineEdit->setText(inputFileName);
}

void MainWindow::on_outputFileSelectionButton_clicked()
{
  QString outputFileName = QFileDialog::getSaveFileName(this,tr("Ausgabedatei auswählen"),"",tr("PDF Datei (*.pdf)"));
  ui->outputFileLineEdit->setText(outputFileName);
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
