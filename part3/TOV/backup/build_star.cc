#include <iostream>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <iomanip> 
#include <string>
#include "build_star.h"

using namespace std;

double TOV(star &star_, double radius_step)
{
  // Physical constants in CGS units
  double G = 6.6742e-8;
  double c = 2.99792458e10;

  double dpressure;
  double mass_ = star_.mass;
  double density_ = star_.density;
  double pressure_ = star_.pressure;
  double radius_ = star_.radius;

 
  dpressure = -G * mass_ * density_ / (radius_ * radius_) 
          * (1 + pressure_ / (density_ * c * c)) 
          * (1 + (4 * M_PI * radius_ * radius_ * radius_ * pressure_) / (mass_ * c * c)) 
          * (1 / (1 - 2 * G * mass_ / (radius_ * c * c))) 
          * radius_step;

  return dpressure;
}

double find_density_for_pressure(struct star *star_, struct read_results *results)
{

  double density;
  double pressure_ = star_->pressure;
  double pressure_0 = 2.31012620945e+33;
  //double pressure_0 = 1.561e19;
  int counter = 0;
  vector<double>::iterator lower_it;
  lower_it = lower_bound(results->pressure.begin(),results->pressure.end(),pressure_);
  int lower = lower_it - results->pressure.begin();
  int end = distance(results->pressure.begin(),results->pressure.end()) -1 ;

    if (lower != 0 && lower < end )
    {

        double a = (results->EOS_data[lower][1] - results->EOS_data[lower - 1][1]) / (results->EOS_data[lower][0] - results->EOS_data[lower - 1][0]);
        density = a * (pressure_ - results->EOS_data[lower - 1][0]) + results->EOS_data[lower - 1][1];
        counter = 1;
    }


    if (counter == 0)
    {
      if (pressure_ > pressure_0)
      {
        int imax = results->EOS_data.size();
        double c = (results->EOS_data[imax - 1][1] - results->EOS_data[imax - 2][1]) / (results->EOS_data[imax - 1][0] - results->EOS_data[imax - 2][0]);
        density = c * (pressure_ - results->EOS_data[imax - 2][0]) + results->EOS_data[imax - 2][1];
      }
      else
      {
        double c = (results->EOS_data[1][1] - results->EOS_data[0][1]) / (results->EOS_data[1][0] - results->EOS_data[0][0]);
        density = c * (pressure_ - results->EOS_data[0][0]) + results->EOS_data[0][1];
      }
    }

  return density;
}


double find_pressure_for_density(struct star *star_, struct read_results *results)
{

  double pressure;
  double density_ = star_->density;
  double density_0 = 5e12;
  int counter = 0;
 
  for (int i = 0; i < results->EOS_data_swapped.size(); i++)
  {
    if (i != 0)
    {
      if (results->EOS_data_swapped[i - 1][0] <= density_ && density_ <= results->EOS_data_swapped[i][0])
      {
        double a = (results->EOS_data_swapped[i][1] - results->EOS_data_swapped[i - 1][1]) / (results->EOS_data_swapped[i][0] - results->EOS_data_swapped[i - 1][0]);
        pressure = a * (density_ - results->EOS_data_swapped[i - 1][0]) + results->EOS_data_swapped[i - 1][1];
        counter = 1;
      }
    }
  }

  if (counter == 0)
  {
    if (density_ > density_0)
    {
      int imax = results->EOS_data_swapped.size();
      double c = (results->EOS_data_swapped[imax - 1][1] - results->EOS_data_swapped[imax - 2][1]) / (results->EOS_data_swapped[imax - 1][0] - results->EOS_data_swapped[imax - 2][0]);
      pressure = c * (density_ - results->EOS_data_swapped[imax - 2][0]) + results->EOS_data_swapped[imax - 2][1];
    }
    else
    {
      double c = (results->EOS_data_swapped[1][1] - results->EOS_data_swapped[0][1]) / (results->EOS_data_swapped[1][0] - results->EOS_data_swapped[0][0]);
      pressure = c * (density_ - results->EOS_data_swapped[0][0]) + results->EOS_data_swapped[0][1];
    }
  }
  return pressure;
}

void death_star(struct star *star_){

  star_->mass = 0;
  star_->pressure = 0;
  star_->density  = 0;
  star_->radius = 0;
}

struct read_results read_files(bool convert,bool theory,ifstream *EOS_DATA_FILE){

  double density_conv_fact = 1.695e15;   // [/fm^3] to [g/cm^3]
  double pressure_conv_fact = 1.6022e33;
  struct read_results results;
  vector<double> input; 
  vector<double> input_swapped;
  double a,b,c;
if (convert == true && theory == false)
{
  while (*EOS_DATA_FILE >> a >> b >> c)
  {
    input.push_back(c * pressure_conv_fact );
    //input.push_back(a *density_conv_fact);
    input.push_back(b);
    //input_swapped.push_back(a *density_conv_fact );
    input_swapped.push_back(b);
    input_swapped.push_back(c *pressure_conv_fact );
    results.EOS_data.push_back(input);
    results.EOS_data_swapped.push_back(input_swapped);
    results.pressure.push_back(c*pressure_conv_fact);
    results.density.push_back(b);
    input.clear();
    input_swapped.clear();
  }
}
if (convert == false && theory == false){
  while (*EOS_DATA_FILE >> a >> b >> c)
  {
    input.push_back(c);
    input.push_back(b);
    input_swapped.push_back(b);
    input_swapped.push_back(c);
    results.EOS_data.push_back(input);
    results.EOS_data_swapped.push_back(input_swapped);
    results.pressure.push_back(c);
    results.density.push_back(b);
    input.clear();
    input_swapped.clear();
  }
}
if(theory == true){
  while (*EOS_DATA_FILE >> a >> b )
  {
    input.push_back(b*pressure_conv_fact);
    input.push_back(a*density_conv_fact);
    input_swapped.push_back(a*density_conv_fact);
    input_swapped.push_back(b*pressure_conv_fact);
    results.EOS_data.push_back(input);
    results.EOS_data_swapped.push_back(input_swapped);
    results.pressure.push_back(b*pressure_conv_fact);
    results.density.push_back(a*density_conv_fact);
    input.clear();
    input_swapped.clear();
  }
}




return results;
  
}

void print_EOS(vector < vector < double > > EOS_data){

  for(int i = 0; i< EOS_data.size();i++){

      cout << EOS_data[i][0] << " " << EOS_data[i][1] << endl;
  }
  
}

void step_contruction(int i, struct read_results results,ofstream *all_out,struct constant_variables variables_set){
  double central_density;
  double central_pressure;
 //double transition_pressure;
  //double transition_density = 0.0792 ; // [/fm^3] for ELFc
  //double transition_density = 0.0719 ; // [/fm^3] for ELFd
  string output_all_out_file_name; //output names for file to be incremented/change by different calculation
  string output_transition_out_file_name;//type
  string changed_part;
  //transition_density *= density_conv_fact;
  star test_star;
  double total_mass, total_radius;


    //Initialisation for the start of the loop
    test_star.radius = 10;
    central_density = variables_set.saturation_density + i * (variables_set.max_density*variables_set.saturation_density) / variables_set.N;
    test_star.density = central_density;
    central_pressure = find_pressure_for_density(&test_star, &results);

    test_star.pressure = central_pressure;
    test_star.mass = 0.0;
    //while still in star ie that the density if above the limit density of the star
    while (test_star.density > variables_set.epsilon)
    {
      test_star.radius += variables_set.radius_step;
      test_star.mass += test_star.density * 4 * M_PI * test_star.radius * test_star.radius * variables_set.radius_step;
      test_star.pressure += TOV(test_star, variables_set.radius_step);
      //check to see if the pressure if still coherent with being in a star
      if (test_star.pressure > 1e20)
      {
          test_star.density = find_density_for_pressure(&test_star,&results); 
      }
      else
        break;

      }

      //Normalisation of the values to [km] and [solar mass]
    total_mass = test_star.mass / variables_set.solar_mass;
    total_radius = test_star.radius / 100000;

    //Output all relevant data to the out file
    *all_out << total_radius << " " << total_mass << " " << central_density << " " << central_pressure << "\n";

    //Normalisation of the stages in the star ie core and crust
  }
  
 void set_variables(struct constant_variables *variable_set){
  variable_set->density_conv_fact = 1.695e15; 
  variable_set->pressure_conv_fact = 1.6022e33; 
  variable_set->radius_step = 100; //[cm]
  variable_set->solar_mass = 1.989e33; 
  variable_set->N = 50; //number of star runs x2, increase compute time by x4 ish
  variable_set->saturation_density = 0.1595 * variable_set->density_conv_fact;
  variable_set->epsilon = 1e-10 * variable_set->density_conv_fact ;
  variable_set->max_density = 2 ; //N+1 * saturation density
}


void print_variables(struct constant_variables variable_set) {
  cout << "=======================================================================================" << "\n";
  cout << "The start saturation density (n_sat) is set as : " << variable_set.saturation_density 
       << " [g/cm^3] or "<<variable_set.saturation_density/variable_set.density_conv_fact
       << " [/fm^3]" << "\n";
  cout << "Number of points calculated is : " << variable_set.N << "\n";
  cout << "The epsilon density limit is set at : "<< variable_set.epsilon 
       << " [g/cm^3] or "<<variable_set.epsilon/variable_set.density_conv_fact
       << " [/fm^3] " << "\n";
  cout << "The maximum density is : " << variable_set.saturation_density*(variable_set.max_density+1)/variable_set.density_conv_fact 
                                      << " [/fm^3] " <<" or "<<(variable_set.max_density+1)<<"n_sat"<< "\n";
  cout << "The chosen stept for the radius is : " << variable_set.radius_step << " [cm]"<< "\n" ;
  cout << "=======================================================================================" << "\n";
  cout << "\n" << "\n";
}

  void EOS_calculation(int i ,struct constant_variables variables_set,string output_all_out_file_name,ifstream &EOS_DATA_FILE,bool theory ){
    ofstream all_out;
    all_out.open(output_all_out_file_name.c_str());
    struct read_results read_results_storage;

    if (theory == true) read_results_storage = read_files(false,theory,&EOS_DATA_FILE);
    if (theory == false) read_results_storage = read_files(true,theory,&EOS_DATA_FILE);

  for (int i = 0; i < variables_set.N; i++)
  {
    step_contruction(i,read_results_storage,&all_out,variables_set);
  }

    all_out.close();

  }


void calculation_for_multiple_EOS(struct constant_variables variables_set, int Number_of_files){


  cout << "**************************************"<< "\n";
  cout << "The number of the loop implies : " << "\n";    
  cout << "[0 --> c] , [1 --> d] , [2 --> DH], [3 --> sly] "<<"\n";

  //********************For on number of stars*************************/
for (int i=0; i<=Number_of_files; i++){


  string changed_part;
  ifstream EOS_DATA_FILE; 
  string output_all_out_file_name;
  bool theory = false;


    if(i == 0){
    EOS_DATA_FILE.open("input/eos_c.out");
    changed_part = "_c";
    output_all_out_file_name = "output/all_out" +changed_part+ ".txt";
    }
    else if(i == 1){
    EOS_DATA_FILE.open("input/eos_d.out");
    changed_part = "_d";
    output_all_out_file_name = "output/all_out" +changed_part+ ".txt";
    }
    else if(i == 2){
    EOS_DATA_FILE.open("input/eos_theory_converted.out");
    changed_part = "_DH";
    output_all_out_file_name = "output/all_out" +changed_part+ ".txt";
    theory = true;
    }
    else if(i == 3){
    EOS_DATA_FILE.open("input/eos_sly.out");
    changed_part = "_sly";
    output_all_out_file_name = "output/all_out" +changed_part+ ".txt";
  }
  
  cout << "Calculation for loop number : " << i << "\n";  
  EOS_calculation(i,variables_set,output_all_out_file_name,EOS_DATA_FILE,theory);

}
  cout << "**************************************"<< "\n";
  cout << "\n" << "\n";
}


void calculation_for_list_of_EOS(struct constant_variables variables_set, int Number_of_files){

  cout << "**************************************"<< "\n";
  cout << "The number of the of EOS_files is  : " << Number_of_files << "\n";

  string changed_part;
  ifstream EOS_DATA_FILE; 
  string output_all_out_file_name;
  bool theory = false;
  for (int i = 0; i< Number_of_files; i++){
  string input_file = "input/eos_" + to_string(i) + ".txt";
  EOS_DATA_FILE.open(input_file);
  output_all_out_file_name = "output/all_out" +to_string(i)+ ".txt";

  cout << "Calculation for loop number : " << i << "\n";  
  EOS_calculation(i,variables_set,output_all_out_file_name,EOS_DATA_FILE,theory);
}

  cout << "**************************************"<< "\n";
  cout << "\n" << "\n";

}
