//Nelson Lee 
//Lab 5, Pascal's Triangle
//February 18, 2021
#include <stdio.h>
#include <math.h>

//Functions Used
int choose();
int factorial();
int spaces();

//Main
int main (int argc, char **argv)
{
    int rows = 0; //Declare Variable for Row Input
    while (rows >= 0)  //Loops and Checks if Input is Proper
    {
        printf("Enter the number of rows: ");
        scanf("%d", &rows); //Prompt Input

        if (rows<=13 && rows>0) //Checks if Number is Fine, Does Nothing if over 13
        {
            rows = rows-1; 

            for (int n = 0; n <= rows; n++) //Initial Loop per Row
            {
                int spacesPut = spaces(rows, n); //Call Spaces Function

                for (int f=1;f<=spacesPut; f++) //Loops Initial Spaces
                {
                    printf(" ");
                }

                for (int r = n; r>=0; r--) //Loops Row Values
                {
                    //Declare Variables for Spacing and Output
                    int outputNum = choose(n, r); //Call Choose Function
                    int count = 0;
                    long long nn = outputNum;

                    while (nn != 0) //Count Digits of Output
                    {
                        nn /= 10;     // n = n/10
                        ++count;
                    }
                    printf("%d", outputNum); //Output

                    for (int g=1;g<=6-count;g++) //Loop for Spacing In Between Values
                    {
                        printf(" ");
                    }
                }
                printf("\n");
            } 
            rows = rows + 1;
        }
    }
    return 0;
}

//Choose Function
int choose (int n, int r)
{
    int ncr; //Declare Variables
    ncr =factorial(n)/(factorial(r)*factorial(n-r));
    return ncr;
}

//Factorial Function
int factorial(int n)
{
    int i,f=1; //Declare Variables

    for(i=1;i<=n;i++) //Loop to Multiply Values
    {
        f=f*i;
    }
    return f;
}

//Spaces Function
int spaces (int rows, int n)
{
    int spaces=0; //Declare Variables
    spaces = (rows - n)*3; //Determine Initial Spacing
    return spaces;
}
