#include <cstdio>
#include <Windows.h>
#include <iostream>

struct structure {
    int *arr;
    int size = 0;
    int min = 0;
    int max = 0;
    double average = 0;
};

extern int errno;

void replace(structure *a) {
    for (int i = 0; i < a->size; i++) {
        if (a->arr[i] == a->max || a->arr[i] == a->min) {
            a->arr[i] = a->average;
        }
    }
}

DWORD WINAPI minAndMax(LPVOID param) {
    structure *a = (structure *) param;
    int min = a->arr[0];
    int max = min;
    for (int i = 1; i < a->size; i++) {
        if (min > a->arr[i]) {
            min = a->arr[i];
            Sleep(7);
        }
        if (max < a->arr[i]) {
            max = a->arr[i];
            Sleep(7);
        }
    }
    a->max = max;
    a->min = min;
    std::cout << "The maximum value in the array: ";
    std::cout <<max;
    std::cout <<std::endl;
    std::cout << "The minimum value in the array: ";
    std::cout <<min;
    std::cout <<std::endl;
    return 0;
}

DWORD WINAPI average(LPVOID param) {
    structure *a = (structure *) param;
    int sum = a->arr[0];
    for (int i = 1; i < a->size; i++) {
        sum += a->arr[i];
        Sleep(12);
    }
    a->average = sum / a->size;
    std::cout << "The average value in the array: ";
    printf("%.2f", a->average);
    std::cout <<std::endl;
    return 0;
}

void print(structure *a) {
    for (int i = 0; i < a->size - 1; i++) {
        std::cout << a->arr[i] << " ";

    }
    std::cout << a->arr[a->size - 1];
}

int main(void) {
    int size;
    std::cout << "Enter the dimentions of an array:";
    std::cin >> size;
    std::cout << "Enter the elements of an array:";
    if (size <= 0) {
        errno = EDOM;
        perror("Error:\t");
        return EDOM;
    }
    int *arr = (int *) malloc(sizeof(double) * size);
    if (arr == NULL) {
        errno = ENOMEM;
        perror("Error:\t");
        return ENOMEM;
    }
    for (int i = 0; i < size; i++) {
        scanf_s("%d", &arr[i]);
    }
    structure *resArr = (structure *) malloc(sizeof(structure));
    resArr->arr = arr;
    resArr->size = size;
    HANDLE hThread_minAndMax;
    DWORD IDThread_minAndMax;
    hThread_minAndMax = CreateThread(NULL, 0, minAndMax, (void *) resArr, 0, &IDThread_minAndMax);
    HANDLE hThread_average;
    DWORD IDThread_average;
    hThread_average = CreateThread(NULL, 0, average, (void *) resArr, 0, &IDThread_average);
    if (hThread_minAndMax == NULL || hThread_average == NULL) {
        return GetLastError();
    }
    WaitForSingleObject(hThread_minAndMax, INFINITE);
    WaitForSingleObject(hThread_average, INFINITE);
    CloseHandle(hThread_minAndMax);
    CloseHandle(hThread_average);
    replace(resArr);
    print(resArr);
    return 0;
}