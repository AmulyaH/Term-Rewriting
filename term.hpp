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
    vector<int> _path;

    term<T>() {}

    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef term_iterator<T> iterator;
    typedef forward_iterator_tag iterator_category;

    virtual term<T> &operator=(term<T> const &var)
    {
        this->_name = var._name;
        this->_path = var._path;
        return *this;
    }

    template <class U>
    term<T>(term<U> const &var)
    {

        this->_name = var._name;
        this->_path = var._path;
    }

    term(term<T> const &var)
    {

        this->_name = var._name;
        this->_path = var._path;
    }

    template <typename U>
    friend ostream &operator<<(ostream &out, const term<U> &t);
    virtual void print(ostream &out) const = 0;

    virtual bool isVariable() const = 0;
    virtual inline bool operator==(const term<T> &rhs)
    {
        return false;
    }

    virtual vector<term_ptr<T>> getChildren()
    {
        return vector<term_ptr<T>>();
    }
    virtual void setChild(uint32_t pos, term_ptr<T> term) = 0;
    virtual void copy(string name) = 0;

    term_iterator<T> begin() { return term_iterator<T>(this, true); }
    term_iterator<T> end() { return term_iterator<T>(this, false); }

    string _name;
};

template <typename T>
class variable : public term<T>
{
public:
    string _value;

public:
    variable(string s)
    {
        this->_value = s;
        this->_name = s;
    }

    inline bool operator==(const term<T> &rhs) override
    {
        return true;
    }

    void print(ostream &out) const
    {
        out << this->_value;
    }
    bool isVariable() const override
    {
        return true;
    }

    vector<term_ptr<T>> getChildren()
    {
        return vector<term_ptr<T>>();
    }
    void setChild(uint32_t pos, term_ptr<T> term)
    {
    }
    void copy(string name)
    {

        this->_name = name;
    }
};

template <typename T>
class literal : public term<T>
{
public:
    T _value;

public:
    literal(T lit)
    {
        this->_value = lit;
        this->_name = lit; //  std::to_string(lit);// (lit == true) ? "True" : "False";
    }

    void print(ostream &out) const
    {
        out << this->_value;
    }
    inline bool operator==(const term<T> &rhs) override
    {
        /* return this->_value == rhs._value &&
               ((this->_one == nullptr && rhs._one == nullptr) ||
                *(this->_one) == *(rhs._one) && *(rhs._one) == *(this->_one)) &&
               ((this->_two == nullptr && rhs._two == nullptr) ||
       
                *(this->_two) == *(rhs._two) && *(rhs._two) == *(this->_two)); */
    }
    bool isVariable() const override
    {
        return false;
    }
    vector<term_ptr<T>> getChildren()
    {
        return vector<term_ptr<T>>();
    }
    virtual void setChild(uint32_t pos, term_ptr<T> term)
    {
    }
    void copy(string name)
    {
    }
};

template <typename T>
class function : public term<T>
{

private:
    string _value;
    size_t _arity;
    vector<term_ptr<T>> children;

    //vector<term_ptr<T>> _t;
public:
    function(string n, size_t k, vector<term_ptr<T>> t)
    {
        this->_value = n;
        this->_arity = k;
        this->children = t;
        this->_name = n;
    }

    function(const function<T> &var)
    {
        this->_value = var._value;
        this->_arity = var._arity;
        this->children = var.children;
        this->_name = var._name;
    }

    function<T> &operator=(term<T> const &var)
    {
        try
        {
            // if(!var.isVariable())
            {
                const function<T> *varF = dynamic_cast<const function<T> *>(&var);
                *this = *varF;
            } /*else
            {
                const variable<T> *varV = dynamic_cast<const variable<T> *>(&var);
                *this = *varV;
            }  */
        }
        catch (std::bad_cast &e)
        {
        }

        return *this;
    }

    //template <class U>
    function<T> &operator=(function<T> const &var)
    {
        //term<U>::operator=(var);
        ///cout << " fffff assing operator \n";

        this->_value = var._value;
        this->_arity = var._arity;
        this->children = var.children;
        this->_name = var._name;
        return *this;
    }

    /*
     function<T> &operator=(variable<T> const& var)
    {
        //term<U>::operator=(var);
                 cout <<" fffff assing operator \n";

        this->_value = var._value;
        this->_arity = 0;
        this->children =  vector<term_ptr<T>>();
        this->_name = "";
        return *this;
    } 
*/
    vector<term_ptr<T>> getChildren()
    {
        return children;
    }

    virtual void setChild(uint32_t pos, term_ptr<T> term)
    {

        children[pos] = term;
    }

    void print(ostream &out) const
    {
        out << this->_value << "(";

        for (int i = 0; i < _arity; i++)
        {
            children[i]->print(out);
            if (i % 2 == 0 && i != _arity - 1)
            {
                out << " , ";
            }
        }

        out << ")";
    }
    inline bool operator==(const term<T> &rhs) override
    {
    }

    bool isVariable() const override
    {
        return false;
    }
    void copy(string name)
    {
        this->_value = name;
        this->_name = name;
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
    return false;
}

/* template <typename T>
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
 */

template <typename T>
bool preorder(term_ptr<T> t, term_ptr<T> rule, vector<int> &path, Sub<T> &sigma, int step, bool preMatched)
{
    bool found = false;
    if (t == nullptr && rule == nullptr)
        return true;

    cout << "x :" << *t.get() << " y :" << *rule.get() << endl;

    vector<term_ptr<T>> c1 = t->getChildren();
    vector<term_ptr<T>> rC = rule->getChildren();

    if (c1.size() == 0)
    {
        //return false;
    }
    if ((t->_name == rule->_name))
    {
        cout << c1.size() << " " << rC.size() << endl;
        if (rule->isVariable()) // c1.size() != rC.size() && (this was inside if rule)
        {
            sigma.extend(rule->_name, t);
            return true;
        }

        for (uint32_t i = 0; i < c1.size(); i++)
        {
            //if( i == 0 ){path.push_back(i);}
            path.push_back(i);
            if (preorder(c1[i], rC[i], path, sigma, i, true))
            {
                path.erase(path.end() - 1);
                found = true;
            } /*else if(preorder(c1[i],  rule, path, sigma, i,true))
            {
               return true; 
            }*/
            else
            {
                if (!preMatched)
                {
                    path.erase(path.end() - 1);
                }
                return false;
            }
        }

        if (c1.size() == 0)
        {
            return true;
        }
    }
    else if (rule->isVariable())
    {
        sigma.extend(rule->_name, t);
        return true;
    }
    else
    {
        if (!preMatched)
        {
            for (uint32_t i = 0; i < c1.size(); i++)
            {
                path.push_back(i);
                if (preorder(c1[i], rule, path, sigma, i, false))
                {
                    //path.erase(path.end() - 1);
                    return true;
                }
                else
                {
                    path.erase(path.end() - 1);
                    //return false;
                }
            }
        }
        else
        {
            return false;
        }
    }

    return found;
}

template <typename T>
bool match(term_ptr<T> t, term_ptr<T> r, vector<int> &path, Sub<T> &sigma)
{
    bool found = false;
    //auto end = t->end();
    int step = 0;

    found = preorder(t, r, path, sigma, step, false);

    sigma.print();
    std::cout << " path : ";

    for (auto p : path)
    {
        cout << p << " ";
    }
    cout << endl;
    return found;
}

/////////////////////////////////////////////////////////////////
//
// reduce
//
/////////////////////////////////////////////////////////////////

template <typename T>
term_ptr<T> reduce(term_ptr<T> term, const std::vector<rule<T>> &rules)
{
    vector<int> path;
    Sub<T> sigma;
    int step = 0;

    //for (const auto &r : rules)
    //while (true)
    {
        bool foundInThisRun = false;
        for (uint32_t r = 0; r < rules.size(); r++)
        {
            term_ptr<T> f = (rules[r].first);
            cout << " trying to reduce => " << *term << " with rule => " << *f << endl;
            //cout <<*f <<" => " << *(rules[r].second)<<"\n";
            Sub<T> sigma;
            bool found = match(term, rules[r].first, path, sigma);
            if (found)
            {
                foundInThisRun = true;
                if (false)
                {
                    cout << " found at " << r << " -- ";
                    for (auto p : path)
                    {
                        cout << p << " ";
                    }
                    cout << " | " << endl;
                }
                //match.extend("a", tand(tnot(tor(var("x"), var("x"))), tnot(lit(true))));
                cout << "rewrite " << *term << endl;
                cout << "rhs  " << *rules[r].second << endl;
                term = rewrite(term, *rules[r].second, path, sigma);
                break;
                cout << "to " << *term << endl;
                cout << endl;
            }
        }
        if (!foundInThisRun)
        {
        }
    }

    cout << *term << "\n";
    return term;
}

template <typename T>
void copy(term<T> &t1, term<T> &t2)
{
    t1 = t2; ///t1.=(t2);
    /*
    function<T>* t1f = dynamic_cast<function<T>*> (&t1);
    function<T>* t2f = dynamic_cast<function<T>*> (&t2);
    *t1f = *t2f;*/
}

template <typename T>
void substitue(term<T> &t1, const Sub<T> &sigma)
{
    vector<term_ptr<T>> children = t1.getChildren();
    for (uint32_t i = 0; i < children.size(); i++)
    {
        term_ptr<T> c = children[i];
        if (c->isVariable() && sigma.contains(c->_name))
        {
            term_ptr<T> sub = sigma(c->_name);
            t1.setChild(i, sub);
        }
    }
}

/////////////////////////////////////////////////////////////////
//
// rewrite
//
/////////////////////////////////////////////////////////////////

template <typename T>
term_ptr<T> rewrite(term_ptr<T> t, term<T> &rhs, std::vector<int> path, const Sub<T> &sigma)
{
    term_ptr<T> ruleSubstitution;

    vector<term_ptr<T>> c1 = t->getChildren();

    cout << " t is :" << *t << endl;

    term_ptr<T> cur = t;

    cout << " cur is :" << *cur << endl;

    for (int i = 0; i < path.size(); i++)
    {
        cout << " in for loop , cur is :" << *cur << endl;
        c1 = cur->getChildren();
        cur = c1[path[i]];
    }

    cout << "after for loop , cur is :" << *cur << endl;
    if (!rhs.isVariable())
    {
        copy(*cur.get(), rhs);

        for (term<T> &it : *cur)
        {
            substitue(it, sigma);
            //cout << *cur << endl;

            //cout << endl;
        }
        cout << "cur after sub" << *cur << endl;
    }
    else
    {
        if (sigma.contains(rhs._name))
        {
            term_ptr<T> sub = sigma(rhs._name);
            vector<term_ptr<T>> ch = sub->getChildren();
            if (ch.size() > 0)
            {
                *cur = *sub;
            }
            else if (sub->isVariable())
            {
                return make_shared<variable<T>>(variable<T>(sub->_name));
            }
            else
            {
                const literal<T> *varL = dynamic_cast<const literal<T> *>(sub.get());

                return make_shared<literal<T>>(literal<T>(varL->_value));
                //cout << *cur << endl;
            }
        }
    }

    cout << " adadasdada is " << *t << endl;
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
    //cout << "example " << endl;
}

#endif // TERM_HPP
