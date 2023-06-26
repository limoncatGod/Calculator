#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define BUF_SIZE 1024

typedef struct variable{
    char name[BUF_SIZE];
    char val[BUF_SIZE];
}VAR;

double replace_vars(char *str, int, VAR*);
double eval(char *str);
double number(char *, unsigned *);
double expr(char *, unsigned *);
double term(char *, unsigned *);
double factor(char *, unsigned *);

int main()
{
    char str[BUF_SIZE];
    int num_of_vars = 0;

    printf("Enter expression:\n");
    gets(str);
    printf("Enter number of variables:\n");
    scanf("%d", &num_of_vars);
    printf("Enter variables:\n");
    VAR* arr_vars = malloc(num_of_vars*sizeof(VAR));
    for(int i = 0; i < num_of_vars; i++){
        scanf("%s = %s", arr_vars[i].name, arr_vars[i].val);
    }

    printf("Result: %lf\n", replace_vars(str, num_of_vars, arr_vars));

   // printf("Result: %lf\n", eval(str));

    return 0;
}


double replace_vars(char *str, int size, VAR* mass){
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    char buffer[BUF_SIZE];
    char new_str[BUF_SIZE];
    while(str[i] != '\0'){
        if(((str[i] >= '0' && str[i] <= '9') || str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '^' || str[i] == '(' || str[i] == ')') == 0){
            buffer[j] = str[i];
            j++;
            for(int z = 0; z < size; z++){
                if(strcmp(mass[z].name, buffer) == 0){
                    while(mass[z].val[l] != '\0'){
                        new_str[k] = mass[z].val[l];
                        l++;
                        k++;
                    }
                    l = 0;
                }
            }
        } else{
            j = 0;
            l = 0;
            new_str[k] = str[i];
            k++;
        }
        i++;

    }

    return eval(new_str);
}

double eval(char *str)
{
    unsigned i = 0;

    return expr(str, &i);
}

double number(char *str, unsigned *idx)
{
    double result = 0.0;
    double div = 10.0;
    int sign = 1;

    if (str[*idx] == '-')
    {
        sign = -1;
        ++*idx;
    }

    while (str[*idx] >= '0' && str[*idx] <= '9')
    {
        result = result * 10.0 + (str[*idx] - '0');

        ++*idx;
    }

    if (str[*idx] == '.')
    {
        ++*idx;

        while (str[*idx] >= '0' && str[*idx] <= '9')
        {
            result = result + (str[*idx] - '0') / div;
            div *= 10.0;

            ++*idx;
        }
    }

    return sign * result;
}

double expr(char *str, unsigned *idx)
{
    double result = term(str, idx);

    while (str[*idx] == '+' || str[*idx] == '-')
    {
        switch (str[*idx])
        {
            case '+':
                ++*idx;

                result += term(str, idx);

                break;
            case '-':
                ++*idx;

                result -= term(str, idx);

                break;
        }
    }

    return result;
}

double term(char *str, unsigned *idx)
{
    double result = factor(str, idx);
    double div;

    while (str[*idx] == '*' || str[*idx] == '/')
    {
        switch (str[*idx])
        {
            case '*':
                ++*idx;

                result *= factor(str, idx);

                break;
            case '/':
                ++*idx;

                div = factor(str, idx);

                if (div != 0.0)
                {
                    result /= div;
                }
                else
                {
                    printf("Division by zero!\n");
                    exit(-1);
                }

                break;
        }
    }

    return result;
}

double factor(char *str, unsigned *idx)
{
    double result;
    int sign = 1;

    if (str[*idx] == '-')
    {
        sign = -1;

        ++*idx;
    }

    if (str[*idx] == '(')
    {
        ++*idx;

        result = expr(str, idx);

        if (str[*idx] != ')')
        {
            printf("Brackets unbalanced!\n");
            exit(-2);
        }

        ++*idx;
    }
    else
        result = number(str, idx);

    if (str[*idx] == '^')
    {
        ++*idx;

        result = pow(result, factor(str, idx));
    }

    return sign * result;
}