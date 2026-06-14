#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <numeric>
#include <string>
#include <string_view>

void print_map(const std::unordered_map<std::string, int>& m, std::string_view label = "") {
    if (!label.empty()) std::cout << label << ":\n";
    for (const auto& [k, v] : m) std::cout << "  " << k << "=" << v << "\n";
}

void print_set(const std::unordered_set<int>& s, std::string_view label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (int n : s) std::cout << n << " ";
    std::cout << "\n";
}

int main() {
    std::cout << "=== unordered_map<string,int> ===\n";
    std::unordered_map<std::string, int> counts;
    counts["apple"]  += 2;
    counts["banana"] += 1;
    print_map(counts, "Initial");

    std::cout << std::boolalpha
              << "contains(apple)="  << counts.contains("apple")  << "\n"
              << "contains(cherry)=" << counts.contains("cherry") << "\n";

    if (auto it = counts.find("banana"); it != counts.end())
        std::cout << "banana (via iterator)=" << it->second << "\n";

    std::cout << "Keys: ";
    for (const auto& [k, _] : counts) std::cout << k << " ";
    std::cout << "\n";

    counts.insert({"orange", 1});
    print_map(counts, "After insert(orange)");

    counts.emplace("grape", 4);
    print_map(counts, "After emplace(grape,4)");

    const int total = std::accumulate(counts.begin(), counts.end(), 0,
        [](int acc, const auto& kv){ return acc + kv.second; });
    std::cout << "total=" << total << "\n";

    std::cout << "bucket_count=" << counts.bucket_count()
              << " load_factor=" << counts.load_factor() << "\n";

    counts.rehash(20);
    std::cout << "bucket_count after rehash=" << counts.bucket_count() << "\n";

    std::cout << "\n=== unordered_set<int> ===\n";
    std::unordered_set<int> nums{1, 2, 3, 2, 1};
    std::cout << "Unique count=" << nums.size() << "\n"
              << "contains(2)=" << nums.contains(2) << "\n";

    nums.insert(10);
    print_set(nums, "After insert(10)");

    nums.erase(1);
    std::cout << "size after erase(1)=" << nums.size() << "\n";

    std::unordered_set<int> other{50, 60};
    nums.swap(other);
    print_set(nums, "After swap");
    print_set(other, "other after swap");

    if (auto it = nums.find(50); it != nums.end())
        std::cout << "found 50 in nums\n";

    std::cout << "\n=== vector → unordered_set ===\n";
    const std::vector<int> raw{7, 7, 8, 9, 9};
    const std::unordered_set<int> from_vec{raw.begin(), raw.end()};
    std::cout << "converted size=" << from_vec.size() << "\n";

    std::unordered_set<int> temp = nums;
    temp.clear();
    std::cout << "temp after clear: size=" << temp.size() << " empty=" << temp.empty() << "\n";

    return 0;
}
