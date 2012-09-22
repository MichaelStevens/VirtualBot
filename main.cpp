#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include "VirtualBot.h"


void center(QWidget &widget)
{
  int x, y;
  int screenWidth;
  int screenHeight;

  int WIDTH = 1100;
  int HEIGHT = 550;

  QDesktopWidget *desktop = QApplication::desktop();

  screenWidth = desktop->width();
  screenHeight = desktop->height();

  x = (screenWidth - WIDTH) / 2;
  y = (screenHeight - HEIGHT) / 2;

  widget.setGeometry(x, y, WIDTH, HEIGHT);
}


int main(int argc, char *argv[])
{
  QApplication app(argc, argv); 

  VirtualBot window;

  window.setWindowTitle("VirtualBot");
  window.show();
  center(window);
  
  return app.exec();
}
