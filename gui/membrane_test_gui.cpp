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
#include <QComboBox>
#include <QString>
#include <QLineEdit>
#include <QGroupBox>
#include <QFormLayout>

#include "membrane_test_gui.h"


NewMembraneTestGUI::NewMembraneTestGUI(QWidget *parent) : QWidget(parent) {
  
  //Define basic plugin outline
  QVBoxLayout *plugin_layout = new QVBoxLayout(this);
  plugin_layout->setContentsMargins(QMargins(0,0,0,0));
  
  //Create Cancel, Update, and Reset buttons
  QWidget *utility = new QWidget;
  QHBoxLayout *utility_layout = new QHBoxLayout;
  QPushButton *utility_cancelbutton = new QPushButton("Cancel", this);
  QPushButton *utility_updatebutton = new QPushButton("Update", this);
  QPushButton *utility_resetbutton = new QPushButton("Reset", this);
  utility_resetbutton->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
  utility_cancelbutton->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
  utility_updatebutton->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
  utility_layout->addWidget(utility_cancelbutton, 1, Qt::AlignRight);
  utility_layout->addWidget(utility_updatebutton, 0, Qt::AlignRight);
  utility_layout->addWidget(utility_resetbutton, 0, Qt::AlignRight);
  utility->setLayout(utility_layout);
  
  //Create the middle grid layout
  QWidget *middle = new QWidget;
  QGridLayout *middle_layout = new QGridLayout;
  middle->setLayout(middle_layout);
  
  //Define Hold Potential group
  /**/QGroupBox *hold_group = new QGroupBox("Holding Potential");
  QFormLayout *hold_layout = new QFormLayout;
  QPushButton *hold_toggle = new QPushButton("Pulse");
  QLineEdit *hold_volt = new QLineEdit;
  QLineEdit *hold_peak = new QLineEdit;
  QLineEdit *hold_period = new QLineEdit;
  hold_layout->addRow(hold_toggle);
  hold_layout->addRow("Hold Voltage (mV)", hold_volt);
  hold_layout->addRow("Amplitude (mV)", hold_peak);
  hold_layout->addRow("Period (ms)", hold_period);
  /**/hold_group->setLayout(hold_layout);
  middle_layout->addWidget(hold_group, 0, 0);
  
  //Define ZAP!! group
  QGroupBox *zap_group = new QGroupBox("ZAP!!");
  QFormLayout *zap_layout = new QFormLayout;
  QPushButton *zap_button = new QPushButton("ZAP");
  QLineEdit *zap_peak = new QLineEdit;
  QLineEdit *zap_time = new QLineEdit;
  zap_layout->addRow(zap_button);
  zap_layout->addRow("Amplitude (mV)", zap_peak);
  zap_layout->addRow("Duration (ms)", zap_time);
  zap_group->setLayout(zap_layout);
  middle_layout->addWidget(zap_group, 0, 1);
  
  //Define Membrane Properties
  QGroupBox *properties_group = new QGroupBox("Membrane Properties");
  QFormLayout *properties_layout = new QFormLayout;
  QPushButton *properties_continuous = new QPushButton("Continuous");
  QPushButton *properties_single = new QPushButton("Single");
  QGridLayout *properties_options_layout = new QGridLayout;
  properties_options_layout->addWidget(properties_continuous, 0, 0);
  properties_options_layout->addWidget(properties_continuous, 0, 1);
  QLineEdit *properties_rate = new QLineEdit;
  QLineEdit *properties_steps = new QLineEdit;
  properties_layout->addRow(properties_continuous, properties_single);
  properties_layout->addRow("Update Rate (Hz)", properties_rate);
  properties_layout->addRow("Steps to Average", properties_steps);
  properties_group->setLayout(properties_layout);
  middle_layout->addWidget(properties_group, 1, 0);
  
  //Membrane Properties output
//  QGroupBox *output_group = new QGroupBox("Output");
  
  //Resistance at Bottom
  
  
  //Put all the plugins together
  plugin_layout->addWidget(utility);
  plugin_layout->addWidget(middle);
  
 // /**/plugin_layout->addWidget(hold_group);
  
};
