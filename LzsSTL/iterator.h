#ifndef ITERATOR_HH
#define ITERATOR_HH

#include "iterator_traits.h"

struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag: public input_iterator_tag {};
struct bidirectional_iteartor_tag: public forward_iterator_tag{}
struct random_iterator_tag: public bidirectional_iterator_tag{};

// what is Distance 
template<class T, class Distance>
class input_iterator
{
    typedef input_iterator_tag iterator_category;
    typedef T  value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef Distance difference_type;
};


template<class Category, class T, class Distace=ptrdiff_t,
    class Pointer=T*, class Reference= T&>
struct iterator
{
    
    typedef Category    iterator_category;
    typedef T           value_type;
    typedef Distance    difference_type;
    typedef Pointer     pointer;
    typedef Reference   reference;
};


template<class Iterator>
inline typename iterator_traits<Iterator>::itrerator_category
iteratory_category(const Iterator &){

    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
}

// implemet advance by function overload throuth the category
template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type 
distance(InputIterator first, InputIterator last)
{
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    __distance(InputIterator first,InputIterator last, category());
}

#endif
