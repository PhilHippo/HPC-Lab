#include <iostream>

#include "fraction_toolbox.hpp"

void print_fraction(fraction frac)
{
    std::cout << frac.num << '/' << frac.denom << std::endl;
}

void print_fraction_array(fraction frac_array[], int n)
{
    std::cout << "[ " << frac_array[0].num << '/' << frac_array[0].denom << std::endl;
    for (int i = 1; i < n-1; i++)
    {
        std::cout << "  ";
        print_fraction(frac_array[i]);
    }
    std::cout << "  " << frac_array[n-1].num << '/' << frac_array[n-1].denom << " ]" << std::endl;
}

fraction square_fraction(fraction frac)
{
    fraction result;
    result.num = frac.num * frac.num;
    result.denom = frac.denom * frac.denom;
    return result;
}

void square_fraction_inplace(fraction & frac)
{
    frac.num = frac.num * frac.num;
    frac.denom = frac.denom * frac.denom;
}


double fraction2double(fraction frac)
{
   double result = (double)frac.num / (double)frac.denom;
   return result;
}

int gcd(int a, int b)
{
    if (b == 0)
    {
        return a;
    }
    return gcd(b, a % b);

}

int gcd(fraction frac)
{
    int a = frac.num;
    int b = frac.denom;

    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}


void reduce_fraction_inplace(fraction & frac)
{
    int GCD = gcd(frac);
    frac.num = frac.num / GCD;
    frac.denom = frac.denom / GCD;

    //we are calling the iterative version of the gcd function defined at line 52 because we are passing a fraction as an argument.
}

fraction add_fractions(fraction frac1, fraction frac2)
{
    fraction result;
    result.num = frac1.num * frac2.denom + frac2.num * frac1.denom;
    result.denom = frac1.denom * frac2.denom;
    reduce_fraction_inplace(result);
    return result;
}

double sum_fraction_array_approx(fraction frac_array[], int n)
{
    double  result = 0;
    for (int i = 0; i < n; i++)
    {
        result += fraction2double(frac_array[i]);
    }
    return result;
   
    //It's an approximation because we are converting the fraction into a rational number stored in a double variable, which is limited in precision.
    //Another way to do it would be to use the add_fractions function and then reduce the final fraction inplace and then convert it to a double, but it would still be an approximation per the same reason.
}

fraction sum_fraction_array(fraction frac_array[], int n)
{
    // assume array is non-empty
    fraction result;
    result.num = frac_array[0].num;
    result.denom = frac_array[0].denom;
    for (int i = 1; i < n; i++)
    {
        result = add_fractions(result, frac_array[i]);
    }
    return result;
}


void fill_fraction_array(fraction frac_array[], int n)
{
    fraction temp_frac;
    temp_frac.num = 1;
    for (int i = 1; i <= n; i++)
    {
        temp_frac.denom = i * (i+1);
        frac_array[i-1] = temp_frac;
    }
}

