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

// Phương pháp tiếp tuyến với sai số epsilon cho trước theo công thức sai số 1
void tiep_tuyen_ss1(double coeffs[], int degree, double a, double b, double epsilon, int precision) {
    printf("Phương pháp tiếp tuyến với sai số epsilon cho trước theo công thức sai số 1\n");
    double x0 = (f(a, coeffs, degree) * d2f(a, coeffs, degree) > 0) ? a : b;
    double x = x0;
    double x_prev;

    // Tính m1 = min |f'(x)| và m2 = max |f'(x)|
    double m1 = fmin(fabs(df(a, coeffs, degree)), fabs(df(b, coeffs, degree)));
    double m2 = fmax(fabs(d2f(a, coeffs, degree)), fabs(d2f(b, coeffs, degree)));

    if (m1 == 0) {
        printf("m1 = 0, không thể tính sai số theo công thức 1 (chia cho 0).\n");
        return;
    }

    // Chuỗi định dạng theo precision, ví dụ "%.6f"
    char format[50];
    sprintf(format, "| %%2d  |   %%.%df  |          %%.%df            |\n", precision, precision);

    printf("--------------------------------------------------\n");
    printf("|  n  |    x_n     |            Sai so            \n");
    printf("--------------------------------------------------\n");

    int i = 1;
    while (i<=1000) {
        double fx = f(x, coeffs, degree);
        double dfx = df(x, coeffs, degree);

        if (dfx == 0) {
            printf("Đạo hàm bằng 0, không thể tiếp tục.\n");
            break;
        }

        x_prev = x;
        x -= fx / dfx;

        // Tính sai số
        double error = fabs(x - x_prev);

        // In kết quả với số chữ số thập phân tùy chỉnh
        printf(format, i, x, error);
        // Kiểm tra điều kiện dừng
        if (fabs(f(x, coeffs, degree)) / m1 < epsilon || fabs(x - x_prev) < epsilon) {
            printf("--------------------------------------------------\n");
            printf("Thoả mãn điều kiện hội tụ, dừng thuật toán\n");
            break;
        }
        i++;
    }
    printf("--------------------------------------------------\n");
    // In kết quả 
    printf("Nghiệm gần đúng: x = %.*f\n", precision, x);
}

// Phương pháp tiếp tuyến với sai số epsilon cho trước theo công thức sai số 2
void tiep_tuyen_ss2(double coeffs[], int degree, double a, double b, double epsilon, int precision) {
    printf("Phương pháp tiếp tuyến với sai số epsilon cho trước theo công thức sai số 2\n");
    double x0 = (f(a, coeffs, degree) * d2f(a, coeffs, degree) > 0) ? a : b;
    double x = x0;
    double x_prev;

    // Tính m1 = min |f'(x)| và m2 = max |f'(x)|
    double m1 = fmin(fabs(df(a, coeffs, degree)), fabs(df(b, coeffs, degree)));
    double m2 = fmax(fabs(d2f(a, coeffs, degree)), fabs(d2f(b, coeffs, degree)));

    if (m1 == 0) {
        printf("m1 = 0, không thể tính sai số theo công thức 1 (chia cho 0).\n");
        return;
    }

    // Chuỗi định dạng theo precision, ví dụ "%.6f"
    char format[50];
    sprintf(format, "| %%2d  |   %%.%df  |          %%.%df            |\n", precision, precision);

    printf("--------------------------------------------------\n");
    printf("|  n  |    x_n     |            Sai so            \n");
    printf("--------------------------------------------------\n");

    int i = 1;
    while (i<=1000) {
        double fx = f(x, coeffs, degree);
        double dfx = df(x, coeffs, degree);

        if (dfx == 0) {
            printf("Đạo hàm bằng 0, không thể tiếp tục.\n");
            break;
        }

        x_prev = x;
        x -= fx / dfx;

        // Tính sai số
        double threshold = sqrt((2 * epsilon * m1) / m2);

        double error = fabs(x - x_prev);

        // In kết quả với số chữ số thập phân tùy chỉnh
        printf(format, i, x, error);
        // Kiểm tra điều kiện dừng
        if (error < threshold) {
            printf("--------------------------------------------------\n");
            printf("Thoả mãn điều kiện hội tụ, dừng thuật toán\n");
            break;
        }
        i++;

    }
    printf("--------------------------------------------------\n");
    // In kết quả 
    printf("Nghiệm gần đúng: x = %.*f\n", precision, x);
}

int main() {
    // Định nghĩa hệ số đa thức và bậc
    double coeffs[] = {-6, 11, -6, 1}; // Hệ số cho đa thức 2x^4 - 3x^3 + x^2 + 5x + 7
    int degree = sizeof(coeffs) / sizeof(coeffs[0]) - 1;
    double a = 0.5; // Giới hạn tìm kiếm
    double b = 3.5; // Giới hạn tìm kiếm
    double epsilon = 0.0001; // Sai số cho trước
    int precision = 5; // Số chữ số thập phân

    // Phương pháp tiếp tuyến với sai số epsilon cho trước theo công thức sai số 1
    tiep_tuyen_ss1(coeffs, degree, a, b, epsilon, precision);
    printf("\n");
    // Phương pháp tiếp tuyến với sai số epsilon cho trước theo công thức sai số 2
    tiep_tuyen_ss2(coeffs, degree, a, b, epsilon, precision);

    return 0;
}