#include <iostream>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>
#include "build_star.h"

using namespace std;

int main(void)
{

  cout << "Program Start" << endl;
  double density_conv_fact = 1.695e15;   // [/fm^3] to [g/cm^3]
  double pressure_conv_fact = 1.6022e33; // [MeV/fm^3] to [dyn/cm^2]
  bool convert = true;
  int Number_of_files =325;

 
/********************Initialisation of for variables*************************/

  struct constant_variables variables_set ;
  set_variables(&variables_set);
  variables_set.max_density = 1; // THIS IS BEING CHANGE MANUALLY HERE CAREFUL
  print_variables(variables_set);

  calculation_for_list_of_EOS(variables_set,Number_of_files);

  cout << "Program End" << "\n";
  return 0;
}

