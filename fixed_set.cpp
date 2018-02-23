#include <iostream>
#include <vector>
#include <algorithm>
#include <random>


class HashFunction {
public:
    int64_t GetValue(int key) const;
    static HashFunction MakeHashFunction(std::mt19937& generator);
private:
    static int64_t BIG_PRIME;
    int64_t linear_coeff_;
    int64_t free_coeff_;
};

class Bucket {
public:
    void Initialize(const std::vector<int>& elements, std::mt19937& generator);
    bool Contains(int value_to_found) const;
private:
    const int DEFAULT_VALUE = 1000000001;
    bool IsHashWithoutCollisions(const std::vector<int>& elements, HashFunction hash_function);
    std::vector<int> buckets_;
    HashFunction hash_function_;
};

class FixedSet {
public:
    void Initialize(const std::vector<int>& elements);
    bool Contains(int value_to_found) const;
private:
    const int COUNT_BUCKETS = 4;
    void ChooseHash(const std::vector<int>& elements, std::mt19937& generator);
    std::vector<Bucket> hash_table_;
    HashFunction hash_function_;
};

std::vector<bool> FindElements(const FixedSet& fixed_set, 
                               const std::vector<int>& value_to_found);

int64_t HashFunction::BIG_PRIME = 2000000009;

void FixedSet::Initialize(const std::vector<int>& elements) {
    std::mt19937 generator = std::mt19937 (3449);
    ChooseHash(elements, generator);
    std::vector<std::vector<int>> buckets (elements.size(), std::vector<int> ());
    for (auto element : elements) {
        buckets[hash_function_.GetValue(element) % buckets.size()].push_back(element);
    }
    hash_table_ = std::vector<Bucket> (elements.size());
    for (size_t i = 0; i < elements.size(); ++i) {
        hash_table_[i].Initialize(buckets[i], generator);
    }
    return;
}

void FixedSet::ChooseHash(const std::vector<int>& elements, std::mt19937& generator) {
    size_t sum_squares;
    do {
        hash_function_ = HashFunction::MakeHashFunction(generator);
        std::vector<int> buckets_size (elements.size());
        for (auto element : elements) {
            ++buckets_size[hash_function_.GetValue(element) % buckets_size.size()];
        }
        sum_squares = 0;
        for (auto size : buckets_size) {
            sum_squares += size * size;
        }
    } while (sum_squares > COUNT_BUCKETS * elements.size());
    return;
}

void Bucket::Initialize(const std::vector<int>& elements, std::mt19937& generator) {
    do {
        hash_function_ = HashFunction::MakeHashFunction(generator);
    } while (!IsHashWithoutCollisions(elements, hash_function_));
    buckets_ = std::vector<int> (elements.size() * elements.size(), DEFAULT_VALUE);
    for (auto element : elements) {
        buckets_[hash_function_.GetValue(element) % buckets_.size()] = element; 
    }
    return;
}

bool Bucket::IsHashWithoutCollisions(const std::vector<int>& elements, 
                                     HashFunction hash_function) {
    std::vector<bool> cells = std::vector<bool> (elements.size() * elements.size());
    for (auto element : elements) {
        if (cells[hash_function.GetValue(element) % cells.size()] == 0) {
            cells[hash_function.GetValue(element) % cells.size()] = 1;
        } else {
            return false;
        }
    }
    return true;
}

bool FixedSet::Contains(int value_to_found) const {
    if (hash_table_.empty()) {
        return 0;
    }
    return hash_table_[hash_function_.GetValue(value_to_found) 
                       % hash_table_.size()].Contains(value_to_found);
}

bool Bucket::Contains(int value_to_found) const {
    if (buckets_.empty()) {
        return 0;
    }
    return value_to_found == buckets_[hash_function_.GetValue(value_to_found) % buckets_.size()];
}

HashFunction HashFunction::MakeHashFunction(std::mt19937& generator) {
    HashFunction hash_function;
    std::uniform_int_distribution<int64_t> linear_value(1, HashFunction::BIG_PRIME - 1);
    hash_function.linear_coeff_ = linear_value(generator);
    std::uniform_int_distribution<int64_t> free_value(0, HashFunction::BIG_PRIME - 1);
    hash_function.free_coeff_ = free_value(generator);
    return hash_function;
}

int64_t HashFunction::GetValue(int key) const {
    int64_t mod = (static_cast<int64_t>(key) * linear_coeff_ + free_coeff_) % BIG_PRIME;
    return (mod >= 0 ? mod : mod + BIG_PRIME);
}

std::vector<bool> FindElements(const FixedSet& fixed_set, 
                                const std::vector<int>& value_to_found) {
    std::vector<bool> result (value_to_found.size());
    for (size_t i = 0; i < value_to_found.size(); ++i) {
        result[i] = fixed_set.Contains(value_to_found[i]);
    }
    return result;
}
