#include "membrane_test_gui.h"
#include <iostream>

#include <QApplication>

int main(int argv, char **argc) {

  QApplication app(argv, argc);
  
  NewMembraneTestGUI window;
//  double fuckall = window.hold_voltage->value();
//  std::cout<<fuckall<<std::endl;
//  window->createGUI();
//  window.setFixedSize(500,350);
  window.show();
  
  return app.exec();
  
};
