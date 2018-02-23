#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>


template <class RandomAccessIterator, class GetValue>
void CountSort(RandomAccessIterator first, RandomAccessIterator last, 
               int bits, int shift, GetValue getValue) { 
    std::vector<int> counts (static_cast<size_t> (pow(2, bits)));
    std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type> 
               newArray (last - first);
    for (auto now = first; now != last; ++now) {
        ++counts[((getValue(*now) >> shift) & (static_cast<int64_t> (pow(2, bits) - 1)))];
    }
    counts.back() = (last - first) - counts.back();
    for (int i = counts.size() - 2; i >= 0; --i) {
        counts[i] = counts[i+1] - counts[i];
    }
    for (auto now = first; now != last; ++now) {
        size_t index = (getValue(*now) >> shift) & (static_cast<int64_t> (pow(2, bits) - 1));
        newArray[counts[index]] = *now;
        ++counts[index];
    }
    for (size_t index = 0; index < static_cast<size_t> (last - first); ++index) {
        *(first + index) = newArray[index];
    }
    return;
}

template <class RandomAccessIterator, class GetValue>
size_t NumCountSorts(RandomAccessIterator first, RandomAccessIterator last,
                     GetValue getValue, int bits) {
    int64_t maxValue = 0;
    for (auto now = first; now != last; ++now) {
        int64_t value =  static_cast<int64_t> (getValue(*now));
        if (maxValue < value) {
            maxValue = value;
        }
    }
    size_t length = 1;
    int64_t maxAllow = 1;
    while (maxAllow < maxValue) {
        ++length;
        maxAllow = (maxAllow + 1) * 2 - 1;
    }
    return (length + bits - 1) / bits;
}

template <class RandomAccessIterator, class GetValue>
void IntegerSort(RandomAccessIterator first, RandomAccessIterator last, GetValue getValue) {
    if (first == last) 
        return;
    for (size_t numSort = 0; numSort < NumCountSorts(first, last, getValue, 8);
    ++numSort) {
        CountSort(first, last, 8, numSort * 8, getValue);
    }
    return;
}
