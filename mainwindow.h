#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "QtAwesome.h"
#include "newfromurldialog.h"

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
  void on_actionNew_From_Clipboard_triggered();

  void on_actionNew_From_File_triggered();

  void on_btHide_clicked();

  void on_actionHide_Dock_toggled(bool arg1);

  void on_actionNew_From_Url_triggered();

  void on_teJson_textChanged();

  void on_btIndent_clicked();

private:
  Ui::MainWindow *ui;

  NewFromUrlDialog* nfud;

  QtAwesome* awesome;

  bool showLeftDock;
  void toggle_leftDock(int isShow = 0);

  bool validateJson();

};

#endif // MAINWINDOW_H
