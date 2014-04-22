#include "membrane_test_gui.h"

#include <QApplication>

int main(int argv, char **argc) {

  QApplication app(argv, argc);
  
  NewMembraneTestGUI window;
//  window->setFixedSize(300,300);
  window.show();
  
  return app.exec();
  
};
