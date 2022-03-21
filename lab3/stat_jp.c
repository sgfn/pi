#include <stdio.h>
#include <stdlib.h>
#define TWO_DICE_SUM 11

// Calculates arithmetic mean and variance of numbers from n-element array v[]
// for n = 0: mean = variance = 0
void mean_variance(const double v[], double *mean, double *variance, int n) {
    double mean_val = 0, variance_val = 0;
    if (n != 0) {
        for (int i=0; i<n; ++i) {
            mean_val += v[i];
            variance_val += v[i]*v[i];
        }
        mean_val /= n;
        variance_val = (variance_val/n) - (mean_val*mean_val);
    }
    *mean = mean_val;
    *variance = variance_val;
}

// Fill n-element array v[] with Bernoulli sample (binary values)
void bernoulli_gen(int v[], int bernoulli_percentage, int n) {
    for (int i=0; i<n; ++i) {
        int r = rand() % 100;
        v[i] = (r < bernoulli_percentage) ? 1 : 0;
    }
}

// Calculate pmf (probability mass function) of discrete random variable
// (percentage values from [0, 100]).
// Two dice are thrown simultaneously The random variable is the sum of the
// numbers shown (the result is from [2, 12]).
// trials - number of trials
void pmf(double v[], int trials) {
    for (int i=0; i<TWO_DICE_SUM; ++i)    v[i] = 0;
    for (int i=0; i<trials; ++i) {
        int d1 = rand() % 6;
        int d2 = rand() % 6;
        v[d1+d2]++;
    }
    for (int i=0; i<TWO_DICE_SUM; ++i)  v[i] = v[i]*100/trials;
}

// Calculates the cumulative distribution function (cdf) for 'trials' numbers
// (percentage values from [0, 100]).
// Two dice are thrown simultaneously The random variable is the sum of the
// numbers shown (the result is from [2, 12]).
// trials - number of trials
void calculate_cdf(double cdf[], int trials) {
    for (int i=0; i<TWO_DICE_SUM; ++i)    cdf[i] = 0;
    for (int i=0; i<trials; ++i) {
        int d1 = rand() % 6;
        int d2 = rand() % 6;
        cdf[d1+d2]++;
    }
    int prev = 0;
    for (int i=0; i<TWO_DICE_SUM; ++i) {
        cdf[i] += prev;
        prev = cdf[i];
        cdf[i] = cdf[i]*100/trials;
    }
}

// Simulate Monty-Hall problem
// input: n - number of trials
// output: wins - number od wins if door switched
// output: win_probability - probability of win if door switched
void monty_hall(int *wins, double *win_probability, int n) {
	*wins = 0;
    for(int i = 0; i < n; i++) {
		int prize = rand() % 3; // the winning door
        int choice = rand() % 3; // the first choice door
		int reveal; // the door revealed by the host (neither your choice nor the prize door)
		// set reveal	
		do {
			reveal = rand() % 3;
		} while (reveal == prize || reveal == choice);
		// your second choice (if you switch)
        int remaining;
        if      (reveal==0)     remaining = (choice==1) ? 2 : 1;
        else if (reveal==1)     remaining = (choice==0) ? 2 : 0;
        else                    remaining = (choice==0) ? 1 : 0;

		if (remaining == prize) ++(*wins); // you win if you switched
    }
    *win_probability = (double)(*wins) * 100.0 / n;
}

// read double vector of size n
void read_vector(double v[], int n) {
    for (int i=0; i<n; ++i) {
		scanf("%lf", &v[i]);
	}
}


// print double vector of size n (with 2 figures after the decimal point)
void print_vector(const double v[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("%.2f ", v[i]);
    }
    printf("\n");
}

// print integer vector
void print_int_vector(const int v[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

int main(void) {
    int to_do, len, seed, bernoulli_percentage, m_h_wins, i_tab[100];
    double arithmetic_mean, variance, m_h_probability, d_tab[100] = { 0 };
    scanf("%d", &to_do);

    if(to_do > 1) {
        scanf("%d",&seed);
        srand((unsigned)seed);
    }

    switch (to_do) {
    case 1: // mean_variance
        arithmetic_mean = 0.0;
	    variance = 0.0;
        scanf("%d", &len);
        read_vector(d_tab, len);
	    mean_variance(d_tab, &arithmetic_mean, &variance, len);
        printf("%.2f %.2f\n", arithmetic_mean, variance);
        break;
    case 2: // bernoulli_gen
        scanf("%d", &len);
        scanf("%d", &bernoulli_percentage);
        bernoulli_gen(i_tab, bernoulli_percentage, len);
	    print_int_vector(i_tab, len);
        break;
    case 3: // pmf
        scanf("%d", &len);
	    pmf(d_tab, len);
        print_vector(d_tab, TWO_DICE_SUM);
        break;
    case 4: // calculate_cdf
        scanf("%d", &len);
	    calculate_cdf(d_tab, len);
        print_vector(d_tab, TWO_DICE_SUM);
        break;
    case 5: // monty_hall
        scanf("%d", &len);
        monty_hall(&m_h_wins, &m_h_probability, len);
        printf("%d %.2f %d %.2f\n", m_h_wins, m_h_probability,
			   len - m_h_wins, 100.0 - m_h_probability);
        break;
    default:
        printf("NOTHING TO DO FOR %d\n", to_do);
        break;
    }
    return 0;
}

