#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QPlainTextEdit>
#include <QStatusBar>

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  QLabel *statusLabel;
  QStatusBar *statusBar;
  QPlainTextEdit *textEditor;
  QMenuBar *menuBar;

  enum FileOpType { SAVE, OPEN };
  struct ActionParam {
    QString actionName;
    QKeySequence shortcut;
    void (MainWindow::*slot)();
  };

  void createWidgets();
  void setupMenu();

  void setupAction(ActionParam actionParam, QMenu *targetMenu);

  QString getFileName(QString propose, FileOpType);
  QString getSaveFileName();
  QString getFilePath();
  QString readFileContents(QString fileName);
  void setFilePath(QString filePath);

  void exitApp();
  void openFile();
  void save();
  void saveAs();
  void saveAndClose();
};
#endif  // MAINWINDOW_H
