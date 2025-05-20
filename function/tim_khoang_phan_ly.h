#ifndef TIM_KHOANG_PHAN_LY
#define TIM_KHOANG_PHAN_LY

void f_prime(double coeffs[], double deriv[], int degree);
bool check_monotonicity(double coeffs[], int degree, double x1, double x2, double step);
void find_interval(double coeffs[], int degree, double x1, double x2, int precision, double result[]);

#endif