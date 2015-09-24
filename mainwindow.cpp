#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QMessageBox>
#include <QJsonParseError>

#include "myjsonmodel.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  showLeftDock(true)
{
  ui->setupUi(this);

  awesome = new QtAwesome(this);
  awesome->initFontAwesome();
  awesome->setDefaultOption("scale-factor", 0.8);

  ui->actionNew_From_Clipboard->setIcon(awesome->icon(fa::clipboard));
  ui->actionNew_From_File->setIcon(awesome->icon(fa::file));
  ui->actionNew_From_Url->setIcon(awesome->icon(fa::link));

  ui->btHide->setIcon(awesome->icon(fa::caretleft));
  ui->btHide->setToolTip(tr("hide this"));
  ui->btIndent->setIcon(awesome->icon(fa::indent));
  ui->btIndent->setToolTip(tr("indent text"));
  ui->btIndent->setEnabled(false);
  ui->btAnalyse->setIcon(awesome->icon(fa::rocket));
  ui->btAnalyse->setToolTip(tr("analyse it"));
  ui->btAnalyse->setEnabled(false);
}

MainWindow::~MainWindow()
{
  delete awesome;
  delete ui;
}

void MainWindow::toggle_leftDock(int isShow)
{
	switch (isShow) {
		case 1:
			showLeftDock = true;
			break;
		case -1:
			showLeftDock = false;
			break;
		default:
			showLeftDock = !showLeftDock;
	}

	showLeftDock ? ui->widget->show() : ui->widget->hide();
	ui->actionHide_Dock->setChecked(showLeftDock);
}

bool MainWindow::validateJson()
{
	QJsonParseError e;
	bool ret;
	MyJsonModel m;
	ret = m.loadJson(ui->teJson->toPlainText().toUtf8(), &e);

	if (!ret && ui->cbWarningMe->checkState() == Qt::Checked) {
		QMessageBox::warning(this, tr("Json Validate"), tr("Json parse error: ") + e.errorString(), QMessageBox::Ok);
	}

	ui->btIndent->setEnabled(ret);
	ui->btAnalyse->setEnabled(ret);

	return ret;
}

void MainWindow::on_btHide_clicked()
{
	toggle_leftDock(-1);
}

void MainWindow::on_actionNew_From_Clipboard_triggered()
{
  QClipboard* p = QApplication::clipboard();
  ui->teJson->setPlainText(p->text());
  toggle_leftDock(1);
}

void MainWindow::on_actionNew_From_File_triggered()
{
  QString filename = QFileDialog::getOpenFileName(this, tr("Open Json File"), "", tr("Json File (*.json *.js *.txt)"));
  if (filename.isEmpty()) {
    return;
  }

  QFile file(filename);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    QMessageBox::critical(this, tr("New From File"), tr("File open error: ") + filename, QMessageBox::Ok);
    return;
  }

	QTextStream in(&file);
	ui->teJson->setPlainText(in.readAll());
	file.close();
	toggle_leftDock(1);
}

void MainWindow::on_actionNew_From_Url_triggered()
{
	if (!nfud) {
		nfud = new NewFromUrlDialog(this);
	}

	if (nfud->exec()) {
		ui->teJson->setPlainText(nfud->getBuff());
		toggle_leftDock(1);
	}
}

void MainWindow::on_actionHide_Dock_toggled(bool arg1)
{
	toggle_leftDock(arg1 ? 1 : -1);
}

void MainWindow::on_teJson_textChanged()
{
	validateJson();
}

void MainWindow::on_btIndent_clicked()
{
	QJsonParseError e;
	bool ret;
	MyJsonModel m;
	ret = m.loadJson(ui->teJson->toPlainText().toUtf8(), &e);
	if (ret) {
		ui->teJson->setPlainText(QString(m.toJson()));
	}
}
