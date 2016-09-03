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

#include <default_gui_model.h>

class MembraneTest : public DefaultGUIModel
{
  Q_OBJECT

public:
  MembraneTest(void);
  virtual ~MembraneTest(void);

  void initParameters(void);
  void customizeGUI(void);
  void execute(void);
  // void refresh(void);

protected:
  virtual void update(DefaultGUIModel::update_flags_t);

private:
  double Rt, Cm, Ra, Rm;
  double period;

  // Pulse parameters
  double V1, V2, V3, Vapp;
  double hold_amplitude;
  double hold_period;
  size_t hold_index, hold_count;
  enum VoltSelect_t
  {
    VOLTAGE1 = 0,
    VOLTAGE2 = 1,
    VOLTAGE3 = 2
  };
  VoltSelect_t volt_mode;
  bool volt_changed; // not used for anything yet
  QButtonGroup* membuttonsgroup;
  QPushButton *v1_button, *v2_button, *v3_button;
  QLabel* resistance_label;
  QTimer* label_timer;

  // ZAP parameters
  bool zap_on;
  double zap_amplitude;
  double zap_duration;
  size_t zap_index, zap_count;
  QPushButton* zap_button;

  // Membrane properties parameters
  enum MemTestMode_t
  {
    SIMPLE = 0,
    DETAILED = 1
  };
  // int mem_update;
  MemTestMode_t mem_mode;
  bool mode_changed;
  int mem_steps, mem_steps_saved;
  double I1, I2, dI;
  size_t mem_index;
  std::vector<double> newdata, data;
  QButtonGroup* voltbuttonsgroup;
  QPushButton *simple_button, *detailed_button;

private slots:
  void toggleZap(void);
  void selectMode(int);
  void selectVoltage(int);

  void computeMembraneProperties(void);
  void updateUIResistance(void);
};
