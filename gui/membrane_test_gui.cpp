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
#include <iostream>

#include "membrane_test_gui.h"

NewMembraneTestGUI::NewMembraneTestGUI(void) {
	beHappy();
	createGUI();
};


void NewMembraneTestGUI::beHappy(void) {
	std::cout<<"Don't worry. Be happy.\n\n...Bitch"<<std::endl;
}


void NewMembraneTestGUI::createGUI(void) {
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
//  utility_resetbutton->setMinimumHeight(10);
//  utility_resetbutton->setMinimumWidth(40);
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
  QGroupBox *hold_group = new QGroupBox("Holding Potential");
  QFormLayout *hold_layout = new QFormLayout;
  QPushButton *hold_toggle = new QPushButton("Pulse");
  QLineEdit *hold_volt = new QLineEdit;
  QLineEdit *hold_peak = new QLineEdit;
  QLineEdit *hold_period = new QLineEdit;
  hold_layout->addRow(hold_toggle);
  hold_layout->addRow("Voltage (mV)", hold_volt);
  hold_layout->addRow("Amplitude (mV)", hold_peak);
  hold_layout->addRow("Period (ms)", hold_period);
  hold_group->setLayout(hold_layout);
  middle_layout->addWidget(hold_group, 0, 0, 2, 1);
  
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
  middle_layout->addWidget(zap_group, 2, 0, 2, 1);
  
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
  middle_layout->addWidget(properties_group, 0, 1, 1, 1);
  
  //Plugin Output
  QGroupBox *output_group = new QGroupBox("Output");
  QGridLayout *output_layout = new QGridLayout;
  QLabel *resistance_access_title = new QLabel("<font size=\"+1\">R</font><font size=\"-2\">a</font> :");
  QLabel *capacitance_membrane_title = new QLabel("<font size=\"+1\">C</font><font size=\"-2\">m</font> :");
  QLabel *resistance_membrane_title = new QLabel("<font size=\"+1\">R</font><font size=\"-2\">m</font> :");
  QLabel *resistance_access = new QLabel("149 MO");
  QLabel *capacitance_membrane = new QLabel("22 pF");
  QLabel *resistance_membrane = new QLabel("124 GO");
//  output_layout->addWidget(membrane_properties_title, 0, 0, 1, 3, Qt::AlignCenter);
  output_layout->addWidget(resistance_access_title, 1, 0, Qt::AlignLeft);
  output_layout->addWidget(resistance_membrane_title, 1, 1, Qt::AlignLeft);
  output_layout->addWidget(capacitance_membrane_title, 1, 2, Qt::AlignLeft);
  output_layout->addWidget(resistance_access, 2, 0, Qt::AlignCenter);
  output_layout->addWidget(resistance_membrane, 2, 1, Qt::AlignCenter);
  output_layout->addWidget(capacitance_membrane, 2, 2, Qt::AlignCenter);
  output_group->setLayout(output_layout);
  middle_layout->addWidget(output_group, 1, 1, 1, 1);
  
  //Resistance at Bottom
  QGroupBox *resist_group = new QGroupBox("Resistance");
  QHBoxLayout *resist_layout = new QHBoxLayout;
  QLabel *resistance = new QLabel;
  resistance->setAlignment((Qt::AlignCenter));
  resistance->setText("Please don't kill me.\nI don't want to die.");
  resist_layout->addWidget(resistance);
  resist_group->setLayout(resist_layout);
  middle_layout->addWidget(resistance, 3, 1, 1, 1);
  middle_layout->addWidget(utility, 4, 1, 1, 1);
  
  //Put all the widgets together
//  plugin_layout->addWidget(utility);
  plugin_layout->addWidget(middle);
};

