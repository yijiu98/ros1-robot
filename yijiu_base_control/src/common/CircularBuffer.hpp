#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>

template<typename T>
class CircularBuffer {
private:
    std::vector<T> buffer;   // 存储任意类型数据的环形缓冲区
    size_t head = 0;         // 缓冲区的头部（数据读取位置）
    size_t tail = 0;         // 缓冲区的尾部（数据写入位置）
    size_t capacity;         // 缓冲区的最大容量
    bool full = false;       // 标志位，指示缓冲区是否已满

    std::mutex mtx;          // 互斥锁，用于线程安全
    std::condition_variable not_full;  // 条件变量，控制缓冲区非满
    std::condition_variable not_empty; // 条件变量，控制缓冲区非空

public:
    explicit CircularBuffer(size_t size) : capacity(size), buffer(size) {}

    void put(const T& item) {
        std::unique_lock<std::mutex> lock(mtx);
        not_full.wait(lock, [this]() { return !full; });

        buffer[tail] = item;
        tail = (tail + 1) % capacity;
        full = tail == head;

        not_empty.notify_one();
    }

    T get() {
        std::unique_lock<std::mutex> lock(mtx);
        not_empty.wait(lock, [this]() { return head != tail || full; });

        T item = buffer[head];
        full = false;
        head = (head + 1) % capacity;

        not_full.notify_one();
        return item;
    }
};

// template<typename T>
// void producer(CircularBuffer<std::vector<T>>& buffer) {
//     while (true) {
//         const std::vector<T> texts = {'a', 'b', 'c'};
//         buffer.put(texts);
//         std::this_thread::sleep_for(std::chrono::seconds(2));
//         const std::vector<T> texts2 =  {'a', 'b', 'c','d'};
//         buffer.put(texts2);
//         std::this_thread::sleep_for(std::chrono::seconds(2));
//     }
// }

// template<typename T>
// void consumer(CircularBuffer<std::vector<T>>& buffer) {
//     while (true) {
//         std::vector<T> block = buffer.get();
//         std::cout << "Consumed: ";
//         for (T num : block) {
//             std::cout << num << " ";
//         }
//         std::cout << std::endl;
//     }
// }

// int main() {
//     CircularBuffer<std::vector<char>> buffer(10);
//     std::thread producer_thread(producer<char>, std::ref(buffer));
//     std::thread consumer_thread(consumer<char>, std::ref(buffer));

//     producer_thread.join();
//     consumer_thread.join();

//     return 0;
// }
