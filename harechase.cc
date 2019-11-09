#include <math.h>

#include "harechase.h"

#include "ui_harechase.h" // generated file

static const int END_SCORE = 2;

static const int SCORE_WIDTH = 120;
static const int SCORE_HEIGHT = 25;

static const int GAMEOVER_WIDTH = 150;
static const int GAMEOVER_HEIGHT = 90;

static const int HARE_RADIUS = 10;
static const int HARE_V_DELTA = 2;
static const int HUNTER_RADIUS = 15;

static const int KEY_PRESS_DELTA = 10;
HareChase::HareChase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    timerId(0)
{
  ui->setupUi(this);
  score.reset(new QLabel(this));
  startGame();
}

void HareChase::startGame()
{
  if (timerId != 0) {
    killTimer(timerId);
  }
  timerId = startTimer(50);

  srand(time(NULL));
  int randX = 3*(rand() % (2*HARE_V_DELTA+1) - HARE_V_DELTA);
  int randY = 3*(rand() % (2*HARE_V_DELTA+1) - HARE_V_DELTA);

  hareVelocity.setX(randX);
  hareVelocity.setY(randY);

  hare.setX(width()/2);
  hare.setY(height()/2);
  setHareSize();
  setHunterSize();
  caughtHares = 0;

  score->setGeometry((width() - SCORE_WIDTH)/2, 7, SCORE_WIDTH, SCORE_HEIGHT);
  paintScore();
  
  setStyleSheet("background-color: black");

  isOver = false;
}

void HareChase::paintScore()
{
  score->setText(QString("<h2><font align=center color=#33F000>Caught: %1</font></h2>")
		 .arg(caughtHares));
}

void HareChase::setHareSize()
{
  QSize size(HARE_RADIUS, HARE_RADIUS);
  hare.setSize(size);
}

void HareChase::setHunterSize()
{
  QSize size(HUNTER_RADIUS, HUNTER_RADIUS);
  hunter.setSize(size);
}

void HareChase::doTimeStep()
{
  hare.moveTo(hare.x() + hareVelocity.x(),
	      hare.y() + hareVelocity.y());

  int randX = rand() % (2*HARE_V_DELTA+1) - HARE_V_DELTA;
  int randY = rand() % (2*HARE_V_DELTA+1) - HARE_V_DELTA;

  hareVelocity.setX(hareVelocity.x() + randX);
  hareVelocity.setY(hareVelocity.y() + randY);
}

void HareChase::handleCollisions()
{
  if (hare.right() > width() - 1) {
    hareVelocity.setX(-abs(hareVelocity.x()));
  } else if (hare.bottom() > height() - 1) {
    hareVelocity.setY(-abs(hareVelocity.y()));
  } else if (hare.x() < 1) {
    hareVelocity.setX(abs(hareVelocity.x()));
  } else if (hare.top() < 0) {
    hareVelocity.setY(abs(hareVelocity.y()));
  }

  int dx = hare.x() - hunter.x();
  int dy = hare.y() - hunter.y();
  int dist = std::sqrt(dx*dx + dy*dy);

  if (dist < HARE_RADIUS + HUNTER_RADIUS) {
    caughtHares++;
    hare.moveTo((width() - HARE_RADIUS) / 2,
		(height() - HARE_RADIUS) / 2);
  }
}

void HareChase::gameOver()
{
  killTimer(timerId);
  timerId = 0;
  isOver = true;
}

void HareChase::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  if (!isOver) {
    QPen harePen(Qt::yellow);
    QBrush hareBrush(Qt::yellow);
    painter.setPen(harePen);
    painter.setBrush(hareBrush);
    painter.drawEllipse(hare);
    
    QPen hunterPen(Qt::green);
    QBrush hunterBrush(Qt::red);
    painter.setPen(hunterPen);
    painter.setBrush(hunterBrush);
    painter.drawEllipse(hunter);
    
    paintScore();
  } else {
    QPen scoreLine(Qt::green);
    scoreLine.setWidth(4);
    painter.setPen(scoreLine);
    score->setText(QString("<h2 align=\"center\"><font color=#33E111>Game over</font></h2>"));
    score->setGeometry((width() - GAMEOVER_WIDTH)/ 2, (height() - GAMEOVER_HEIGHT)/2, GAMEOVER_WIDTH, GAMEOVER_HEIGHT);
  }

  update();
}

void HareChase::timerEvent(QTimerEvent *)
{
  if (caughtHares == END_SCORE)
    gameOver();
  doTimeStep();
  handleCollisions();
}

void HareChase::keyPressEvent(QKeyEvent *pevent)
{
  switch (pevent->key()) {
  case Qt::Key_Escape:
    QCoreApplication::quit();
    break;
  case Qt::Key_Space:
    startGame();
    break;
  case Qt::Key_Up:
    if (hunter.y() <= HUNTER_RADIUS)
      return;
    hunter.moveTop(hunter.y() - KEY_PRESS_DELTA);
    break;
  case Qt::Key_Down:
    if (hunter.y() >= height() - HUNTER_RADIUS - KEY_PRESS_DELTA)
      return;
    hunter.moveTop(hunter.y() + KEY_PRESS_DELTA);
    break;
  case Qt::Key_Left:
    if (hunter.x() <= HUNTER_RADIUS)
      return;
    hunter.moveLeft(hunter.x() -KEY_PRESS_DELTA);
    break;
  case Qt::Key_Right:
    if (hunter.x() >= width() - HUNTER_RADIUS - KEY_PRESS_DELTA)
      return;
    hunter.moveLeft(hunter.x() + KEY_PRESS_DELTA);
    break;
  }
}
