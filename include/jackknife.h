#ifndef JACKKNIFE
#define JACKKNIFE

double jackknife(double* data,int n)
{
    double mean = 0;
    int i; for(i = 0; i < n; i++)
        mean += data[i];
    mean /= n;
    for(i = 0; i < n; i++)
        data[i] = mean+(mean-data[i])/(n-1);
    return mean;
}

#endif
