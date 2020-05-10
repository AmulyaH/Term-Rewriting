#ifndef TERM_ITERATOR_HPP
#define TERM_ITERATOR_HPP

#include <exception>
#include <stack>
#include <iostream>
#include <memory>

using namespace std;

template <typename T>
class term;

//template <typename T>
//using term_ptr = std::shared_ptr<term<T>>;

template <typename T>
class term_iterator
{
private:
    stack<term<T> *> _terms;
    //stack<stack<int>> _paths;
    //term<T>* _root;

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
        //cout << "creating iterator" << "\n";
        uint32_t step = 0;
        //uint32_t currentStep = 0;
        if (begin)
        {
            vector<int> curPath;
            preorder(n,  curPath, 0) ;
            /*
            //cout << "begin" << "\n";
            if (n == nullptr)
                return;

            stack<term<T> *> local_stack;

            // start from root node (set current node to root node)
            term<T> *curr = n;

            vector<int> curPath;
            // run till stack is not empty or current is
            // not NULL
            while (!local_stack.empty() || curr != nullptr)
            {
                // Print left children while exist
                // and keep pushing right into the
                // stack.
                while (curr != nullptr)
                {
                    //curPath.push_back(step);
                    curr->_path = curPath;
                    _terms.push(curr);
                   
                    cout << "pushing " << curr->_value << " at " << step << " \n";
                    for (auto p : curPath)
                    {
                        cout << p << " ";
                    }
                    cout << endl;

                    //_paths.push(curPath);

                    if (curr->_two)
                    {
                        local_stack.push(curr->_two.get());
                    }

                    curr = curr->_one.get();
                    step = 1;

                
                    //currentStep = 1;
                    //step += currentStep;
                }

                // We reach when curr is NULL, so We
                // take out a right child from stack
                if (local_stack.empty() == false)
                {
                    curr = local_stack.top();
                    step = 2;
                    local_stack.pop();
                    
                    
                    //
                }
            }
            */
        }
    }

    void preorder(term<T> *term, vector<int> curPath, int step) 
{ 
    if (term == nullptr) 
        return; 
  
    if(step !=0) {
        curPath.push_back(step);
    }
    term->_path = curPath;
    _terms.push(term);

    preorder(term->_one.get(), curPath, 1);  

    preorder(term->_two.get(), curPath, 2); 
}  

    term<T> *operator*()
    {
        term<T> *t = _terms.top();
        return t;
    }

    term<T> *operator->() const
    {
        return _terms.top();
    }
    /*
    term_iterator<T> operator++(int)
    {
        term_iterator<T> tmp(*this);
        ++this;
        return tmp;
    }*/

    term_iterator<T> &operator++(int)
    {
        if (!_terms.empty())
        {
            if (_terms.top()->_two)
            {
                //cout << *_terms.top()->_two << endl;
                _terms.push(_terms.top()->_two.get());
                while (_terms.top()->_one)
                {
                    _terms.push(_terms.top()->_one.get());
                }
            }
            else
            {
                term<T> *child = _terms.top();
                _terms.pop();
                while (!_terms.empty() && _terms.top()->_two.get() == child)
                {
                    child = _terms.top();
                    _terms.pop();
                }
            }
        }
        return *this;
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

/*
template<typename T>
term_iterator<T>::term_iterator(term<T>* n, bool begin)
{
    _value = n;
    if(begin)
    {
        for(; n->_one; n = n->_one)
        {
            _terms.push(n);
        }
        _terms.push(n);
    }
}*/

/*
template<typename T>
term_iterator<T>& term_iterator<T> :: operator++() 
{
    
}*/

#endif // TERM_ITERATOR_HPP