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

private:
    Ui::MainWindow *ui;
    bool readyForTimetableCreation();
    void enableCreationButtons();
    void disableCreationButtons();
};

#endif // MAINWINDOW_H
