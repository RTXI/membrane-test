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
	/*
	 * Define plugin outline and groups
	 */
	//Overall plugin
	QVBoxLayout *plugin_layout = new QVBoxLayout(this);
	plugin_layout->setContentsMargins(QMargins(0,0,0,0));
	
	//Define utility box
	QWidget *utility = new QWidget; //define the widget that will contain the utility buttons
	QHBoxLayout *utility_layout = new QHBoxLayout;
	utility->setLayout(utility_layout);

	//Create middle widget for membrane-test buttons, editors, and GUI outputs
	QWidget *middle = new QWidget;
	QHBoxLayout *middle_layout = new QHBoxLayout;
	middle->setLayout(middle_layout);
	QWidget *middle_column1 = new QWidget;
	QVBoxLayout *middle_column1_layout = new QVBoxLayout;
	middle_column1->setLayout(middle_column1_layout);
	QWidget *middle_column2 = new QWidget;
	QVBoxLayout *middle_column2_layout = new QVBoxLayout;
	middle_column2->setLayout(middle_column2_layout);
	middle_layout->addWidget(middle_column1);
	middle_layout->addWidget(middle_column2);
	
	//Create widget for 1st column of middle widget 
	QGroupBox *hold_group = new QGroupBox("Holding Potential:");
	QGroupBox *zap_group = new QGroupBox("ZAP!!!:");
	middle_column1_layout->addWidget(hold_group);
	middle_column1_layout->addWidget(zap_group);
	
	//Create widget for 2nd column of middle widget
	QGroupBox *properties_group = new QGroupBox("Membrane Properties:");
	QGroupBox *output_group = new QGroupBox("Output:");
	QGroupBox *resistance_group = new QGroupBox("Resistance:");
	middle_column2_layout->addWidget(properties_group);
	middle_column2_layout->addWidget(output_group);
	middle_column2_layout->addWidget(resistance_group);

	//Put GUI boxes, groups. etc together
//	plugin_layout->addWidget(utility);
	plugin_layout->addWidget(middle);

	/*
	 * Flesh out the plugin outline with all the buttons, text, etc. 
	 */
	//Create Cancel, Update, and Reset buttons
	QPushButton *utility_cancelbutton = new QPushButton("Cancel", this);
	QPushButton *utility_updatebutton = new QPushButton("Update", this);
	QPushButton *utility_resetbutton = new QPushButton("Reset", this);
	utility_resetbutton->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
	utility_cancelbutton->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
	utility_updatebutton->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
	utility_layout->addWidget(utility_cancelbutton, 1, Qt::AlignRight);
	utility_layout->addWidget(utility_updatebutton, 0, Qt::AlignRight);
	utility_layout->addWidget(utility_resetbutton, 0, Qt::AlignRight);
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
  
	//Define ZAP!! group
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
  
	//Define Membrane Properties
	QFormLayout *properties_layout = new QFormLayout;
	QPushButton *properties_continuous = new QPushButton("Continuous");
	properties_continuous->setCheckable(true);
	QPushButton *properties_single = new QPushButton("Single");
	QComboBox *properties_rate = new QComboBox;
	properties_rate->setEditable(true);
	QComboBox *properties_steps = new QComboBox;
	properties_steps->setEditable(true);
	properties_layout->addRow(properties_continuous, properties_single);
	properties_layout->addRow("Update Rate (Hz)", properties_rate);
	properties_layout->addRow("Steps to Average", properties_steps);
	properties_group->setLayout(properties_layout);
  
	//Plugin Output
	QGridLayout *output_layout = new QGridLayout;
	output_layout->setVerticalSpacing(0);
	output_layout->setHorizontalSpacing(0);
	QLabel *resistance_access_title = new QLabel("<font size=\"+1\">R</font><font size=\"-2\">a</font> :");
	QLabel *capacitance_membrane_title = new QLabel("<font size=\"+1\">C</font><font size=\"-2\">m</font> :");
	QLabel *resistance_membrane_title = new QLabel("<font size=\"+1\">R</font><font size=\"-2\">m</font> :");
	QLabel *resistance_access = new QLabel("149 MO");
	QLabel *capacitance_membrane = new QLabel("22 pF");
	QLabel *resistance_membrane = new QLabel("124 GO");
	output_layout->addWidget(resistance_access_title, 0, 0, Qt::AlignLeft);
	output_layout->addWidget(resistance_membrane_title, 1, 0, Qt::AlignLeft);
	output_layout->addWidget(capacitance_membrane_title, 2, 0, Qt::AlignLeft);
	output_layout->addWidget(resistance_access, 0, 1, Qt::AlignLeft);
	output_layout->addWidget(resistance_membrane, 1, 1, Qt::AlignLeft);
	output_layout->addWidget(capacitance_membrane, 2, 1, Qt::AlignLeft);
	output_group->setLayout(output_layout);
  
   //Resistance at Bottom
	QHBoxLayout *resistance_layout = new QHBoxLayout;
	QLabel *resistance = new QLabel;
	resistance->setAlignment((Qt::AlignCenter));
	resistance->setText("Please don't close me.\nI'm afraid to not exist.");
	resistance_layout->addWidget(resistance);
	resistance_group->setLayout(resistance_layout);
};
/*
void NewMembraneTestGUI::togglePulse(bool toggled) {
	if(toggled == true) {
	
	
};
*/
