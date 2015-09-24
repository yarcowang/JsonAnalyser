#ifndef NEWFROMURLDIALOG_H
#define NEWFROMURLDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QString>

namespace Ui {
  class NewFromUrlDialog;
}

class NewFromUrlDialog : public QDialog
{
  Q_OBJECT

public:
  explicit NewFromUrlDialog(QWidget *parent = 0);
  ~NewFromUrlDialog();

  const QString getBuff() const { return buff; }

protected:
  void leaveEvent(QEvent*);

private slots:
  void on_leUrl_textChanged(const QString &arg1);

  void on_btGet_clicked();

  void on_json_downloaded(QNetworkReply*);

private:
  Ui::NewFromUrlDialog *ui;

  QNetworkAccessManager m;

  QString buff;
};

#endif // NEWFROMURLDIALOG_H
