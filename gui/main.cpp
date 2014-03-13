#include "membrane_test_gui.h"

#include <QApplication>

int main(int argv, char **argc) {

  QApplication app(argv, argc);
  
  NewMembraneTestGUI window;
  window.show();
  
  return app.exec();
  
};
