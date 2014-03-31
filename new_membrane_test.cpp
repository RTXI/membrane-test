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
  /**/QGroupBox *hold_group = new QGroupBox("Holding Potential");
  QFormLayout *hold_layout = new QFormLayout;
  QPushButton *hold_toggle = new QPushButton("Pulse");
  QLineEdit *hold_volt = new QLineEdit;
  QLineEdit *hold_peak = new QLineEdit;
  QLineEdit *hold_period = new QLineEdit;
  hold_layout->addRow(hold_toggle);
  hold_layout->addRow("Hold Voltage (mV)", hold_volt);
  hold_layout->addRow("Amplitude (mV)", hold_peak);
  hold_layout->addRow("Period (ms)", hold_period);
  /**/hold_group->setLayout(hold_layout);
  middle_layout->addWidget(hold_group, 0, 0);
  
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
  middle_layout->addWidget(zap_group, 0, 1);
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
  middle_layout->addWidget(properties_group, 1, 0);
  
  //Membrane Properties output
//  QGroupBox *output_group = new QGroupBox("Output");
  
  //Resistance at Bottom  
  
  //Put all the plugins together
  plugin_layout->addWidget(utility);
  plugin_layout->addWidget(middle);
  
 // /**/plugin_layout->addWidget(hold_group);

  
  
  
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

}
