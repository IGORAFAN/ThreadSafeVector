#include <mutex>
#include <vector>
#include <iostream>
#include <thread>
template <class T>
class ThrSafeVector
{
public:
	void PushBack(T var)
	{
		std::unique_lock<std::mutex> mutex(m_mutex);
		m_vec.push_back(var);
	}
	int operator[](size_t index)
	{
		std::unique_lock<std::mutex> mutex(m_mutex);
		return m_vec[index];
	}

private:
	std::vector<T> m_vec;
	std::mutex m_mutex;
};
void func(ThrSafeVector<int> &vec)
{
	for (size_t iter = 20; iter < 30; ++iter)
	{
		vec.PushBack(iter);
	}
}
int main()
{
	ThrSafeVector<int> vector;
	std::thread thr1(func, std::ref(vector));
	std::thread thr2(func, std::ref(vector));
	for (size_t iter = 0; iter < 10; ++iter)
	{
		vector.PushBack(iter);
	}
	thr1.join();
	thr2.join();
	for (size_t iter = 0; iter < 30; ++iter)
	{
		std::cout << vector[iter] << std::endl;
	}
	return 0;
}