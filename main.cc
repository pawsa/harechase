/** harechase is a simple game for chasing a hare across an empty field. */

#include <QApplication>

#include "harechase.h"

int main(int argc, char *argv[])
{  
  QApplication app(argc, argv);
  HareChase game;
  game.show();
  
  return app.exec();
}
