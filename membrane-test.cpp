/*
 * Copyright (C) 2014 Weill Medical College of Cornell University
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License 
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#include <QtGui>
#include <main_window.h>
#include <cmath>
#include <gsl/gsl_linalg.h>
#include "membrane-test.h"

// Create RTXI plugin object
extern "C" Plugin::Object *createRTXIPlugin(void *) {
	return new MembraneTest();
}

//Create varable array for MembraneTest module. 
static DefaultGUIModel::variable_t vars[] = {
	{ "Current Input (A)", "", DefaultGUIModel::INPUT, },
	{ "Voltage Output (V)", "", DefaultGUIModel::OUTPUT, },
	{ "Hold (V1)", "Set the holding potential of the membrane", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
	{ "Hold (V2)", "Set the holding potential of the membrane", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
	{ "Hold (V3)", "Set the holding potential of the membrane", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
	{ "Hold Amplitude", "Set the amplitude of the sine wave used for the holding potential", DefaultGUIModel::PARAMETER | DefaultGUIModel::PARAMETER, },
	{ "Hold Period", "Set the period, or width, of the holding pulses", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
	{ "Zap Amplitude", "Set the amplitude for the single zap", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
	{ "Zap Duration", "Duration of the zap pulse", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
//	{ "Update Rate", "Update rate for membrane properties measurement", DefaultGUIModel::PARAMETER | DefaultGUIModel::UINTEGER, },
	{ "Num. Steps", "Number of steps to use for calculating membrane properties", DefaultGUIModel::PARAMETER | DefaultGUIModel::UINTEGER, },
	{ "Rt", "Total Resistance (Ra + Rm)", DefaultGUIModel::STATE, },
	{ "Cm", "Membrane capacitance", DefaultGUIModel::STATE, },
	{ "Ra", "Access Resistance", DefaultGUIModel::STATE, },
	{ "Rm", "Membrane Resistance", DefaultGUIModel::STATE, },
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

MembraneTest::MembraneTest(void) : DefaultGUIModel("Membrane Properties", ::vars, ::num_vars) {
	setWhatsThis("<p><b>MembraneTest:</b><br>Allows users to hold membranes at designated potentials and then zap them or measure their capcitance, access resistance, and overall resistance.</p>");
	DefaultGUIModel::createGUI(vars, num_vars);
	initParameters(); // initialize parameters
	customizeGUI(); // customize the GUI
	update( INIT ); // sync the GUI with the intialized parameters
	refresh(); // refresh the GUI to it displays the synced changes
	QTimer::singleShot(0, this, SLOT(resizeMe()));
};

MembraneTest::~MembraneTest(void) {}

/* Executes the real-time portion of the module. There are two modes: simple and detailed
 *   Simple - calculate resistance from measuring current generated by feeding in square pulses
 *   Detailed - calculate resistances and capacitance (more resource-intensive)
 *
 * The zap function overrides all data acquisition and outputs elevated voltage.
 */

void MembraneTest::execute(void) {
	//perform the zap function
	if (zap_on) {
		if ( ++zap_index < zap_count )
			output(0) = ( zap_amplitude + Vapp ) * 1e-3; //mV
		else {
			zap_on = false;
		}
		return;
	}

	//clears data if the acquistion mode changes. Guarantees data is obtained contiguously
	if (mode_changed) {
		I1 = I2 = dI = 0;
		mem_steps_saved = 1;
		mem_index = 0;
		newdata.clear();
		newdata.resize(hold_count, 0);
		
		mode_changed = false;
	}

	if (volt_changed) {
		switch (volt_mode) {
			case VOLTAGE1:
				Vapp = V1;
				break;

			case VOLTAGE2:
				Vapp = V2;
				break;

			case VOLTAGE3:
				Vapp = V3;
				break;

			default:
				break;
		}
		volt_changed = false;
	}
	
	// Takes input from 2nd and 4th quarters of 1 periodic square pulse.
	if (mem_mode == SIMPLE) {
		if  (hold_index < (hold_count/2)) {
			if (hold_index > (hold_count/4)) {
				I1 += input(0);
			}
			output(0) = ( Vapp + hold_amplitude ) * 1e-3; //mV
		}
		else {
			if ( hold_index > (3*hold_count) / 4 ) {
				I2 += input(0);
			}
			output(0) = Vapp * 1e-3; //mV
		}
	
		if ( !( ++hold_index %= hold_count )) { //if hold_count points are acquired, do calculation
			dI = (I1 - I2) / (hold_count/4);
			I2 = I1 = 0.0;
		}
	}

	// collects mem_steps * hold_count data points and stores them for later analysis
	else if (mem_mode == DETAILED) {
		if ( hold_index < hold_count / 2 ) {
			newdata[mem_index] += input(0);
			mem_index++;
		
			output(0) = ( Vapp + hold_amplitude ) * 1e-3; //mV
		}
		else {
			newdata[mem_index] += input(0);
			mem_index++;

			output(0) = Vapp * 1e-3; //mV
		}
		
		// increment hold_index and output chunk of data when hold_count points are collected
		if ( !( ++hold_index %= hold_count )) {
			mem_index = 0; 
			
			// number of steps saved (each element has mem_steps points added to it) == mem_steps
			if ( ++mem_steps_saved > mem_steps ) {
				mem_steps_saved = 1;
				data = newdata; //save collected data into 'data' for later
				mem_index = 0;
				newdata.clear(); //reset 'newdata' for next round of data collection
				newdata.resize(hold_count, 0); //resize in case period changed. 
			}
		}	
	}
	return;
}

void MembraneTest::initParameters(void) {
	V1 = Vapp = 0; V2 = -80; V3 = -40; //mv
	hold_amplitude = 10;
	hold_period = 10;
	zap_amplitude = 1000;
	zap_duration = 10;
	zap_on = false;
//	mem_update = 1;
	mem_steps = 5;
	mem_steps_saved = 1;
	mem_mode = SIMPLE;
	volt_mode = VOLTAGE1;
	period = RT::System::getInstance()->getPeriod() * 1e-9; //ms
	
	hold_count = hold_period * 2 * 1e-3 / period;
	hold_index = 0;
	I1 = I2 = dI = 0;
	zap_count = zap_duration / period;
	zap_index = 0;
	
	Rt = 0;
	Cm = 0;
	Ra = 0;
	Rm = 0;
}

void MembraneTest::update(DefaultGUIModel::update_flags_t flag) {
	switch (flag) {
		case INIT:
			setParameter("Hold (V1)", V1);
			setParameter("Hold (V2)", V2);
			setParameter("Hold (V3)", V3);
			setParameter("Hold Amplitude", hold_amplitude);
			setParameter("Hold Period", hold_period);
			setParameter("Zap Amplitude", zap_amplitude);
			setParameter("Zap Duration", zap_duration);
//			setParameter("Update Rate", mem_update);
			setParameter("Num. Steps", mem_steps);
			setState("Rt", Rt);
			setState("Cm", Cm);
			setState("Ra", Ra);
			setState("Rm", Rm);
			selectMode(mem_mode);
			selectVoltage(volt_mode);
			break;
		
		case MODIFY:
			V1 = getParameter("Hold (V1)").toDouble();
			V2 = getParameter("Hold (V2)").toDouble();
			V3 = getParameter("Hold (V3)").toDouble();
			hold_amplitude = getParameter("Hold Amplitude").toDouble();
			hold_period = getParameter("Hold Period").toDouble();
			zap_amplitude = getParameter("Zap Amplitude").toDouble();
			zap_duration = getParameter("Zap Duration").toDouble();
//			mem_update = getParameter("Update Rate").toInt();
			mem_steps = getParameter("Num. Steps").toInt();
			zap_on = false;
			I1 = I2 = dI = 0;
			hold_index = zap_index = 0;
			selectMode(mem_mode); //redundant. Consider deleting this line
			selectVoltage(volt_mode); //redundant. Consider deleting this line
			break;
		
		case PAUSE:
			zap_button->setEnabled(false);
			break;
		
		case UNPAUSE:
			zap_button->setEnabled(true);
			break;
		
		case PERIOD:
			period = RT::System::getInstance()->getPeriod() * 1e-9; //ms
			hold_count = hold_period * 2 * 1e-3/ period;
			zap_count = zap_duration * 1e-3 / period;
			mode_changed = true;
			break;
		
		default:
			break;
	}
}

// create the custom GUI for the module. Trust me, it's pretty rad. (use online Qt4 documentation)
void MembraneTest::customizeGUI(void) {
	QGridLayout *customlayout = DefaultGUIModel::getLayout(); //get the current layout instance
	
	QGroupBox *buttongroup = new QGroupBox("Controls"); //create group for control buttons 
	QVBoxLayout *buttongrouplayout = new QVBoxLayout;
	buttongroup->setLayout(buttongrouplayout);

	QHBoxLayout *zapbuttonslayout = new QHBoxLayout;
	zap_button = new QPushButton("Zap"); //button starts zap funtionality
	zapbuttonslayout->addWidget(zap_button);
	buttongrouplayout->addLayout(zapbuttonslayout);

	// these pushbuttons set the acquisition type (SINGLE or DETAILED)
	QHBoxLayout *membuttonslayout = new QHBoxLayout;
	QButtonGroup *membuttonsgroup = new QButtonGroup;
	membuttonsgroup->setExclusive(true);
	simple_button = new QPushButton("Simple");
	simple_button->setCheckable(true);
	detailed_button = new QPushButton("Detailed");
	detailed_button->setCheckable(true);
	membuttonsgroup->addButton(simple_button, SIMPLE);
	membuttonsgroup->addButton(detailed_button, DETAILED);
	membuttonslayout->addWidget(simple_button);
	membuttonslayout->addWidget(detailed_button);
	buttongrouplayout->addLayout(membuttonslayout);

	// set the voltage used for acquisiton (V1, V2, or V3)
	QHBoxLayout *voltbuttonslayout = new QHBoxLayout;
	QButtonGroup *voltbuttonsgroup = new QButtonGroup;
	voltbuttonsgroup->setExclusive(true);
	v1_button = new QPushButton("V1"); 
	v1_button->setCheckable(true);
	v2_button = new QPushButton("V2"); 
	v2_button->setCheckable(true);
	v3_button = new QPushButton("V3"); 
	v3_button->setCheckable(true);
	voltbuttonsgroup->addButton(v1_button, VOLTAGE1);
	voltbuttonsgroup->addButton(v2_button, VOLTAGE2);
	voltbuttonsgroup->addButton(v3_button, VOLTAGE3);
	voltbuttonslayout->addWidget(v1_button);
	voltbuttonslayout->addWidget(v2_button);
	voltbuttonslayout->addWidget(v3_button);
	buttongrouplayout->addLayout(voltbuttonslayout);

	//connect the buttons to their respective functions
	QObject::connect(zap_button, SIGNAL(clicked(void)), this, SLOT(toggleZap(void))); 
	QObject::connect(membuttonsgroup, SIGNAL(buttonClicked(int)), this, SLOT(selectMode(int)));
	QObject::connect(voltbuttonsgroup, SIGNAL(buttonClicked(int)), this, SLOT(selectVoltage(int)));

	// label displays the total restance in big, blue font. You can't miss it.
	resistance_label = new QLabel;
	resistance_label->setStyleSheet("color: blue; qproperty-alignment: AlignCenter; font: 32pt");
	resistance_label->setText("------");
	
	customlayout->addWidget(buttongroup, 0, 0);
	customlayout->addWidget(resistance_label, 2, 0);
	setLayout(customlayout); //sets the changes made here to the existing GUI already created
}

// toggles the zap. Does nothing if a zap is already running
void MembraneTest::toggleZap(void) {
	if (!zap_on) {
		zap_on = true;
		zap_index = 0;
	}
	return;
}

// sets the mode for acquisiton. Sets mode_changed flag when done. 
void MembraneTest::selectMode(int mode) {
	switch (mode) {
		case SIMPLE:
			mem_mode = SIMPLE;
			simple_button->setChecked(true);
			break;

		case DETAILED:
			mem_mode = DETAILED;
			detailed_button->setChecked(true);
			break;

		default:
			break;			
	}

	mode_changed = true;
	return;
}

// change the voltage option based on the GUI selection. 
void MembraneTest::selectVoltage(int mode) {
	switch (mode) {
		case VOLTAGE1:
//			Vapp = V1;
			volt_mode = VOLTAGE1;
			v1_button->setChecked(true);
			break;

		case VOLTAGE2:
//			Vapp = V2;
			volt_mode = VOLTAGE2;
			v2_button->setChecked(true);
			break;

		case VOLTAGE3:
//			Vapp = V3;
			volt_mode = VOLTAGE3;
			v3_button->setChecked(true);
			break;

		default:
			break;
	}

	volt_changed = true; 
	return;
}

// computes the membrane properties based on the data acquired in the execute() function
void MembraneTest::computeMembraneProperties(void) {
	if ( !getActive() ) return; // if the module is paused, no need to bother computing anything
	
	if (mem_mode == SIMPLE) {
		Rt = fabs( hold_amplitude * 1e-3 / dI ); // SIMPLE mode just uses Ohms law
	}
	else if (mem_mode == DETAILED) { // DETAILED is more complex (by Johnathan Bettencourt)
		double Vpp = hold_amplitude;
		unsigned int data_size = hold_count;
		if( data_size != data.size() ) // Check to make sure data size is correct
			return ;
		
		// Taken from electrophys_plugin, written by Jonathan Bettencourt
		for(size_t i = 0;i<data_size;++i)
			data[i] /= mem_steps;
			
		double I1 = 0.0;
		for(size_t i=static_cast<size_t>(round(data_size/2-ceil(data_size/8)));i<data_size/2;++i)
			I1 += data[i];
		I1 /= ceil(data_size/8);
				
		double I2 = 0.0;
		for(size_t i=static_cast<size_t>(round(data_size-ceil(data_size/8)));i<data_size;++i)
			I2 += data[i];
		I2 /= ceil(data_size/8);
					
		double dt = RT::System::getInstance()->getPeriod() * 1e-6;
				
		double Q11;
		double tau1;
		{
			Q11 = 0.0;
			for(size_t i=0;i<data_size/2-1;++i)
				Q11 += dt*1e-3*(data[i]+data[i+1]-2*I1)/2;
			Q11 = fabs(Q11);
						
			size_t xi = 0;
			for(;data[xi] <= data[xi+1];++xi);
	
			double sy = 0.0;
			double Y = data[xi];
			double SY = sy;
			double tSY = 0.0;
			double YSY = data[xi]*sy;
			double SYSY = sy*sy;
			double t = 0.0;
			double tt = 0.0;
			double Yt = 0.0;
			for(size_t i=xi+1;i<data_size/2;++i) {
				sy += dt*1e-3*(data[i-1]+data[i])/2;
								
				Y += data[i];
				SY += sy;
				tSY += (i-xi)*dt*1e-3*sy;
				YSY += data[i]*sy;
				SYSY += sy*sy;
				t += (i-xi)*dt*1e-3;
				tt += ((i-xi)*dt*1e-3)*((i-xi)*dt*1e-3);
				Yt += (i-xi)*dt*1e-3*data[i];
			}
	
			double A[3*3] = {
				data_size/2-xi,	SY,	t,
				SY,	SYSY,	tSY,
				t,	tSY,	tt,
			};
			double B[3] = {Y, YSY, Yt,};
			double V[3*3];
			double S[3];
			double x[3];

			gsl_matrix_view a = gsl_matrix_view_array(A,3,3);
			gsl_matrix_view b = gsl_matrix_view_array(V,3,3);
			gsl_vector_view c = gsl_vector_view_array(S,3);
			gsl_vector_view d = gsl_vector_view_array(x,3);
			gsl_vector_view e = gsl_vector_view_array(B,3);

			gsl_linalg_SV_decomp(
				&a.matrix,
				&b.matrix,
				&c.vector,
				&d.vector
			);
			gsl_linalg_SV_solve(
				&a.matrix,
				&b.matrix,
				&c.vector,
				&e.vector,
				&d.vector
			);
			tau1 = fabs(1.0/x[1]);
		}

		double Q12;
		double tau2;
		{
			Q12 = 0.0;
			for(size_t i=data_size/2;i<data_size;++i)
				Q12 += dt*1e-3*(data[i]+data[i+1]-2*I2)/2;
			Q12 = fabs(Q12);
	
			size_t xi = data_size/2;
			for(;data[xi] >= data[xi+1];++xi);
	
			double sy = 0.0;
			double Y = data[xi];
			double SY = sy;
			double tSY = 0.0;
			double YSY = data[xi]*sy;
			double SYSY = sy*sy;
			double t = 0.0;
			double tt = 0.0;
			double Yt = 0.0;
			for(size_t i=xi+1;i<data_size;++i) {
				sy += dt*1e-3*(data[i-1]+data[i])/2;
				Y += data[i];
				SY += sy;
				tSY += (i-xi)*dt*1e-3*sy;
				YSY += data[i]*sy;
				SYSY += sy*sy;
				t += (i-xi)*dt*1e-3;
				tt += ((i-xi)*dt*1e-3)*((i-xi)*dt*1e-3);
				Yt += (i-xi)*dt*1e-3*data[i];
			}

			double A[3*3] = {
				data_size-xi,  SY,       t,
				SY,           SYSY,     tSY,
				t,            tSY,      tt,
			};
			double B[3] = {
				Y,
				YSY,
				Yt,
			};
			double V[3*3];
			double S[3];
			double x[3];
	
			gsl_matrix_view a = gsl_matrix_view_array(A,3,3);
			gsl_matrix_view b = gsl_matrix_view_array(V,3,3);
			gsl_vector_view c = gsl_vector_view_array(S,3);
			gsl_vector_view d = gsl_vector_view_array(x,3);
			gsl_vector_view e = gsl_vector_view_array(B,3);

			gsl_linalg_SV_decomp(
				&a.matrix,
				&b.matrix,
				&c.vector,
				&d.vector
			);
			gsl_linalg_SV_solve(
				&a.matrix,
				&b.matrix,
				&c.vector,
				&e.vector,
				&d.vector
			);
			tau2 = fabs(1.0/x[1]);
		}
		double tau = (tau1+tau2)/2.0;
	
		double Q1 = (Q11+Q12)/2.0;
		double Q2 = fabs(I1-I2)*tau;
		double Qt = Q1+Q2;

		Rt = Vpp*1e-3/fabs(I1-I2); //calculates the STATE variable (Rt for total resistance)

		Ra = tau*Vpp*1e-3/Qt; //access resistance
		Rm = Rt-Ra;  //membrane resistance
		Cm = Qt*Rt/(Vpp*1e-3*Rm); // membrance capacitance
	
		// round them so they look pleasant in the GUI
		Rt = round(Rt*1e-6*10)/10;
		Ra = round(Ra*1e-6*10)/10;
		Rm = round(Rm*1e-6*10)/10;
		Cm = round(Cm*1e12*10)/10;
	}
}

// updates the blue text near the bottom with the resistance. Takes Rt (either acquisiton mode)
void MembraneTest::updateUIResistance(void){
	double R = Rt;
	size_t exp = 0;
	QChar omega = QChar(0x3A9);

	// figure out which exponent is needed (k, M G, or T) for displaying the Ohm units
	if ( (R != INFINITY) && (R != NAN) ){
		while (R >= 1e3) {
			R *= 1e-3;
			exp++;
		}
	}

	QString RString;
	RString.sprintf("%7.3f", R);

	switch (exp) {
		case 0: 
			RString.append(" ").append(omega);
			break;
		case 1:
			RString.append(" k").append(omega);
			break;
		case 2: 
			RString.append(" M").append(omega);
			break;
		case 3:
			RString.append(" G").append(omega);
			break;
		case 4:
			RString.append(" T").append(omega);
		default:
			QString suffix;
			suffix.sprintf(" * 1e%lu", 3*exp); // if the resistance is very, very big, just show it in scientific notation.
	}

	resistance_label->setText(RString);	//set the string
}

// overloaded DefaultGUIModel refresh function to also include computing membrane properties and updating the resistance display (the blue text)
void MembraneTest::refresh(void) {
	for (std::map<QString, param_t>::iterator i = parameter.begin(); i!= parameter.end(); ++i) {
		if (i->second.type & (STATE | EVENT)) {
			i->second.edit->setText(QString::number(getValue(i->second.type, i->second.index)));
			palette.setBrush(i->second.edit->foregroundRole(), Qt::darkGray);
			i->second.edit->setPalette(palette);
		} else if ((i->second.type & PARAMETER) && !i->second.edit->isModified() && i->second.edit->text() != *i->second.str_value) {
			i->second.edit->setText(*i->second.str_value);
		} else if ((i->second.type & COMMENT) && !i->second.edit->isModified() && i->second.edit->text() != QString::fromStdString(getValueString(COMMENT, i->second.index))) {
			i->second.edit->setText(QString::fromStdString(getValueString(COMMENT, i->second.index)));
		}
	}

	if (zap_on) return;

	computeMembraneProperties();
	updateUIResistance();

	pauseButton->setChecked(!getActive());
}
