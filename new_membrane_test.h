/*
 * This is a template header file for a user modules derived from
 * DefaultGUIModel with a custom GUI.
 */

#include <default_gui_model.h>

class MembraneTest : public DefaultGUIModel
{

  Q_OBJECT

public:

  MembraneTest(void);
  virtual
  ~MembraneTest(void);

  void
  execute(void);
  void
  createGUI(DefaultGUIModel::variable_t *, int);

protected:

  virtual void
  update(DefaultGUIModel::update_flags_t);

private:
	QLineEdit *hold_volt;
	QLineEdit *hold_peak; 
	QLineEdit *hold_period;

private slots:
// these are custom functions that can also be connected
// to events through the Qt API. they must be implemented
// in new_membrane_test.cpp

};
