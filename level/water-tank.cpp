#include <iostream>
#include <thread>
#include <fstream>
#include <unistd.h>
#include <chrono>
#include <vector>
#include <algorithm>
#include <functional>


using namespace std;

int get_gpio_value(char* path)
{
    ifstream ifs(path);
    std::string line;
    if (ifs)
    {
        getline(ifs, line);
        // cout << line << endl;
    }
    return line == "1";
}

void set_gpio_value(const char* path, const char* value)
{
    ofstream ofs(path);
    std::string line;
    if (ofs)
    {
        ofs << value;
    }
}



class Timer
{
public:
    Timer() :
        m_start(m_clock::now())
    {
    }

    void reset()
    {
        m_start = m_clock::now();
    }

    double elapsed() const
    {
        return std::chrono::duration_cast<microseconds> (m_clock::now() - m_start).count();
    }

private:
    typedef std::chrono::steady_clock m_clock;
    typedef std::chrono::duration<int, std::micro> microseconds;
    std::chrono::time_point<m_clock> m_start;
};


char* trigger_file= "/gpio/pin25/value";
char* pulse_file = "/gpio/pin24/value";
std::chrono::microseconds timespan(10);
std::chrono::milliseconds wait(200);
std::vector<int> waits;
class Sampler
{
public:
    Sampler()
    {
    }

    int sample()
    {
        set_gpio_value(trigger_file, "1");
        std::this_thread::sleep_for(timespan);
        set_gpio_value(trigger_file, "0");
        Timer t1;
        int waited_zero = 0;
        int waited_one = 0;

        while (get_gpio_value(pulse_file) == 0)
        {
            ++waited_zero;
            t1.reset();
        }

        while (get_gpio_value(pulse_file) == 1)
        {
            // wait here
            ++waited_one;
        }

        // printf(" %d:%d\n", waited_zero, waited_one);
        waits.push_back(waited_one);
        return t1.elapsed();
    }
private:
};

int main(int argc, char** argv)
{
    Sampler sampler;
    int max_samples = 5;
    int num_samples = 0;
    std::vector<int> samples;
    while (num_samples < max_samples)
    {
        auto elapsed = sampler.sample();
        samples.push_back(elapsed);
        cout << elapsed << ":" <<  elapsed / 58 << endl;
        ++num_samples;
        std::this_thread::sleep_for(wait);
    }

    std::nth_element(samples.begin(), samples.begin() + samples.size() / 2, samples.end());

    for (auto& s : samples)
    {
        cout << s << ":" <<  s / 58 << endl;
    }
    std::cout << "median:" << samples[samples.size() / 2] / 58 << std::endl;

    return 0;
}
