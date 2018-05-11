#include <math.h>
#include <string.h>

#include "../../nseos/nuclear_en.h"
#include "../../nseos/crust.h"
#include "../../nseos/core.h"
#include "../../nseos/modeling.h"

#define N (325) // wc -l list_of_good_sets.data

struct parameters read_table_of_good_sets(FILE *, float *, float *);
struct transtion_qtt EOS(int i,struct parameters satdata);

struct transtion_qtt
{
    double nt;
    double pt;
};
struct transtion_qtt eval_transition_qtt(struct parameters);

struct stats
{
    double average[15];
    double variance[15];
    double deviation[15];
    double correlation[15][15];
};
struct stats calc_stats(double data[15][N]);

int main(void)
{
    FILE *good_sets = NULL;

    good_sets = fopen("../../input/list_of_good_sets.data", "r");
    if(good_sets == NULL)
    {
        fprintf(stderr, "ERROR: file issue\n");
        return 1;
    }

    FILE *transition = NULL;
    FILE *statistics = NULL;
    FILE *matrix = NULL;
    struct parameters satdata;
    float m, dm;
    struct transtion_qtt tqtt;
    double p[15][N];
    struct stats st;

    transition = fopen("transition.out", "w+"); 

    for(int i = 0; i < N; i++)
    {
        satdata = read_table_of_good_sets(good_sets, &m, &dm);

        print_parameters(satdata); // test
        fprintf(stderr, "\n==============================================\n\n");

        tqtt = EOS(i,satdata);
        fprintf(transition, "%g %g\n", tqtt.nt, tqtt.pt);

        p[0][i] = tqtt.nt;
        p[1][i] = tqtt.pt;
        p[2][i] = satdata.rhosat0;
        p[3][i] = satdata.lasat0;
        p[4][i] = satdata.ksat0;
        p[5][i] = satdata.qsat0;
        p[6][i] = satdata.zsat0;
        p[7][i] = satdata.jsym0;
        p[8][i] = satdata.lsym0;
        p[9][i] = satdata.ksym0;
        p[10][i] = satdata.qsym0;
        p[11][i] = satdata.zsym0;
        p[12][i] = m;
        p[13][i] = dm;
        p[14][i] = satdata.b;
    }

    st = calc_stats(p);

    statistics = fopen("statistics.out", "w+"); 
    matrix = fopen("matrix.out", "w+"); 

    for(int i = 0; i < 15; i++)
        fprintf(statistics, "%g %g %g\n", st.average[i], st.variance[i], st.deviation[i]);

    for(int i = 0; i < 15; i++)
        fprintf(matrix, "%g %g %g %g %g %g %g %g %g %g %g %g %g %g %g\n", st.correlation[i][0],
                st.correlation[i][1], st.correlation[i][2], st.correlation[i][3], st.correlation[i][4], 
                st.correlation[i][5], st.correlation[i][6], st.correlation[i][7], st.correlation[i][8], 
                st.correlation[i][9], st.correlation[i][10], st.correlation[i][11], st.correlation[i][12], 
                st.correlation[i][13], st.correlation[i][14]);

    fclose(good_sets);
    fclose(transition);
    fclose(statistics);
    fclose(matrix);

    return 0;
}

struct parameters read_table_of_good_sets(FILE *good_sets, float *m, float *dm)
{
    struct parameters satdata;
    float effm, isosplit;

    // reading the parameters
    fscanf(good_sets, "%f %f %f %f %f %f %f %f %f %f %f %f %f", 
            &satdata.lasat0, &satdata.rhosat0, &satdata.ksat0,
            &satdata.qsat0, &satdata.zsat0, &satdata.jsym0, 
            &satdata.lsym0, &satdata.ksym0, &satdata.qsym0, 
            &satdata.zsym0, &effm, &isosplit, &satdata.b);

    // deleting factors
    satdata.lasat0 = -satdata.lasat0/10.;
    satdata.rhosat0 /= 1000.;
    effm /= 100.;
    isosplit /= 100.;
    satdata.b /= 10.;

    // (effm,isosplit) <-> (barm,bardel)
    satdata.barm = 1./effm - 1.;
    satdata.bardel = (sqrt((satdata.barm + 1.)*(satdata.barm + 1.)*isosplit*isosplit + 1.)
            + satdata.barm * isosplit - 1.)/isosplit;

    *m = effm;
    *dm = isosplit;

    return satdata;
}

struct stats calc_stats(double data[15][N])
{
    struct stats result;

    // initializing the sum
    double sumi[15] = {0., 0., 0., 0., 0., 0., 0.,
        0., 0., 0., 0., 0., 0., 0.};

    // average
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < N; j++)
            sumi[i] += data[i][j];
    for(int i = 0; i < 15; i++)
        result.average[i] = sumi[i] / N;

    // initializing the sum
    for(int i = 0; i < 15; i++)
        sumi[i] = 0.;

    // variance and deviation
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < N; j++)
            sumi[i] += pow(data[i][j] - result.average[i], 2.); 
    for(int i = 0; i < 15; i++)
    {
        result.variance[i] = sumi[i] / N;
        result.deviation[i] = sqrt(result.variance[i]);
    }

    double sumij[15][15];
    // initializing the sum
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            sumij[i][j] = 0.;

    // correlation matrix: cor(x,y) = cov(x,y)/(sigx*sigy)
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            for(int k = 0; k < N; k++)
                sumij[i][j] += (data[i][k] - result.average[i])
                    *(data[j][k] - result.average[j]);
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            result.correlation[i][j] = fabs(sumij[i][j] / N)
                /(result.deviation[i]*result.deviation[j]);

    return result;
}


struct transtion_qtt EOS(int i,struct parameters satdata)
{

    FILE *mycrust;
    FILE *mycore;
    FILE *myeos;
    char eos_name[256] = "EOS/eos_";
    char temp[256];
    sprintf(temp, "%d", i);
    strcat(eos_name,temp);
    strcat(eos_name,"_.out");

    mycrust = fopen("EOS/crust.out","w+");
    mycore = fopen("EOS/core.out","w+");
    myeos = fopen(eos_name,"w+");

    print_parameters(satdata);
    fprintf(stderr, "p = %d\n\n", P_SURF_TENSION);
    fprintf(stderr, "==============================================\n\n");
    struct sf_params sparams = fit_sf_params(satdata);

    struct compo comp;
    double muncl = -1.; // sign of muncl is negative is the outer crust
    double guess_oc[3] = {60., 0.15, 0.1595}; // initial guess for the outer crust

    double nb = 1e-10;
    struct transtion_qtt tqtt;

    while(1)
    {
        comp = calc_ocrust3d_composition(nb, guess_oc, satdata, sparams);
        if (guess_oc[0] != guess_oc[0]) // break if nan
        {
            fprintf(stderr, "%g\n", nb); // debug
            break;
        }

        muncl = calc_muncl(satdata, sparams, comp, nb);
        if (muncl > 0.)
            break;

        print_state_crust(satdata, sparams, comp, nb, mycrust, myeos);

        nb += nb/50.;
    }

    fprintf(stderr, "n_d = %g /fm^3\n\n", nb);
    fprintf(stderr, "==============================================\n\n");

    double epsws_ic;
    double epsws_core;
    int transition = 0;
    double guess_ic[4] = {guess_oc[0], guess_oc[1], guess_oc[2], 1.e-4}; // initial guess for the inner crust
    struct core_compo ccomp;
    double guess_npecore = 0.7; // initial guess for the core

    while(1)
    {
        comp = calc_icrust4d_composition(nb, guess_ic, satdata, sparams);
        if (guess_ic[0] != guess_ic[0]) // break if nan
            break;

        if (nb > 0.04)
        {
            // calculation of the energy density in the cell in the inner crust
            epsws_ic = calc_crust_ws_cell_energy_density(satdata, sparams, comp, nb);

            ccomp = calc_npecore_composition(nb, &guess_npecore, satdata);
            if (guess_npecore != guess_npecore) // break if nan
                break;

            // calculation of the energy density in the cell in the core
            epsws_core = calc_core_ws_cell_energy_density(satdata, ccomp, nb);

            if (epsws_core < epsws_ic) // crust-core transition
            {
                fprintf(stderr, "e_core < e_crust\n");
                transition = 1; // transition occurs
                break;
            }
        }

        print_state_crust(satdata, sparams, comp, nb, mycrust, myeos);

        nb += 0.0001;
    }

    if (transition == 0)
        fprintf(stderr, "e_core - e_crust = %g MeV/fm^3\n", epsws_core - epsws_ic);

    fprintf(stderr, "n_t = %g /fm^3\n", nb);
    fprintf(stderr, "P_t = %g MeV/fm^3\n\n", calc_core_ws_cell_pressure(satdata, ccomp, nb));
    fprintf(stderr, "==============================================\n\n");
    tqtt.nt = nb; 
    tqtt.pt = calc_core_ws_cell_pressure(satdata, ccomp, nb);



    double mueltot = 0.; // initializing

    while(1)
    {
        ccomp = calc_npecore_composition(nb, &guess_npecore, satdata);
        if (guess_npecore != guess_npecore) // break if nan
            break;

        mueltot = calc_egas_chemical_potential(nb*(1.-ccomp.del)/2.);
        if (mueltot - MMU > 0.)
            break;

        print_state_core(satdata, ccomp, nb, mycore, myeos);

        nb += 0.001;
    }

    fprintf(stderr, "muons appear at %g /fm^3\n\n", nb);
    fprintf(stderr, "==============================================\n\n");

    double guess_npeucore[2] = {guess_npecore, 1.e-5};

    while (nb < 2.*satdata.rhosat0)
    {
        ccomp = calc_npeucore_composition(nb, guess_npeucore, satdata);
        if (guess_npeucore[0] != guess_npeucore[0]) // break if nan
            break;

        print_state_core(satdata, ccomp, nb, mycore, myeos);

        nb += 0.005;
        //fprintf(stderr,"%f \n",nb);
    }

    fclose(mycrust);
    fclose(mycore);
    fclose(myeos);

    //fprintf(stderr, "END OF EOS FOR THIS ONE\n");
    return tqtt;
}