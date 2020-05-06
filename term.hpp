#ifndef TERM_HPP
#define TERM_HPP

#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <exception>
#include "sub.hpp"
#include "term_iterator.hpp"

using namespace std;

/////////////////////////////////////////////////////////////////
//
// forward declarations;
//
/////////////////////////////////////////////////////////////////

// This just makes life a lot more convenient
template <typename T>
using term_ptr = std::shared_ptr<term<T>>;

template <typename T>
using rule = std::pair<term_ptr<T>, term_ptr<T>>;

/////////////////////////////////////////////////////////////////
//
// class definitions
//
// The overall structure is pretty simple
// variable <: term
// literal  <: term
// function <: term
//
// Each term can have subterms in it.
//
/////////////////////////////////////////////////////////////////

template <typename T>
class term
{
public:
    
    typedef T           value_type;
    typedef T*          pointer;
    typedef T&          reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
    typedef term_iterator<T>    iterator;
    typedef forward_iterator_tag  iterator_category; 

    iterator begin() {return iterator(this);}

    iterator end() {return iterator();}

    template <typename U>
    friend ostream &operator<<(ostream &out, const term<U> &t);
    virtual void print(ostream &out) const = 0;

    //virtural term<T>() = 0;

};

template <typename T>
class variable : public term<T>
{
protected:
    string _value;

public:
    variable(string s) : _value(s) {}

    /*
    //copy constructor
    variable(variable<T> const &var)
    {
        _value = var._value;
    }

    // assignment operator
    variable<T> &operator=(variable<T> const &var)
    {
        _value = var._value;
    } */

    void print(ostream &out) const
    {
        out << _value;
    }
};

template <typename T>
class literal : public term<T>
{
protected:
    T _lit;

public:
    literal(T lit)
    {
        _lit = lit;
    }

    void print(ostream &out) const
    {
        out << _lit;
    }
};

template <typename T>
class function : public term<T>
{
protected:
    string _n;
    vector<term_ptr<T>> _t;
    size_t _k;

public:

    function(string n, size_t k, vector<term_ptr<T>> t)
    {
        _n = n;
        _t = t;
        _k = k;
    }

    function(function<T> const &var)
    {
        _n = var._n;
        _t = var._t;
        _k = var._k;
    }

    function<T> &operator=(function<T> const &var)
    {
        _n = var._n;
        _t = var._t;
        _k = var._k;
    }

    void print(ostream &out) const
    {
        out << _n << "(";
       
        for (int i = 0; i < _k; i++)
        {
            term<bool>& tt = *_t[i];
            tt.print(out);
            if(i>=0 && i != _k-1)
            { 
                cout << ",";
            }
        }

        out << ")";
    }
};

/////////////////////////////////////////////////////////////////
//
// unify
//
/////////////////////////////////////////////////////////////////

template <typename T>
bool unify(const term<T> &t1, const term<T> &t2, Sub<T> &sigma)
{
}

/////////////////////////////////////////////////////////////////
//
// reduce
//
/////////////////////////////////////////////////////////////////

template <typename T>
term_ptr<T> reduce(term_ptr<T> t, const std::vector<rule<T>> &rules)
{
}

/////////////////////////////////////////////////////////////////
//
// rewrite
//
/////////////////////////////////////////////////////////////////

template <typename T>
term_ptr<T> rewrite(term_ptr<T> t, term<T> &rhs, std::vector<int> path, const Sub<T> &sigma)
{
    
}

/////////////////////////////////////////////////////////////////
//
// operator<<
//
/////////////////////////////////////////////////////////////////

template <typename T>
std::ostream &operator<<(std::ostream &out, const term<T> &tem)
{
   tem.print(out);
}

#endif // TERM_HPP
