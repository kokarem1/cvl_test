#include <iostream>

#include <cstdlib>
#include <cmath>
#include <utility>
#include <list>
#include <algorithm>


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

//  Получить индекс в массиве по двум координатам и ширине (размерность x)
inline unsigned int in(int x, int y, int width) {
    return x + y*width;
}

template <typename T>
void debug_print(T *image, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int num = image[in(j, i, N)];
            std::cout << num << ((num / 10) >= 1? " ": "  ");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
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

    //  Будем вращать вокруг центра
    double centerCoordinate = (N - 1) / 2.0;

    //  Для упрощения будем считать, что мы всегда компилируем компилятором,
    //  у которого unsigned int - это 4 байта, вне зависимости от платформы.
    unsigned int *image = new unsigned int[size];

    //  Заполняем массив
    //  Заполним значениями от 1 до size
    for (int i = 0; i < size; ++i)
        image[i] = i + 1;

    std::cout << "Изображение " << N <<" на " << N << " до поворота:" << std::endl << std::endl;
    debug_print(image, N);

    //  Координаты по нумерации до swap'а
    int oldX = 0;
    int oldY = 0;
    //  Координаты точки, из которой будет происходить swap
    int srcX = 0;
    int srcY = 0;
    //  Помеченные индексы. Нужны для того, чтобы не вертеть одни и те же точки повторно
    std::list<unsigned int> markedIndexes;
    //  Помеченные точки, которые уже были повёрнуты. Нужны, чтобы не вертеть одни и те же точки повторно
    std::list< std::pair<int, int> > markedRotations;

    for(unsigned int i = 0; i < size; ++i) {

        if (std::find(markedIndexes.cbegin(), markedIndexes.cend(), i) != markedIndexes.cend())

            continue;

        //  Если индекс не помечен, то берём его в обработку
        else {

            //  Получаем из индекса координаты
            srcX = i % N;
            srcY = i / N;
            oldX = srcX;
            oldY = srcY;

        }

        while (true) {
            std::pair<int, int> rotatedPoint = rotate_point_from_axis<int>(oldX, oldY, centerCoordinate, centerCoordinate, 1.57);

            //  Условие выхода из цикла. Если уже было такое верчение точки,
            //  значит из точки (srcX, srcY) уже вертеть не получится
            //  и все точки пройденные через этот цикл стоят на своих местах.
            //  Тогда надо проверять следующий индекс в image (во внешнем цикле for)
            if (std::find(markedRotations.cbegin(), markedRotations.cend(), rotatedPoint) != markedRotations.cend()) {
                markedRotations.clear();
                break;
            } else

                //  Помечаем точку
                markedRotations.push_back(rotatedPoint);


            unsigned int destIndex = in(rotatedPoint.first, rotatedPoint.second, N);
            //  Ставим точку на место
            std::swap(image[in(srcX, srcY, N)], image[destIndex]);

            //  Помечаем индекс
            markedIndexes.push_back(destIndex);

            //  Готовим следующую итерацию. Будем точку, которую вытеснила первая, ставить на место
            oldX = rotatedPoint.first;
            oldY = rotatedPoint.second;

//            debug_print(image, N);
        }
    }

    std::cout << std::endl << "Изображение " << N <<" на " << N << " после поворота на 90 градусов:" << std::endl <<std::endl;
    debug_print(image, N);

    delete[] image;

    return 0;
}
