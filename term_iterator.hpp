#ifndef TERM_ITERATOR_HPP
#define TERM_ITERATOR_HPP

#include <exception>
#include <stack>
#include <iostream>
#include <memory>

using namespace std;

template <typename T>
class term;

template <typename T>
using term_ptr = std::shared_ptr<term<T>>;

template <typename T>
class term_iterator
{
private:
    stack<term_ptr<T>> _path;
    //term_ptr<T> _root;

public:
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef term_iterator<T> iterator;
    typedef forward_iterator_tag iterator_category;

    term_iterator<T>() = delete;
/*
    term_iterator<T>(const term<T>& n, bool begin)
    { 
        //cout << "creating iterator" << "\n";
        uint32_t step = 0;
        //uint32_t currentStep = 0;
        if (begin)
        {
            //cout << "begin" << "\n";
            if (n == nullptr)
                return;

            stack<term_ptr<T>> local_stack;

            // start from root node (set current node to root node)
            term_ptr<T> curr = make_shared<term<T>>(n);

            // run till stack is not empty or current is
            // not NULL
            while (!local_stack.empty() || curr != nullptr)
            {
                // Print left children while exist
                // and keep pushing right into the
                // stack.
                while (curr != nullptr)
                {
                    //cout <<"pushing "<< curr->_value << " at "<< step << " \n";
                    _path.push(curr);

                    if (curr->_two)
                    {
                        local_stack.push(curr->_two);
                    }

                    curr = curr->_one;
                    step = 1;
                    //currentStep = 1;
                    //step += currentStep;
                }

                // We reach when curr is NULL, so We
                // take out a right child from stack
                if (local_stack.empty() == false)
                {
                    curr = local_stack.top();
                    step =2;
                    local_stack.pop();
                }
            }
        }}

*/
    term_iterator<T>(term_ptr<T> n, bool begin)
    { 
        //cout << "creating iterator" << "\n";
        uint32_t step = 0;
        //uint32_t currentStep = 0;
        if (begin)
        {
            //cout << "begin" << "\n";
            if (n == nullptr)
                return;

            stack<term_ptr<T>> local_stack;

            // start from root node (set current node to root node)
            term_ptr<T> curr = n;

            // run till stack is not empty or current is
            // not NULL
            while (!local_stack.empty() || curr != nullptr)
            {
                // Print left children while exist
                // and keep pushing right into the
                // stack.
                while (curr != nullptr)
                {
                    //cout <<"pushing "<< curr->_value << " at "<< step << " \n";
                    _path.push(curr);

                    if (curr->_two)
                    {
                        local_stack.push(curr->_two);
                    }

                    curr = curr->_one;
                    step = 1;
                    //currentStep = 1;
                    //step += currentStep;
                }

                // We reach when curr is NULL, so We
                // take out a right child from stack
                if (local_stack.empty() == false)
                {
                    curr = local_stack.top();
                    step =2;
                    local_stack.pop();
                }
            }
        }
        /* 
        if (begin)
        {
            for(; n->_one; n = n->_one)
        {
            _path.push(n);
        }
        _path.push(n);
        }
        */
        /*term_ptr<T> _root = n;
        //cout << "one = " << *_root->_one << ":  two = " << *_root->_two << endl;
        if (begin)
        {

            if (_root->_two) 
            {
                _path.push(_root->_two);
            }
              _path.push(_root) ;
           
  
            // Set root as root's left child   
            _root = _root->_one; 

            /*
            _path.push(_root);
            while(n->_one != nullptr)
            {
                _path.push(n->_one);
                //cout << *n->_one << endl;
                _path.push(n->_two);
                //cout << *n->_two << endl;
                 n = n->_one;
            }*/

        /*

            for (; n->_one; n = n->_one)
            {
                cout << *n << endl;
                _path.push(n);
            }
            / * n = _root;
            for (; n->_two; n = n->_two)
            {
                //cout << *n->_two << endl;
                _path.push(n);
            } * /
            //cout << *n << endl;
            //_path.push(n);
        }
       // _root = _path.top;
       */
    }

    term_ptr<T> &operator*()
    {
        term_ptr<T> &t = _path.top();
        ;
        return t;
    }

    term_ptr<T> operator->() const
    {
        return _path.top();
    }

    term_iterator<T> &operator++(int)
    {
        
        if (!_path.empty())
        {
            if (_path.top()->_two)
            {
                cout << *_path.top()->_two << endl;
                _path.push(_path.top()->_two);
                while (_path.top()->_one)
                {
                    _path.push(_path.top()->_one);
                }
            }
            else
            {
                term_ptr<T> child = _path.top();
                _path.pop();
                while (!_path.empty() && _path.top()->_two == child)
                {
                    child = _path.top();
                    _path.pop();
                }
            }
        }
        return *this;
    }

    /*
        term_iterator<T> operator++(int)
        {
            term_iterator<T> tmp(*this);
            ++this;
            return tmp;
        }*/

    term_iterator<T> &operator+=(unsigned int n)
    {
        for (int i = 0; i < n; i++)
            ++*this;
        return *this;
    }

    bool operator==(const term_iterator<T> &rhs) const
    {
        return _path == rhs._path;
    }

    bool operator!=(const term_iterator<T> &rhs) const
    {
        return _path != rhs._path;
    }
};

/*
template<typename T>
term_iterator<T>::term_iterator(term_ptr<T> n, bool begin)
{
    _value = n;
    if(begin)
    {
        for(; n->_one; n = n->_one)
        {
            _path.push(n);
        }
        _path.push(n);
    }
}*/

/*
template<typename T>
term_iterator<T>& term_iterator<T> :: operator++() 
{
    
}*/

#endif // TERM_ITERATOR_HPP