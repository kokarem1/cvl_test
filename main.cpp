#include <iostream>

#include <cstdlib>
#include <cmath>
#include <utility>

/*  Поворот точки вокруг оси на указанный угол
 *    x, y - точка, которую будем поворачивать
 *    x0, y0 - точка оси вращения
 *    angle - угол в радианах, на который поворачиваем
 */
template <typename T = double>
std::pair<T, T> rotate_point_from_axis(double x, double y, double x0, double y0, double angle);

//  Результат в double'ах
template <>
std::pair<double, double> rotate_point_from_axis<double>(double x, double y, double x0, double y0, double angle) {
    double rotatedX = x0 + (x - x0) * cos(angle) - (y - y0) * sin(angle);
    double rotatedY = y0 + (y - y0) * cos(angle) + (x - x0) * sin(angle);

    return std::make_pair(rotatedX, rotatedY);
}

//  Результат в int'ах, округляем round'ом
template<>
std::pair<int, int> rotate_point_from_axis<int>(double x, double y, double x0, double y0, double angle) {
    double rotatedX = x0 + (x - x0) * cos(angle) - (y - y0) * sin(angle);
    double rotatedY = y0 + (y - y0) * cos(angle) + (x - x0) * sin(angle);

    return std::make_pair(round(rotatedX), round(rotatedY));
}

int main(int argc, char *argv[])
{
    int N = 5;

    if (argc == 2)
        N = atoi(argv[1]);

    //  Корректно только, когда дали целое число (аргумент) или вообще не дали аргументов.
    //  Если не дали, тогда останется значение присвоенное при инициализации
    if ((argc != 2 || N < 1) && argc != 1) {
        std::cout << "Incorrect format." << std::endl;
        return 0;
    }

    unsigned int size = N * N;

    double centerCoordinate = (N - 1) / 2.0;

    double *image = new double[size];

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::pair<double, double> point = rotate_point_from_axis<int>(j, i, centerCoordinate, centerCoordinate, 1.57);
            std::cout << "(" << point.first << " " << point.second << ")" << "  ";

            image[i + j*N] = i + j*N + 1;

        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    for (int i = 0; i < size; ++i)
        std::cout << image[i] << " ";
    std::cout << std::endl;

    delete[] image;

    return 0;
}
