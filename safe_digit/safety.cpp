#include <stdio.h>
#include <assert.h>
#include <limits>
#include <iostream>

struct unsigned_tag {};

struct signed_tag {};


template <bool T>
struct checked_traits {
    typedef signed_tag tag;
};

template <>
struct checked_traits<false> {
    typedef unsigned_tag tag;
};

template <typename T>
T check_add(T left, T right, signed_tag) {
    assert(!(((right > 0) && (std::numeric_limits<T>::max() - right < left)) || ((right < 0) && (std::numeric_limits<T>::min() - right > left))));
    return left + right;
}

template <typename T>
T check_add(T left, T right, unsigned_tag) {
    assert(!((right > 0) && (std::numeric_limits<T>::max() - right < left)));
    return left + right;
}

template <typename T>
T check_sub(T left, T right, signed_tag) {
    assert(!(((right > 0) && (left < std::numeric_limits<T>::min() + right)) || ((right < 0) && (left > std::numeric_limits<T>::max() + right))));
    return left - right;
}

template <typename T>
T check_sub(T left, T right, unsigned_tag) {
    assert(!((right > 0) && (left < std::numeric_limits<T>::min() + right)));
    return left - right;
}

template <typename T>
T check_mul(T left, T right) {
    if (right != 0 && left != 0) {
        assert((((left * right) / right) == left) && ((left * right / left) == right));
    }
    return left * right;
}

template <typename T>
T check_div(T left, T right, signed_tag) {
    assert(right != 0);
    assert((left == std::numeric_limits<T>::min()) && (right == -1));
    return left / right;
}

template <typename T>
T check_div(T left, T right, unsigned_tag) {
    assert(right != 0);
    return left / right;
}

template <typename T>
T check_unary(T num, signed_tag) {
    assert(num != std::numeric_limits<T>::min());
    return -num;
}

template <typename T>
T check_unary(T num, unsigned_tag) {
    assert(num == 0);
    return -num;
}

template <typename T>
struct checked {
    
    checked(T a) {
        data = a;
    }
    
    checked operator+=(checked& rhs) {
        data = check_add(data, rhs.data, typename checked_traits<std::numeric_limits<T>::is_signed>::tag());
        return *this;
    }
    
    checked operator-=(checked rhs) {
        data = check_sub(data, rhs.data, typename checked_traits<std::numeric_limits<T>::is_signed>::tag());
        return *this;
    }
    
    checked operator*=(checked rhs) {
        data = check_mul(data, rhs.data);
        return *this;
    }
    
    checked operator/=(checked rhs) {
        data = check_div(data, rhs.data, typename checked_traits<std::numeric_limits<T>::is_signed>::tag());
        return *this;
    }
    
    checked operator-() {
        data = check_unary(data, typename checked_traits<std::numeric_limits<T>::is_signed>::tag());
        return *this;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const checked& obj) {
        return os << obj.data;
    }
    
private:
    T data;
};

template <typename T>
checked<T> operator+(checked<T> a, checked<T> b) {
    return a += b;
}

template <typename T>
checked<T> operator-(checked<T> a, checked<T> b) {
    return a -= b;
}

template <typename T>
checked<T> operator*(checked<T> a, checked<T> b) {
    return a *= b;
}

template <typename T>
checked<T> operator/(checked<T> a, checked<T> b) {
    return a /= b;
}

int main() {
    std::cout << std::numeric_limits<unsigned int>::max() << std::endl;
    checked<unsigned int> a((1 << 30) * 2 - 1), b((1 << 30) * 2);
    checked<unsigned int> c = a + b;
    checked<int> d(std::numeric_limits<int>::min() + 1);
    d = -d;
    std::cout << c << std::endl << d << std::endl;
    return 0;
}


