// compiler MSVC
// Author: Safianova Mariia K25
// Variant: 4
#include <coroutine>
#include <exception>
#include <iostream>
#include <queue>
#include <random>

using namespace std;

class NumberTask
{
public:
    struct promise_type
    {
        NumberTask get_return_object()
        {
            return NumberTask{coroutine_handle<promise_type>::from_promise(*this)};
        }
        suspend_always initial_suspend() 
        { 
            return {}; 
        }
        suspend_always final_suspend() noexcept 
        { 
            return {}; 
        }
        void return_void() {}
        void unhandled_exception() 
        { 
            terminate(); 
        }
    };

    NumberTask(const NumberTask &) = delete;
    NumberTask &operator=(const NumberTask &) = delete;
    ~NumberTask() 
    { 
        coro.destroy();
    }

    void resume() 
    { 
        coro.resume(); 
    }

private:
    NumberTask(coroutine_handle<promise_type> h) : coro(h) {}
    coroutine_handle<promise_type> coro;
};

struct OddAwaiter
{
    int number;

    bool await_ready() 
    { 
        return number % 2 == 0; 
    }
    void await_suspend(coroutine_handle<>) {}
    void await_resume() {}
};

//Coroutine. Generates random numbers from [1, 256] and pushes them into queue
NumberTask generate_numbers(queue<int> &numbers)
{
    mt19937 gen(random_device{}());
    uniform_int_distribution<int> dist(1, 256);

    while (true)
    {
        int number = dist(gen);
        numbers.push(number);
        co_await OddAwaiter{number};
    }
}

int main()
{
    queue<int> numbers;
    NumberTask task = generate_numbers(numbers);

    for(int i = 1; i <= 5; ++i)
    {
        task.resume();

        cout << "Pause " << i << ", queue:";
        while (!numbers.empty())
        {
            cout << " " << numbers.front();
            numbers.pop();
        }
        cout << endl;
    }
}
