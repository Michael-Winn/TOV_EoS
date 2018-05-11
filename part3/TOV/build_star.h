#ifndef BUILD_STAR
#define BUILD_STAR

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;
struct constant_variables {

  double density_conv_fact;   // [/fm^3] to [g/cm^3]
  double pressure_conv_fact; // [MeV/fm^3] to [dyn/cm^2]
  double radius_step; //cm has been set to 100 put just increases compute x20, no noticeable effect
  double solar_mass; // tabulated value of the sun used to normalise mass
  double N; // number of star for this EOS run
  double saturation_density; // saturation density usually 2.3e15
  double epsilon; // density a which point it is no longer considered a star
  int max_density; // calculation goes up to N+1 * saturation density

};


struct star
{
  double density;
  double mass;
  double pressure;
  double radius;
};

struct read_results{

  vector < vector < double > > EOS_data;
  vector < vector < double > > EOS_data_swapped;
  vector < double > density;
  vector < double > pressure;

};



void set_variables(struct constant_variables *variable_set);
//sets the standard variables for the calculations ie epsilon density, number of stars, solar mass
//radius step, saturation denisty etc.
void print_variables(struct constant_variables variable_set);
//prints said varaibles

double TOV(star &star, double radius_step);
//calculates the varaition in pressure from on step to the next following the TOV equation

double find_density_for_pressure(struct star *star_, struct read_results *results);
//runs throught the tabulated data in the EOS to find a corresponding density for the given pressure
//If outside of tabulated values extrapolates linearly using the last points as reference


double find_pressure_for_density(struct star *star_,struct read_results *results);
//runs throught the tabulated data in the EOS to find a corresponding pressure for the given density
//If outside of tabulated values extrapolates linearly using the last points as reference

void print_EOS(vector < vector < double > > EOS_data_);
//Prints the imported values form the file for the EOS

struct read_results read_files(bool convert,bool theory,ifstream *EOS_DATA_FILE);
//Reads the file and imports into two vector tables of EOS(pressure density) and EOS swapped(density pressure)

void death_star(struct star *star_);
//clears the star of all information

void step_contruction(int i, struct read_results results,ofstream *all_out,struct constant_variables variables_set);
//Calculates step by step the make up of the star and writes to the provided file

void EOS_calculation(int i ,struct constant_variables variables_set,string output_all_out_file_name,ifstream &EOS_DATA_FILE,bool theory);
//calculates the step_construction for one EOS included in the input file

void calculation_for_multiple_EOS(struct constant_variables variables_set, int Number_of_files);
//loops on multiple EOS files

void calculation_for_list_of_EOS(struct constant_variables variables_set, int Number_of_files);
//loops on a list of eos files using i between 0 and Number_of_files

#endif