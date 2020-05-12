#ifndef TERM_ITERATOR_HPP
#define TERM_ITERATOR_HPP

#include <exception>
#include <queue>
#include <iostream>
#include <memory>

using namespace std;

template <typename T>
class term;

/* template <typename T>
using term_ptr = std::shared_ptr<term<T>>; */

template <typename T>
class term_iterator
{
protected:
    queue<term<T> *> _terms;

public:
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef term_iterator<T> iterator;
    typedef forward_iterator_tag iterator_category;

    term_iterator<T>() = delete;

    term_iterator<T>(term<T> *n, bool begin)
    {
        uint32_t step = 0;
        if (begin)
        {
            vector<int> curPath;
            preorder(n,  curPath, 0) ;
        }
    }

    term_iterator<T>(const term_iterator<T>& i) : _terms(i._terms) {}
    

    void preorder(term<T> *term, vector<int> curPath, int step) 
    { 
        if (term == nullptr) 
            return; 
    
        if(step !=0) {
            curPath.push_back(step);
        }
        term->_path = curPath;
        _terms.push(term);

        std::vector<term_ptr<T>> children = term->getChildren();

        for(uint32_t ch = 0 ; ch < children.size() ; ch++)
        {   
            term_ptr<T> child=  children[ch];
             preorder(child.get(), curPath, ch);  
        }

    }  

    term_iterator<T>& operator++();

    term<T>& operator*()
    {
        term<T> * t = _terms.front();
        return *t;
    }

    term<T> * operator->() const
    {
        return _terms.front();
    }

    term_iterator<T> operator++(int)
    {
        term_iterator<T> tmp(*this);
        ++*this;
        return tmp;
    }

    term_iterator<T> &operator+=(unsigned int n)
    {
        for (int i = 0; i < n; i++)
            ++*this;
        return *this;
    }

    bool operator==(const term_iterator<T> &rhs) const
    {
        return _terms == rhs._terms;
    }

    bool operator!=(const term_iterator<T> &rhs) const
    {
        return _terms != rhs._terms;
    }
};

template<typename T>
term_iterator<T>& term_iterator<T>::operator++()
{
    if(!_terms.empty())
    {
        _terms.pop();
    }
    return *this;
}

#endif // TERM_ITERATOR_HPP