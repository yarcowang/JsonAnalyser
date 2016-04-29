#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myjsonmodel.h"

namespace Ui {
  class MainWindow;
}

class QtAwesome;
class NewFromUrlDialog;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_actionNew_From_Clipboard_triggered();

  void on_actionNew_From_File_triggered();

  void on_btHide_clicked();

  void on_actionHide_Dock_toggled(bool arg1);

  void on_actionNew_From_Url_triggered();

  void on_teJson_textChanged();

  void on_btIndent_clicked();

  void on_btAnalyse_clicked();

  void on_cbAuto_toggled(bool checked);

private:
  Ui::MainWindow *ui;

  NewFromUrlDialog* nfud;

  QtAwesome* awesome;

  bool showLeftDock;
  void toggle_leftDock(int isShow = 0);

  bool validateJson();

  MyJsonModel* model;

};

#endif // MAINWINDOW_H
