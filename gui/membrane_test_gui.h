#pragma once

#include <QObject>
#include <QWidget>
#include <QLineEdit>

class NewMembraneTestGUI : public QWidget {

  Q_OBJECT
  
  public:
    NewMembraneTestGUI(void);
	 void createGUI(void);
	 void beHappy(void);
  
  private:
    QLineEdit *hold_volt;
    QLineEdit *hold_peak; 
    QLineEdit *hold_period;

  private slots:
  
};
