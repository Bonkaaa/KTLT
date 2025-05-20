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

// Hàm tìm miền chứa nghiệm 
void find_root_intervals_auto(double coeffs[], int degree, double max_range, double step, int precision, double result[]) {
    double x1 = -max_range;
    double x2 = x1 + step;
    double y1,y2;
    int found_interval = 0;
    result[0] = 0;
    result[1] = 0;

    char format[50];
    sprintf(format, "Nghiệm nằm trong khoảng: [%%.%dlf, %%.%dlf]\n", precision, precision);

    printf("Các miền chứa nghiệm của phương trình là:\n");
    while(x2 <= max_range){
        y1 = f(x1, coeffs, degree);
        y2 = f(x2, coeffs, degree);
        
        // Nếu hàm đổi dấu giữa x1 và x2 thì có nghiệm trong khoảng này
        if (y1*y2 <= 0) {
            printf(format, x1, x2);
            found_interval ++;
            result[0] = x1;
            result[1] = x2;
        }
        
        x1 = x2;
        x2 += step;
    }

    if(found_interval == 0) {
        printf("Không tìm thấy miền chứa nghiệm trong khoảng [-%.2lf, %.2lf]\n", max_range, max_range);
    }
}

int main() {
    // Định nghĩa hệ số đa thức và bậc
    double coeffs[] = {2, -3, 1}; // x^2 - 3x + 2
    int degree = sizeof(coeffs) / sizeof(coeffs[0]) - 1;
    double max_range = 10.0; // Giới hạn tìm kiếm
    double step = 0.1; // Bước nhảy
    int precision = 5; // Số chữ số thập phân
    double result[2]; // Kết quả trả về

    // Tìm miền chứa nghiệm
    find_root_intervals_auto(coeffs, degree, max_range, step, precision, result);

    return 0;
}
