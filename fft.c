#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 131072

int Log2(int N) {
    int k = N, i = 0;
    while (k) {
        k >>= 1;
        i++;
    }
    return i - 1;
}

int reverse(int N, int n) {
    int j, p = 0;
    for (j = 1; j <= Log2(N); j++) {
        if (n & (1 << (Log2(N) - j)))
            p |= 1 << (j - 1);
    }
    return p;
}

void transform(double fr[], double fi[], int N) {
    // Print the initial values before reversing
    printf("Initial values before reversing:\n");
    for (int i = 0; i < N; i++) {
        printf("%d: %lf + %lf j\n", i, fr[i], fi[i]);
    }

    // ordina function task
    double fr2[MAX], fi2[MAX];
    for (int i = 0; i < N; i++) {
        fr2[i] = fr[reverse(N, i)];
        fi2[i] = fi[reverse(N, i)];
    }
    for (int j = 0; j < N; j++) {
        fr[j] = fr2[j];
        fi[j] = fi2[j];
    }

    for (int i = 0; i < N; i++)
        printf("%lf + %lf j\n", fr[i], fi[i]);

    printf("Log2%d\n", Log2(16));

    double Wr[MAX], Wi[MAX];
    Wr[1] = cos(-2. * M_PI / N);
    Wi[1] = sin(-2. * M_PI / N);
    Wr[0] = 1;
    Wi[0] = 0;


   // Open file to store twiddle factors
    FILE *twiddle_fp = fopen("twiddle_factors.dat", "w");
    if (twiddle_fp == NULL) {
        printf("Error opening twiddle factors file\n");
        return;
    }

    // Print and save twiddle factors
    printf("Twiddle Factors:\n");
    printf("Index\tReal\tImaginary\n");
    printf("0\t%.5lf\t%.5lf\n", Wr[0], Wi[0]);
    printf("1\t%.5lf\t%.5lf\n", Wr[1], Wi[1]);
    fprintf(twiddle_fp, "Twiddle Factors:\n");
    fprintf(twiddle_fp, "Index\tReal\tImaginary\n");
    fprintf(twiddle_fp, "0\t%.5lf\t%.5lf\n", Wr[0], Wi[0]);
    fprintf(twiddle_fp, "1\t%.5lf\t%.5lf\n", Wr[1], Wi[1]);

    for (int i = 2; i < N / 2; i++) {
        double angle = -2. * M_PI * i / N;
        Wr[i] = cos(angle);
        Wi[i] = sin(angle);
        printf("%d\t%.5lf\t%.5lf\n", i, Wr[i], Wi[i]);
        fprintf(twiddle_fp, "%d\t%.5lf\t%.5lf\n", i, Wr[i], Wi[i]);
    }
    fclose(twiddle_fp);
    /*
    printf("Printing twiddle factors ....\n");
    printf("1 %lf + %lf j\n", Wr[1], Wi[1]);

    for (int i = 2; i < N / 2; i++) {
        double angle = -2. * M_PI * i / N;
        Wr[i] = cos(angle);
        Wi[i] = sin(angle);
        printf("%d %lf , %lf\n", i, Wr[i], Wi[i]);
    }
    printf("\n");
    */
    int n = 1;
    int a = N / 2;

    printf("log2%f\n", log2(16));
    for (int j = 0; j < log2(N); j++) {
        for (int i = 0; i < N; i++) {
            if (!(i & n)) {
                double tempr = fr[i];
                double tempi = fi[i];
                double Tempr, Tempi;
                Tempr = Wr[(i * a) % (n * a)] * fr[i + n] - Wi[(i * a) % (n * a)] * fi[i + n];
                Tempi = Wr[(i * a) % (n * a)] * fi[i + n] + Wi[(i * a) % (n * a)] * fr[i + n];
                /*
                // Debug statements
                printf("i = %d, n = %d, a = %d\n", i, n, a);
                printf("tempr = %lf, tempi = %lf\n", tempr, tempi);
                printf("Tempr = %lf, Tempi = %lf\n", Tempr, Tempi);
                printf("fr[%d] = %lf + %lf j\n", i, fr[i], fi[i]);
                printf("fr[%d] = %lf + %lf j\n", i + n, fr[i + n], fi[i + n]);
                */
                fr[i] = tempr + Tempr;
                fi[i] = tempi + Tempi;
                fr[i + n] = tempr - Tempr;
                fi[i + n] = tempi - Tempi;
            }
        }
        n *= 2;
        a = a / 2;
    }

    printf("...Printing the FFT of the array specified\n");
    for (int i = 0; i < N; i++) {
        printf("%d %.5lf + %.5lf j\n", i, fr[i], fi[i]);
    }

    FILE *fp = fopen("nonds_r.dat", "w");
    if (fp == NULL) {
        printf("Error: file not found\n");
        return;
    }
    for (int i = 0; i < N; i++) {
        fprintf(fp, "%.100lf\n", fr[i]);
    }
    fclose(fp);

    fp = fopen("nonds_i.dat", "w");
    if (fp == NULL) {
        printf("Error: file not found\n");
        return;
    }
    for (int i = 0; i < N; i++) {
        fprintf(fp, "%.100lf\n", fi[i]);
    }
    fclose(fp);
}

void FFT(double fr[], double fi[], int n, double d) {
    transform(fr, fi, n);
    for (int i = 0; i < n; i++) {
        fr[i] *= d;
        fi[i] *= d;
    }
}

int main() {
    int a;
    scanf("%d", &a);
    int n = pow(2, a);
    double d = 1;

    FILE *file;
    double vecr[MAX], veci[MAX];

    file = fopen("pilotXsubc.dat", "r");
    if (file == NULL) {
        printf("Error: file not found\n");
        return 1;
    }
    for (int i = 0; i < n; i++) {
        veci[i] = 0;
        fscanf(file, "%lf", &vecr[i]);
    }
    fclose(file);

    FFT(vecr, veci, n, d);

    return 0;
}

