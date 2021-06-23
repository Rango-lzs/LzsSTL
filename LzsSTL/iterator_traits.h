#ifndef ITERATOR_TRAITS_H
#define ITERATOR_TRAITS_H

//__type_traits can get the iterator's asociate type  by nested typename definitin
// but when the iterator is raw pointer, it should be partial specified

template<typename iterator>
struct iterator_traits{
    typedef typename    iterator::value_type                value_type;
    typedef typename    iterator::pointer_type              pointer_type;
    typedef typename    iterator::const_value_type          const_value_type;
    typedef typename    iterator::const_pointer_type        const_pointer_type;
    typedef typename    iterator::reference_type            reference_type;
    typedef typename    iterator::const_reference_type      const_reference_type;
    typedef typename    iterator::pointer_different_type    pointer_different_type;
};


//partial specification for raw pointer and const raw pointer
template <typename T>
struct iterator_traits<T*>{
    typedef typename    T           value_type;
    typedef typename    T*          pointer_type;
    typedef typename    const T     const_value_type;
    typedef typename    const T*    const_pointer_type;
    typedef typename    T&          reference_type;
    typedef typename    const T&    const_reference_type;
    typedef typename    ptrdiff_t   pointer_different_type; 
};

template <typename T>
struct iterator_traits<const T*>{
    typedef typename    T           value_type;
    typedef typename    T*          pointer_type;
    typedef typename    const T     const_value_type;
    typedef typename    const T*    const_pointer_type;
    typedef typename    T&          reference_type;
    typedef typename    const T&    const_reference_type;
    typedef typename    ptrdiff_t   pointer_different_type; 
};

#endif
