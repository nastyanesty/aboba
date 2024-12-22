#include <iostream>
#include <vector>
#include <stack> 
#include <locale.h>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cctype>
using namespace std;

struct Pair {
    int index;
    int size;
};

int getMinRun(int n) {
    int r = 0;
    while (n >= 64) {
        r |= (n & 1);
        n >>= 1;
    }
    return n + r;
}

void InsertionSort(vector<int>& arr, int l, int r) {
    for (int j = l + 1; j < r + 1; j++) {
        int value = arr[j];
        int i = j - 1;
        while (i >= l && arr[i] > value) {
            arr[i + 1] = arr[i];
            arr[i] = value;
            i--;
        }
    }
}

void MergeSort(vector <int>& arr, int left, int mid, int right) {

    int left_size = mid - left + 1;
    int right_size = right - mid;

    vector <int> left_arr(left_size);
    vector <int> right_arr(right_size);
    for (int i = mid + 1, j = 0; i <= right; i++, j++) {
        right_arr[j] = arr[i];
    }
    for (int i = left, j = 0; i <= mid; i++, j++) {
        left_arr[j] = arr[i];
    }

    int left_pointer = 0;
    int right_pointer = 0;
    int arr_pointer = left;
    int left_counter = 0;
    int right_counter = 0;

    while (left_pointer < left_arr.size() && right_pointer < right_arr.size())
    {
        if (left_arr[left_pointer] <= right_arr[right_pointer])
        {
            arr[arr_pointer++] = left_arr[left_pointer++];
            left_counter++;
            right_counter = 0;
        }
        else
        {
            arr[arr_pointer++] = right_arr[right_pointer++];
            right_counter++;
            left_counter = 0;
        }
        if (left_counter == 7) {
            while (right_pointer < right_arr.size() && left_pointer < left_arr.size()) {
                int target = right_arr[right_pointer];
                int low = left_pointer;
                int high = left_arr.size();

                while (low < high) {
                    int mid = low + (high - low) / 2;
                    if (left_arr[mid] < target) {
                        low = mid + 1;
                    }
                    else {
                        high = mid;
                    }
                }

                while (left_pointer < low) {
                    arr[arr_pointer++] = left_arr[left_pointer++];
                }
                arr[arr_pointer++] = target;
                right_pointer++;
            }
        }
        else if (right_counter == 7) {
            while (left_pointer < left_arr.size() && right_pointer < right_arr.size()) {
                int target = left_arr[left_pointer];
                int low = right_pointer;
                int high = right_arr.size();

                while (low < high) {
                    int mid = low + (high - low) / 2;
                    if (right_arr[mid] < target) {
                        low = mid + 1;
                    }
                    else {
                        high = mid;
                    }
                }

                while (right_pointer < low) {
                    arr[arr_pointer++] = right_arr[right_pointer++];
                }
                arr[arr_pointer++] = target;
                left_pointer++;
            }
        }

    }
    while (left_pointer < left_size) {
        arr[arr_pointer++] = left_arr[left_pointer++];
    }

    while (right_pointer < right_size) {
        arr[arr_pointer++] = right_arr[right_pointer++];
    }
}

void FindRun(vector<int>& arr) {
    int n = arr.size();
    int min_run = getMinRun(n);
    int start = 0;
    stack <Pair> run_stack;
    while (start < n) {
        int end = start;
        while (end + 1 < n && arr[end] <= arr[end + 1]) {
            end++;
        }
        if (end + 1 < n && arr[end] > arr[end + 1]) {
            while (end + 1 < n && arr[end] > arr[end + 1]) {
                end++;
            }
            int left = start;
            int right = end;
            while (left < right) {
                int temp = arr[left];
                arr[left] = arr[right];
                arr[right] = temp;
                left++;
                right--;
            }
        }
        if (end - start + 1 < min_run) {
            if (start + min_run - 1 < n) {
                end = start + min_run - 1;
            }
            else {
                end = n - 1;
            }
        }
        InsertionSort(arr, start, end);
        run_stack.push({ start, end - start + 1 });
        start = end + 1;
    }
    while (run_stack.size() > 1) {
        Pair run1 = run_stack.top();
        run_stack.pop();
        Pair run2 = run_stack.top();
        run_stack.pop();

        if (run_stack.empty()) {
            MergeSort(arr, run2.index, run2.index + run2.size - 1, run1.index + run1.size - 1);
            run_stack.push({ run2.index, run2.size + run1.size });
        }
        else {
            Pair run3 = run_stack.top();
            run_stack.pop();

            int X = run1.size;
            int Y = run2.size;
            int Z = run3.size;

            if (Z > Y + X && Y > X) {
                run_stack.push(run3);
                run_stack.push(run2);
                run_stack.push(run1);
                break;
            }

            if (X <= Z) {
                MergeSort(arr, run3.index, run3.index + run3.size - 1, run2.index + run2.size - 1);
                run_stack.push({ run3.index, run3.size + run2.size });
                run_stack.push(run1);
            }
            else {
                MergeSort(arr, run2.index, run2.index + run2.size - 1, run1.index + run1.size - 1);
                run_stack.push({ run2.index, run2.size + run1.size });
                run_stack.push(run3);
            }
        }
    }
}

void TimSort(vector <int>& arr) {
    if (arr.size() > 63) {
        FindRun(arr);
    }
    else {
        InsertionSort(arr, 0, arr.size() - 1);
    }
}

bool isSorted(vector<int>& arr) {
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    return true;
}

int getValidLength() {
    string input;
    int length = 0;
    while (true) {
        system("cls");
        cout << "Введите длину массива: ";
        getline(cin, input);

        bool valid = true;

        for (char c : input) {
            if (!isdigit(c)) {
                valid = false;
                break;
            }
        }

        if (valid) {
            length = stoi(input);
            if (length > 1000000000) {
                cout << "Слишком большая величина" << endl;
            }
            else {
                return length;
            }
        }
        cout << "Ошибка: введите только цифры от 0 до 9.\n";
        system("pause");
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    vector<int> arr;
    int len;
    len = getValidLength();
    srand(time(0));
    for (int i = 0; i < len; i++)
    {
        arr.push_back(rand() % 1000);
    }
    cout << "Исходный массив: ";
    for (int i = 0; i < len; i++)
        cout << arr[i] << " ";

    TimSort(arr);

    cout << endl << "Отсортированный массив: ";
    for (int i = 0; i < len; i++)
        cout << arr[i] << " ";

    if (isSorted(arr)) {
        cout << endl << "Массив отсортирован верно";
    }
    if (!isSorted(arr)) {
        cout << "Массив отсортирован не верно";
    }
    cout << endl;
    return 0;
}