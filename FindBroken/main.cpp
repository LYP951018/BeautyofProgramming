#include <range\v3\all.hpp>
#include <iostream>
#include <array>
#include <utility>
#include <cstdint>

using namespace ranges;

using u32 = std::uint32_t;

constexpr auto MaxInput = std::size_t{10};
constexpr std::array<u32, MaxInput + 2> OriginalID =
{
    1,2,3,4,5,6,1,2,3,4,5,6
};

//»±…Ÿ¡© ID

class Solver
{
public:
    Solver(const std::array<u32, MaxInput>& input)
        :input_{input}
    {}

    std::pair<u32, u32> Solve()
    {
        u32 result{};
        for (auto i : input_)
            result ^= i;
        if (result == u32{})
        {
            //A == B
            auto totalSum = accumulate(OriginalID, u32{});
            auto partialSum = accumulate(input_, u32{});
            auto res = (totalSum - partialSum) >> 1;
            return{res,res};
        }
        else
        {
            auto rightMostBit = result & -result;
            auto l = partition(input_, [rightMostBit](auto i) {
                return (i & rightMostBit) == u32{};
            });
            u32 result1 = {}, result2 = {};
            for_each(input_.begin(), l, [&result1](auto i) {
                result1 ^= i;
            });
            for_each(l, input_.end(), [&result2](auto i) {
                result2 ^= i;
            });
            return {result1,result2};
        }
    }

private:
    std::array<u32, MaxInput> input_;
};

int main()
{
    std::array<u32, MaxInput> input = {3,4,5,6,1,2,3,4,5,6};
    Solver solver{input};
    auto res = solver.Solve();
    std::cout << res.first << " " << res.second << "\n";
    std::getchar();
}