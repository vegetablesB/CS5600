/*
 * @Author: Cao Ning
 * @Date: 2023-01-19 17:40:52
 * @FilePath: CaoN.CS5600.S2.LearnC.c
 * @Assignment:
 * @Course: CS5600 Spring 2023
 */
#include <stdio.h>
#include <string.h>

/*  mpg2kml() that takes miles per gallon as an argument and
    returns the equivalent kilometers per liter.  */
float mpg2kml(float mpg);

/*  mpg2lphm() that takes miles per gallon as an argument and
    returns the equivalent liters per 100 kilometers.  */
float mpg2lphm(float mpg);

/*  kml2mpg() that takes kilometers per liter as an argument and
    returns the equivalent miles per gallon.  */
float kml2mpg(float kml);

// main function
int main()
{
    /* Add some test cases to test that the functions work.  */
    printf("mpg2kml(10) = %f, should be 4.25144 \n", mpg2kml(10.0));
    printf("mpg2lphm(10) = %f, should be 425.144 \n", mpg2lphm(10.0));
    printf("kml2mpg(10) = %f, should be 23.5215 \n", kml2mpg(10.0));

    /*  add some test cases to ensure that you functions work
        even when "bad" argument values are passed.  */
    printf("\nmpg2kml(-1) = %f, should be 0 \n", mpg2kml(-1.0));
    printf("mpg2lphm(-1) = %f, should be 0 \n", mpg2lphm(-1.0));
    printf("kml2mpg(-1) = %f, should be 0 \n", kml2mpg(-1.0));

    return 0;
}

float mpg2kml(float mpg)
{
    if (mpg <= 0)
    {
        return 0;
    }
    float kml = mpg * 1.609344 / 3.785411784;

    return kml;
}

float mpg2lphm(float mpg)
{
    if (mpg <= 0)
    {
        return 0;
    }
    // float lphm = mpg * 1.609344 / 3.785411784 * 100; wrong
    float lphm = 235.215 / mpg;

    return lphm;
}

float kml2mpg(float kml)
{
    if (kml <= 0)
    {
        return 0;
    }
    float mpg = kml * 3.785411784 / 1.609344;

    return mpg;
}
