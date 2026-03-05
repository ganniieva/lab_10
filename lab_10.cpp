#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <stdexcept>
#include <cctype>

template <class T>
class MyPipeline {
private:
    struct Step {
        std::string name;
        std::function<T(T)> func;
    };
    
    std::vector<Step> steps;

public:
    MyPipeline() = default;

    size_t size() const { return steps.size(); }
    bool empty() const { return steps.empty(); }
    
    void clear() {
        steps.clear();
    }

    template <class F>
    void addStep(const std::string& name, F func) {
        if (name.empty())
            throw std::invalid_argument("Пустое имя");

        Step newStep;
        newStep.name = name;
        newStep.func = std::function<T(T)>(func);
        steps.push_back(newStep);
    }

    void removeStep(size_t index) {
        if (index >= steps.size())
            throw std::out_of_range("Индекс вне диапазона");
        steps.erase(steps.begin() + index);
    }

    T run(T value) const {
        for (const auto& step : steps)
            value = step.func(value);
        return value;
    }

    std::vector<T> trace(T value) const {
        std::vector<T> res;
        for (const auto& step : steps) {
            value = step.func(value);
            res.push_back(value);
        }
        return res;
    }

    friend std::ostream& operator<<(std::ostream& os, const MyPipeline<T>& p) {
        os << "шагов: " << p.steps.size() << "\n";
        for (size_t i = 0; i < p.steps.size(); ++i)
            os << i << ") " << p.steps[i].name << "\n";
        return os;
    }
};

int main() {
    // 6.1. int
    std::cout << "1. MyPipeline<int>\n";
    MyPipeline<int> ip;
    ip.addStep("Умножить на 5", [](int x) { return x * 5; });
    ip.addStep("Прибавить 1", [](int x) { return x + 1; });
    ip.addStep("Возвести в квадрат", [](int x) { return x * x; });
    
    std::cout << ip;
    int start = 5;
    std::cout << "Run(" << start << ") = " << ip.run(start) << "\n";
    
    std::vector<int> tr = ip.trace(start);
    std::cout << "Trace: ";
    for (int v : tr) std::cout << v << " ";
    std::cout << "\n\n";

    // 6.2. string
    std::cout << "2. MyPipeline<string>\n";
    MyPipeline<std::string> sp;
    sp.addStep("Добавить ->", [](std::string s) { return "->" + s; });
    sp.addStep("Добавить <-", [](std::string s) { return s + "<-"; });
    sp.addStep("Верхний регистр", [](std::string s) {
        for (char& c : s) 
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        return s;
    });
    
    std::string str = "hello";
    std::cout << "run(\"" << str << "\") = \"" << sp.run(str) << "\"\n\n";

    // 6.3. исключение
    std::cout << "3. Исключение\n";
    MyPipeline<double> dp;
    
    try {
        std::cout << "Пытаемся удалить шаг с индексом 100...\n";
        dp.removeStep(100);
    }
    catch (const std::out_of_range& e) {
        std::cout << "Поймано исключение: " << e.what() << "\n";
    }
    
    return 0;
}