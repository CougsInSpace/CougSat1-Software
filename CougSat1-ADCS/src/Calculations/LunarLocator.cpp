/*


*/


#include <math.h>
#include <iostream>
#include <string>

using namespace std;

//function declar.
double T(double x);
double L(double x);
double D(double x);
double MS(double x);
double ML(double x);
double F(double x);
double RLT360(double x);
double A1(double x);
double A2(double x);
double A3(double x);
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

double pi = 3.14159265358979323846;

//Periodic Terms Variable sets
//Coeffecients in order: D, M ,M', F

//These arguments are for longitude and Disrance
double Arg1[60][4] = { {0, 0, 1, 0}, {2, 0, -1, 0}, {2, 0, 0, 0}, {0, 0, 2, 0},
{0, 1, 0, 0}, {0, 0, 0, 2}, {2, 0, -2, 0}, {2, -1, -1, 0},
{2, 0, 1, 0}, {2, -1, 0, 0}, {0, 1, -1, 0}, {1, 0, 0, 0},
{0, 1, 1, 0}, {2, 0, 0, -2}, {0, 0, 1, 2}, {0, 0, 1, -2},
{4, 0, -1, 0}, {0, 0, 3, 0}, {4, 0, -2, 0}, {2, 1, -1, 0},
{2, 1, 0, 0}, {1, 0, -1, 0}, {1, 1, 0, 0}, {2, -1, 1, 0},
{2, 0, 2, 0}, {4, 0, 0, 0}, {2, 0, -3, 0}, {0, 1, -2, 0},
{2, 0, -1, 2}, {2, -1, -2, 0}, {1, 0, 1, 0}, {2, -2, 0, 0},
{0, 1, 2, 0}, {0, 2, 0, 0}, {2, -2, -1, 0}, {2, 0, 1, -2},
{2, 0, 0, 2}, {4, -1, -1, 0}, {0, 0, 2, 2}, {3, 0, -1, 0},
{2, 1, 1, 0}, {4, -1, -2, 0}, {0, 2, -1, 0}, {2, 2, -1, 0},
{2, 1, -2, 0}, {2, -1, 0, -2}, {4, 0, 1, 0}, {0, 0, 4, 0},
{4, -1, 0, 0}, {1, 0, -2, 0}, {2, 1, 0, -2}, {0, 0, 2, -2},
{1, 1, 1, 0}, {3, 0, -2, 0}, {4, 0, -3, 0}, {2, -1, 2, 0},
{0, 2, 1, 0}, {1, 1, -1, 0}, {2, 0, 3, 0}, {2, 0, -1, -2} };

double sum_long_elts[60] = { 6288773, 1274027, 658314, 213618, -185116, -114332, 58793, 57066,
53322, 45758, -40923, -34720, -30383, 15327, -12528, 10980, 10675,
10034, 8548, -7888, -6766, -5163, 4987, 4036, 3994, 3861, 3665,
-2689, -2602, 2390, -2348, 2236, -2120, -2069, 2048, -1773, -1595,
1215, -1110, -892, -810, 759, -713, -700, 691, 596, 549, 537, 520,
-487, -399, -381, 351, -340, 330, 327, -323, 299, 294, 0 };

double sum_dist_elts[60] = { -20905355, -3699111, -2955968, -569925, 48888, -3149, 246158, -152138,
-170733, -204586, -129620, 108743, 104755, 10321, 0, 79661, -34782, -23210,
-21636, 24208, 30824, -8379, -16675, -12831, -10445, -11650, 14403, -7003, 0, 10056,
6322, -9884, 5751, 0, -4950, 4130, 0, -3958, 0, 3258, 2616, -1897, -2117, 2354, 0, 0,
-1423, -1117, -1571, -1739, 0, -4421, 0, 0, 0, 0, 1165, 0, 0, 8752 };

//These arguments are for Latitude
double Arg2[60][4] = { {0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, -1}, {2, 0, 0, -1},
{2, 0, -1, 1}, {2, 0, -1, -1}, {2, 0, 0, 1}, {0, 0, 2, 1},
{2, 0, 1, -1}, {0, 0, 2, -1}, {2, -1, 0, -1}, {2, 0, -2, -1},
{2, 0, 1, 1}, {2, 1, 0, -1}, {2, -1, -1, 1}, {2, -1, 0, 1},
{2, -1, -1, -1}, {0, 1, -1, -1}, {4, 0, -1, -1}, {0, 1, 0, 1},
{0, 0, 0, 3}, {0, 1, -1, 1}, {1, 0, 0, 1}, {0, 1, 1, 1},
{0, 1, 1, -1}, {0, 1, 0, -1}, {1, 0, 0, -1}, {0, 0, 3, 1},
{4, 0, 0, -1}, {4, 0, -1, 1}, {0, 0, 1, -3}, {4, 0, -2, 1},
{2, 0, 0, -3}, {2, 0, 2, -1}, {2, -1, 1, -1}, {2, 0, -2, 1},
{0, 0, 3, -1}, {2, 0, 2, 1}, {2, 0, -3, -1}, {2, 1, -1, 1},
{2, 1, 0, 1}, {4, 0, 0, 1}, {2, -1, 1, 1}, {2, -2, 0, -1},
{0, 0, 1, 3}, {2, 1, 1, -1}, {1, 1, 0, -1}, {1, 1, 0, 1},
{0, 1, -2, -1}, {2, 1, -1, -1}, {1, 0, 1, 1}, {2, -1, -2, -1},
{0, 1, 2, 1}, {4, 0, -2, -1}, {4, -1, -1, -1}, {1, 0, 1, -1},
{4, 0, 1, -1}, {1, 0, -1, -1}, {4, -1, 0, -1}, {2, -2, 0, 1} };

double sum_lat_elts[60] = { 5128122, 280602, 277693, 173237, 55413, 46271, 32573, 17198,
9266, 8822, 8216, 4324, 4200, -3359, 2463, 2211, 2065, -1870,
1828, -1794, -1749, -1565, -1491, -1475, -1410, -1344, -1335,
1107, 1021, 833, 777, 671, 607, 596, 491, -451, 439, 422, 421, -366,
-351, 331, 315, 302, -283, -229, 223, 223, -220, -220, -185, 181,
-177, 176, 166, -164, 132, -119, 115, 107 };

int main(void)
{
	double t = 2458754.114583;
	double Time = T(t);
	cout.precision(16);
	cout << "T: " << Time << endl;
	//Reduced Angular Terms (L, D, M, M', F)
	double l = RLT360(L(Time));
	double d = RLT360(D(Time));
	double ms = RLT360(MS(Time));
	double ml = RLT360(ML(Time));
	double f = RLT360(F(Time));

	double a1 = RLT360(A1(Time));
	double a2 = RLT360(A2(Time));
	double a3 = RLT360(A3(Time));


	double sumlong = calc_long_sums(sum_long_elts, Arg1, d, ms, ml, Time, f);
	double sumlat = calc_lat_sums(sum_lat_elts, Arg2, d, ms, ml, Time, f);
	double sumdist = calc_dist_sums(sum_dist_elts, Arg1, d, ms, ml, Time, f);

	double sumlong_final = sumlong + 3958 * sin(a1) + 1962 * sin(l - f) + 318 * sin(a2);
	double sumlat_final = sumlat - 2235 * sin(l) + 382 * sin(a3) + 175 * sin(a1 - f) + 175 * sin(a1 + f) + 127 * sin(l - ml) - 115 * sin(l + ml);

	double Longitude = Lambda(l, sumlong_final);
	double Latitude = Beta(sumlat_final);
	double Distance = Delta(sumdist);

	double RA = GimmeRA(Longitude, Latitude);
	double dec = GimmeDec(Longitude, Latitude);
	/********************************
		TESTING OUTPUTS
	********************************/
	/*
	cout.precision(16);
	cout << "Sumlong out func: " << sumlong << endl;
	cout << "Sumlat out func: " << sumlat << endl;
	cout << "Sumdist out func: " << sumdist << endl;

	//cout << "T: " << Time << endl;
	cout << "L: " << l << endl;
	cout << "D: " << d << endl;
	cout << "MS: " << ms << endl;
	cout << "ML: " << ml << endl;
	cout << "F: " << f << endl;
	cout << "A1: " << a1 << endl;
	cout << "A2: " << a2 << endl;
	cout << "A3: " << a3 << endl;
	double temp = E(Time);
	cout << "Eccen. of earth orbit: " << temp << endl;
	cout << "Suml: " << sumlong_final << endl;
	cout << "Sumb: " << sumlat_final << endl;
	cout << "Sumr: " << sumdist << endl;
	*/
	
	final_calcs_and_print_data(RA, dec, Longitude, Latitude, Distance); //what a name lol
	return 0;
}
void final_calcs_and_print_data(double RA, double dec, double Longitude, double Latitude, double Distance)
{
	double RA_degrees = degtoArc_degrees(RA);
	double dec_Deg = degtoArc_degrees(dec);
	double RA_Hours = degtoArc_hours(RA);
	double dec_Hours = degtoArc_hours(dec);
	double DeclinationMinutes = degtoArc_declination_minutes(dec, dec_Deg);
	double RA_Min = degtoArc_minutes(RA, RA_Hours);
	double dec_Min = degtoArc_minutes(dec, dec_Hours);
	double DecSecs = degtoArc_DecSecs(dec, dec_Deg, DeclinationMinutes);
	double RA_Sec = degtoArc_seconds(RA, RA_Hours, RA_Min);
	double dec_Sec = degtoArc_seconds(dec, dec_Hours, dec_Min);

	char degr = 248;

	cout.precision(19);
	cout << "Geocentric Coordinates:" << endl;
	cout << "Longitude: " << Longitude << endl;
	cout << "Latitude: " << Latitude << endl;
	cout << "Distance from the Earth (km): " << Distance << endl;
	cout << endl;
	cout << "Right Ascention (degrees): " << RA <<endl;
	cout << "Right Ascention in Arctime: " << RA_Hours << " hr, " << RA_Min << " m, " << RA_Sec << " s. " << endl;
	cout << endl;
	cout << "Declination (degrees): " << dec << endl;
	cout << "Declination in Arctime: " << dec_Deg << degr << " " << DeclinationMinutes << " ' " << DecSecs << endl;
}

double T(double x)
{
	return (x - 2451545.0) / 36525;
}

//Mean longitude
double L(double x)
{
	double ltemp = 218.3164477 + (481267.88123421 * x) - 0.0015786 * pow(x, 2) + pow(x, 3) / 538841 - pow(x, 4) / 65194000;
	//cout << ltemp << endl;
	//cout << x << endl;
	return ltemp;
}

//Mean Elongation of Moon
double D(double x)
{
	return 297.8501921 + (445267.1114034 * x) - 0.0018819 * pow(x, 2) + (pow(x, 3) / 545868) - (pow(x, 4) / 113065000);
}

//Solar Mean Anomaly
double MS(double x)
{
	return 357.5291092 + (35999.0502909 * x) - 0.0001536 * pow(x, 2) + (pow(x, 3) / 24490000);
}

//Lunar Mean Anomaly
double ML(double x)
{
	return 134.9633964 + (477198.8675055 * x) + 0.0087414 * pow(x, 2) + (pow(x, 3) / 69699) - (pow(x, 4) / 14712000);
}

//Lunar Argument of Latitude
double F(double x)
{
	return 93.2720950 + (483202.0175233 * x) - 0.0036539 * pow(x, 2) - (pow(x, 3) / 3526000) + (pow(x, 4) / 863310000);
}

//Reduce less than 360 degrees
double RLT360(double x)
{
	double rlt = x - 360.0 * int(x / 360.0); //original python was rlt = x - 360.0*int(x/360.0)
	if (rlt < 0)
	{
		rlt = rlt + 360;
	}
	return rlt;
}

//Venus Perturbation
double A1(double x)
{
	return 119.75 + (131.848 * x);
}

//Jupiter Perturbation
double A2(double x)
{
	return 53.09 + (479264.290 * x);
}

//Other Perturbation
double A3(double x)
{
	return 313.45 + (481266.484 * x);
}

//Earth's eccentricity consideration [for M/-M multiply by E, for 2M/-2M multiply by E^2]
double E(double x)
{
	return 1 - (0.002516 * x) - 0.0000074 * pow(x, 2);
}

//Geocentric Longitude (Inputs: L, Sum-Long, Output: degrees)
double Lambda(double x, double y)
{
	return x + y / 1000000;
}

//Geocentric Latitude (Input: Sum-Lat, Output: degrees)
double Beta(double x)
{
	return x / 1000000;
}

//Distance (Input: Sum Distance, Output: km)
double Delta(double x)
{
	return 385000.56 + x / 1000;
}

//Functions to sum up al of the objects
double sinesum(double a, double b, double c, double d, double e, double f, double g, double h, double l, double m)
{
	int k = 0;
	if (d == 2 || d == -2)
	{
		k = 2;
	}
	else if (d == 1 || d == -1)
	{
		k = 1;
	}
	else
	{
		k = 0;
	}
	return a * sin((b * c + d * e + f * g + l * m) * pi / 180) * pow(h, k); //figure pi thing out
}
double cossum(double a, double b, double c, double d, double e, double f, double g, double h, double l, double m)
{
	
	int k = 0;
	if (d == 2 || d == -2)
	{
		k = 2;
	}
	else if (d == 1 || d == -1)
	{
		k = 1;
	}
	else
	{
		k = 0;
	}
	//cout << "k: " << k << endl;
	//cout << endl;
	return a * cos((b * c + d * e + f * g + l * m) * pi / 180) * pow(h, k); //figure pi thing out
}

double calc_long_sums(double(&pass_sum_long_elts)[60], double(&pass_Arg1)[60][4], double d, double ms, double ml, double Time, double f)
{
	double sumlong = 0;
	double Q = 0;
	for (int i = 0; i < 60; i++)
	{
		//cout << Arg1[i][0] << endl;
		Q = sinesum(pass_sum_long_elts[i], pass_Arg1[i][0], d, pass_Arg1[i][1], ms, pass_Arg1[i][2], ml, E(Time), pass_Arg1[i][3], f);
		//cout.precision(19);
		//cout << Q << endl;
		sumlong = sumlong + Q;
		//cout << pass_Arg1[i][1] << endl;
	}
	return sumlong;
}

double calc_lat_sums(double(&pass_sum_lat_elts)[60], double(&pass_Arg2)[60][4], double d, double ms, double ml, double Time, double f)
{
	double sumlat = 0;
	double J = 0;
	for (int i = 0; i < 60; i++)
	{
		J = sinesum(pass_sum_lat_elts[i], pass_Arg2[i][0], d, pass_Arg2[i][1], ms, pass_Arg2[i][2], ml, E(Time), pass_Arg2[i][3], f);

		sumlat = sumlat + J;
	}
	return sumlat;
}

double calc_dist_sums(double(&pass_sum_dist_elts)[60], double(&pass_Arg1)[60][4], double d, double ms, double ml, double Time, double f)
{
	double sumdist = 0;
	double W = 0;
	for (int i = 0; i < 60; i++)
	{
		W = cossum(pass_sum_dist_elts[i], pass_Arg1[i][0], d, pass_Arg1[i][1], ms, pass_Arg1[i][2], ml, E(Time), pass_Arg1[i][3], f);

		sumdist = sumdist + W;
	}
	return sumdist;
}

//a is lambda and b is beta
double GimmeRA(double a, double b)
{
	double sinep = 0.397777156;
	double cosep = 0.917482062;
	double RA = atan2((sin(a * pi / 180) * cosep - tan(b * pi / 180) * sinep), cos(a * pi / 180));
	return RA * 180 / pi;
}


double GimmeDec(double a, double b)
{
	double sinep = 0.397777156;
	double cosep = 0.917482062;
	double dec = asin(sin(b * pi / 180) * cosep + cos(b * pi / 180) * sinep * sin(a * pi / 180));
	return dec * 180 / pi;
}

double degtoArc_degrees(double x)
{
	return int(x);
}

double degtoArc_hours(double x)
{
	return int(x / 15);
}

double degtoArc_declination_minutes(double x, double degrees)
{
	return int((x - degrees) * 60);
}

double degtoArc_minutes(double x, double Hours)
{
	return int((x - Hours * 15) / 0.25);
}

double degtoArc_DecSecs(double x, double Degrees, double DeclinationMinutes)
{
	return (x - Degrees - DeclinationMinutes / 60) * 3600;
}

double degtoArc_seconds(double x, double Hours, double Minutes)
{
	return (x - Hours * 15 - Minutes * 0.25) / 0.004166;
}

