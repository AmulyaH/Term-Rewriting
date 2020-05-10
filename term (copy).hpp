#ifndef TERM_HPP
#define TERM_HPP

#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <exception>
#include "sub.hpp"
//#include "term_iterator.hpp"

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
/*
    typedef T           value_type;
    typedef T*          pointer;
    typedef T&          reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
    typedef term_iterator    iterator;
    typedef forward_iterator_tag  iterator_category; */

    friend ostream &operator<<(ostream &out, const term<T> &_value);
    virtual void print(ostream &out) const = 0;
};

template <typename T>
class variable : public term<T>
{
protected:
    string _value;

public:
    variable(string s) : _value(s) {}

    //copy constructor
    variable(variable<T> const &var)
    {
        _value = var._value;
    }

    // assignment operator
    variable<T> &operator=(variable<T> const &var)
    {
        _value = var._value;
    }

    void print(ostream &out) const
    {
        out << _value << endl;
    }
    friend ostream &operator<<(ostream &out, const variable<T> &_value);
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
        out << _lit << endl;
    }

    friend ostream &operator<<(ostream &out, const literal<T> &lit);
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
    void print(ostream &out) const
    {
        cout << _n << " : ";
        /*
        for (int i = 0; i < _t.size(); i++)
        {
            term_ptr<bool> t = _t[i];

            t->print(out);
        }

        cout << endl;*/
    }

    friend ostream &operator<<(ostream &out, const function<T> &lit);
};
/*
ostream & operator << (ostream &out, const term<bool>& c)
{
    cout <<"term " ;//<< endl;
} */

ostream &operator<<(ostream &out, const literal<bool> &c)
{
    //cout << c._lit << endl;
    c.print(out);
}

ostream &operator<<(ostream &out, const variable<bool> &c)
{
    c.print(out);
    //cout << c._value << endl;
}

ostream &operator<<(ostream &out, const function<bool> &c)
{
    //c.print(out);

    out << c._n << " : ";
    for (int i = 0; i < c._t.size(); i++)
    {
        term_ptr<bool> t = (c._t[i]);
        //        variable<bool>* t = static_cast<variable<bool>*>(c._t[i].get());
        t->print(out);
        //cout << *t << " " ;
    }

    out << endl;

    // cout << endl;
}

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
std::ostream &operator<<(std::ostream &out, const term<T> &t)
{
}

#endif // TERM_HPP
