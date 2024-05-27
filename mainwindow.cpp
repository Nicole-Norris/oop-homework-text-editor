#include "mainwindow.h"
#include <QPlainTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMenuBar* menuBar = new QMenuBar(this);
    this->statusBar = new QStatusBar(this);
    this->textEditor = new QPlainTextEdit(this);
    this->statusLabel = new QLabel();
    this->statusBar->addWidget(this->statusLabel);
    setStatusBar(statusBar);
    setCentralWidget(this->textEditor);
    setMenuBar(menuBar);

    QMenu* fileMenu = menuBar->addMenu("檔案");
    QAction* openNewAction = new QAction("開啟新檔");
    QAction* openAction = new QAction("開啟舊檔");
    QAction* saveAndCloseAction = new QAction("存檔並關閉");

    fileMenu->addAction(openNewAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAndCloseAction);

    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveAndCloseAction, &QAction::triggered, this, &MainWindow::save);
    connect(openNewAction, &QAction::triggered, this, &MainWindow::save);
}

MainWindow::~MainWindow() {}

void MainWindow::openFile()
{
    QString selectedFilter{"Text Documents (*.txt)"};
    QString fileName = QFileDialog::getOpenFileName(this, "開啟檔案", "", "All File (*.*);;Text Documents (*.txt)", &selectedFilter);
    this->statusLabel->setText(fileName);
    QFile file{fileName};
    QString line;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        while (!stream.atEnd()){
            line.append(stream.readLine()+"\n");
        }
        this->textEditor->setPlainText(line);
    }
    file.close();
}

void MainWindow::save() {
    QString selectedFilter{"Text Documents (*.txt)"};
    QString fileName = QFileDialog::getSaveFileName(this, "儲存檔案", "", "All File (*.*);;Text Documents (*.txt)", &selectedFilter);
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << (this->textEditor->toPlainText());
        file.close();
    }
    this->statusLabel->setText(fileName);
}

void MainWindow::exitApp()
{
    close();
}

