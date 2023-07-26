#include <iostream>
#include <omp.h>

template <class T>
int partition(int p, int r, T *data)
{
    T pivot = data[r];
    int i = p - 1;

    for (int j = p; j < r; j++)
    {
        if (data[j] <= pivot)
        {
            i++;
            std::swap(data[i], data[j]);
        }
    }

    std::swap(data[i + 1], data[r]);
    return i + 1;
}

template <class T>
void q_sort_tasks(int p, int r, T *data, int low_limit)
{
    if (p < r)
    {
        if (r - p < low_limit)
        {
            // Small list => sequential.
            // Do sequential sorting here or call seq_qsort function.
            // ...
        }
        else
        {
            int q = partition(p, r, data);

#pragma omp task shared(data)
            q_sort_tasks(p, q - 1, data, low_limit);

#pragma omp task shared(data)
            q_sort_tasks(q + 1, r, data, low_limit);
        }
    }
}

int main()
{
    // Example array for sorting
    int array[] = {9, 5, 7, 1, 8, 3, 6, 4};

    int size = sizeof(array) / sizeof(array[0]);

// Call q_sort_tasks to sort the array in parallel
#pragma omp parallel
    {
#pragma omp single
        q_sort_tasks(0, size - 1, array, 4);
    }

    // Output the sorted array
    std::cout << "Sorted array: ";
    for (int i = 0; i < size; i++)
    {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
