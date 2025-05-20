#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// f(x)
double f(double x, double coeffs[], int degree) {
    double result = 0.0;
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * pow(x, i);
    }
    return result;
}

// f'(x)
double df(double x, double coeffs[], int degree) {
    double result = 0.0;
    for (int i = 1; i <= degree; i++) {
        result += i * coeffs[i] * pow(x, i - 1);
    }
    return result;
}

// f''(x)
double d2f(double x, double coeffs[], int degree) {
    double result = 0.0;
    for (int i = 2; i <= degree; i++) {
        result += i * (i - 1) * coeffs[i] * pow(x, i - 2);
    }
    return result;
}

// Phương pháp tiếp tuyến với lần lặp cho n trước
void tiep_tuyen(double coeffs[], int degree, int n, double a, double b, int precision) {
    double x0 = (f(a, coeffs, degree) * d2f(a, coeffs, degree) > 0) ? a : b;
    double x = x0;
    double x_prev;

    // Chuỗi định dạng theo precision, ví dụ "%.6f"
    char format[50];
    sprintf(format, "| %%2d  |     %%%d.%df      |\n", precision + 4, precision);

    printf("----------------------------\n");
    printf("|  n  |         x_n        |\n");
    printf("----------------------------\n");

    int i = 1;
    while (n--) {
        double fx = f(x, coeffs, degree);
        double dfx = df(x, coeffs, degree);

        if (dfx == 0) {
            printf("Đạo hàm bằng 0, không thể tiếp tục.\n");
            break;
        }

        x_prev = x;
        x -= fx / dfx;

        // In kết quả với số chữ số thập phân tùy chỉnh
        printf(format, i, x);
        i++;
    }

    printf("----------------------------\n");

    // Tính m1 và m2 tại a và b
    double m1 = fmin(fabs(df(a, coeffs, degree)), fabs(df(b, coeffs, degree)));
    double m2 = fmax(fabs(d2f(a, coeffs, degree)), fabs(d2f(b, coeffs, degree)));

    // Tính sai số
    double error_formula1 = fabs(f(x, coeffs, degree)) / m1;
    double error_formula2 = m2 / (2 * m1) * pow(x - x_prev, 2);

    // In kết quả
    char float_format[20];
    sprintf(float_format, "%%s: %%.%df\n", precision);

    printf(float_format, "Nghiem gan dung", x);
    printf(float_format, "Sai so theo cong thuc 1", error_formula1);
    printf(float_format, "Sai so theo cong thuc 2", error_formula2);

}

int main() {
    // Hệ số đa thức
    double coeffs[] = {2, -3, 1, 5, 6};
    int degree = sizeof(coeffs) / sizeof(coeffs[0]) - 1;

    int n = 10;
    double a = -2.0, b = 3.0;
    int precision = 5;

    tiep_tuyen(coeffs, degree, n, a, b, precision);
    return 0;
}
