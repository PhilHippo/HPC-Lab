#include <iostream>

struct fraction{
    int num;
    int denom;
};

void print_fraction(fraction frac);

void print_fraction_array(fraction frac_array[], int n);
// 2
fraction square_fraction(fraction frac);
// 3
void square_fraction_inplace(fraction & frac);
// 4
double fraction2double(fraction frac);

int gcd(int a, int b);
// 6
int gcd(fraction frac);
// 7
void reduce_fraction_inplace(fraction & frac);

fraction add_fractions(fraction frac1, fraction frac2);

double sum_fraction_array_approx(fraction frac_array[], int n);

fraction sum_fraction_array(fraction frac_array[], int n);

void fill_fraction_array(fraction frac_array[], int n);