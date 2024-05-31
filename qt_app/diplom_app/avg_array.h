#ifndef AVG_ARRAY_H
#define AVG_ARRAY_H

#include "coordinates.h"

#define AVG_ARR_SIZE 200     // Размер массива для усреднения
#define PRED_ARR_SIZE 100     // Размер массива для предсказания


class AveragingArray
{
private:
    coordinates avg_array[AVG_ARR_SIZE]{};
    coordinates pred_array[PRED_ARR_SIZE]{};
    int cur_avg = 0;                // итератор усредняющего массива
    int cur_pred = 0;               // итератор предсказания направления
    coordinates avg_summ = {0.0, 0.0, 0.0};
    coordinates avg = {0.0, 0.0, 0.0};
    coordinates pred_sum = {0.0, 0.0, 0.0};
    coordinates pred = {0.0, 0.0, 0.0};

public:
    AveragingArray()
    {
    }

    void AddPoint(coordinates point)
    {
        if ( ((point.x < 50.0) && (point.y < 50.0) && (point.z < 50.0))
            && ((point.x > 0.03) && (point.y > 0.03) && (point.z > 0.03)) )
        {
            avg_summ -= avg_array[cur_avg];
            avg_summ += point;
            avg_array[cur_avg] = point;
            cur_avg = (cur_avg >= (AVG_ARR_SIZE - 1) ? 0 : cur_avg + 1);
        }

    }

    coordinates GetAvg()
    {
        avg = avg_summ / AVG_ARR_SIZE;
        return avg;
    }

    void AddPredictVector(coordinates point)
    {
        pred_sum -= pred_array[cur_pred];
        pred_array[cur_pred] = point - avg_array[(cur_avg - 1)];
        pred_sum += pred_array[cur_pred];
        cur_pred = (cur_pred >= (PRED_ARR_SIZE - 1) ? 0 : cur_pred + 1);
    }

    coordinates GetPredictVector()
    {
        pred = pred_sum / PRED_ARR_SIZE;
        return pred;
    }
};

#endif // AVG_ARRAY_H
