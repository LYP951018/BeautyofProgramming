#define RANGES_SUPPRESS_IOTA_WARNING

#include <set>
#include <cstdint>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <functional>
#include <cmath>
#include <range\v3\all.hpp>

using namespace ranges;

using u32 = std::uint32_t;
using namespace std::string_literals;

class PointsSolver
{
public:
    PointsSolver(const std::array<u32,4>& input)
        :input_{input}
    {
        for (auto i : view::iota(0, 4))
            solutions_[(1u << i) - 1].push_back({std::to_string(input_[i]), static_cast<double>(input_[i])});
    }

    struct Item
    {
        std::string Expression_;
        double Result_;

        bool operator < (const Item& other) const noexcept
        {
            return Result_ < other.Result_;
        }
    };

    auto Solve() -> std::vector<Item>
    {
        std::vector<Item> result;
        for (auto i : view::closed_ints(u32(1), u32(15)))
            f(i);
        for (const auto& item : solutions_.back())
            if (std::abs(item.Result_ - 24.0) <= 1e-6) result.push_back(item);
        return result;
    }

private:
    std::array<std::vector<Item>, 15> solutions_;

    auto Fork(const std::vector<Item>& lhs, const std::vector<Item>& rhs) -> std::set<Item>
    {
        std::set<Item> result;
        for(const auto& lItem : lhs)
            for (const auto& rItem : rhs)
            {
                result.insert(
                {
                    {"("s + lItem.Expression_ + ")" + "+" + "(" + rItem.Expression_ + ")", lItem.Result_ + rItem.Result_},
                    {"("s + lItem.Expression_ + ")" + "-" + "(" + rItem.Expression_ + ")", lItem.Result_ - rItem.Result_},
                    {"("s + rItem.Expression_ + ")" + "-" + "(" + lItem.Expression_ + ")", rItem.Result_ - lItem.Result_},
                    {"("s + lItem.Expression_ + ")" + "*" + "(" + rItem.Expression_ + ")", lItem.Result_ * rItem.Result_}
                });

                if (std::abs(rItem.Result_ - 0.0) > 1e-6)
                    result.insert({"("s + lItem.Expression_ + ")" + "/" + "(" + rItem.Expression_ + ")", lItem.Result_ / rItem.Result_});
                if (std::abs(lItem.Result_ - 0.0) > 1e-6)
                    result.insert({"("s + rItem.Expression_ + ")" + "/" + "(" + lItem.Expression_ + ")", rItem.Result_ / lItem.Result_});
            }
        return result;
    }

    auto f(u32 i) ->const std::vector<Item>&
    {
        auto& solution = solutions_[i - 1];
        if (solution.size() != 0)
            return solution;
        for(auto x : view::iota(u32(1), i))
            if((x & i) == x)
            {
                auto set = Fork(f(x), f(i - x));
                solution.insert(solution.end(),set.begin(), set.end());
            }
        return solution;
    }
  
    std::array<u32, 4> input_;
};


int main()
{
    std::array<u32, 4> input;
    for (auto& i : input)
        std::cin >> i;
    PointsSolver solver{input};
    auto answer = solver.Solve();
    for (const auto& ans : answer)
        std::cout << ans.Expression_ << "\n";
    std::getchar();
    std::getchar();
    return 0;
}