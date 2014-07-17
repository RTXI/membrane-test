#include <QtGui>
#include <default_gui_model.h>

class MembraneTest : public DefaultGUIModel {

	Q_OBJECT

	public:
		MembraneTest(void);
		virtual ~MembraneTest(void);

		void initParameters(void);
		void customizeGUI(void);
		void execute(void);
		void refresh(void);
	
	protected:
		virtual void update(DefaultGUIModel::update_flags_t);

	private:
		double Rt, Cm, Ra, Rm;
		double period;

		//Pulse parameters
		double V1, V2, V3, Vapp;
		double hold_amplitude;
		double hold_period;
		size_t hold_index, hold_count;
		enum VoltSelect_t {
			VOLTAGE1 = 1, 
			VOLTAGE2 = 2, 
			VOLTAGE3 = 3
		};
		VoltSelect_t volt_mode;
		bool volt_changed; //not used for anything yet
		QPushButton *v1_button, *v2_button, *v3_button;
		QLabel *resistance_label;

		//ZAP parameters
		bool zap_on;
		double zap_amplitude;
		double zap_duration;
		size_t zap_index, zap_count;
		QPushButton *zap_button;

		//Membrane properties parameters
		enum MemTestMode_t {
			SIMPLE=0,
			DETAILED=1
		};
//		int mem_update;
		MemTestMode_t mem_mode;
		bool mode_changed;
		int mem_steps, mem_steps_saved;
		double I1, I2, dI;
		size_t mem_index;
		std::vector<double> newdata, data;
		QPushButton *simple_button, *detailed_button;
	
		void computeMembraneProperties(void);
		void updateUIResistance(void);
		
	private slots:
		void toggleZap(void);
		void selectMode(int);
		void selectVoltage(int);
};
