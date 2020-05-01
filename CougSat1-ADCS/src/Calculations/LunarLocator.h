#include <math.h>
#include <iostream>
#include <string>

using namespace std;

//wrapper function is lunar_prop, requires the UCT date int and UTC time of day double

//function declar.
void lunar_prop(int utc_date, double utc_time);
double julian_date(int utc_date, double utc_time);
double T(double x);
double L(double x);
double D(double x);
double MS(double x);
double ML(double x);
double F(double x);
double RLT360(double x);
double A11(double x);
double A22(double x);
double A33(double x);
double E(double x);
double Lambda(double x, double y);
double Beta(double x);
double Delta(double x);
double Delta(double x);
double sinesum(double a, double b, double c, double d, double e, double f, double g, double h, double l, double m);
double cossum(double a, double b, double c, double d, double e, double f, double g, double h, double l, double m);
double calc_long_sums(double(&pass_sum_long_elts)[60], double(&pass_Arg1)[60][4], double d, double ms, double ml, double Time, double f);
double calc_lat_sums(double(&pass_sum_lats_elts)[60], double(&pass_Arg2)[60][4], double d, double ms, double ml, double Time, double f);
double calc_dist_sums(double(&pass_sum_dist_elts)[60], double(&pass_Arg1)[60][4], double d, double ms, double ml, double Time, double f);
double GimmeRA(double a, double b);
double GimmeDec(double a, double b);
void final_calcs_and_print_data(double RA, double dec, double Longitude, double Latitude, double Distance);
double degtoArc_degrees(double x);
double degtoArc_hours(double x);
double degtoArc_declination_minutes(double x, double degrees);
double degtoArc_minutes(double x, double Hours);
double degtoArc_DecSecs(double x, double Degrees, double DeclinationMinutes);
double degtoArc_seconds(double x, double Hours, double Minutes);