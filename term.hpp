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

    term<T> (){}
    term<T> (const term_ptr<T>& t)
    {
        this->_value = t->_value;
        this->_path = t->_path;
        this->_one = t->_one;
        this->_two = t->_two;
        this->_three = t->_three;
    }
    term<T> &operator=(term<T> const &t)
    {
         this->_value = t->_value;
        this->_path = t->_path;
        this->_one = t->_one;
        this->_two = t->_two;
        this->_three = t->_three;
    }
    
    typedef T                       value_type;
    typedef T*                      pointer;
    typedef T&                      reference;
    typedef size_t                  size_type;
    typedef ptrdiff_t               difference_type;
    typedef term_iterator<T>        iterator;
    typedef forward_iterator_tag    iterator_category; 

    template <typename U>
    friend ostream &operator<<(ostream &out, const term<U> &t);
    virtual void print(ostream &out) const = 0;



    term_iterator<T> begin() {return term_iterator<T>(this, true);}
    term_iterator<T> end() {return term_iterator<T>(this, false);}
};

template <typename T>
class variable : public term<T>
{
protected:
    //string _value;

public:
    variable(string s)  {

        this->_value = s;
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
        this->_value = (lit == true) ? "True" : "False";
    }

    void print(ostream &out) const
    {
        out << this->_value;
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
        if(t.size() > 1)
        {
               this->_two = t[1];
        }
     
        if(t.size() > 2)
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
        out << this->_value <<"(";
        
    
        //for (int i = 0; i < _arity; i++)
        {
            if(this->_one)
            {
                    this->_one->print(out);
                   
            }
              if(this->_two)
            {
                  out << " , ";
                    this->_two->print(out);
                    
            }
              if(this->_three)
            {
                 out << " , ";
                    this->_three->print(out);
                  
            }
            
            
        }

        out << ")";
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
bool unify(const term<T>& t1, const term<T>& t2, Sub<T>& sigma)
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
bool match(term_ptr<T> t, term_ptr<T> t1, vector<int>& path)
{
    

    return true;
}

/////////////////////////////////////////////////////////////////
//
// reduce
//
/////////////////////////////////////////////////////////////////

template <typename T>
term_ptr<T> reduce(term_ptr<T> t, const std::vector<rule<T>>& rules)
{
    vector<int> path ;

    bool x = match(t,rules[8].first,path);
    for(const auto& r : rules)
    {

        term_ptr<T> f = (r.first);
        cout <<*f <<" => " << *(r.second)<<"\n";
    }
    cout << *t<<"\n";
    return t;
}

/////////////////////////////////////////////////////////////////
//
// rewrite
//
/////////////////////////////////////////////////////////////////

template <typename T>
term_ptr<T> rewrite(term_ptr<T> t, term<T>& rhs, std::vector<int> path, const Sub<T>& sigma)
{
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
std::ostream &operator<<(std::ostream &out, const term<T>& t)
{
   t.print(out);
}

#endif // TERM_HPP
