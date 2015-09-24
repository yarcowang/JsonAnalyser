#include "newfromurldialog.h"
#include "ui_newfromurldialog.h"

#include <QNetworkRequest>
#include <QNetworkReply>

NewFromUrlDialog::NewFromUrlDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::NewFromUrlDialog)
{
  ui->setupUi(this);

  setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

  QRegExp re("http[s]?://.+");
  ui->leUrl->setValidator(new QRegExpValidator(re, this));

  connect(&m, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_json_downloaded(QNetworkReply*)));
}

NewFromUrlDialog::~NewFromUrlDialog()
{
  delete ui;
}

void NewFromUrlDialog::leaveEvent(QEvent *)
{
  hide();
}

void NewFromUrlDialog::on_leUrl_textChanged(const QString &arg1)
{
  ui->btGet->setEnabled(ui->leUrl->hasAcceptableInput());
}

void NewFromUrlDialog::on_btGet_clicked()
{
	QNetworkRequest r(QUrl(ui->leUrl->text()));
	m.get(r);
}

void NewFromUrlDialog::on_json_downloaded(QNetworkReply *r)
{
	buff = QString(r->readAll());
	r->deleteLater();
	accept();
}


