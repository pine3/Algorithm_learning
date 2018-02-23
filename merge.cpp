#include <iostream>
#include <vector>
#include <algorithm>

int64_t Merge (std::vector<int64_t>* sum, size_t first, size_t second, size_t third,
int64_t left, int64_t right) {
    int64_t res = 0;
    size_t lt = first;
    int cnt = 0;
    for (size_t rt = second; (rt < third) && (lt < second); ++rt) {
            while ((lt < second) && ((*sum)[rt] - (*sum)[lt] > right)) {
                ++lt;
                --cnt;
            }
            cnt = std::max(0, cnt);
            while ((lt + cnt < second) && ((*sum)[rt] - (*sum)[lt + cnt] >= left)) {
                ++cnt;
            }
            res += cnt;
    }
    lt = first;
    size_t now = 0;
    std::vector<int64_t> newSum (third - first);
    for (size_t rt = second; rt < third; ++rt) {
        while ((lt < second) && (*sum)[lt] < (*sum)[rt]) {
            newSum[now] = (*sum)[lt];
            ++now;
            ++lt;
        }
        newSum[now] = (*sum)[rt];
        ++now;
    }
    while (lt < second) {
        newSum[now] = (*sum)[lt];
        ++now;
        ++lt;  
    }
    for (size_t i = 0; i < third - first; ++i) {
        (*sum)[i + first] = newSum[i];
    }
    return res;
}

int64_t FindCounrsSubsegment (std::vector<int64_t>* sum, int64_t left, int64_t right) {
    int64_t res = 0;
    for (size_t step = 1; step < sum->size(); step *= 2) {
        for (size_t second = step; second < sum->size(); second += 2 * step) {
            res += Merge (sum, second - step, second, std::min(second + step, sum->size())
                , left, right);
        }
    }
    return res;
}

int main() {
    size_t count;
    std::cin >> count;
    int input;
    std::vector<int64_t> sum(count + 1);
    for (size_t i = 1; i <= count; ++i) {
        std::cin >> input;
        sum[i] = sum[i-1] + input;
    }
    int64_t left, right;
    std::cin >> left >> right;
    std::cout <<  FindCounrsSubsegment (&sum, left, right); 
    std::cout << "\n";
    return 0;
}
