#include <catch2/catch.hpp>
#include <debugger_trace.hpp>
#include <vector>
#include <span>
#include <numeric>
#include <random>

class CalculatorImpl
{
public:
    float FindMax(std::span<float> vals)
    {
        float max_val = std::numeric_limits<float>::min();
        for (auto val : vals)
        {
            if (val > max_val)
            {
                max_val = val;
            }
        }
        return max_val;
    }

};
class Calculator1
{
public:
    Calculator1()
        : _impl(std::make_unique<CalculatorImpl>())
    {

    }

    float FindMax(std::span<float> vals)
    {
        return _impl->FindMax(vals);
    }
private:
    std::unique_ptr<CalculatorImpl> _impl;
};

class ICalculator
{
public:
    virtual ~ICalculator() {};

    virtual float FindMax(std::span<float> vals) = 0;
};
class Calculator2 : public ICalculator
{
    float FindMax(std::span<float> vals) override
    {
        float max_val = std::numeric_limits<float>::min();
        for (auto val : vals)
        {
            if (val > max_val)
            {
                max_val = val;
            }
        }
        return max_val;
    }
};

TEST_CASE("pimpl vs virtual", "[raii]")
{
    std::random_device rnd_device;

    std::mt19937 mersenne_engine{ rnd_device() };  // Generates random integers
    std::uniform_real_distribution<float> dist{ -9214325.0f, 9214325.0f };

    auto gen = [&dist, &mersenne_engine]() {
        return dist(mersenne_engine);
    };

    std::vector<float> vals(1024 * 1024);
    std::generate(std::begin(vals), std::end(vals), gen);

    std::unique_ptr<ICalculator> virtualCalc = std::make_unique<Calculator2>();

    Calculator1 pimplCalc;
    CalculatorImpl nonCalc;

    float maxVal = 0.0f;
    BENCHMARK("pimpl")
    {
        maxVal = pimplCalc.FindMax(vals);
    };


    DEBUGGER_TRACE("max val = {}", maxVal);

    BENCHMARK("virtual")
    {
        maxVal = virtualCalc->FindMax(vals);
    };

    DEBUGGER_TRACE("max val = {}", maxVal);

    BENCHMARK("non")
    {
        maxVal = nonCalc.FindMax(vals);
    };

    DEBUGGER_TRACE("max val = {}", maxVal);
}