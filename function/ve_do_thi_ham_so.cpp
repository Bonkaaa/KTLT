#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Hàm f(x): Định nghĩa hàm cần vẽ
double f(double x, double coeffs[], int degree) {
    double result = 0.0;
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * pow(x, i);
    }
    return result;
}

// Hàm ghi dữ liệu vào file
void write_data_to_file(const char* filename, double x1, double x2, double step, double coeffs[], int degree) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Khong the mo file %s\n", filename);
        return;
    }
    printf("Ghi du lieu vao file %s...\n", filename);
    for(double x = x1; x <= x2; x += step) {
        double y = f(x, coeffs, degree);
        fprintf(file, "%lf %lf\n", x, y);
    }
    fclose(file);
    printf("Da ghi du lieu vao file %s\n", filename);
}

void plot_graph(const char* filename) {
    FILE* gnuplotpipe = popen("gnuplot -persistent", "w");
    if (!gnuplotpipe) {
        printf("Khong the mo gnuplot\n");
        return;
    }
    fprintf(gnuplotpipe, "set title 'Do thi ham so'\n");
    fprintf(gnuplotpipe, "set xlabel 'x'\n");
    fprintf(gnuplotpipe, "set ylabel 'f(x)'\n");
    fprintf(gnuplotpipe, "set grid linewidth 1\n"); // Đường lưới mỏng bình thường
    fprintf(gnuplotpipe, "set border linewidth 1\n"); // Đặt viền mỏng
    fprintf(gnuplotpipe, "set xtics axis\n");
    fprintf(gnuplotpipe, "set ytics axis\n");
    // Thiết lập phạm vi trục
    fprintf(gnuplotpipe, "set xrange [-10:10]\n");
    fprintf(gnuplotpipe, "set yrange [-10:10]\n");

    // Vẽ trục tung (x=0) in đậm
    fprintf(gnuplotpipe, "set arrow from 0,-10 to 0,10 nohead linewidth 3 lc rgb 'black'\n");

    // Vẽ trục hoành (y=0) in đậm
    fprintf(gnuplotpipe, "set arrow from -10,0 to 10,0 nohead linewidth 3 lc rgb 'black'\n");

    // Vẽ đồ thị
    fprintf(gnuplotpipe, "plot '%s' with lines title 'f(x)'\n", filename);
    pclose(gnuplotpipe);
    printf("Da ve do thi ham so tu file %s\n", filename);
}


int main() {
    // Định nghĩa hệ số đa thức và bậc
    double coeffs[] = {2, -3, 1}; // x^2 - 3x + 2
    int degree = sizeof(coeffs) / sizeof(coeffs[0]) - 1;
    double x1 = -5.0; // Giới hạn tìm kiếm
    double x2 = 5.0; // Giới hạn tìm kiếm
    double step = 0.1; // Bước nhảy

    // Ghi dữ liệu vào file
    write_data_to_file("data.txt", x1, x2, step, coeffs, degree);

    // Vẽ đồ thị hàm số
    plot_graph("data.txt");
    return 0;
}