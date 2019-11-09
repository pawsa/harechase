#ifndef HARECHASE_H
#define HARECHASE_H

#include <QDialog>
#include <QLabel>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>

#include <stdlib.h>
#include <time.h>

#include <memory>

namespace Ui {
  class Dialog;
}

class HareChase : public QDialog
{
    Q_OBJECT

public:
  explicit HareChase(QWidget *parent = 0);
  void doTimeStep();
  void handleCollisions();
  void gameOver();

private:
  Ui::Dialog* ui;
  std::unique_ptr<QLabel> score;
  QPoint hareVelocity;
  QRect hare;
  QRect hunter;
  
  int caughtHares;

  int timerId;
  bool isOver;

  void paintScore();
  void setHareSize();
  void setHunterSize();
  void startGame();
protected:
  void paintEvent(QPaintEvent *);
  void timerEvent(QTimerEvent *event);
  void keyPressEvent(QKeyEvent *pevent);
};

#endif // HARECHASE_H
