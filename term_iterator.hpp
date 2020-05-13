#ifndef TERM_ITERATOR_HPP
#define TERM_ITERATOR_HPP

#include <exception>
#include <queue>
#include <iostream>
#include <memory>

using namespace std;

template <typename T>
class term;

template <typename T>
class term_iterator
{
private:
    /// contents of current iterator
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

    /**
     * Constructor 
     *  @param inputTerm   term for which iterator is constructed
     *  @param begin       if true, begin constructor is constructed else end constructor is constrcuted.
     */
    term_iterator<T>(term<T> *inputTerm, bool begin)
    {
        uint32_t step = 0;
        if (begin)
        {
            levelOrder(inputTerm);
        }
    }

    // Copy Constructor
    term_iterator<T>(const term_iterator<T> &input) : _terms(input._terms) {}

    /**
     * Level order traversal to populate subterms in the term
     * @param term  term on which traversal is operated
     * 
     */
    void levelOrder(term<T> *term)
    {
        if (term == nullptr)
            return;

        _terms.push(term);

        std::vector<term_ptr<T>> children = term->getChildren();

        for (uint32_t ch = 0; ch < children.size(); ch++)
        {
            term_ptr<T> child = children[ch];
            levelOrder(child.get());
        }
    }

    term_iterator<T> &operator++()
    {
        if (!_terms.empty())
        {
            _terms.pop();
        }
        return *this;
    }

    term_iterator<T> operator++(int)
    {
        term_iterator<T> next(*this);
        ++*this;
        return next;
    }

    term<T> &operator*()
    {
        term<T> *firstTerm = _terms.front();
        return *firstTerm;
    }

    term<T> *operator->() const
    {
        return _terms.front();
    }

    term_iterator<T> &operator+=(unsigned int size)
    {
        for (int i = 0; i < size; i++)
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

#endif // TERM_ITERATOR_HPP