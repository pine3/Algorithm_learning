#include <iostream>
#include <vector>

template<class Type>
class Binary_heap {
public:
    Binary_heap() {
        size = 0;
        max_size = 1;
        elements = std::vector<Type> (max_size);
    }

    void print() {
        for (int i = 0; i < size; i++) {
            std::cout << elements[i] << " ";
        }
        std::cout << std::endl;
    }

    void Include(Type new_element) {
        if (size == max_size) {
            max_size *= 2;
            elements.resize(max_size);
        }
        elements[size] = new_element;
        size++;
        Shift_up(size - 1);
        return;
    }

    void Remove_by_index(int deleted_element) {
        std::swap(elements[deleted_element], elements[size - 1]);
        --size;
        Shift_down(Shift_up(deleted_element));
        return;
    }

    void Remove_head() {
        Remove_by_index(0);
        return;
    }

    Type Get_min() {
        return elements[0];
    }
        
private:
    int Parent(int in) {
        return (in - 1) / 2;
    }

    int SunLeft(int in) {
        return 2 * in + 1;
    }

    int SunRight(int in) {
        return 2 * in + 2;
    }

    int Shift_up(int in) {
        while ((in != 0) && (elements[in] < elements[Parent(in)])) {
            std::swap(elements[in], elements[Parent(in)]);
            in = Parent(in);
        }
        return in;
    }

    int Shift_down(int in) {
        while ((2 * in + 2 < size) && 
        ((elements[SunLeft(in)] < elements[in]) || (elements[SunRight(in)] < elements[in]))) {
            if (elements[SunLeft(in)] < elements[SunRight(in)]) {
                std::swap(elements[SunLeft(in)], elements[in]);
                in = SunLeft(in);
            } else {
                std::swap(elements[SunRight(in)], elements[in]);
                in = SunRight(in);
            }
        }
        if ((2 * in + 1 < size) && (elements[SunLeft(in)] < elements[in])) {
                std::swap(elements[SunLeft(in)], elements[in]);
                in = SunLeft(in);
        }
        return in;
    }
    
    std::vector<Type> elements;
    size_t size;
    size_t max_size;
};
