#pragma once

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>

class NewMembraneTestGUI : public QWidget {

	Q_OBJECT

	public:
		NewMembraneTestGUI(void);
		void createGUI(void);

//	private:
		QDoubleSpinBox *hold_voltage;
		QDoubleSpinBox *hold_amplitude;
		QDoubleSpinBox *hold_period;
		
		QDoubleSpinBox *zap_amplitude;
		QDoubleSpinBox *zap_duration;
		
		QDoubleSpinBox *properties_rate;
		QDoubleSpinBox *properties_steps;
		
		/*QComboBox *hold_voltage;
		QComboBox *hold_amplitude; 
		QComboBox *hold_period;

		QComboBox *zap_amplitude;
		QComboBox *zap_duration;
    
		QComboBox *properties_rate;
		QComboBox *properties_steps;*/


/*		struct RTFlags {
			bool pulse;
			bool zap;
		};
*/

/*    bool toggle_pulse;
		bool toogle_zap;
		bool toggle_membrane_continuous;
		bool toggle_membrane_single;
*/    
	private slots:
	/*
		void togglePulse(bool);
		void toggleZap(void);
		void toggleMembraneContinuous(void);
		void toggleMembraneSingle(void);
		
		void calculateMembraneProperties(void);

		void updatePulseVoltage(void);
		void updatePulseAmplitude(void);
		void updatePulsePeriod(void);

		void updateZapAmplitude(void);
		void updateZapDuration(void);

		void updatePropertiesRate(void);
		void updatePropertiesSteps(void);
	*/
};
