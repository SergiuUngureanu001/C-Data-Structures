#include <iostream>
#include <algorithm>
#include <memory>
#include <limits>
#include <typeinfo>
#include <iomanip>
#include <initializer_list>
#include <stdexcept>
#include <utility>

using namespace std;

template <typename T>
class Lista {
private:
    unique_ptr<T[]> vec;
    size_t cap;
    size_t sz;
    static const T NMAX;

    void grow(bool ok = false, size_t n = 69) {
        size_t new_cap;
        if(this->cap == 0) {
            new_cap = ok ? n + 1 : 1;
        } else {
            new_cap = ok ? n + 1 : this->cap * 2;
        }
        unique_ptr<T[]> aux = make_unique<T[]>(new_cap);
        for(size_t i = 0; i < this->sz; i++) {
            aux[i] = move(vec[i]);
        }
        vec = move(aux);
        this->cap = new_cap;
    }

    void reserve(size_t new_cap) {
        if(new_cap > this->cap) {
            unique_ptr<T[]> aux = make_unique<T[]>(new_cap);
            for(size_t i = 0; i < this->sz; i++) {
                aux[i] = move(vec[i]);
            }
            vec = move(aux);
            this->cap = new_cap;
        }
    }

public:
    class Iterator {
    private:
        T *ptr;

    public:
        /// Foarte important pentru a putea folosi iteratorii in compozitia altor functii precum sort
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        explicit Iterator(T *p) : ptr(p) {}

        T& operator*() {
            return *ptr;
        }
        T* operator->() {
            return ptr;
        }

        Iterator& operator++() {
            ++ptr;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++ptr;
            return tmp;
        }
        Iterator& operator--() {
            --ptr;
            return *this;
        }
        Iterator operator--(int) {
            Iterator tmp = *this;
            --ptr;
            return tmp;
        }

        Iterator operator+(difference_type n) const {
            return Iterator(ptr + n);
        }
        Iterator operator-(difference_type n) const {
            return Iterator(ptr - n);
        }
        Iterator& operator+=(difference_type n) {
            ptr += n; return *this;
        }
        Iterator& operator-=(difference_type n) {
            ptr -= n; return *this;
        }

        T& operator[](difference_type n) const {
            return ptr[n];
        }

        bool operator==(const Iterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const Iterator& other) const {
            return ptr != other.ptr;
        }
        bool operator<(const Iterator& other) const {
            return ptr < other.ptr;
        }
        bool operator>(const Iterator& other) const {
            return ptr > other.ptr;
        }
        bool operator<=(const Iterator& other) const {
            return ptr <= other.ptr;
        }
        bool operator>=(const Iterator& other) const {
            return ptr >= other.ptr;
        }

        // Distance
        difference_type operator-(const Iterator& other) const {
            return ptr - other.ptr;
        }

        T* get() const { return ptr; }
    };

    class ReverseIterator {
    private:
        T* ptr;

    public:
        /// Foarte important pentru a putea folosi iteratorii in compozitia altor functii precum sort
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        explicit ReverseIterator(T* p) : ptr(p) {}

        reference operator*() const {
            return *ptr;
        }
        pointer operator->() const {
            return ptr;
        }

        ReverseIterator& operator++() {
            --ptr;
            return *this;
        }
        ReverseIterator operator++(int) {
            ReverseIterator tmp = *this;
            --ptr;
            return tmp;
        }
        ReverseIterator& operator--() {
            ++ptr;
            return *this;
        }
        ReverseIterator operator--(int) {
            ReverseIterator tmp = *this;
            ++ptr;
            return tmp;
        }

        ReverseIterator operator+(difference_type n) const {
            return ReverseIterator(ptr - n);
        }
        ReverseIterator operator-(difference_type n) const {
            return ReverseIterator(ptr + n);
        }
        ReverseIterator& operator+=(difference_type n) {
            ptr -= n;
            return *this;
        }
        ReverseIterator& operator-=(difference_type n) {
            ptr += n;
            return *this;
        }

        bool operator==(const ReverseIterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const ReverseIterator& other) const {
            return ptr != other.ptr;
        }
        bool operator<(const ReverseIterator& other) const {
            return ptr > other.ptr;
        }
        bool operator>(const ReverseIterator& other) const {
            return ptr < other.ptr;
        }
        bool operator<=(const ReverseIterator& other) const {
            return ptr >= other.ptr;
        }
        bool operator>=(const ReverseIterator& other) const {
            return ptr <= other.ptr;
        }

        difference_type operator-(const ReverseIterator& other) const {
            return other.ptr - ptr;
        }

        reference operator[](difference_type n) const {
            return *(ptr - n);
        }
    };

    Lista() : cap(1), sz(0), vec(make_unique<T[]>(1)) {}

    Lista(size_t n, const T &val) : cap(0), sz(0), vec(nullptr) {
        if(n > 0) {
            reserve(n);
            for(auto i = 0; i < n; i++) {
                vec[i] = val;
            }
            this->sz = n;
        }
    }

    Lista(size_t n) : cap(0), sz(0), vec(nullptr) {
        if(n > 0) {
            reserve(n);
            for(auto i = 0; i < n; i++) {
                vec[i] = 0;
            }
            this->sz = n;
        }
    }

    Lista(initializer_list<T> init) : cap(0), sz(0), vec(nullptr) {
        reserve(init.size());
        for (const auto& item : init) {
            vec[sz++] = item;
        }
    }

    Lista(const Lista<T> &other) : cap(0), sz(0), vec(nullptr) {
        reserve(other.sz);
        for(size_t i = 0; i < other.sz; i++) {
            vec[i] = other.vec[i];
        }
        this->sz = other.sz;
    }

    ~Lista() = default;

    void push(const T &val) {
        if(this->sz == this->cap) {
            if(this->cap >= static_cast<size_t>(Lista<T>::NMAX) / 2) {
                throw out_of_range("Overflow\n");
            }
            grow();
        }
        vec[sz++] = val;
    }

    void pop() {
        if(this->sz == 0) {
            throw out_of_range("Lista e goala\n");
        }
        this->sz--;
    }

    void assigned(size_t new_cap, const T& val) {
        reserve(new_cap);
        for(size_t i = 0; i < new_cap; i++) {
            vec[i] = val;
        }
        this->sz = new_cap;
    }

     void insert_at(Iterator it, const Lista<T>& values) {
        size_t pos = it.get() - vec.get();
        if(pos > this->sz) {
            throw out_of_range("Iterator out of range");
        }

        size_t num_elements = values.get_size();
        if(num_elements == 0)
            return;

        while(sz + num_elements > cap) {
            grow();
        }

        if(pos < this->sz) {
            for(size_t i = sz; i > pos; i--) {
                size_t src = i - 1;
                size_t dest = src + num_elements;
                vec[dest] = move(vec[src]);
            }
        }

        for(size_t i = 0; i < num_elements; i++) {
            vec[pos + i] = values[i];
        }

        this->sz += num_elements;
    }

    void cleared() {
        this->resized(0);
    }

    T& get_front() const {
        return vec[0];
    }

    T& get_end() const {
        return vec[sz - 1];
    }

    bool isEmpty() const {
        return this->sz == 0;
    }

    Lista<T>& operator=(const Lista<T> &other) {
        if(this != &other) {
            Lista<T> aux(other);
            swap(*this, aux);
        }
        return *this;
    }

    friend void swap(Lista<T> &mylist, Lista<T> &other) {
        using std::swap;
        swap(mylist.vec, other.vec);
        swap(mylist.cap, other.cap);
        swap(mylist.sz, other.sz);
    }

    friend void print(Lista<T> &mylist) {
        for(int i = 0; i < mylist.sz; i++) {
            if(typeid(T) == typeid(float) || typeid(T) == typeid(double)) {
                cout << fixed << setprecision(3) << mylist.vec[i] << " ";
            } else {
                cout << mylist.vec[i] << " ";
            }
        }
        cout << '\n';
    }

    size_t get_size() const {
        return this->sz;
    }

    size_t get_cap() const {
        return this->cap;
    }

    T& operator[](size_t i) {
        return vec[i];
    }

    const T& operator[](size_t i) const {
        return vec[i];
    }


    void resized(size_t new_cap) {
        if(new_cap == cap) return;

        unique_ptr<T[]> aux = make_unique<T[]>(new_cap);
        size_t elements_to_copy = min(sz, new_cap);

        for(size_t i = 0; i < elements_to_copy; i++) {
            aux[i] = move(vec[i]);
        }

        if(new_cap > sz) {
            for(size_t i = sz; i < new_cap; i++) {
                aux[i] = T();
            }
        }

        this->vec = move(aux);
        this->cap = new_cap;
        this->sz = new_cap;
    }

    Iterator begin() {
        return Iterator(vec.get());
    }

    Iterator end() {
        return Iterator(vec.get() + this->sz);
    }

    ReverseIterator rbegin() {
        return ReverseIterator(vec.get() + sz - 1);
    }

      ReverseIterator rend() {
        return ReverseIterator(vec.get() - 1);
    }

};

template <typename T>
const T Lista<T>::NMAX = numeric_limits<T>::max();

int main()
{
    /// CODE TESTING
    Lista<int> v(10, 69);
    for(int i = 0; i < v.get_size(); i++) {
        cout << v[i] << " ";
    }
    cout << '\n';
    for(int i = 0; i < 8; i++) {
        v.pop();
    }
    for(int i = 0; i < v.get_size(); i++) {
        cout << v[i] << " ";
    }
    cout << '\n';
    for(int i = 0; i < 5; i++) {
        v.push(i);
    }
    for(int i = 0; i < v.get_size(); i++) {
        cout << v[i] << " ";
    }
    cout << '\n';
    cout << v[1] << " " << v[3] << '\n';
    print(v);

    Lista<int> arr = v;
    print(arr);
    for(int i = 0; i < 3; i++) {
        arr.push(420 + i);
    }
    v = arr;
    print(v);
    v.resized(15);
    print(v);
    Lista<Lista<int>> adj;
    adj.resized(10);
    for(int i = 0; i < 5; i++) {
        adj[i + 1].push(5);
        adj[i + 2].push(3);
    }
    for(int i = 0; i < adj.get_size(); i++) {
        print(adj[i]);
    }

    for(auto &x : v) {
        cout << x << " ";
    }
    cout << '\n';
    v.get_front() = 420;
    print(v);
    v.get_end() = 169;
    print(v);
    for(auto it = v.rbegin(); it != v.rend(); it++) {
        cout << *it << " ";
    }
    cout << '\n';
    v.cleared();
    print(v);
    v.push(10);
    print(v);
    v.push(11);
    v.push(12);
    v.push(13);
    print(v);
    Lista<int> hatz(5, 69);
    v.insert_at(v.end(), hatz);
    print(v);
    auto it = v.begin();
    it++;
    ++it;
    v.insert_at(it, Lista<int> (2, 100));
    print(v);
    Lista<float> fl(5, 69.42069);
    print(fl);
    Lista<int> new_list = {10, 2, 3, 1, 5};
    print(new_list);
    sort(new_list.begin(), new_list.end());
    print(new_list);
    sort(new_list.begin(), new_list.end(), [](int a, int b) -> bool {
         return a > b; });
    print(new_list);
    cout << *max_element(new_list.begin(), new_list.end()) << '\n';
    sort(new_list.rbegin(), new_list.rend());
    print(new_list);
    new_list.assigned(5, 50);
    print(new_list);
}
