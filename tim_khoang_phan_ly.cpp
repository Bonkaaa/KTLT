#include <stdio.h>
#include <math.h>   

// Định nghĩa hàm f(x): truyền hệ số đa thức và bậc
double f(double x, double coeffs[], int degree) {
    double result = 0.0;
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * pow(x, i);
    }
    return result;
}

// Tìm hệ số của đạo hàm
void f_prime(double coeffs[], double deriv[], int degree) {
    for (int i = 1; i <= degree; i++) {
        deriv[i-1] = coeffs[i] * i;
    }
}

// Kiểm tra tính đơn điệu của hàm số
bool check_monotonicity(double coeffs[], int degree, double x1, double x2, double step) {
    double deriv[degree];
    f_prime(coeffs, deriv, degree);

    double x = x1;
    double prev = f(x, deriv, degree-1);
    bool is_increasing = (prev > 0);
    bool is_decreasing = (prev < 0);

    while(x <= x2) {
        double current = f(x, deriv, degree-1);
        if (current > 0) {
            is_decreasing = false;
        } else if (current < 0) {
            is_increasing = false;
        }
        else {
            is_increasing = false;
            is_decreasing = false;
        }
        x += step;
    }
    if(is_decreasing || is_increasing) {
        return true;
    }
    else {
        return false;
    }
}

void find_interval(double coeffs[], int degree, double x1, double x2, int precision, double result[]) {
    char format[50];
    sprintf(format, "Khoảng phân ly nghiệm: [%%.%df, %%.%df]\n", precision, precision);
    
    // Kiểm tra điều kiện ban đầu
    if(f(x1, coeffs, degree) * f(x2, coeffs, degree) > 0 || !check_monotonicity(coeffs, degree, x1, x2, 0.1)) {
        result[0] = -1;
        result[1] = -1;
        return;
    }

    while(fabs(x2-x1) > 0.5) {
        double mid = (x1 + x2) / 2;
        if(f(x1, coeffs, degree) * f(mid, coeffs, degree) < 0) {
            x2 = mid;
        } else {
            x1 = mid;
        }
    }
    result[0] = x1;
    result[1] = x2;
}

// Test
int main()
{
    double coeffs[] = {-3, 0, -2, 0 , 1}; // Đa thức bậc 2: x^2 - 3x + 2
    int degree = 4;
    double x1 = -2;
    double x2 = -1;
    double step = 0.1;
    int precision = 5;

    double interval[2];
    find_interval(coeffs, degree, x1, x2, precision, interval);
    if(interval[0] != -1) {
        char format[50];
        sprintf(format, "Khoảng phân ly nghiệm: (%%.%df, %%.%df)\n", precision, precision);
        printf(format, interval[0], interval[1]);
    }
    else {
        printf("Không tìm thấy khoảng phân ly nghiệm.\n");
    }
    printf("%lf %lf", interval[0], interval[1]);
    return 0;
}

