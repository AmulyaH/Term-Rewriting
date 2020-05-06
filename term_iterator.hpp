#ifndef TERM_ITERATOR_HPP
#define TERM_ITERATOR_HPP

#include<exception>
#include<stack>
#include<iostream>
#include<utility>
#include<memory>

using namespace std;

template<typename T>
class term;

template<typename T>
using term_ptr = std::shared_ptr<term<T>>;

template<typename T>
class term_iterator
{
    private:
    
        stack<term_ptr<T>> path;
    
    public:
        term_iterator<T>(term_ptr<T> n);

        term<T>& operator*() const
        {
            return *(path->top());
        }

        term_ptr<T> operator->() const 
        {
            return path->top();
        }

        term_iterator<T>& operator++();

};


#endif // TERM_ITERATOR_HPP