#include "mainwindow.h"

#include <QFileDialog>
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
  ActionParam openNewAction{"開啟新檔", QKeySequence(Qt::CTRL | Qt::Key_N),
                            &MainWindow::save};
  ActionParam openAction{"開啟舊檔", QKeySequence(Qt::CTRL | Qt::Key_O),
                         &MainWindow::openFile};
  ActionParam saveAction{"存檔", QKeySequence(Qt::CTRL | Qt::Key_S),
                         &MainWindow::save};
  ActionParam saveAsAction{"另存新檔",
                           QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_S),
                           &MainWindow::saveAs};
  ActionParam saveCloseAction{"存檔並關閉",
                              QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S),
                              &MainWindow::saveAndClose};
  setupAction(openNewAction, fileMenu);
  setupAction(openAction, fileMenu);
  setupAction(saveAction, fileMenu);
  setupAction(saveAsAction, fileMenu);
  setupAction(saveCloseAction, fileMenu);
}

void MainWindow::setupAction(ActionParam actionParam, QMenu* targetMenu) {
  QAction* action = new QAction(actionParam.actionName);
  targetMenu->addAction(action);
  action->setShortcut(actionParam.shortcut);
  connect(action, &QAction::triggered, this, actionParam.slot);
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
