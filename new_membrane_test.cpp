/*
 * This is a template implementation file for a user module derived from
 * DefaultGUIModel with a custom GUI.
 */

#include <new_membrane_test.h>
#include <main_window.h>
//#include <cmath>
//#include <gsl/gsl_linalg.h>
//#include <qgridview.h>
//#include <qhbox.h>
//#include <qhbuttongroup.h>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QTimer>
#include <QToolTip>
#include <QValidator>
//#include <qvbox.h>
#include <QWhatsThis>

#include <QButtonGroup>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QSpinBox>
#include <QTabWidget>
#include <QRadioButton>
#include <QComboBox>
#include <QString>
#include <QLineEdit>
#include <QGroupBox>
#include <QFormLayout>

extern "C" Plugin::Object *
createRTXIPlugin(void)
{
  return new MembraneTest();
}

static DefaultGUIModel::variable_t vars[] =
  {
    { "Input (A)", "", DefaultGUIModel::INPUT, },
    { "Output (V)", "", DefaultGUIModel::OUTPUT, }, 
    { "Potential (mV)", "", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
    { "Amplitude (mV)", "", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, }, 
    { "Period (ms)", "", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, }, 
    { "Amplitude (mV)", "",  DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, }, 
    { "Duration (ms)", "", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, }, 
    { "Update Rate (Hz)", "", DefaultGUIModel::PARAMETER | DefaultGUIModel::INTEGER, }, 
    { "Steps to Average", "", DefaultGUIModel::PARAMETER | DefaultGUIModel::INTEGER, },
  };

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

MembraneTest::MembraneTest(void) :
  DefaultGUIModel("MembraneTest with Custom GUI", ::vars, ::num_vars)
{

  QWhatsThis::add(this, "<p><b>MembraneTest:</b><br>QWhatsThis description.</p>");
  createGUI(vars, num_vars); // this is required to create the GUI
  update( INIT); // this is optional, you may place initialization code directly into the constructor
  refresh(); // this is required to update the GUI with parameter and state values
}

MembraneTest::~MembraneTest(void)
{
}

void
MembraneTest::execute(void)
{
  return;
}

void
MembraneTest::update(DefaultGUIModel::update_flags_t flag)
{
  switch (flag)
    {
  case INIT:
    period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
  case MODIFY:
    some_parameter = getParameter("GUI label").toDouble();
    break;
  case UNPAUSE:
    break;
  case PAUSE:
    break;
  case PERIOD:
    period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
    break;
  default:
    break;

    }

}
/*
 * The following code is an example of how to overload the createGUI()
 * function to generate a custom GUI. Sections of this code are copied
 * from DefaultGUIModel.
 */

void
MembraneTest::createGUI(DefaultGUIModel::variable_t *var, int size)
{

  setMinimumSize(200, 300); // Qt API for setting window size
  
  QBoxLayout *plugin_layout = new QVBoxLayout(this);
  QTabWidget *tab_widget = new QTabWidget(this);

  QWidget *resist_tab = new QWidget;
  QHBoxLayout *resist_layout = new QHBoxLayout;
  
  QGroupBox *hold_group = new QGroupBox(tr("Hold Potential");
  QVBoxLayout *hold_layout = new QVBoxLayout;
  QPushButton *hold_toggle = new QPushButton("Hold");
  QHBoxLayout *hold_utility = new QHBoxLayout);
  QPushButton *hold_update = new QPushButton("Update");
  QPushButton *hold_revert = new QPushButton("Cancel");
  hold_utility->addWidget(hold_update);
  hold_utility->addWidget(hold_update);
  QFormLayout *hold_variables = new QFormLayout;
  hold_layout->addWidget(hold_toggle);
  hold_layout->addWidget(hold_variables);
  hold_layout->add
  
  
  QVBoxLayout *zap_layout = new QVBoxLayout; 
  
  
  
/*  //overall GUI layout with a "horizontal box" copied from DefaultGUIModel

  QBoxLayout *layout = new QHBoxLayout(this);

  //additional GUI layouts with "vertical" layouts that will later
  // be added to the overall "layout" above
  QBoxLayout *leftlayout = new QVBoxLayout();
  //QBoxLayout *rightlayout = new QVBoxLayout();

  // this is a "horizontal button group"
  QHButtonGroup *bttnGroup = new QHButtonGroup("Button Panel:", this);

  // we add two pushbuttons to the button group
  QPushButton *aBttn = new QPushButton("Button A", bttnGroup);
  QPushButton *bBttn = new QPushButton("Button B", bttnGroup);

  // clicked() is a Qt signal that is given to pushbuttons. The connect()
  // function links the clicked() event to a function that is defined
  // as a "private slot:" in the header.
  QObject::connect(aBttn, SIGNAL(clicked()), this, SLOT(aBttn_event()));
  QObject::connect(bBttn, SIGNAL(clicked()), this, SLOT(bBttn_event()));

  //these 3 utility buttons are copied from DefaultGUIModel
  QHBox *utilityBox = new QHBox(this);
  pauseButton = new QPushButton("Pause", utilityBox);
  pauseButton->setToggleButton(true);
  QObject::connect(pauseButton, SIGNAL(toggled(bool)), this, SLOT(pause(bool)));
  QPushButton *modifyButton = new QPushButton("Modify", utilityBox);
  QObject::connect(modifyButton, SIGNAL(clicked(void)), this, SLOT(modify(void)));
  QPushButton *unloadButton = new QPushButton("Unload", utilityBox);
  QObject::connect(unloadButton, SIGNAL(clicked(void)), this, SLOT(exit(void)));

  // add custom button group at the top of the layout
  leftlayout->addWidget(bttnGroup);

  // copied from DefaultGUIModel DO NOT EDIT
  // this generates the text boxes and labels
  QScrollView *sv = new QScrollView(this);
  sv->setResizePolicy(QScrollView::AutoOneFit);
  leftlayout->addWidget(sv);

  QWidget *viewport = new QWidget(sv->viewport());
  sv->addChild(viewport);
  QGridLayout *scrollLayout = new QGridLayout(viewport, 1, 2);
*/
  size_t nstate = 0, nparam = 0, nevent = 0, ncomment = 0;
  for (size_t i = 0; i < num_vars; i++)
    {
      if (vars[i].flags & (PARAMETER | STATE | EVENT | COMMENT))
        {
          param_t param;

          param.label = new QLabel(vars[i].name, viewport);
          scrollLayout->addWidget(param.label, parameter.size(), 0);
          param.edit = new DefaultGUILineEdit(viewport);
          scrollLayout->addWidget(param.edit, parameter.size(), 1);

          QToolTip::add(param.label, vars[i].description);
          QToolTip::add(param.edit, vars[i].description);

          if (vars[i].flags & PARAMETER)
            {
              if (vars[i].flags & DOUBLE)
                {
                  param.edit->setValidator(new QDoubleValidator(param.edit));
                  param.type = PARAMETER | DOUBLE;
                }
              else if (vars[i].flags & UINTEGER)
                {
                  QIntValidator *validator = new QIntValidator(param.edit);
                  param.edit->setValidator(validator);
                  validator->setBottom(0);
                  param.type = PARAMETER | UINTEGER;
                }
              else if (vars[i].flags & INTEGER)
                {
                  param.edit->setValidator(new QIntValidator(param.edit));
                  param.type = PARAMETER | INTEGER;
                }
              else
                param.type = PARAMETER;
              param.index = nparam++;
              param.str_value = new QString;
            }
          else if (vars[i].flags & STATE)
            {
              param.edit->setReadOnly(true);
              param.edit->setPaletteForegroundColor(Qt::darkGray);
              param.type = STATE;
              param.index = nstate++;
            }
          else if (vars[i].flags & EVENT)
            {
              param.edit->setReadOnly(true);
              param.type = EVENT;
              param.index = nevent++;
            }
          else if (vars[i].flags & COMMENT)
            {
              param.type = COMMENT;
              param.index = ncomment++;
            }

          parameter[vars[i].name] = param;
        }
    }

  // end DO NOT EDIT

  // add the 3 utility buttons to the bottom of the layout
  leftlayout->addWidget(utilityBox);

  // layouts can contain other layouts. if you added components to
  // "rightlayout" and added "rightlayout" to "layout," you would
  // have left and right panels in your custom GUI.
  layout->addLayout(leftlayout);
  //layout->addLayout(rightlayout);

  show(); // this line is required to render the GUI
}

// functions designated as Qt slots are implemented as regular C++ functions
void
MembraneTest::aBttn_event(void){

}

void
MembraneTest::bBttn_event(void){

}