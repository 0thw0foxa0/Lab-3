#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;
using namespace std::chrono;

// Обычный двоичный поиск
int binarySearch(const vector<int>& arr, int val) {
    int a = 0, b = arr.size() - 1;
    while (a <= b) {
        int m = a + (b - a) / 2;
        if (arr[m] == val) return m;
        if (arr[m] < val) a = m + 1;
        else b = m - 1;
    }
    return -1;
}

// Интерполяционный поиск
int interpolationSearch(const vector<int>& arr, int val) {
    int a = 0, b = arr.size() - 1;
    while (a <= b && val >= arr[a] && val <= arr[b]) {
        if (a == b) {
            if (arr[a] == val) return a;
            return -1;
        }
        // Формула интерполяции: m = a + (b - a) * (val - arr[a]) / (arr[b] - arr[a])
        int m = a + static_cast<long long>(b - a) * (val - arr[a]) / (arr[b] - arr[a]);
        if (m < a || m > b) return -1; // Проверка границ
        if (arr[m] == val) return m;
        if (arr[m] < val) a = m + 1;
        else b = m - 1;
    }
    return -1;
}

// Генерация равномерно возрастающих данных
vector<int> generateUniformData(int n, int step) {
    vector<int> data(n);
    data[0] = 1;
    for (int i = 1; i < n; ++i) {
        data[i] = data[i - 1] + step;
    }
    return data;
}

// Генерация данных вида a1, a2, ..., b, где b >> ai
vector<int> generateSkewedData(int n, int largeValue) {
    vector<int> data(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100);
    for (int i = 0; i < n - 1; ++i) {
        data[i] = dis(gen);
    }
    sort(data.begin(), data.end() - 1); // Сортируем все, кроме последнего
    data[n - 1] = largeValue;
    return data;
}

// Тестирование производительности
void testPerformance(const vector<int>& data, int searches) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(data.front(), data.back());

    // Тестирование интерполяционного поиска
    auto start = high_resolution_clock::now();
    for (int i = 0; i < searches; ++i) {
        int val = dis(gen);
        interpolationSearch(data, val);
    }
    auto end = high_resolution_clock::now();
    auto interpolationTime = duration_cast<microseconds>(end - start).count();

    // Тестирование двоичного поиска
    start = high_resolution_clock::now();
    for (int i = 0; i < searches; ++i) {
        int val = dis(gen);
        binarySearch(data, val);
    }
    end = high_resolution_clock::now();
    auto binaryTime = duration_cast<microseconds>(end - start).count();

    cout << "Interpolation Search: " << interpolationTime << " microseconds\n";
    cout << "Binary Search: " << binaryTime << " microseconds\n";
}

int main() {
    const int n = 1000000; // Размер массива
    const int searches = 10000; // Количество поисков для теста

    // Тест 1: Равномерно возрастающие данные
    cout << "Testing on uniformly increasing data:\n";
    auto uniformData = generateUniformData(n, 2);
    testPerformance(uniformData, searches);

    // Тест 2: Данные с большим последним элементом
    cout << "\nTesting on skewed data (large last element):\n";
    auto skewedData = generateSkewedData(n, 1000000000);
    testPerformance(skewedData, searches);

    return 0;
}
