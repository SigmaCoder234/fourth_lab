#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include "LazySequence.h"
#include "Stream.h"

template <typename T>
T ReadValue() {
    T value;
    while (true) {
        if (std::cin >> value) {
            while (std::cin.get() != '\n' && !std::cin.eof()) {}
            return value;
        } else {
            std::cout << "Invalid input. Try again: ";
            std::cin.clear();
            while (std::cin.get() != '\n' && !std::cin.eof()) {}
        }
    }
}

//tests
void RunTests() {
    std::cout << "\n============= GTEST UNIT TESTS ==========\n";
    std::system("run_tests.exe");
    std::cout << "\n============= ALL TESTS ENDED =============\n";
}

// 1. Правило генерации (Бесконечный счетчик: 0, 1, 2, 3...)
class NaturalNumbersRule : public IGeneratorRule<long long> {
public:
    long long Generate(const Ordinal& index) override {
        return index.GetFinite();
    }
    
    IGeneratorRule<long long>* Clone() const override {
        return new NaturalNumbersRule();
    }
};

// 1.1 Правило генерации Фибоначчи
class FibonacciRule : public IGeneratorRule<long long> {
public:
    long long Generate(const Ordinal& index) override {
        int n = index.GetFinite();
        if (n == 0) return 0;
        if (n == 1) return 1;
        long long a = 0;
        long long b = 1;
        for (int i = 2; i <= n; ++i) {
            long long c = a + b;
            a = b;
            b = c;
        }
        return b;
    }
    IGeneratorRule<long long>* Clone() const override {
        return new FibonacciRule();
    }
};

// 1.2 Правило Арифметической прогрессии
class ArithmeticProgressionRule : public IGeneratorRule<long long> {
private:
    long long start;
    long long step;
public:
    ArithmeticProgressionRule(long long s, long long st) : start(s), step(st) {}
    long long Generate(const Ordinal& index) override {
        return start + step * index.GetFinite();
    }
    IGeneratorRule<long long>* Clone() const override {
        return new ArithmeticProgressionRule(start, step);
    }
};

// 1.3 Правило ручной конечной последовательности
class ManualSequenceRule : public IGeneratorRule<long long> {
private:
    DynamicArray<long long> data;
public:
    ManualSequenceRule(const DynamicArray<long long>& arr) : data(arr) {}
    long long Generate(const Ordinal& index) override {
        return data.Get(index.GetFinite());
    }
    IGeneratorRule<long long>* Clone() const override {
        return new ManualSequenceRule(data);
    }
};

// 2. Функция Map: Умножаем на 10
class MultiplyByTen : public IMapFunction<long long, long long> {
public:
    long long Map(long long item) override {
        return item * 10;
    }
};

// 3. Функция Where: Оставляем только те числа, которые больше 15
class GreaterThan15 : public IWhereFunction<long long> {
public:
    bool Check(const long long& item) override {
        return item > 15;
    }
};

// 4. Функция Reduce: Вычисляет сумму
class SumReducer : public IReduceFunction<long long> {
public:
    long long Combine(const long long& acc, const long long& item) override {
        return acc + item;
    }
};

// Функция для красивого вывода первых N элементов последовательности
template <class T>
void PrintSequence(LazySequence<T>* seq, int count, const std::string& name) {
    std::cout << "   " << name << " (first " << count << " elements): [";
    for (int i = 0; i < count; ++i) {
        std::cout << seq->Get(i);
        if (i < count - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

void PrintMenu() {
    std::cout << "\n===== LAZY SEQUENCE MENU =====\n";
    std::cout << "1. Map (x10)\n";
    std::cout << "2. Where (> 15)\n";
    std::cout << "3. GetSubsequence (saves to Subsequence)\n";
    std::cout << "4. Concat (with a new created sequence)\n";
    std::cout << "5. Insert (a new created sequence)\n";
    std::cout << "6. Print first N elements\n";
    std::cout << "7. Get element by Ordinal\n";
    std::cout << "8. Reduce (Sum first N elements)\n";
    std::cout << "9. Reset Base Sequence\n";
    std::cout << "10. Print Subsequence\n";
    std::cout << "0. Exit\n";
    std::cout << "Choice: ";
}

LazySequence<long long>* CreateSequence() {
    while (true) {
        std::cout << "\nChoose base sequence:\n";
        std::cout << "1. Natural Numbers (0, 1, 2...)\n";
        std::cout << "2. Fibonacci Numbers (0, 1, 1, 2, 3...)\n";
        std::cout << "3. Arithmetic Progression\n";
        std::cout << "4. Manual Finite Sequence (enter elements)\n";
        std::cout << "5. Run all tests\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";
        int choice = ReadValue<int>();

        switch (choice) {
            case 1: {
                return new LazySequence<long long>(new NaturalNumbersRule());
            }

            case 2: {
                return new LazySequence<long long>(new FibonacciRule());
            }

            case 3: {
                long long start, step;
                std::cout << "Enter start: "; 
                start = ReadValue<long long>();

                std::cout << "Enter step: "; 
                step = ReadValue<long long>();

                return new LazySequence<long long>(new ArithmeticProgressionRule(start, step));
            }

            case 4: {
                int n;
                std::cout << "Enter number of elements: "; 
                n = ReadValue<int>();
                DynamicArray<long long> arr(n);
                std::cout << "Enter " << n << " elements:\n";
                for (int i = 0; i < n; ++i) {
                    long long val = ReadValue<long long>();
                    arr.Set(i, val);
                }
                return new LazySequence<long long>(new ManualSequenceRule(arr), Ordinal(0, n));
            }

            case 5: {
                RunTests();
                break; // Выходим из switch и идём на новую итерацию while
            }

            case 0: {
                std::cout << "Exiting...\n";
                exit(0);
            }
            
            default: {
                std::cout << "Wrong choice\n";
            }
        }
    }
}

int main() {
    LazySequence<long long>* currentSeq = CreateSequence();
    LazySequence<long long>* subSeq = nullptr;
    
    MultiplyByTen mapper;
    GreaterThan15 filter;
    SumReducer reducer;

    int choice = -1;
    while (choice != 0) {
        PrintMenu();
        choice = ReadValue<int>();

        switch (choice) {
            case 1: {
                LazySequence<long long>* nextSeq = currentSeq->Map(&mapper);
                currentSeq = nextSeq;
                std::cout << "Mapped (* 10)\n";
                break;
            }
            case 2: {
                LazySequence<long long>* nextSeq = currentSeq->Where(&filter);
                currentSeq = nextSeq;
                std::cout << "Filtered (> 15)\n";
                break;
            }
            case 3: {
                int start, end;
                std::cout << "Start index: "; start = ReadValue<int>();
                std::cout << "End index: "; end = ReadValue<int>();
                try {
                    LazySequence<long long>* nextSeq = currentSeq->GetSubsequence(start, end);
                    if (subSeq) delete subSeq;
                    subSeq = nextSeq;
                    std::cout << "Subsequence created and saved to Subsequence variable!\n";
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 4: {
                auto* otherSeq = CreateSequence();
                LazySequence<long long>* nextSeq = currentSeq->Concat(otherSeq);
                currentSeq = nextSeq;
                std::cout << "Concatenated with new Sequence\n";
                break;
            }
            case 5: {
                int omega, finite;
                std::cout << "Position Omega: "; omega = ReadValue<int>();
                std::cout << "Position Finite: "; finite = ReadValue<int>();
                auto* otherSeq = CreateSequence();
                try {
                    LazySequence<long long>* nextSeq = currentSeq->Insert(otherSeq, Ordinal(omega, finite));
                    currentSeq = nextSeq;
                    std::cout << "Inserted new Sequence at w*" << omega << " + " << finite << "\n";
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 6: {
                Ordinal len = currentSeq->GetLength();
                int omegas = len.GetOmega();
                int parts = omegas + (len.GetFinite() > 0 && omegas > 0 ? 1 : 0);
                if (omegas == 0) parts = 1;
                
                if (parts > 1) {
                    std::cout << "The sequence spans multiple concatenated parts (" << parts << " parts).\n";
                    for (int p = 0; p < parts; ++p) {
                        int count;
                        std::cout << "How many elements to print from sequence part " << (p+1) << " (w*" << p << ")? ";
                        count = ReadValue<int>();
                        if (count > 0) {
                            std::cout << "   Part " << (p+1) << " (first " << count << " elements): [";
                            for (int i = 0; i < count; ++i) {
                                try {
                                    std::cout << currentSeq->Get(Ordinal(p, i));
                                    if (i < count - 1) std::cout << ", ";
                                } catch (const std::exception&) {
                                    std::cout << " <End of part> ";
                                    break;
                                }
                            }
                            std::cout << "]\n";
                        }
                    }
                } else {
                    int count;
                    std::cout << "How many elements to print? "; count = ReadValue<int>();
                    try {
                        std::cout << "   CurrentSeq (first " << count << " elements): [";
                        for (int i = 0; i < count; ++i) {
                            try {
                                std::cout << currentSeq->Get(Ordinal(0, i));
                                if (i < count - 1) std::cout << ", ";
                            } catch (const std::exception&) {
                                std::cout << " <End of part>";
                                break;
                            }
                        }
                        std::cout << "]\n";
                    } catch (const std::exception& e) {
                        std::cout << "Error: " << e.what() << "\n";
                    }
                }
                break;
            }
            case 7: {
                int omega, finite;
                std::cout << "Index Omega: "; omega = ReadValue<int>();
                std::cout << "Index Finite: "; finite = ReadValue<int>();
                try {
                    long long val = currentSeq->Get(Ordinal(omega, finite));
                    std::cout << "Value at w*" << omega << " + " << finite << " = " << val << "\n";
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 8: {
                int count;
                std::cout << "How many elements to reduce? "; count = ReadValue<int>();
                try {
                    long long sum = currentSeq->Reduce(&reducer, 0, count);
                    std::cout << "Sum of first " << count << " elements = " << sum << "\n";
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 9: {
                currentSeq = CreateSequence();
                std::cout << "Reset to new Base Sequence.\n";
                break;
            }
            case 10: {
                if (!subSeq) {
                    std::cout << "Subsequence is not created yet! Use option 3 first.\n";
                } else {
                    int count;
                    std::cout << "How many elements to print from subsequence? "; count = ReadValue<int>();
                    try {
                        PrintSequence(subSeq, count, "Subsequence");
                    } catch (const std::exception& e) {
                        std::cout << "Error: " << e.what() << "\n";
                    }
                }
                break;
            }

            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice.\n";
        }
    }

    if (currentSeq) delete currentSeq;
    if (subSeq) delete subSeq;
    return 0;
}