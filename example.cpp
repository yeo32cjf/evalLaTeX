#include "eTree.h"

/*
Calculate LaTeX equation from command line.

Compile 
./compile.sh

Calculate equation with no independent variable.
./example "2e+3"
>> 8.43656

Calculate equation with independent variable and its value.
function and its derivative's value will be calculated.
./example "x^3-2ex" "1" "2"
>> f(1)=-4.43656366,  f'(1)=-2.43656366
f(2)=-2.87312731,  f'(2)=6.56343634
*/

int main(int argc,char *argv[])
{
    expTree::SetValue("a", "4");             // set a = 4
expTree::SetValue("b", 3.6);             // set b = 3.6
expTree::SetValue("c", "4+3\\pi");       // set c = 4+3pi
double x = atof_latex("a+b+c");          // x = a+b+c = 11.6+3pi = 
printf("%f",x);
    if(argc == 1)
    {
        printf("Usage: './example \"Your_Latex_Equation\"' for constant. './example \"Your_Latex_Equation_by_x\" \"x_value_1\" \"x_value_2\" ...' for equation by x\n");
        printf("error\n");
        return 1;
    }

    latexEquation *exp = new latexEquation;

    if(argc == 2)
    {
        printf("evaluate constant %s\n",argv[1]);
        exp->parse(argv[1],"constant");

        if(exp->parseSuccess)
        {
            printf("%.8f\n",exp->calculate());
            delete exp;
            return 0;
        }
        else
        {
            printf("error at ");
            printf(expTree::eTreeErrorMessage);
            delete exp;
            return 1;
        }
    }
    else
    {
        printf("evaluate f(x) = %s for ",argv[1]);

        for(int i=0;i<argc-2;i++)
        {
            if(i == 0)
                printf("x=%s",argv[i+2]);
            else
                printf(",%s",argv[i+2]);
        }

        printf("\n");

        exp->parse(argv[1],"f(x)");
        //exp->printTree("test.txt","a+",0);
       

        if(exp->parseSuccess == false)
        {
            printf("error at ");
            printf(expTree::eTreeErrorMessage);
            delete exp;
            return 1;
        }

        latexEquation *differential = exp->differential();
        //differential->printTree("test.txt","a+",0);

        
        for(int i=0;i<argc-2;i++)
        {
            latexEquation *val = new latexEquation(argv[i+2]);

            if(val->parseSuccess == true)
            {
                //double xVal = val->calculate();
                //printf("f(%s)=%.8f,  f'(%s)=%.8f\n", argv[i + 2],exp->calculate(xVal), argv[i + 2],differential->calculate(xVal));

                printf("f(%s)=%.8f,  f'(%s)=%.8f\n", argv[i + 2], exp->calculate(argv[i + 2]), argv[i + 2], differential->calculate(argv[i + 2]));
            }
            else
            {
                printf("error at value %d: %s\n",i+1,expTree::eTreeErrorMessage);
            }

            delete val;
        }
        
        delete exp;
        delete differential;
        return 0;
    }
}