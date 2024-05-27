#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QPlainTextEdit>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void exitApp();
    void openFile();
    void save();
    QLabel *statusLabel;
    QStatusBar *statusBar;
    QPlainTextEdit *textEditor;
};
#endif // MAINWINDOW_H
