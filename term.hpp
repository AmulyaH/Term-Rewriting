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
    string _name;

    typedef T                           value_type;
    typedef T                           *pointer;
    typedef T                           &reference;
    typedef size_t                      size_type;
    typedef ptrdiff_t                   difference_type;
    typedef term_iterator<T>            iterator;
    typedef term_iterator<const T>      const_iterator;
    typedef forward_iterator_tag        iterator_category;

    term<T>() {}

    virtual term<T> &operator=(term<T> const &var)
    {
        this->_name = var._name;
        return *this;
    }

    template <typename U>
    friend ostream &operator<<(ostream &out, const term<U> &outputTerm);

    // print the input from operator << 
    virtual void print(ostream &out) const = 0;

    // check is variable
    virtual bool isVariable() const {};
    
    // copy the term names
    virtual void copyName(string name) = 0;

    // sets children's/subterms
    virtual void setChild(uint32_t position, term_ptr<T> term) = 0;

    // gets immediate children's/subterms
    virtual vector<term_ptr<T>> getChildren() 
    {
                return vector<term_ptr<T>>();
    }

    // iterator to set the locator/pointer at the begining or end of term
    iterator begin() { return iterator(this, true); }
    iterator end() { return iterator(this, false); }

    // constent term iterator
    iterator cbegin() { return const_iterator(this, true); }
    iterator cend() { return const_iterator(this, false); }
};

/////////////////////////////////////////////////////////////////
//
// class definitions
//
// variable <: term
// Derived from base class term
// Can hold only value for the variable
//
/////////////////////////////////////////////////////////////////

template <typename T>
class variable : public term<T>
{
private:
    string _value;

public:
    variable(string value)
    {
        this->_value = value;
        this->_name = value; // base class variable
    }

    //to gets children's of variable, since variable wont have any children's or subterms returning empty vector
    vector<term_ptr<T>> getChildren() override
    {
        return vector<term_ptr<T>>();
    };

    // No implimenation since variable wont have chlildrnes
    void setChild(uint32_t position, term_ptr<T> term) override{};

    // print the input from operator << 
    void print(ostream &out) const
    {
        out << this->_value;
    }

    /// check for variable 
    bool isVariable() const override
    {
        return true;
    }

    //copy constructor for variable
    variable<T> &operator=(variable<T> const &var)
    {
        this->_name = var._name;
    }

    /**
     * copy the variable
     * @param var variable to be copied
     */
    void copyName(string var) override
    {
        this->_name = var;
    }
};

/////////////////////////////////////////////////////////////////
//
// class definitions
//
// literal <: term
// Derived from base class term
// for literals
//
/////////////////////////////////////////////////////////////////

template <typename T>
class literal : public term<T>
{

public:
    T _value; // literls type is not known at complie time so templated.

    //default constructor
    literal(T lit)
    {
        this->_value = lit;
        this->_name = lit;
    }

    // print the input from operator << 
    void print(ostream &out) const
    {
        out << this->_value;
    }

    //check is variable
    bool isVariable() const override
    {
        return false;
    }

    //returns empty vector = no children's
    vector<term_ptr<T>> getChildren()
    {
        return vector<term_ptr<T>>();
    }

    // no implimentation = no childrems
    void setChild(uint32_t pos, term_ptr<T> term) override{};
    void copyName(string name) override{};
};

/////////////////////////////////////////////////////////////////
//
// class definitions
//
// function <: term
// Derived from base class term
// for function
//
/////////////////////////////////////////////////////////////////

template <typename T>
class function : public term<T>
{

private:
    string _value;  // function name
    size_t _arity;  // size of the function/ no of subterms or children's
    vector<term_ptr<T>> _children; // a function can have more than one children's / subterms

public:

    function(string functionName, size_t functionSize, vector<term_ptr<T>> functionChildren)
    {
        this->_value = functionName;
        this->_arity = functionSize;
        this->_children = functionChildren;
        this->_name = functionName;  // base class variable
    }

    function(const function<T>& functionTerm)
    {
        this->_value = functionTerm._value;
        this->_arity = functionTerm._arity;
        this->_children = functionTerm._children;
        this->_name = functionTerm._name;  // base class variable
    }

    // Assignment operator overloading 
    function<T> &operator=(term<T> const &subTerm)
    {
        try
        {
            // dynamic casting to call appropriate assignment operator 
            const function<T> *functionVar = dynamic_cast<const function<T> *>(&subTerm);
            *this = *functionVar;
        }
        catch (std::bad_cast &e)
        {
            std::cout << e.what() << '\n';
        }
        return *this;
    }

    // Assignment operator overloading for function
    function<T> &operator=(function<T> const &subTerm)
    {
        this->_value = subTerm._value;
        this->_arity = subTerm._arity;
        this->_children = subTerm._children;
        this->_name = subTerm._name;
        return *this;
    }

    // to get children
    vector<term_ptr<T>> getChildren()
    {
        return _children;
    }
    
    /**
     * assign child at a given position.
     * @param position location to set the child
     * @param childTerm input subterm to be assigned
     */ 
    void setChild(uint32_t position, term_ptr<T> childTerm)
    {
        _children[position] = childTerm;
    }

    /**
     * prints the subterms from each level of the term
     * @param out output stream which has data
     */ 
    void print(ostream &out) const
    {
        out << this->_value << "(";

        for (int i = 0; i < _arity; i++)
        {
            _children[i]->print(out);
            if (i % 2 == 0 && i != _arity - 1)
            {
                out << " , ";
            }
        }

        out << ")";
    }

    // check for is variable
    bool isVariable() const override
    {
        return false;
    }

    //to copy function name
    void copyName(string name) override
    {
        this->_value = name;
        this->_name = name;
    }
};

/////////////////////////////////////////////////////////////////
//
// 
// @parms : t1 - term
// @parms : t3 - term
// sigma  : map to hold computed substitution
//
/////////////////////////////////////////////////////////////////

/**
 * unify : a function to computes a substitution given two terms and find if one term unifies with other.
 * unify returns true, when sigma(firstTerm) and sigma(secondTerm) is same. unify is two sided 
 * 
 * @param firstTerm one term
 * @param secondTerm another term
 * @param sigma map to store the variable and it's substituations 
 * 
 */ 
template <typename T>
bool unify(term_ptr<T> firstTerm, const term_ptr<T> secondTerm, Sub<T> &sigma)
{
    bool found = false;

    vector<term_ptr<T>> firstTChildren = firstTerm->getChildren();
    vector<term_ptr<T>> secondTChildren = secondTerm->getChildren();

    if ((firstTerm->_name == secondTerm->_name))
    {
       // cout << c1.size() << " " << rC.size() << endl;

        if (firstTerm->isVariable())
        {
            if (!sigma.contains(firstTerm->_name))
            {
                sigma.extend(firstTerm->_name, secondTerm);
            }
            return unify(sigma(firstTerm->_name), secondTerm , sigma);
        }
        if (secondTerm->isVariable())
        {
            if (!sigma.contains(secondTerm->_name))
            {
                sigma.extend(secondTerm->_name, firstTerm);
            }
            return unify(firstTerm, sigma(secondTerm->_name) , sigma);
        }
       
        for (uint32_t i = 0; i < firstTChildren.size(); i++)
        {
            if (unify(firstTChildren[i], secondTChildren[i], sigma))
            {
                found = true;
            }
            else
            {
                return false;
            }
        }

    }
    else if (secondTerm->isVariable())
    {
        if (!sigma.contains(secondTerm->_name))
        {
            sigma.extend(secondTerm->_name, firstTerm);
        }
        return true;
    }

    return found;
}

/////////////////////////////////////////////////////////////////
//
// reduce : a function to reduce a term using a list of rules
// @parms : term - which has to be reduced
// @rules : list of rules for reduction
//
/////////////////////////////////////////////////////////////////

template <typename T>
term_ptr<T> reduce(term_ptr<T> term, const std::vector<rule<T>> &rules)
{
    vector<int> path;
    Sub<T> sigma;

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
                cout << "rewrite " << *term << endl;
                cout << "rhs  " << *rules[r].second << endl;
                term = rewrite(term, *rules[r].second, path, sigma);
                break;
                cout << "to " << *term << endl;
                cout << endl;
            }
        }
    }

    cout << *term << "\n";
    return term;
}

/////////////////////////////////////////////////////////////////
//
// rewrite : a function to rewrite the term to a rule using substitutions.
// @parms  : t - input term
// rhs     : right hand side of the rule to which we need to substitute and rewrite
// sigma   : list of substitutions
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
        c1 = cur->getChildren();
        cur = c1[path[i]];
    }
    if (!rhs.isVariable())
    {
        copy(*cur.get(), rhs);

        for (term<T> &it : *cur)
        {
            substitue(it, sigma);
            cout << *cur << endl;

            cout << endl;
        }
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
                cout << *cur << endl;
            }
        }
    }

    cout << " adadasdada is " << *t << endl;
    return t;
}

/////////////////////////////////////////////////////////////////
//
// substitue : a function to substitute the term with substitutions.
// @parms : t - term
// @parms : rule - which needs to match with term
// @parms : sigma - map to store the substitution details
// @parms : preMatched - flag to check weather the function of rule and term previously matched
//
/////////////////////////////////////////////////////////////////
template <typename T>
void substitue(term<T> &t, const Sub<T> &sigma)
{
    vector<term_ptr<T>> children = t.getChildren();
    for (uint32_t i = 0; i < children.size(); i++)
    {
        term_ptr<T> child = children[i];
        if (child->isVariable() && sigma.contains(child->_name))
        {
            term_ptr<T> substitutionRule = sigma(child->_name);
            t.setChild(i, substitutionRule);
        }
    }
}

/// to copy one term to another terms
template <typename T>
void copy(term<T> &t1, term<T> &t2)
{
    t1 = t2;
}

/////////////////////////////////////////////////////////////////
//
// findMatch : a function to find the match between rule and term and gets the substitution
// @parms : t - term
// @parms : rule - which needs to match with term
// @parms : sigma - map to store the substitution details
// @parms : preMatched - flag to check weather the function of rule and term previously matched
//
/////////////////////////////////////////////////////////////////

template <typename T>
bool findMatch(term_ptr<T> t, term_ptr<T> rule, vector<int> &path, Sub<T> &sigma, bool preMatched)
{
    bool found = false;
    if (t == nullptr && rule == nullptr)
        return true;

    cout << "x :" << *t.get() << " y :" << *rule.get() << endl;

    vector<term_ptr<T>> c1 = t->getChildren();
    vector<term_ptr<T>> rC = rule->getChildren();

    if ((t->_name == rule->_name))
    {
        cout << c1.size() << " " << rC.size() << endl;
        if (rule->isVariable())
        {
            if (!sigma.contains(rule->_name))
            {
                sigma.extend(rule->_name, t);
            }
            return true;
        }

        for (uint32_t i = 0; i < c1.size(); i++)
        {
            path.push_back(i);
            if (findMatch(c1[i], rC[i], path, sigma, true))
            {
                path.erase(path.end() - 1);
                found = true;
            }
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
        if (!sigma.contains(rule->_name))
        {
            sigma.extend(rule->_name, t);
        }
        return true;
    }
    else
    {
        if (!preMatched)
        {
            for (uint32_t i = 0; i < c1.size(); i++)
            {
                path.push_back(i);
                if (findMatch(c1[i], rule, path, sigma, false))
                {
                    return true;
                }
                else
                {
                    path.erase(path.end() - 1);
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

/////////////////////////////////////////////////////////////////
//
// match : a function to find the match between rule and term and gets the substitution
// @parms : t - term
// @parms : rule - which needs to match with term
// @parms : sigma - map to store the substitution details
// @parms : preMatched - flag to check weather the function of rule and term previously matched
//
/////////////////////////////////////////////////////////////////

template <typename T>
bool match(term_ptr<T> t, term_ptr<T> rule, vector<int> &path, Sub<T> &sigma)
{
    bool found = false;

    found = findMatch(t, rule, path, sigma, false);

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
// operator << overloading
// @parms : out - output stream
// t      : term
//
/////////////////////////////////////////////////////////////////

template <typename T>
std::ostream &operator<<(std::ostream &out, const term<T> &t)
{
    t.print(out);
}

#endif // TERM_HPP
