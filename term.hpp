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
/*
template <typename T>
class termTree
{
    public:
        
        termTree(term<T>* &_r):_root(_r){}
        term<T>* _root = nullptr;

    term_iterator<T> begin() {return term_iterator<T>(&_root, true);}
    term_iterator<T> end() {return term_iterator<T>(&_root, false);}
};
*/

template <typename T>
class term
{

public:
    string _value;
    vector<int> _path;
    term_ptr<T> _one = nullptr;
    term_ptr<T> _two = nullptr;
    term_ptr<T> _three = nullptr;

    term<T>() {}

    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef term_iterator<T> iterator;
    typedef forward_iterator_tag iterator_category;

    template <typename U>
    friend ostream &operator<<(ostream &out, const term<U> &t);
    virtual void print(ostream &out) const = 0;

    term_iterator<T> begin() { return term_iterator<T>(this, true); }
    term_iterator<T> end() { return term_iterator<T>(this, false); }

    virtual inline bool operator==(const term<T> &rhs)
    {

        return false;
    }

    /*
    inline bool operator==(const term<T> &rhs) 
    {
        this->_value == rhs._value  && 
        (this->_one == rhs._one) &&
        (this->_two == rhs._two);
        //return cmp(lhs,rhs) == 0;
        //return true;
    }*/
};

template <typename T>
class variable : public term<T>
{
protected:
    //string _value;

public:
    variable(string s)
    {

        this->_value = s;
        this->_one = nullptr;
        this->_two = nullptr;
    }

    inline bool operator==(const term<T> &rhs) override
    {
        return ((rhs._one == nullptr) &&
               (rhs._two == nullptr));
        //this->_value == rhs._value;
        //return cmp(lhs,rhs) == 0;
        //return true;
    }
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
        out << this->_value;
    }
};

template <typename T>
class literal : public term<T>
{
protected:
    //T _lit;

public:
    literal(T lit)
    {
        this->_value =   std::to_string(lit);// (lit == true) ? "True" : "False";
    }

    void print(ostream &out) const
    {
        out << this->_value;
    }
    inline bool operator==(const term<T> &rhs) override
    {
        return this->_value == rhs._value &&
               ((this->_one == nullptr && rhs._one == nullptr) ||
                *(this->_one) == *(rhs._one) && *(rhs._one) == *(this->_one)) &&
               ((this->_two == nullptr && rhs._two == nullptr) ||
                *(this->_two) == *(rhs._two) && *(rhs._two) == *(this->_two));
    }
};

template <typename T>
class function : public term<T>
{

private:
    //string _name;
    size_t _arity;

    //vector<term_ptr<T>> _t;
public:
    function(string n, size_t k, vector<term_ptr<T>> t)
    {
        this->_value = n;
        _arity = k;
        this->_one = t[0];
        if (t.size() > 1)
        {
            this->_two = t[1];
        }

        if (t.size() > 2)
        {
            this->_three = t[2];
        }
    }

    function(function<T> const &var)
    {
        this->_value = var._value;
        _arity = var._arity;
        this->_one = var._one;
        this->_two = var._two;

        {
            this->_three = var._three;
        }
    }

    function<T> &operator=(function<T> const &var)
    {
        this->_value = var._value;
        _arity = var._arity;
        this->_one = var._one;
        this->_two = var._two;

        {
            this->_three = var._three;
        }
    }

    void print(ostream &out) const
    {
        out << this->_value << "(";

        //for (int i = 0; i < _arity; i++)
        {
            if (this->_one)
            {
                this->_one->print(out);
            }
            if (this->_two)
            {
                out << " , ";
                this->_two->print(out);
            }
            if (this->_three)
            {
                out << " , ";
                this->_three->print(out);
            }
        }

        out << ")";
    }
    inline bool operator==(const term<T> &rhs) override
    {
        bool equal = ((this->_value == rhs._value));

        if(equal)
        equal= ((this->_one == nullptr && rhs._one == nullptr) ||
                        *(this->_one) == *(rhs._one) ); 
                        //&& *(rhs._one) == *(this->_one));

        if(equal)
        equal = ((this->_two == nullptr && rhs._two == nullptr) ||
                         *(this->_two) == *(rhs._two) );
                         //&& *(rhs._two) == *(this->_two));

        //bool equal = root && leftLeg && rightLeg;
        if (!equal)
        {
            if (this->_value == "not")
            {
                equal = (rhs._one == nullptr && rhs._two == nullptr && this->_one->_value == rhs._value);
            }
            else if (rhs._value == "not")
            {
                equal = (this->_one == nullptr && this->_two == nullptr && rhs._one->_value == this->_value);
            }
        }
        return equal;
    }
};

/*
template<typename T>
term_ptr<T> insert_node(const T& elem, term_ptr<T> at)
{
    if(!at)
    {
        return term_ptr<T>(term<T>(elem));
    }
    if(elem < at->value)
    {
        at->_one = insert_node(elem, at->_one);
    }
    if(elem > at->value)
    {
        at->_two = insert_node(elem, at->_two);
    }
    return at;
}

template<typename T>
void tree<T>::insert(const T& elem)
{
    _root = insert_node(elem, _root);
}

*/

/////////////////////////////////////////////////////////////////
//
// unify
//
/////////////////////////////////////////////////////////////////

template <typename T>
bool unify(const term<T> &t1, const term<T> &t2, Sub<T> &sigma)
{
    // termTree<bool> tree(t1);
    /*
    auto end =  tree.end();
    for( auto it = tree.begin(); it != end;it++)
    {
        if( sigma.contains(it->_value) )
        {
            cout <<"sub of " << (it->_value) <<" -> " << sigma(it->_value) << endl;
        }
    }
    */
    return false;
}

template <typename T>
void findMatch(Sub<T> &match, term_ptr<T> lhs, term_ptr<T> rhs)
{
    if (lhs == nullptr)
    {
        return;
    }
    if (lhs->_value != rhs->_value)
    {
        match.extend(rhs->_value, lhs);
        return;
    }
    // if(lhs->_one !=  nullptr )
    findMatch(match, lhs->_one, rhs->_one);
    //if(lhs->_two !=  nullptr )
    findMatch(match, lhs->_two, rhs->_two);
}
template <typename T>
bool match(term_ptr<T> t, term_ptr<T> t1, vector<int> &path, Sub<T> &sigma)
{
    bool found = false;
    auto end = t->end();
    for (auto it = t->begin(); it != end; it++)
    {
        //cout << "checking for" << *t << " of " << **it << " with " << *t1 << "\n";
        if (*(*it) == *t1)
        {
            cout << "found " << *t1 << "\n";
            *(*it) == *t1;
            found = true;
            path = it->_path;

            findMatch(sigma, it->_one, t1->_one);
            findMatch(sigma, it->_two, t1->_two);

            break;
        }
    }

    return found;
}

/////////////////////////////////////////////////////////////////
//
// reduce
//
/////////////////////////////////////////////////////////////////

template <typename T>
term_ptr<T> reduce(term_ptr<T> t, const std::vector<rule<T>> &rules)
{
    vector<int> path;
  
    //for (const auto &r : rules)
    while (true)
    {
        bool foundInThisRun = false;
        for (uint32_t r = 0; r < rules.size(); r++)
        {
            term_ptr<T> f = (rules[r].first);
            cout <<*f <<" => " << *(rules[r].second)<<"\n";
            Sub<T> sigma;
            bool found = match(t, rules[r].first, path, sigma);
            if (found)
            {
                foundInThisRun = true;
                cout << " found at " << r << " -- ";
                for (auto p : path)
                {
                    cout << p << " ";
                }
                cout << " | " << endl;

                //match.extend("a", tand(tnot(tor(var("x"), var("x"))), tnot(lit(true))));
                cout << "rewrite " << *t << endl;
                cout << "rhs  " << *rules[r].second << endl;
                t = rewrite(t, rules[r].second, path, sigma);
                cout << "to " << *t << endl;
                cout << endl;
            }
        }
        if (!foundInThisRun)
        {
            if(
            t->_value == "->" && 
            (t->_one !=nullptr && t->_two !=nullptr) &&
            *t->_one == *t->_two ) 
            
            {
                t = t->_one;
            }
            break;
        }
    }

    cout << *t << "\n";
    return t;
}

/////////////////////////////////////////////////////////////////
//
// rewrite
//
/////////////////////////////////////////////////////////////////

template <typename T>
term_ptr<T> rewrite(term_ptr<T> t, term_ptr<T> rhs, std::vector<int> path, const Sub<T> &sigma)
{
   
    sigma.print();

    term_ptr<T> currTerm = t;
    for (uint16_t i = 0; i < path.size()-1; i++)
    {
        uint16_t p = path[i];
        if (p == 1)
        {
            currTerm = currTerm->_one;
        }
        else if (p == 2)
        {
            currTerm = currTerm->_two;
        }
    }
    cout << "repalcing " << *currTerm << " with rhs " << *rhs << endl;
    if (path.back() == 1)
    {
        if (true && rhs && sigma.contains(rhs->_value))
        {
            term_ptr<T> sub = sigma(rhs->_value);
            sub->_value = rhs->_value;
            currTerm->_one = sub;
        }
        else
        {
            currTerm->_one = rhs;
        }
    }
    else if (path.back() == 2)
    {
        //currTerm->_two = rhs;
        
        if (true && rhs && sigma.contains(rhs->_value))
        {
            term_ptr<T> sub = sigma(rhs->_value);
            sub->_value = rhs->_value;
            currTerm->_two = sub;
        }
        else
        {
            currTerm->_two = rhs;
        }
    }

    /*
    cout << " input " << *t << endl;
    cout << " rhs " << *rhs<<"\n";
    term<T>& sub = sigma("a");
   cout <<" sub " << sub <<endl;

   // 

    term_ptr<T> rhsPtr(rhs);// =  make_shared<term<bool>>(rhs);
    
    termTree<bool> tree(rhsPtr);
    auto end =  tree.end();
    for( auto it = tree.begin(); it != end;it++)
    {
        if( sigma.contains(it->_value) )
        {
            cout <<"sub of " << (it->_value) <<" -> " << sigma(it->_value) << endl;
        }
    }

    /*
    for( auto it = tree.begin(); it != end;it++)
    {
        // cout << *(*it) << endl;
        cout <<"checking for substitution of "<< it->_value <<" ... \n";
        if( sigma.contains(it->_value) )
        {
               cout <<"sub of " << (it->_value) <<" -> " << sigma(it->_value) << endl;
        }
     
    }*/

    return t;
}

/////////////////////////////////////////////////////////////////
//
// operator<<
//
/////////////////////////////////////////////////////////////////

template <typename T>
std::ostream &operator<<(std::ostream &out, const term<T> &t)
{
    t.print(out);
}

#endif // TERM_HPP
