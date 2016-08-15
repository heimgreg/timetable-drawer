#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_colorCheckBox_toggled(bool checked);

  void on_inputFileSelectionButton_clicked();

  void on_outputFileSelectionButton_clicked();

  void on_inputFileLineEdit_textChanged(const QString &arg1);

  void on_outputFileLineEdit_textChanged(const QString &arg1);

  void on_colorTable_cellChanged(int row, int column);

  void on_colorTable_cellClicked(int row, int column);

  void on_colorTable_cellPressed(int row, int column);

  void on_actionNew_triggered();

  void on_actionChooseInputFile_triggered();

  void on_actionChooseOutputFile_triggered();

  void on_actionQuit_triggered();

private:
    Ui::MainWindow *ui;
    void chooseInputFile();
    void chooseOutputFile();
    bool readyForTimetableCreation();
    void enableCreationButtons();
    void disableCreationButtons();
    void chooseCellColor(int row, int column);
};

#endif // MAINWINDOW_H
