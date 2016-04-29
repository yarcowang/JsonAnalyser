#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QMessageBox>
#include <QJsonParseError>

#include "newfromurldialog.h"
#include "myjsonmodelitem.h"
#include "QtAwesome.h"

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

  QVBoxLayout* layout = new QVBoxLayout;
  QLabel* p;
  p = new QLabel("<b>Symbols:</b> @=object | ..@..=objects,show one | (n)=array has n items");
  layout->addWidget(p);
  QString s;
  s = "<b>Colors:</b> ";
  for(int i = 0; i < MyJsonModelItem::T_LEN; i++) {
    s += QString("<font color=\"%1\">%2</font> | ").arg(MyJsonModelItem::mapType2Color[i], MyJsonModelItem::mapType2Label[i]);
  }
  p = new QLabel(s);
  layout->addWidget(p);
  ui->fmLabels->setLayout(layout);

  model = new MyJsonModel;
  ui->tvJsonView->setModel(model);
  ui->tvJsonView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
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

	if (ui->cbAuto->checkState() == Qt::Checked) {
		model->loadJson(ui->teJson->toPlainText().toUtf8(), &e);
		ui->tvJsonView->expandAll();
	}

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

void MainWindow::on_btAnalyse_clicked()
{
	QJsonParseError e;
	model->loadJson(ui->teJson->toPlainText().toUtf8(), &e);
	ui->tvJsonView->expandAll();
}

void MainWindow::on_cbAuto_toggled(bool checked)
{
	if (checked) {
		QJsonParseError e;
		model->loadJson(ui->teJson->toPlainText().toUtf8(), &e);
		ui->tvJsonView->expandAll();
	}
}
