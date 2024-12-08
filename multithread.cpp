#include <iostream>
#include <vector>
#include <thread>
#include <numeric> // Для функции accumulate
#include <mutex>   // Для класса mutex
#include <chrono>  // Для работы с временем

using namespace std;

// Глобальная переменная для синхронизации вывода
mutex output_mutex;

// Функция для вычисления суммы части массива
double partial_sum(const vector<double> &arr, size_t start, size_t end) {
    return accumulate(arr.begin() + start, arr.begin() + end, 0.0);
}

// Функция потока для вычисления суммы и вывода результатов
void thread_function(const vector<double> &arr, size_t start, size_t end, double &result) {
    result = partial_sum(arr, start, end); // Вычисление частичной суммы

    // Синхронизация вывода
    {
        lock_guard<mutex> lock(output_mutex); // Защита вывода от одновременного доступа
        cout << "Thread ID: " << this_thread::get_id() 
             << ", Partial sum from " << start << " to " << end << ": " << result << endl;
    }
}

int main() {
    // Запоминаем время начала выполнения
    auto start = chrono::high_resolution_clock::now();

    const size_t SIZE = 10'000'000; // Размер массива
    vector<double> arr(SIZE, 1.0);  // Инициализация массива значениями 1.0

    int mode;
    cout << "Choose mode (1 - single thread, 2 - multi-threaded): ";
    cin >> mode;

    if (mode == 1) {
        // Однопоточная работа
        double total_sum = partial_sum(arr, 0, SIZE);
        cout << "Total sum: " << total_sum << endl;
    } else if (mode == 2) {
        // Многопоточная работа
        size_t num_threads;
        cout << "Enter the number of threads: ";
        cin >> num_threads;

        // Проверка на корректность ввода
        if (num_threads <= 0) {
            cerr << "Number of threads must be positive." << endl;
            return 1;
        }

        vector<thread> threads;           // Вектор для хранения потоков
        vector<double> results(num_threads); // Вектор для хранения результатов от каждого потока
        size_t chunk_size = SIZE / num_threads; // Размер блока для каждого потока

        // Запуск потоков
        for (size_t i = 0; i < num_threads; ++i) {
            size_t start = i * chunk_size; // Начало блока
            size_t end = (i == num_threads - 1) ? SIZE : start + chunk_size; // Конец блока, последний поток берет остаток
            threads.emplace_back(thread_function, cref(arr), start, end, ref(results[i])); // Запуск потока
        }

        // Ожидание завершения всех потоков
        for (auto &thread : threads) {
            thread.join();
        }

        // Суммируем результаты
        double total_sum = accumulate(results.begin(), results.end(), 0.0);
        cout << "Total sum: " << total_sum << endl;
    } else {
        cerr << "Invalid mode selected." << endl;
        return 1;
    }

    // Запоминаем время окончания выполнения
    auto end = chrono::high_resolution_clock::now();

    // Вычисляем продолжительность
    chrono::duration<double> duration = end - start;

    // Выводим результат
    cout << "Execution time: " << duration.count() << " seconds" << endl;

    return 0; // Завершение программы
}
