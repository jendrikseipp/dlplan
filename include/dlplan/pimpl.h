// Code by Herb Sutter, taken from https://herbsutter.com/gotw/_101/
// We follow option 4 (10/10) described in https://herbsutter.com/gotw/_100/:
//   * Put all private nonvirtual members into impl;
//   * Furthermore, we also use passthrough from public interface
//     to avoid the backpointer to visible class.

#ifndef DLPLAN_INCLUDE_DLPLAN_PIMPL_H_
#define DLPLAN_INCLUDE_DLPLAN_PIMPL_H_

#include <memory>


namespace dlplan {

/**
 * Unique_ptr version without automatically generated copy constructor and copy assignment.
 */
template<typename T>
class pimpl {
private:
    std::unique_ptr<T> m;

public:

    pimpl() : m{ new T{} } { }

    // Variadic templates
    template<typename Arg1>
    pimpl( Arg1&& arg1 )
        : m( new T( std::forward<Arg1>(arg1) ) ) { }

    template<typename Arg1, typename Arg2>
    pimpl( Arg1&& arg1, Arg2&& arg2 )
        : m( new T( std::forward<Arg1>(arg1), std::forward<Arg2>(arg2) ) ) { }

    template<typename Arg1, typename Arg2, typename Arg3>
    pimpl( Arg1&& arg1, Arg2&& arg2, Arg3&& arg3 )
        : m( new T( std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3) ) ) { }

    // Copy constructor.
    pimpl(const pimpl<T> &other) : m( new T(*(other.m))) { }

    // Copy assignment operator
    pimpl<T>& operator=(const pimpl<T> &other) {
        if (this != &other) {
            m = std::unique_ptr<T>(new T(*(other.m)));
        }
        return *this;
    }

    ~pimpl() = default;

    T* operator->() { return m.get(); }

    T& operator*() { return *m.get(); }

    const T* operator->() const { return m.get(); }

    const T& operator*() const { return *m.get(); }
};

/**
 * Shared_ptr version where automatically generated copy constructor and copy assignment copies the shared_ptr.
 */
template<typename T>
class spimpl {
private:
    std::shared_ptr<T> m;

public:
    spimpl() : m{ new T{} } { }

    // Variadic templates
    template<typename Arg1>
    spimpl( Arg1&& arg1 )
        : m( new T( std::forward<Arg1>(arg1) ) ) { }

    template<typename Arg1, typename Arg2>
    spimpl( Arg1&& arg1, Arg2&& arg2 )
        : m( new T( std::forward<Arg1>(arg1), std::forward<Arg2>(arg2) ) ) { }

    template<typename Arg1, typename Arg2, typename Arg3>
    spimpl( Arg1&& arg1, Arg2&& arg2, Arg3&& arg3 )
        : m( new T( std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3) ) ) { }

    // A baseline copy constructor.
    // pimpl(const pimpl<T> &other) : m( new T(*(other.m))) { }

    ~spimpl() = default;

    T* operator->() { return m.get(); }

    T& operator*() { return *m.get(); }

    const T* operator->() const { return m.get(); }

    const T& operator*() const { return *m.get(); }
};


}

#endif