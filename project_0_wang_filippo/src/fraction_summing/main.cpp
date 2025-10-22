#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "fraction_toolbox.hpp"

using namespace std;

// read command line arguments
static void readcmdline(fraction & frac, int argc, char* argv[])
{
    if (argc!=3)
    {
        printf("Usage: n d\n");
        printf("  n        numerator of fraction\n");
        printf("  d        denominator of fraction\n");
        exit(1);
    }

    // read n
    frac.num = atoi(argv[1]);

    // read d
    frac.denom = atoi(argv[2]);
}

static void test23467(int argc, char * argv[])
{
    
    fraction frac;
    fraction original = frac;
    readcmdline(frac, argc, argv);

    // test 2
    fraction result = square_fraction(frac);
    cout << "Square of " << frac.num << "/" << frac.denom << " is ";
    print_fraction(result);

    // test 3
    square_fraction_inplace(frac);
    cout << "The fraction after square_fraction_inplace is ";
    print_fraction(frac);

    // test 4
    double result2 = fraction2double(frac);
    cout << "Conversion to double of " << frac.num << "/" << frac.denom << " is " << result2 << endl;

    // test 6
    int result3 = gcd(frac);
    cout << "GCD of " << frac.num << " and " << frac.denom << " is " << result3 << endl;

    // test 7
    cout << "Reduced fraction of " << frac.num << "/" << frac.denom << " is ";
    reduce_fraction_inplace(frac);
    cout << frac.num << "/" << frac.denom << endl;

}

static void test5()
{
    int a;
    int b;
    cout << "Enter two integers: ";
    cin >> a >> b;
    int GCD = gcd(a, b);
    cout << "The greatest common divisor of " << a << " and " << b << " is " << GCD << endl;
}

static void test_array_functions(int n)
{
    fraction* frac_array = (fraction*)malloc(n * sizeof(fraction));
    fill_fraction_array(frac_array, n);
    //print_fraction_array(frac_array, n);

    fraction sum_exact = sum_fraction_array(frac_array, n);
    cout << "Sum of the fractions for n = " << n << " is ";
    print_fraction(sum_exact);

    double sum_approx = sum_fraction_array_approx(frac_array, n);
    cout << "Sum of the fractions is " << sum_approx << "\n" << endl;

    // breaks at n = 1290. The exact fraction sum function breaks because during the process the 32-bit integer overflows.
    // It does not happen with the approximate sum function because it converts the fractions to doubles and then adds them up. The double precision (64 bits) is large enough to store the values.
}

static void test_toolbox(int argc, char* argv[])
{
    cout << "\n===============  test23467  =============== " << endl;
    test23467(argc, argv);

    cout << "\n=================  test5  ================= " << endl;
    test5();

    cout << "\n==========  test_array_functions  ========= " << endl;
    int n = 1289;
    test_array_functions(n);
}

int main(int argc, char* argv[])
{
    int left = 1250;
    int right = 1300;
    for (int i = left; i < right; i++)
    {
        test_array_functions(i);
    }
    test_toolbox(argc, argv);
    return 0;
}