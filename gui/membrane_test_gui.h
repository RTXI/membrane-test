#pragma once

#include <QObject>
#include <QWidget>
#include <QLineEdit>
/*
#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QTimer>
#include <QToolTip>
#include <QValidator>
#include <QWhatsThis>

#include <QButtonGroup>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QSpinBox>
#include <QTabWidget>
#include <QRadioButton>
#include <QComboBox>
#include <QString>
#include <QLineEdit>
#include <QGroupBox>
#include <QFormLayout>
*/

class NewMembraneTestGUI : public QWidget {

  Q_OBJECT
  
  public:
    NewMembraneTestGUI(QWidget *parent=0);
/*  
    float hold_voltage;
    float hold_amplitude;
    float hold_period;
    float zap_amplitude;
*/  
  private:
    QLineEdit *hold_volt;
    QLineEdit *hold_peak; 
    QLineEdit *hold_period;

  private slots:
  
};
