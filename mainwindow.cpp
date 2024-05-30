#include "mainwindow.h"

#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QPlainTextEdit>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  createWidgets();
  setupMenu();
}

MainWindow::~MainWindow() {}

void MainWindow::createWidgets() {
  this->menuBar = new QMenuBar(this);
  this->statusBar = new QStatusBar(this);
  this->textEditor = new QPlainTextEdit(this);
  this->statusLabel = new QLabel();
  this->statusBar->addWidget(this->statusLabel);
  setMenuBar(menuBar);
  setStatusBar(statusBar);
  setCentralWidget(this->textEditor);
}

void MainWindow::setupMenu() {
  QMenu* fileMenu = menuBar->addMenu("檔案");
  QAction* openNewAction = new QAction("開啟新檔");
  QAction* openAction = new QAction("開啟舊檔");
  QAction* saveAction = new QAction("存檔");
  QAction* saveAsAction = new QAction("另存新檔");
  QAction* saveAndCloseAction = new QAction("存檔並關閉");
  fileMenu->addAction(openNewAction);
  fileMenu->addAction(openAction);
  fileMenu->addAction(saveAction);
  fileMenu->addAction(saveAsAction);
  fileMenu->addAction(saveAndCloseAction);
  connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
  connect(openNewAction, &QAction::triggered, this, &MainWindow::save);
  connect(saveAction, &QAction::triggered, this, &MainWindow::save);
  connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);
  connect(saveAndCloseAction, &QAction::triggered, this,
          &MainWindow::saveAndClose);
}

QString MainWindow::getFileName(QString propose, FileOpType type) {
  QString selectedFilter{"Text Documents (*.txt)"};
  QString fileName;
  if (type == FileOpType::OPEN) {
    fileName = QFileDialog::getOpenFileName(
        this, propose, "", "All File (*.*);;Text Documents (*.txt)",
        &selectedFilter);
  } else {
    fileName = QFileDialog::getSaveFileName(
        this, propose, "", "All File (*.*);;Text Documents (*.txt)",
        &selectedFilter);
  }
  return fileName;
}

QString MainWindow::readFileContents(QString fileName) {
  QFile file{fileName};
  QString fileContents;
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream stream(&file);
    while (!stream.atEnd()) {
      fileContents.append(stream.readLine() + "\n");
    }
  }
  file.close();
  return fileContents;
}

QString MainWindow::getFilePath() { return this->statusLabel->text(); }

void MainWindow::setFilePath(QString filePath) {
  this->statusLabel->setText(filePath);
}

QString MainWindow::getSaveFileName() {
  QString fileName;
  QString filePath = getFilePath();
  if (filePath == "") {
    fileName = getFileName("儲存檔案", FileOpType::SAVE);
  } else {
    fileName = filePath;
  }
  return fileName;
}

void MainWindow::openFile() {
  QString fileName = getFileName("開啟檔案", FileOpType::OPEN);
  setFilePath(fileName);
  QString fileContents = readFileContents(fileName);
  this->textEditor->setPlainText(fileContents);
}

void MainWindow::saveAs() {
    setFilePath("");
    save();
}

void MainWindow::save() {
  QString fileName = getSaveFileName();
  QFile file(fileName);
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream out(&file);
    out << (this->textEditor->toPlainText());
  }
  file.close();
  setFilePath(fileName);
}

void MainWindow::exitApp() { close(); }

void MainWindow::saveAndClose() {
  save();
  close();
}
