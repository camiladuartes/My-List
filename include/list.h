#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <iterator>

/*! Implements a list Abstract Data Type(ADT) based on a doubly linked list.
 * The purpose is to replicate the STL's `std::list` behavior.
 */
namespace sc{
    template <typename T>
    class list{
        public:
            using size_type = unsigned long; //!< The size type.
            using value_type = T; //!< The value type.
            using pointer = value_type*; //!< Pointer to a value stored in the container.
            using reference = value_type&; //!< Reference to a value stored in the container.
            using const_reference = const value_type&; //!< Const reference to a value stored
        //=== Private data.
        private:
            struct Node{
                T data; //!< Data field
                Node *prev; //!< Pointer to the previous node in the list.
                Node *next; //!< Pointer to the next node in the list.
                /// Basic constructor
                Node(const T & d = T(), Node * p = nullptr, Node * n = nullptr)
                : data(d), prev(p), next(n)
                { /* Empty */ }
            };
            int m_size;
            Node *m_head;
            Node *m_tail;
        //=== Public interface.
        public:
        //=== Constructors, Destructors, and Assignment.
            /// Default constructor that creates an empty list.
            list(){
                m_head = new Node();
                m_tail = new Node();
                m_head->next = m_tail;
                m_tail->prev = m_head;
                m_size = 0;
            }

            /// Constructs the list with count default-inserted instances of T.
            explicit list(size_type count){
                m_head = new Node();
                m_tail = new Node();
                m_head->next = m_tail;
                m_tail->prev = m_head;
                m_size = count;
                for(auto i(0u); i < count; i++){
                    Node *node = new Node;
                    node->next = m_tail;
                    m_head->next = node;
                    node->prev = m_head;
                    m_tail->prev = node;
                }
            }

            /// Constructs the list with the contents of the range [first, last).
            template <typename InputIt>
            list(InputIt first, InputIt last){
                m_head = new Node();
                m_tail = new Node();
                m_head->next = m_tail;
                m_tail->prev = m_head;
                m_size = 0;
                while(first!=last){
                    push_back(*first);
                    first++;
                }
            }

            /// Copy constructor. Constructs the list with the deep copy of the contents of other.
            list(const list& other){
                m_head = new Node();
                m_tail = new Node();
                m_size = 0;
                m_head->next = m_tail;
                m_tail->prev = m_head;
                if(other.m_head->next != other.m_tail){
                    Node *other_aux = other.m_head->next;
                    while(other_aux != other.m_tail){
                        push_back(other_aux->data);
                        other_aux = other_aux->next;
                    }
                }
            }

            /// Constructs the list with the contents of the initializer list init.
            list(std::initializer_list<T> ilist){
                m_head = new Node();
                m_tail = new Node();
                m_size = 0;
                m_head->next = m_tail;
                m_tail->prev = m_head;
                for(auto i(0u); i < ilist.size(); i++){
                    push_back(*(ilist.begin()+i));
                }
            }

            /// Destructs the list.
            ~list(){
                m_size = 0;
                Node *target = m_head;
                while(m_head != m_tail->next){
                    m_head = m_head->next;
                    delete target;
                    target = m_head;
                }
            }

            /// Copy assignment operator. Replaces the contents with a copy of the contents of other.
            list& operator=(const list& other){
                clear();
                Node *other_aux = other.m_head->next;
                while(other_aux != other.m_tail){
                    push_back(other_aux->data);
                    other_aux = other_aux->next;
                }
                return *this;
            }

            /// Replaces the contents with those identified by initializer list ilist.
            list& operator=(std::initializer_list<T> ilist){
                clear();
                for(auto i(0u); i < ilist.size(); i++){
                    push_back(*(ilist.begin()+i));
                }
                return *this;
            }

        //=== Common operations to all list implementations
            /// Return the number of elements in the container.
            size_type size() const{
                return m_size;
            }

            /// Remove (either logically or physically) all elements from the container.
            void clear(){
                Node *aux = m_head->next;
                while(aux != m_tail){
                    Node * target = aux;
                    aux = aux-> next; // Avançar para o próximo.
                    m_head->next = aux;
                    aux->prev = m_head;
                    delete target;
                }
                m_size = 0;
            }

            /// Returns true if the container contains no elements, and false otherwise.
            bool empty(){
                return m_head->next == m_tail;
            }

            /// Adds value to the front of the list.
            void push_front(const T &value){
                // Alocar novo nó.
                Node * node = new Node(value, m_head, m_head->next);
                m_head->next = node;
                node->next->prev = node;
                m_size += 1;
            }

            /// Adds value to the end of the list.
            void push_back(const T &value){
                m_size += 1;
                Node *target = new Node(value);
                target->next = m_tail;
                m_tail->prev->next = target;
                target->prev = m_tail->prev;
                m_tail->prev = target;
            }

            /// Removes the object at the end of the list.
            void pop_back(){
                // caso especial #1: lista vazia.
                if (m_head->next == m_tail) return;
                Node *target = m_tail->prev;
                m_size -= 1;
                m_tail->prev = target->prev;
                m_tail->prev->next = m_tail;
                delete target;
            }

            /// Removes the object at the front of the list.
            void pop_front(){
                if (m_head->next == m_tail) return;
                Node *target = m_head->next;
                m_size -= 1;
                m_head->next = target->next;
                target->next->prev = m_head;
                delete target;
            }

            /// Returns the object at the end of the list.
            const T & back() const{
                if(m_head->next == m_tail) 
                    throw std::runtime_error("Invalid Action!");
                return m_tail->prev->data;
            }

            /// Returns the object at the beginning of the list.
            const T & front() const{
                if(m_head->next == m_tail) 
                    throw std::runtime_error("Invalid Action!");
                return m_head->next->data;
            }

            /// Replaces the content of the list with copies of value;
            void assign(const T & value){
                if(m_head->next == m_tail) return;
                Node *temp = m_head->next;
                while(temp != m_tail){
                    temp->data = value;
                    temp = temp->next;
                }
            }

        //=== Iterators
            class const_iterator{
                //=== Private data
                public:
                    const Node *it;
                //=== Public interface
                public:
                    /// Constructor
                    const_iterator(const Node *ptr = nullptr) : it{ptr}{
                       /*empty*/
                    }
                //=== Iterator operations
                    /// Advances iterator to the next location within the list: ++it
                    const_iterator operator++(){
                        //Returns new iterator that points to the next address
                        it = it->next;
                        return *this;
                    }
                    /// Advances iterator to the next location within the list: it++
                    const_iterator operator++(int){
                        const_iterator temp(it); // Creates a temporary iterator with the current address.
                        it = it->next;
                        return temp;
                    }
                    /// Pre increment: --it
                    const_iterator operator--(void){
                        it = it->prev;
                        return *this;
                    }
                    /// Post increment: it--
                    const_iterator operator--(int){
                        const_iterator temp(it); // Creates a temporary iterator with the current address.
                        it = it->prev;
                        return temp;
                    }
                    /// Return a reference to the object located at the position pointed by the iterator
                    const T& operator*(void) const{ //x = *it;
                        return it->data;
                    }
                    /// Returns true if both iterators refer to the same location within the list, and false otherwise.
                    bool operator==(const const_iterator &ptr) const{
                        return this->it == ptr.it;
                    }
                    /// Returns true if both iterators refer to a different location within the list, and false otherwise.
                    bool operator!=(const const_iterator &ptr) const{
                        return this->it != ptr.it;
                    }
            };
            class iterator{ //From category "Biderectional"
                public:
                    typedef T& reference; //!< Reference to the value type.
                    typedef T* pointer; //!< Pointer to the value type.
                    typedef T value_type; //!< Value type the iterator points to.
                    /// Difference type used to calculated distance between iterators.
                    typedef std::ptrdiff_t difference_type;
                    /// Identifies the iterator category to algorithms from STL
                    typedef std::bidirectional_iterator_tag iterator_category; //!< Iterator category.
                //=== Private data
                public:
                    Node *it;
                //=== Public interface
                public:
                    /// Constructor
                    iterator(Node *ptr = nullptr) : it{ptr}{
                       /*empty*/
                    }
                //=== Iterator operations
                    /// Advances iterator to the next location within the list: ++it
                    iterator operator++(){
                        //Returns new iterator that points to the next address
                        it = it->next;
                        return *this;
                    }
                    /// Advances iterator to the next location within the list: it++
                    iterator operator++(int){
                        iterator temp(it); // Creates a temporary iterator with the current address.
                        it = it->next;
                        return temp;
                    }
                    /// Pre increment: --it
                    iterator operator--(void){
                        it = it->prev;
                        return *this;
                    }
                    /// Post increment: it--
                    iterator operator--(int){
                        iterator temp(it); // Creates a temporary iterator with the current address.
                        it = it->prev;
                        return temp;
                    }
                    /// Return a reference to the object located at the position pointed by the iterator
                    reference operator*(void) const{ //*it = x;
                        return it->data;
                    }
                    // /// Return a reference to the object located at the position pointed by the iterator
                    // const T& operator*(void) const{ //x = *it;
                    //     return it->data;
                    // }
                    /// Returns true if both iterators refer to the same location within the list, and false otherwise.
                    bool operator==(const iterator &ptr) const{
                        return this->it == ptr.it;
                    }
                    /// Returns true if both iterators refer to a different location within the list, and false otherwise.
                    bool operator!=(const iterator &ptr) const{
                        return this->it != ptr.it;
                    }
            };

        //=== Getting an iterator
            /// Returns an iterator pointing to the first item in the list
            iterator begin(){
                return iterator(m_head->next);
            }

            /// Returns an iterator pointing to the end mark in the list
            iterator end(){
                return iterator(m_tail);
            }

            /// Returns a constant iterator pointing to the first item in the list.
            const_iterator cbegin() const{
                return const_iterator(m_head->next);
            }

            /// Returns a constant iterator pointing to the end mark in the list
            const_iterator cend() const{
                return const_iterator(m_tail);
            }

        //=== List container operations that require iterators
            /// Adds value into the list before the position given by the iterator pos
            iterator insert(iterator pos, const T & value){
                iterator toReturn = pos;
                toReturn--;
                Node *target = new Node(value);
                target->next = pos.it;
                pos.it->prev->next = target;
                target->prev = pos.it->prev;
                pos.it->prev = target;
                m_size += 1;
                return toReturn;
            }

            /// Inserts elements from the range [first; last) before pos
            template <typename InItr>
            iterator insert(iterator pos, InItr first, InItr last){
                iterator toReturn = pos;
                toReturn--;
                while(first != last){
                    insert(pos, *first);
                    first++;
                }
                return toReturn;
            }

            /// Inserts elements from the initializer list ilist before pos
            iterator insert(iterator pos, std::initializer_list<T> ilist){
                iterator toReturn = pos;
                toReturn--;
                for(auto i(0u); i < ilist.size(); i++){
                    insert(pos, *(ilist.begin()+i));
                }
                return toReturn;
            }

            /// Removes the object at position pos
            iterator erase(iterator pos){
                pos.it->prev->next = pos.it->next;
                pos.it->next->prev = pos.it->prev;
                iterator toReturn = pos.it->next;
                m_size -= 1;
                delete pos.it;
                return toReturn;
            }

            /// Removes elements in the range [first; last)
            iterator erase(iterator first, iterator last){
                while(first != last){
                    erase(first);
                    first++;
                }
                return last;
            }

            /// Replaces the contents with count copies of value value
            void assign(size_t count, const T & value){
                clear();
                for(int i = 0; i < count; i++){
                    push_back(value);
                }
            }

            /// Replaces the contents of the list with copies of the elements in the range [first; last)
            template <typename InItr>
            void assign(InItr first, InItr last){
                clear();
                while(first != last){
                    push_back(*first);
                    first++;
                }
            }

            /// Replaces the contents of the list with the elements from the initializer list ilist
            void assign(std::initializer_list<T> ilist){
                clear();
                for(auto i(0u); i < ilist.size(); i++){
                    push_back(*(ilist.begin()+i));
                }
            }

            friend std::ostream& operator<<(std::ostream& os, const list& l){
                Node *it = l.m_head->next;
                os << "[ ";
                while(it != l.m_tail){
                    os << it->data << " ";
                    it = it->next;
                }
                os << "]";

                return os;
            }
    };
    
    //=== Operator overloading — non-member functions
        /// Checks if the contents of lhs and rhs are equal.
        template <typename T>
        bool operator==(const sc::list<T>& lhs, const sc::list<T>& rhs){
            auto l = lhs.cbegin();
            auto r = rhs.cbegin();
            if(lhs.size() == rhs.size()){
                for(size_t i = 0; i < lhs.size(); i++){
                    if(*l != *r)
                        return false;
                    l++;
                    r++;
                }
                return true;
            }
            else{
                return false;
            }
        }

        /// Similar to the previous operator, but the opposite result.
        template <typename T>
        bool operator!=(const sc::list<T>& lhs, const sc::list<T>& rhs){
            auto l = lhs.cbegin();
            auto r = rhs.cbegin();
            if(lhs.size() == rhs.size()){
                for(size_t i = 0; i < lhs.size(); i++){
                    if(*l != *r)
                        return true;
                    l++;
                    r++;
                }
                return false;
            }
            else{
                return true;
            }
        }
}

#endif