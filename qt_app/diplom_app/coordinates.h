#ifndef COORDINATES_H
#define COORDINATES_H

struct coordinates
{
    double x;
    double y;
    double z;

    // Перегрузка оператора сложения
    coordinates operator+(const coordinates& other) const
    {
        coordinates result;
        result.x = this->x + other.x;
        result.y = this->y + other.y;
        result.z = this->z + other.z;
        return result;
    }

    // Перегрузка оператора вычитания
    coordinates operator-(const coordinates& other) const
    {
        coordinates result;
        result.x = this->x - other.x;
        result.y = this->y - other.y;
        result.z = this->z - other.z;
        return result;
    }

    // Перегрузка оператора сложения с присваиванием
    coordinates& operator+=(const coordinates& other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }

    // Перегрузка оператора вычитания с присваиванием
    coordinates& operator-=(const coordinates& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }

    // Перегрузка оператора деления
    coordinates operator/(double divisor)
    {
        if (divisor != 0.0)
        {
        coordinates result;
        result.x = this->x / divisor;
        result.y = this->y / divisor;
        result.z = this->z / divisor;
        return result;
        }
        return {0.0, 0.0, 0.0};
    }
};
#endif // COORDINATES_H
