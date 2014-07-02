#include <QtGui>
#include <default_gui_model.h>

class MembraneTest : public DefaultGUIModel {

	Q_OBJECT

	public:
		MembraneTest(void);
		virtual ~MembraneTest(void);

		void execute(void);
	
	protected:
		virtual void update(DefaultGUIModel::update_flags_t);

	private:
		double hold_potential;
		double hold_amplitude;
		double hold_period;
		double zap_amplitude;
		double zap_duration;
		int mem_update;
		int mem_steps;
		
		double resistance;
		double capacitance;
		double access_resistance;
		double membrane_resistance;

		double period;
		
	private slots:
};
