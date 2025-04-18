#include <iostream>
#include <memory>
#include <vector>
#include <limits>

using namespace std;

template <typename T>
class Stiva {
private:
    struct Node {
        T data;
        unique_ptr<Node> next;

        Node(T val) : data(val), next(nullptr) {}
    };

    struct NodeListHelper {
        unique_ptr<Node> head = nullptr;
        size_t sz = 0;

        void add(unique_ptr<Node> node) {
            node->next = move(head);
            head = move(node);
            sz++;
        }

        vector<T> getData() const {
            vector<T> result;
            Node* current = head.get();
            while(current) {
                result.push_back(current->data);
                current = current->next.get();
            }
            return result;
        }
    };

    unique_ptr<Node> head;
    size_t cap;
    size_t sz;
    static T NMAX;

public:
    Stiva(int n) : cap(n), sz(0) {}

    Stiva() : cap(1), sz(0) {}

    ~Stiva() = default;

    Stiva(const Stiva<T> &other) : cap(other.cap), sz(0), head(nullptr) {
        if(!other.head)
            return;

        Stiva<T> temp;
        Node* current = other.head.get();
        while(current) {
            temp.push(current->data);
            current = current->next.get();
        }

        current = temp.head.get();
        while(current) {
            this->push(current->data);
            current = current->next.get();
        }
        this->sz = other.sz;
    }

    Stiva& operator=(const Stiva<T> &other) {
        if(this != &other) {
            Stiva<T> temp(other);
            swap(temp.head, this->head);
            swap(temp.cap, this->cap);
            swap(temp.sz, this->sz);
        }
        return *this;
    }

    void push(T val) {
        unique_ptr<Node> new_node = make_unique<Node>(val);
        if(this->sz == this->cap) {
            if(this->cap  >= static_cast<int>(Stiva<T>::NMAX) / 2) {
                cout << "Overflow\n";
                exit(0);
            }
            this->cap = this->cap * 2;
        }
        new_node->next = move(head);
        head = move(new_node);
        sz++;
    }

    void pop() {
        if(sz <= 0) {
            cout << "Invalid\n";
            return;
        }
        head = move(head->next);
        sz--;
    }

    T top() const {
        if(sz <= 0) {
            cout << "Invalid\n";
            return Stiva<T>::NMAX;
        }
        return head->data;
    }

    NodeListHelper eraseAndSave(int k) {
        NodeListHelper List;

        for(int i = 0; i < k && this->sz > 0; i++) {
            unique_ptr<Node> current = move(head);
            head = move(current->next);
            List.add(move(current));
            this->sz--;
        }

        return List;
    }


    size_t get_size() const {
        return this->sz;
    }

    bool isEmpty() const {
        return this->sz == 0;
    }

    friend void print(const Stiva<T> &obj) {
        Node* current = obj.head.get();
        while(current != nullptr) {
            cout << current->data << " ";
            current = current->next.get();
        }
        cout << '\n';
    }
};
template <typename T>
T Stiva<T>::NMAX = numeric_limits<T>::max();

int main() {
    /// CODE TESTING
    Stiva<int> v(10);
    v.push(1);
    v.push(2);
    v.push(10);
    print(v);
    v.pop();
    print(v);
    int top = v.top();
    cout << top << '\n';

    Stiva<double> s;
    s.push(69.43);
    s.push(100.01);
    s.push(420.329);
    print(s);
    s.pop();
    s.pop();
    cout << s.get_size() << '\n';
    s.pop();
    cout << (s.isEmpty() ? "Yes\n" : "No\n");
    cout << '\n';

    Stiva<int> st = v;
    print(st);
    for(int i = 0; i < 5; i++) {
        st.push(69);
    }
    v = st;
    print(v);
    auto saved = v.eraseAndSave(6);
    for(auto &x : saved.getData()) {
        cout << x << " ";
    }

}
