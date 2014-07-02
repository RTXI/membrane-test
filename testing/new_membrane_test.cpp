#include <QtGui>
#include <main_window.h>
#include <cmath>
#include <gsl/gsl_linalg.h>
#include "new_membrane_test.h"

extern "C" Plugin::Object *createRTXIPlugin(void *) {
	return new MembraneTest();
}

static DefaultGUIModel::variable_t vars[] = {
	{ "Current Input (A)", "", DefaultGUIModel::INPUT, },
	{ "Voltage Output (V)", "", DefaultGUIModel::OUTPUT, }, 
	{ "Hold Potential", "Set the holding potential of the membrane", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
	{ "Hold Amplitude", "Set the amplitude of the sine wave used for the holding potential", DefaultGUIModel::PARAMETER | DefaultGUIModel::PARAMETER, },
	{ "Hold Period", "Set the period, or width, of the holding pulses", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, }, 
	{ "Zap Amplitude", "Set the amplitude for the single zap", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
	{ "Zap Duration", "Duration of the zap pulse", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
	{ "Update Rate", "Update rate for membrane properties measurement", DefaultGUIModel::PARAMETER | DefaultGUIModel::UINTEGER, },
	{ "Num. Steps", "Number of steps to use for calculating membrane properties", DefaultGUIModel::PARAMETER | DefaultGUIModel::UINTEGER, },
	{ "Resistance", "", DefaultGUIModel::STATE, }, 
	{ "Capacitance", "", DefaultGUIModel::STATE, }, 
	{ "Access Resistance", "", DefaultGUIModel::STATE, }, 
	{ "Membrane Resistance", "", DefaultGUIModel::STATE, },
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

MembraneTest::MembraneTest(void) : DefaultGUIModel("Test Membrane Properties", ::vars, ::num_vars) {
	setWhatsThis("<p><b>MembraneTest:</b><br>Allows users to hold membranes at designated potentials and then zap them or measure their capcitance, access resistance, and overall resistance.</p>");
	DefaultGUIModel::createGUI(vars, num_vars);
//	customizeGUI();
//	initParameters();
//	update( INIT );
//	refresh();
};

MembraneTest::~MembraneTest(void) {}

void MembraneTest::execute(void) { 
	return; 
}

void MembraneTest::update(DefaultGUIModel::update_flags_t flag) {
	switch (flag) {
		case INIT:
			period = RT::System::getInstance()->getPeriod() * 1e-6; //ms
			setParameter("Hold Potential", hold_potential);
			setParameter("Hold Amplitude", hold_amplitude);
			setParameter("Hold Period", hold_period);
			setParameter("Zap Amplitude", zap_amplitude);
			setParameter("Zap Duration", zap_duration);
			setParameter("Update Rate", mem_update);
			setParameter("Num. Steps", mem_steps);
			setState("Resistance", resistance);
			setState("Capacitance", capacitance);
			setState("Access Resistance", access_resistance);
			setState("Membrane Reistance", membrane_resistance);
			break;
		case MODIFY:
			setParameter("Hold Potential", hold_potential);
			setParameter("Hold Amplitude", hold_amplitude);
			setParameter("Hold Period", hold_period);
			setParameter("Zap Amplitude", zap_amplitude);
			setParameter("Zap Duration", zap_duration);
			setParameter("Update Rate", mem_update);
			setParameter("Num. Steps", mem_steps);
			break;
		case PAUSE:
			break;
		case PERIOD:
			period = RT::System::getInstance()->getPeriod() * 1e-6; //ms
			break;
		default:
			break;
	}
}

/*
void MembraneTest::createGUI(void) {
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
	plugin_layout->addWidget(middle);
	plugin_layout->addWidget(utility);

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

	//Create holding voltage selection group
//	QVBoxLayout *hold_layout = new QVBoxLayout;
	QFormLayout *hold_layout = new QFormLayout;
	QPushButton *hold_toggle = new QPushButton("Pulse");
	hold_toggle->setCheckable(true);
	hold_layout->addRow(hold_toggle);
	QFormLayout *voltage_layout = new QFormLayout;
	hold_layout->addRow("Voltage", voltage_layout);
//	QGroupBox *voltage_list = new QGroupBox("Voltage");
//	QFormLayout *voltage_layout = new QFormLayout;
//	voltage_list->setLayout(voltage_layout);
//	QWidget *remaining_options = new QWidget;
//	QFormLayout *remaining_layout = new QFormLayout;
//	remaining_options->setLayout(remaining_layout);
//	remaining_layout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);

	hold_voltage1 = new QDoubleSpinBox;
	pick_voltage1 = new QRadioButton;
	pick_voltage1->setChecked(true);
	hold_voltage1->setSuffix(" mV");
	hold_voltage1->setButtonSymbols(QAbstractSpinBox::NoButtons);
	hold_voltage1->setValue(20);
	voltage_layout->addRow(pick_voltage1, hold_voltage1);

	hold_voltage2 = new QDoubleSpinBox;
	pick_voltage2 = new QRadioButton;
	hold_voltage2->setSuffix(" mV");
	hold_voltage2->setButtonSymbols(QAbstractSpinBox::NoButtons);
	hold_voltage2->setValue(120);
	voltage_layout->addRow(pick_voltage2, hold_voltage2);

	hold_voltage3 = new QDoubleSpinBox;
	pick_voltage3 = new QRadioButton;
	hold_voltage3->setSuffix(" mV");
	hold_voltage3->setButtonSymbols(QAbstractSpinBox::NoButtons);
	hold_voltage3->setValue(70);
	voltage_layout->addRow(pick_voltage3, hold_voltage3);

	QDoubleSpinBox *hold_amplitude = new QDoubleSpinBox;
	hold_amplitude->setSuffix(" mV");
	hold_amplitude->setButtonSymbols(QAbstractSpinBox::NoButtons);
	QDoubleSpinBox *hold_period = new QDoubleSpinBox;
	hold_period->setSuffix(" ms");
	hold_period->setButtonSymbols(QAbstractSpinBox::NoButtons);
	hold_layout->addRow("Amplitude", hold_amplitude);
	hold_layout->addRow("Period", hold_period);
	
//	hold_layout->addWidget(hold_toggle);
//	hold_layout->addWidget(voltage_list);

//	remaining_layout->addRow("Voltage", hold_voltage);
//	hold_layout->addWidget(remaining_options);
	hold_group->setLayout(hold_layout);
  
	//Define ZAP!! group
	QFormLayout *zap_layout = new QFormLayout;
	zap_layout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	QPushButton *zap_button = new QPushButton("ZAP");
	QDoubleSpinBox *zap_amplitude = new QDoubleSpinBox;
	zap_amplitude->setSuffix(" mV");
	zap_amplitude->setButtonSymbols(QAbstractSpinBox::NoButtons);
	QDoubleSpinBox *zap_duration = new QDoubleSpinBox;
	zap_duration->setSuffix(" ms");
	zap_duration->setButtonSymbols(QAbstractSpinBox::NoButtons);
	zap_layout->addRow(zap_button);
	zap_layout->addRow("Amplitude", zap_amplitude);
	zap_layout->addRow("Duration", zap_duration);
	zap_group->setLayout(zap_layout);
  
	//Define Membrane Properties
	QFormLayout *properties_layout = new QFormLayout;
	QPushButton *properties_continuous = new QPushButton("Continuous");
	properties_continuous->setCheckable(true);
	QPushButton *properties_single = new QPushButton("Single");
	QSpinBox *properties_rate = new QSpinBox;
	properties_rate->setSuffix(" Hz");
	properties_rate->setButtonSymbols(QAbstractSpinBox::NoButtons);
	QSpinBox *properties_steps = new QSpinBox;
	properties_steps->setButtonSymbols(QAbstractSpinBox::NoButtons);
	properties_layout->addRow(properties_continuous, properties_single);
	properties_layout->addRow("Update Rate", properties_rate);
	properties_layout->addRow("Num. Steps", properties_steps);
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
	output_layout->addWidget(resistance_access, 0, 1, Qt::AlignRight);
	output_layout->addWidget(resistance_membrane, 1, 1, Qt::AlignRight);
	output_layout->addWidget(capacitance_membrane, 2, 1, Qt::AlignRight);
	output_group->setLayout(output_layout);
  
   //Resistance at Bottom
	QHBoxLayout *resistance_layout = new QHBoxLayout;
	QLabel *resistance = new QLabel;
	resistance->setAlignment((Qt::AlignCenter));
	resistance->setText("RESISTANCE");
	resistance_layout->addWidget(resistance);
	resistance_group->setLayout(resistance_layout);
};
*/
