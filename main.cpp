#include <stdio.h>
#include <conio.h>  // getch()
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <locale.h>

#define KEY_UP    72   // Mũi tên lên
#define KEY_DOWN  80   // Mũi tên xuống
#define KEY_ENTER 13   // Enter



// Các hàm bổ trợ

// Khởi tạo f(x) - Nguyễn Hữu Kiên - 20237451
double f(double x, double coeffs[], int degree) {
    double result = 0.0;
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * pow(x, i);
    }
    return result;
}

// Khởi tạo f'(x) - Nguyễn Hữu Kiên - 20237451
double df(double x, double coeffs[], int degree) {
    double result = 0.0;
    for (int i = 1; i <= degree; i++) {
        result += i * coeffs[i] * pow(x, i - 1);
    }
    return result;
}

// Khởi tạo f''(x) - Nguyễn Hữu Kiên - 20237451
double d2f(double x, double coeffs[], int degree) {
    double result = 0.0;
    for (int i = 2; i <= degree; i++) {
        result += i * (i - 1) * coeffs[i] * pow(x, i - 2);
    }
    return result;
}

// Tìm hệ số của đạo hàm - Nguyễn Hữu Kiên - 20237451
void f_prime(double coeffs[], double deriv[], int degree) {
    for (int i = 1; i <= degree; i++) {
        deriv[i-1] = coeffs[i] * i;
    }
}

// Kiểm tra tính đơn điệu của hàm số - Nguyễn Hữu Kiên - 20237451
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

// Hàm ghi dữ liệu vào file - Nguyễn Hữu Kiên - 20237451
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



// Các hàm chức năng chính

// Hàm nhập dữ liệu đầu vào - Nguyễn Hữu Kiên - 20237451
void input_data(FILE* file, double coeffs[], int* degree, int* precision) {
    printf("Nhập số bậc của đa thức (n): ");
    scanf("%d", degree);

    // Nếu nhập là chữ thì không cho nhập
    while (*degree < 0) {
        printf("Nhập lại số bậc của đa thức (n): ");
        scanf("%d", degree);
    }

    fprintf(file, "Hệ số đa thức bậc %d:\n", *degree);

    printf("Nhập hệ số (từ bậc 0 đến bậc %d)\n", *degree);
    for (int i = 0; i <= *degree; i++) {
        printf("Hệ số bậc %d: ", i);
        scanf("%lf", &coeffs[i]);
        
        // Nếu nhập là chữ thì không cho nhập
        while (coeffs[i] < -1000000 || coeffs[i] > 1000000) {
            printf("Nhập lại hệ số bậc %d: ", i);
            scanf("%lf", &coeffs[i]);
        }

        fprintf(file, "Hệ số bậc %d: %.2lf\n", i, coeffs[i]);
    }

    printf("Đa thức đã nhập là: ");
    fprintf(file, "Đa thức đã nhập là: ");
    for (int i = *degree; i >= 0; i--) {
        if (i == *degree) {
            printf("%.2lfx^%d", coeffs[i], i);
            fprintf(file, "%.2lfx^%d", coeffs[i], i);
        } else if (coeffs[i] > 0) {
            printf(" + %.2lfx^%d", coeffs[i], i);
            fprintf(file, " + %.2lfx^%d", coeffs[i], i);
        } else if (coeffs[i] < 0) {
            printf(" - %.2lfx^%d", -coeffs[i], i);
            fprintf(file, " - %.2lfx^%d", -coeffs[i], i);
        }
    }
    printf("\n");
    fprintf(file, "\n");

    printf("Nhập số chữ số thập phân sẽ được hiện thị ở kết quả: ");
    scanf("%d", precision);
    fprintf(file, "Số chữ số thập phân sẽ được hiện thị ở kết quả: %d\n", *precision);

    printf("Đã nhập dữ liệu thành công!\n");
    fprintf(file, "Đã nhập dữ liệu thành công!\n");
    fprintf(file, "---------------------------\n");
}

// Hàm tìm miền chứa nghiệm  - Nguyễn Hữu Kiên - 20237451
void find_root_intervals(double coeffs[], int degree, double max_range, double step, int precision, double result[]) {
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
            if (found_interval == 1) { // chỉ lưu miền đầu tiên
                result[0] = x1;
                result[1] = x2;
            }
        }

        x1 = x2;
        x2 += step;
    }

    if(found_interval == 0) {
        printf("Không tìm thấy miền chứa nghiệm trong khoảng [-%.2lf, %.2lf]\n", max_range, max_range);
    }
}

// Hàm tìm khoảng cách ly nghiệm bằng phương pháp chia đôi với độ lệch = 0.5 - Nguyễn Hữu Kiên - 20237451
void find_isolated_root_interval(double coeffs[], int degree, double x1, double x2, int precision, double result[]) {
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

// Hàm vẽ đồ thị hàm số - Nguyễn Hữu Kiên - 20237451
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

// Phương pháp tiếp tuyến với lần lặp cho n trước - Nguyễn Hữu Kiên - 20237451
void find_root_newton_iterations(double coeffs[], int degree, int n, double a, double b, int precision, FILE *file) {
    double x0 = (f(a, coeffs, degree) * d2f(a, coeffs, degree) > 0) ? a : b;
    double x = x0;
    double x_prev;

    // Chuỗi định dạng theo precision, ví dụ "%.6f"
    char format[50];
    sprintf(format, "| %%2d  |     %%%d.%df      |\n", precision + 4, precision);

    printf("---------------------------------\n");
    printf("|  n  |         x_n             |\n");
    printf("---------------------------------\n");

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

    // In kết quả vào file
    fprintf(file, "Nghiệm gần đúng: x = %.*f\n", precision, x);
    fprintf(file, "Sai số theo công thức 1: %.*f\n", precision, error_formula1);
    fprintf(file, "Sai số theo công thức 2: %.*f\n", precision, error_formula2);
}

// Phương pháp tiếp tuyến với sai số epsilon cho trước theo công thức sai số 1 - Nguyễn Hữu Kiên - 20237451
void find_root_newton_epsilon1(double coeffs[], int degree, double a, double b, double epsilon, int precision, FILE *file) {
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

    printf("------------------------------------------------------------\n");
    printf("|  n  |         x_n          |            Sai so            \n");
    printf("------------------------------------------------------------\n");

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
            printf("Dừng thuật toán\n");
            break;
        }
        i++;
    }
    printf("--------------------------------------------------\n");
    // In kết quả 
    printf("Nghiệm gần đúng: x = %.*f\n", precision, x);
    // Ghi kêt quả vào file
    fprintf(file, "Nghiệm gần đúng: x = %.*f\n", precision, x);
}

// Phương pháp tiếp tuyến với sai số epsilon cho trước theo công thức sai số 2 - Nguyễn Hữu Kiên - 20237451
void find_root_newton_epsilon2(double coeffs[], int degree, double a, double b, double epsilon, int precision, FILE *file) {
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
            printf("Dừng thuật toán\n");
            break;
        }
        i++;

    }
    printf("--------------------------------------------------\n");
    // In kết quả 
    printf("Nghiệm gần đúng: x = %.*f\n", precision, x);
    fprintf(file, "Nghiệm gần đúng: x = %.*f\n", precision, x);
}



// Chương trình chính
int main() {
    setlocale(LC_ALL, "");      // Thiết lập ngôn ngữ cho printf

    // Khởi tạo file
    FILE* file = fopen("data.txt", "w");
    if (file == NULL) {
        printf("Khong the mo file data.txt\n");
        return 1;
    }
    printf("===========MENU CHUONG TRINH===========\n");
    // Thiếp lập menu
    int choice = 0, key;
    const char* menu[] = {
        "1. Nhập dữ liệu đa thức",
        "2. Tìm miền chứa nghiệm",
        "3. Tìm khoảng phân ly nghiệm",
        "4. Vẽ đồ thị hàm số với khoảng phân ly nghiệm tìm được",
        "5. Giải gần đúng phương trình f(x)=0 bằng phương pháp tiếp tuyến",
        "6. Thoát",
    };
    int menu_size = sizeof(menu) / sizeof(menu[0]);
    int running = 1;

    // Biến lưu khoảng phân ly nghiệm
    double result0[2];
    double result1[2];
    int has_interval = 0; // Đánh dấu đã có khoảng phân ly nghiệm chưa
    int has_root = 0; // Đánh dấu đã có miền chứa nghiệm chưa
    int has_coeffs = 0; // Đánh dấu đã có hệ số chưa

    // Khởi tạo biến hệ số
    double coeffs[100];
    int degree;
    int precision;



    while (running) {
        // Setup menu mũi tên
        system("cls");
        for (int i = 0; i < menu_size; i++) {
            if (i == choice)
                printf(">  %s  <\n", menu[i]);
            else
                printf("   %s   \n", menu[i]);
        }
        key = getch();
        if (key == 0 || key == 224) {
            key = getch();
            if (key == KEY_UP) {
                choice = (choice - 1 + menu_size) % menu_size;
            } else if (key == KEY_DOWN) {
                choice = (choice + 1) % menu_size;
            }
        } else if (key == KEY_ENTER) {
            system("cls");
            switch (choice) {
                // Chức năng 0: Nhập dữ liệu đa thức
                case 0: {
                    // Nhập dữ liệu ban đầu

                    input_data(file, coeffs, &degree, &precision);
                    has_coeffs = 1;
                    has_root = 0;
                    has_interval = 0;
                    printf("\nNhấn phím bất kỳ để quay lại menu chính...");
                    getch();
                    break;
                }

                // Chức năng 1: Tìm miền chứa nghiệm
                case 1: {
                    char format[50];
                    sprintf(format, "Khoảng chứa nghiệm: [%%.%df, %%.%df]\n", precision, precision);

                    // Kiểm tra xem đã nhập dữ liệu đa thức chưa
                    if(!has_coeffs) {
                        printf("Bạn cần nhập dữ liệu đa thức trước!\n");
                        printf("\nNhấn phím bất kỳ để quay lại menu chính...");
                        getch();
                        break;
                    }
                    
                    // Ghi chức năng sử dụng vào file
                    fprintf(file, "Chức năng 1: Tìm miền chứa nghiệm\n");

                    // Tìm miền chứa nghiệm
                    printf("Nhập khoảng tìm kiếm [-x0, x0] (Chỉ nhập 1 số duy nhất): ");
                    double x0;
                    scanf("%lf", &x0);
                    fprintf(file, "Khoảng tìm kiếm: [-%.*lf, %.*lf]\n", precision, x0, precision ,x0); 

                    // Tìm miền chứa nghiệm tự động
                    find_root_intervals(coeffs, degree, x0, 1, precision, result0);
                    has_root = 1;
                    fprintf(file, "Một miền chứa nghiệm: [%.*lf, %.*lf]\n", precision, result0[0], precision, result0[1]);
                    fprintf(file, "------------------------------------\n"); // Ghi vào file
                    printf("\nNhấn phím bất kỳ để quay lại menu chính...");
                    getch();
                    break;
                }

                // Chức năng 2: Tìm khoảng phân ly nghiệm
                case 2: {
                    // Kiểm tra đã nhập dữ liệu đa thức chưa
                    if(!has_coeffs) {
                        printf("Bạn cần nhập dữ liệu đa thức trước!\n");
                        printf("\nNhấn phím bất kỳ để quay lại menu chính...");
                        getch();
                        break;
                    }

                    // Ghi chức năng sử dụng vào file
                    fprintf(file, "Chức năng 2: Tìm khoảng phân ly nghiệm\n");

                    int sub_choice = 0, select = 1;
                    const char* submenu[] = {
                        "1. Nhập miền chứa nghiệm từ bàn phím",
                        "2. Sử dụng miền chứa nghiệm đã tìm được",
                    };
                    int submenu_size = sizeof(submenu) / sizeof(submenu[0]);
                    double x1, x2;

                    while (select) {
                        system("cls");
                        printf("Chọn một tùy chọn:\n");
                        for (int i = 0; i < submenu_size; i++) {
                            if (i == sub_choice)
                                printf(">  %s  <\n", submenu[i]);
                            else
                                printf("   %s   \n", submenu[i]);
                        }
                        
                        key = getch();
                        if (key == 0 || key == 224) {
                            key = getch();
                            if (key == KEY_UP) {
                                sub_choice = (sub_choice - 1 + submenu_size) % submenu_size;
                            } else if (key == KEY_DOWN) {
                                sub_choice = (sub_choice + 1) % submenu_size;
                            }
                        } else if (key == KEY_ENTER) {
                            
                            system("cls");
                            if (sub_choice == 0) {
                                // Ghi chức năng sử dụng vào file
                                fprintf(file, "Nhập miền chứa nghiệm từ bàn phím\n");

                                printf("Nhập miền chứa nghiệm (x1, x2): ");
                                scanf("%lf %lf", &x1, &x2);
                            } else {
                                if (!has_root) {
                                    printf("Bạn cần tìm miền chứa nghiệm trước!\n");
                                    printf("\nNhấn phím bất kỳ để quay lại menu phụ...");
                                    getch();
                                    break;
                                }
                                x1 = result0[0];
                                x2 = result0[1];

                                printf("Sử dụng miền chứa nghiệm đã tìm được: [%.2lf, %.2lf]\n", x1, x2);
                                fprintf(file, "Sử dụng miền chứa nghiệm đã tìm được: [%.2lf, %.2lf]\n", x1, x2);
                            }
                            find_isolated_root_interval(coeffs, degree, x1, x2, precision, result1);
                            if (result1[0] == -1 && result1[1] == -1) {
                                printf("Không tìm thấy khoảng phân ly nghiệm trong miền [%.*lf, %.*lf]\n", precision, x1, precision, x2);
                            } else {
                                printf("Khoảng phân ly nghiệm: (%.*lf, %.*lf)\n", precision, result1[0], precision, result1[1]);
                                fprintf(file, "Khoảng phân ly nghiệm: (%.*lf, %.*lf)\n", precision, result1[0], precision, result1[1]);
                            }
                            has_interval = 1;
                            printf("\nNhấn phím bất kỳ để quay lại menu phụ...");
                            fprintf(file, "------------------------------------\n"); // Ghi vào file
                            getch();
                            break;
                        }
                    } // end while submenu
                    printf("\nNhấn phím bất kỳ để quay lại menu chính...");
                    getch();
                    break;
                }

                // Chức năng 3: Vẽ đồ thị hàm số
                case 3: {
                    // Kiểm tra đã nhập dữ liệu đa thức chưa
                    if(!has_coeffs) {
                        printf("Bạn cần nhập dữ liệu đa thức trước!\n");
                        printf("\nNhấn phím bất kỳ để quay lại menu chính...");
                        getch();
                        break;
                    }

                    // Ghi chức năng sử dụng vào file
                    fprintf(file, "Chức năng 3: Vẽ đồ thị hàm số\n");

                    // Vẽ đồ thị hàm số với khoảng phân ly nghiệm đã tìm được
                    if (!has_interval) {
                        printf("Bạn cần tìm khoảng phân ly nghiệm trước!\n");
                    } else {
                        printf("Khoảng phân ly nghiệm: (%.*lf, %.*lf)\n", precision, result1[0], precision, result1[1]);

                        // Mở rộng khoảng để vẽ đồ thị đẹp hơn
                        result0[0] -= 10;
                        result0[1] += 10;

                        // Ghi dữ liệu vào file và vẽ đồ thị
                        write_data_to_file("data.txt", result1[0], result1[1], 0.1, coeffs, degree);
                        plot_graph("data.txt");
                        printf("Đã vẽ đồ thị hàm số với khoảng phân ly nghiệm đã tìm được.\n");
                        fprintf(file, "Đã vẽ đồ thị hàm số với khoảng phân ly nghiệm đã tìm được.\n");
                    }
                    printf("\nNhấn phím bất kỳ để quay lại menu chính...");
                    fprintf(file, "------------------------------------\n"); // Ghi vào file
                    getch();
                    break;
                }

                // Chức năng 4: Giải gần đúng phương trình f(x)=0 bằng phương pháp tiếp tuyến
                case 4: {
                    // Kiểm tra đã nhập dữ liệu đa thức chưa
                    if(!has_coeffs) {
                        printf("Bạn cần nhập dữ liệu đa thức trước!\n");
                        printf("\nNhấn phím bất kỳ để quay lại menu chính...");
                        getch();
                        break;
                    }

                    // Ghi chức năng sử dụng vào file
                    fprintf(file, "Chức năng 4: Giải gần đúng phương trình f(x)=0 bằng phương pháp tiếp tuyến\n");
                    int method_choice = 0, sub_choice = 0, select = 1;
                    const char* method_menu[] = {
                        "1. Phương pháp tiếp tuyến với n lần lặp cho trước",
                        "2. Phương pháp tiếp tuyến với sai số cho trước",
                    };
                    const int method_menu_size = sizeof(method_menu) / sizeof(method_menu[0]);

                    const char* submenu[] = {
                        "1. Nhập khoảng phân ly nghiệm từ bàn phím",
                        "2. Sử dụng khoảng phân ly nghiệm đã tìm được",
                    };

                    const int submenu_size = sizeof(submenu) / sizeof(submenu[0]);

                    double a, b;

                    while(select) {
                        system("cls");
                        printf("Chọn một phương pháp giải phương trình:\n");
                        for (int i = 0; i < method_menu_size; i++) {
                            if (i == method_choice)
                                printf(">  %s  <\n", method_menu[i]);
                            else
                                printf("   %s   \n", method_menu[i]);
                        }
                        key = getch();
                        if (key == 0 || key == 224) {
                            key = getch();
                            if (key == KEY_UP) {
                                method_choice = (method_choice - 1 + method_menu_size) % method_menu_size;
                            } else if (key == KEY_DOWN) {
                                method_choice = (method_choice + 1) % method_menu_size;
                            }
                        } else if (key == KEY_ENTER) {
                            system("cls");
                            if (method_choice == 0) {
                                // Ghi chức năng sử dụng vào file
                                fprintf(file, "Phương pháp tiếp tuyến với n lần lặp cho trước\n");
                            } else {
                                // Ghi chức năng sử dụng vào file
                                fprintf(file, "Phương pháp tiếp tuyến với sai số cho trước\n");
                            }
                            while(select) {
                                system("cls");
                                printf("Chọn một cách lấy khoảng phân ly nghiệm:\n");
                                for (int i = 0; i < submenu_size; i++) {
                                    if (i == sub_choice)
                                        printf(">  %s  <\n", submenu[i]);
                                    else
                                        printf("   %s   \n", submenu[i]);
                                }
                                key = getch();
                                if (key == 0 || key == 224) {
                                    key = getch();
                                    if (key == KEY_UP) {
                                        sub_choice = (sub_choice - 1 + submenu_size) % submenu_size;
                                    } else if (key == KEY_DOWN) {
                                        sub_choice = (sub_choice + 1) % submenu_size;
                                    }
                                } else if (key == KEY_ENTER) {
                                    system("cls");
                                    if (sub_choice == 0) {
                                        // Ghi chức năng sử dụng vào file
                                        fprintf(file, "Nhập khoảng phân ly nghiệm từ bàn phím\n");

                                        printf("Nhập khoảng phân ly nghiệm (a b): ");
                                        scanf("%lf %lf", &a, &b);
                                        fprintf(file, "Khoảng phân ly nghiệm: (%.*lf, %.*lf)\n", precision, a, precision, b); // Ghi vào file
                                    } else {
                                        if (!has_interval) {
                                            printf("Bạn cần tìm khoảng phân ly nghiệm trước!\n");
                                            printf("\nNhấn phím bất kỳ để quay lại menu phụ...");
                                            getch();
                                            break;
                                        }
                                        a = result1[0];
                                        b = result1[1];
                                        printf("Sử dụng khoảng phân ly nghiệm đã tìm được: (%.*lf, %.*lf)\n", precision, a, precision, b);
                                        fprintf(file, "Sử dụng khoảng phân ly nghiệm đã tìm được: (%.*lf, %.*lf)\n", precision, a, precision, b); // Ghi vào file
                                    }

                                    // Chọn phương pháp giải
                                    if (method_choice == 0) {
                                        // Phương pháp tiếp tuyến với n lần lặp cho trước
                                        printf("Nhập số lần lặp n: ");
                                        int n;
                                        scanf("%d", &n);
                                        fprintf(file, "Số lần lặp n: %d\n", n); // Ghi vào file

                                        find_root_newton_iterations(coeffs, degree, n, a, b, precision, file);

                                    } else {
                                        // Phương pháp tiếp tuyến với sai số cho trước
                                        printf("Nhập sai số epsilon: ");
                                        double epsilon;
                                        scanf("%lf", &epsilon);
                                        fprintf(file, "Sai số epsilon: %lf\n", epsilon); // Ghi vào file

                                        find_root_newton_epsilon1(coeffs, degree, a, b, epsilon, precision, file);
                                        find_root_newton_epsilon2(coeffs, degree, a, b, epsilon, precision, file);
                                    }
                                    printf("\nNhấn phím bất kỳ để quay lại menu phụ...");
                                    getch();
                                    select = 0; // Thoát khỏi menu phụ
                                    break;
                                }
                            } // end while submenu
                        }
                    } // end while method menu
                    printf("\nNhấn phím bất kỳ để quay lại menu chính...");
                    fprintf(file, "------------------------------------\n"); // Ghi vào file
                    getch();
                    break;
                }
                    
                // Chức năng 5: Thoát
                case 5:
                    // Ghi chức năng sử dụng vào file
                    fprintf(file, "Chức năng 6: Thoát\n");
                    running = 0;
                    break;
            } // end switch
        } // end if key enter
    } // end while
    printf("Đã thoát chương trình.\n");
    fprintf(file, "Đã thoát chương trình.\n");
    fclose(file); // Đóng file
    return 0;
}

// Lưu ý chạy trên cmd để sử dụng được menu mũi tên, cần chạy trước lệnh "chcp 65001" để hỗ trợ tiếng Việt