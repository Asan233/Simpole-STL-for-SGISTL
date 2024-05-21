#ifndef __ALGO_H
#define __ALGO_H

#include "stl_iterator.h"
#include "type_traits.h"
#include <cstring>


/*****copy*****/
template <class RandomAccessIterator, class OutputIterator, class Distance>
inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance *)
{
    for(Distance n = last - first; n > 0; --n, ++first, ++result)
        *result = *first;

    return result;
}

// 指针所指对象具备默认赋值操作符
template <class T>
inline T* __copy_t(T* first, T* last, T* result, __true_type) {
    memmove(result, first, sizeof(T) * (last - first));
    return result + (last - first);
}

// 指针所指对象不具备默认复制操作
template <class T>
inline T* __copy_t(T* first, T* last, T* result, __false_type) {
    return __copy_d(first, last, result, reinterpret_cast<ptrdiff_t *>(0));
}

// InputIterator实例方法
template <class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
    for(; first != last; ++result, ++first) {
        *result = *first;
    }
    return result;
}

// RandomAccesIterator实例方法
template <class RandomAccessIterator, class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_iterator_tag) {
    return __copy_d(first, last, result, distance_type(first) );
}

// 完全泛化，根据first的迭代器类型不同，使用不用的_copy()
template <class InputIterator, class OutputIterator>
struct _copy {
    OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
        return __copy(first, last, result, iterator_category(first));
    }
};

// 特例化T*, T*
template <class T>
struct _copy<T*, T*>
{
    T* operator()(T* first, T* last, T* result)
    {
        // 指针是否有默认的赋值操作
        typedef typename __type_traits<T>::has_trivial_copy_constructor has_trivial_copy_constructor;
        return __copy_t(first, last, result, has_trivial_copy_constructor());
    }
};

// 完全泛化
template <class InputIterator, class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
    return _copy<InputIterator, OutputIterator>()(first, last, result);
}


/**********copy_backward*********/
template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result, bidirectional_iterator_tag) {
    while(last != first) {
        *(--result) = *(--last);
    }
    return result;
}

template<class BidirectionalIterator1, class BidirectionalIterator2>
struct _copy_backward
{
    BidirectionalIterator2 operator()(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result)
    { return __copy_backward( first, last, result, iterator_category(first) ); }
};

template <class BidrectionalIterator1, class BidrectionalIterator2>
inline BidrectionalIterator2 copy_backward(BidrectionalIterator1 first, BidrectionalIterator1 last, BidrectionalIterator2 result) {
    return _copy_backward<BidrectionalIterator1, BidrectionalIterator2>() (first, last, result);
}


#endif
