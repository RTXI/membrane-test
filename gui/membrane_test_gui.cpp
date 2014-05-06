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
	createGUI();	
};



void NewMembraneTestGUI::createGUI(void) {
  //Define basic plugin outline
  QVBoxLayout *plugin_layout = new QVBoxLayout(this);
  plugin_layout->setContentsMargins(QMargins(0,0,0,0));
  
/*  //Create Cancel, Update, and Reset buttons
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
*/  
  //Create the middle grid layout
  QWidget *middle = new QWidget;
  QGridLayout *middle_layout = new QGridLayout;
  middle->setLayout(middle_layout);
  
  //Define Hold Potential group
  QGroupBox *hold_group = new QGroupBox("Holding Potential");
  QFormLayout *hold_layout = new QFormLayout;
  hold_layout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
  QPushButton *hold_toggle = new QPushButton("Pulse");
  hold_toggle->setCheckable(true);
  QComboBox *hold_voltage = new QComboBox;
  hold_voltage->setEditable(true);
  hold_voltage->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
  hold_voltage->setMinimumContentsLength(3);
  QComboBox *hold_amplitude = new QComboBox;
  hold_amplitude->setEditable(true);
  hold_amplitude->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
  hold_amplitude->setMinimumContentsLength(3);
  QComboBox *hold_period = new QComboBox;
  hold_period->setEditable(true);
  hold_period->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
  hold_period->setMinimumContentsLength(3);
  hold_layout->addRow(hold_toggle);
  hold_layout->addRow("Voltage (mV)", hold_voltage);
  hold_layout->addRow("Amplitude (mV)", hold_amplitude);
  hold_layout->addRow("Period (ms)", hold_period);
  hold_group->setLayout(hold_layout);
  middle_layout->addWidget(hold_group, 0, 0, 2, 1);
  
  //Define ZAP!! group
  QGroupBox *zap_group = new QGroupBox("ZAP!!");
  QFormLayout *zap_layout = new QFormLayout;
  zap_layout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
  QPushButton *zap_button = new QPushButton("ZAP");
  QComboBox *zap_amplitude = new QComboBox;
  zap_amplitude->setEditable(true);
  zap_amplitude->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
  zap_amplitude->setMinimumContentsLength(3);
  QComboBox *zap_duration = new QComboBox;
  zap_duration->setEditable(true);
  zap_duration->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
  zap_duration->setMinimumContentsLength(3);
  zap_layout->addRow(zap_button);
  zap_layout->addRow("Amplitude (mV)", zap_amplitude);
  zap_layout->addRow("Duration (ms)", zap_duration);
  zap_group->setLayout(zap_layout);
  middle_layout->addWidget(zap_group, 2, 0, 2, 1);
  
  //Define Membrane Properties
  QGroupBox *properties_group = new QGroupBox("Membrane Properties");
  QFormLayout *properties_layout = new QFormLayout;
  QPushButton *properties_continuous = new QPushButton("Continuous");
  properties_continuous->setCheckable(true);
  QPushButton *properties_single = new QPushButton("Single");
//  QGridLayout *properties_options_layout = new QGridLayout;
//  properties_options_layout->addWidget(properties_continuous, 0, 0);
//  properties_options_layout->addWidget(properties_single, 0, 1);
  QComboBox *properties_rate = new QComboBox;
  properties_rate->setEditable(true);
  QComboBox *properties_steps = new QComboBox;
  properties_steps->setEditable(true);
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
  output_layout->addWidget(resistance_membrane_title, 2, 0, Qt::AlignLeft);
  output_layout->addWidget(capacitance_membrane_title, 3, 0, Qt::AlignLeft);
  output_layout->addWidget(resistance_access, 1, 1, Qt::AlignLeft);
  output_layout->addWidget(resistance_membrane, 2, 1, Qt::AlignLeft);
  output_layout->addWidget(capacitance_membrane, 3, 1, Qt::AlignLeft);

  output_group->setLayout(output_layout);
  middle_layout->addWidget(output_group, 1, 1, 1, 1);

  
  //Resistance at Bottom
  QGroupBox *resist_group = new QGroupBox("Resistance");
  QHBoxLayout *resist_layout = new QHBoxLayout;
  QLabel *resistance = new QLabel;
  resistance->setAlignment((Qt::AlignCenter));
  resistance->setText("Please don't close me.\nI'm afraid to not exist.");
  resist_layout->addWidget(resistance);
  resist_group->setLayout(resist_layout);
  middle_layout->addWidget(resistance, 2, 1, 1, 1);
//  middle_layout->addWidget(utility, 4, 1, 1, 1);

  
  //Put all the widgets together
//  plugin_layout->addWidget(utility);
  plugin_layout->addWidget(middle);
};
/*
void NewMembraneTestGUI::togglePulse(bool toggled) {
	if(toggled == true) {
	
	
}
*/
