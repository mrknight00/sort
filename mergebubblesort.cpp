#include <iostream>
#include <omp.h>

using namespace std;

void sequentialBubbleSort(int *, int);
void parallelBubbleSort(int *, int);
void swap(int &, int &);

void sequentialBubbleSort(int *a, int n)
{
    int swapped;
    for (int i = 0; i < n; i++)
    {
        swapped = 0;
        for (int j = 0; j < n - 1; j++)
        {
            if (a[j] > a[j + 1])
            {
                swap(a[j], a[j + 1]);
                swapped = 1;
            }
        }

        if (!swapped)
            break;
    }
}

void parallelBubbleSort(int *a, int n)
{
    int swapped;
    for (int i = 0; i < n; i++)
    {
        swapped = 0;
        int first = i % 2;
        #pragma omp parallel for shared(a, first)
        for (int j = first; j < n - 1; j++)
        {
            if (a[j] > a[j + 1])
            {
                swap(a[j], a[j + 1]);
                swapped = 1;
            }
        }

        if (!swapped)
            break;
    }
}

void swap(int &a, int &b)
{
    int test;
    test = a;
    a = b;
    b = test;
}

void mergesort(int a[], int i, int j);
void merge(int a[], int i1, int j1, int i2, int j2);

void mergesort(int a[], int i, int j)
{
    if (i < j)
    {
        int mid = (i + j) / 2;

        {

            {
                mergesort(a, i, mid);
            }

            {
                mergesort(a, mid + 1, j);
            }
        }
        merge(a, i, mid, mid + 1, j);
    }
}

void paramergesort(int a[], int i, int j)
{
    if (i < j)
    {
        int mid = (i + j) / 2;
#pragma omp parallel sections
        {
#pragma omp section
            {
                mergesort(a, i, mid);
            }
#pragma omp section
            {
                mergesort(a, mid + 1, j);
            }
        }
        merge(a, i, mid, mid + 1, j);
    }
}


void merge(int a[], int i1, int j1, int i2, int j2)
{
    int temp[100000];
    int i, j, k;
    i = i1;
    j = i2;
    k = 0;

    while (i <= j1 && j <= j2)
    {
        if (a[i] < a[j])
        {
            temp[k++] = a[i++];
        }
        else
        {
            temp[k++] = a[j++];
        }
    }

    while (i <= j1)
    {
        temp[k++] = a[i++];
    }

    while (j <= j2)
    {
        temp[k++] = a[j++];
    }

    for (i = i1, j = 0; i <= j2; i++, j++)
    {
        a[i] = temp[j];
    }
}

int main()
{
    int *a, n;
    cout << "\nEnter total number of elements: ";
    cin >> n;
    a = new int[n];

//     cout << "\n Enter elements=>";
//    for (int i = 0; i < n; i++)
//    {
//        cin >> a[i];
//    }

    // Generate random input
    // srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < n; i++)
    {
        a[i] = rand() % 100; // Generate random numbers between 0 and 99
    }
    for (int i = 0; i < n; ++i){
      cout<<a[i]<<" ";
    }
    cout<<endl;


    // Perform sequential bubble sort
    double start_time = omp_get_wtime();
    sequentialBubbleSort(a, n);
    double end_time = omp_get_wtime();
    cout << "\nSorted array after sequential bubble sort:\n";
    for (int i = 0; i < n; i++)
    {
        cout << a[i] << " ";
    }
    cout << "\nTime taken by sequential bubble sort: " << end_time - start_time << " seconds" << endl;

    // Perform parallel bubble sort
    start_time = omp_get_wtime();
    parallelBubbleSort(a, n);
    end_time = omp_get_wtime();
    cout << "\nSorted array after parallel bubble sort:\n";
    for (int i = 0; i < n; i++)
    {
        cout << a[i] << " ";
    }
    cout << "\nTime taken by parallel bubble sort: " << end_time - start_time << " seconds" << endl;

    // Merge Sort
    cout << "\nEnter total number of elements for merge sort: ";
    cin >> n;
    a = new int[n];
    for (int i = 0; i < n; i++)
    {
        a[i] = rand() % 100; // Generate random numbers between 0 and 99
    }

    start_time = omp_get_wtime(); // start timer for sequential merge sort
    mergesort(a, 0, n - 1);
    end_time = omp_get_wtime(); // end timer for sequential merge sort

    cout << "\nSorted array after sequential merge sort:\n";
    for (int i = 0; i < n; i++)
    {
        cout << a[i] << " ";
    }
    cout << "\nTime taken by sequential merge sort: " << end_time - start_time << " seconds" << endl;

    start_time = omp_get_wtime(); // start timer for parallel merge sort
    paramergesort(a, 0, n - 1);
        
    end_time = omp_get_wtime(); // end timer for parallel merge sort

    cout << "\nSorted array after parallel merge sort:\n";
    for (int i = 0; i < n; i++)
    {
        cout << a[i] << " ";
    }
    cout << "\nTime taken by parallel merge sort: " << end_time - start_time << " seconds" << endl;

    delete[] a; // Don't forget to free the allocated memory

    return 0;
}