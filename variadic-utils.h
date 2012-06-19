#ifndef VARIADIC_UTILS_H_
#define VARIADIC_UTILS_H_

#include <type_traits>
#include "tuple-utils.h"

namespace std {

namespace details {

template<size_t n>
struct get_helper {
  // template<class T, class... Args>
  // inline typename std::tuple_element<n, std::tuple<T, Args...>>::type& operator()(T& t, Args&... args) {
  //   return get_helper<n-1>()(args...);
  // }
  // template<class T, class... Args>
  // inline const typename std::tuple_element<n, std::tuple<T, Args...>>::type& operator()(const T& t, const Args&... args) {
  //   return get_helper<n-1>()(args...);
  // }
  template<class T, class... Args>
  inline typename std::tuple_element<n, std::tuple<T, Args...>>::type&& operator()(T&& t, Args&&... args) {
    return get_helper<n-1>()(args...);
  }
};

template<>
struct get_helper<0> {
  // template<class T, class... Args>
  // inline T& operator()(T& t, Args&... args) {
  //   return t;
  // }
  // template<class T, class... Args>
  // inline const T& operator()(const T& t, const Args&... args) {
  //   return t;
  // }
  template<class T, class... Args>
  inline T&& operator()(T&& t, Args&&... args) {
    return t;
  }
};

}

template<size_t n, class T, class U, class... Args>
inline typename std::tuple_element<n, std::tuple<T, U, Args...>>::type&
    get(T& t, U& u, Args&... args) {
  return std::details::get_helper<n>()(t, u, args...);
  // return std::get<n>(std::tie(t, u, args...));
}

template<size_t n, class T, class U, class... Args>
inline const typename std::tuple_element<n, std::tuple<T, U, Args...>>::type&
    get(const T& t, const U& u, const Args&... args) {
  return std::details::get_helper<n>()(t, u, args...);
  // return std::get<n>(std::forward_as_tuple(t, u, args...));
}

template<size_t n, class T>
// inline typename std::enable_if<!is_tuple<T>::value
//                                && !std::is_array<T>::value, T>::type& get(T& t) {
T& get(T& t) {
  static_assert(n == 0, "Don't be stupid.");
  return t;
}

template<size_t n, class T>
// inline const typename std::enable_if<!is_tuple<T>::value
//                                && !std::is_array<T>::value, T>::type& get(const T& t) {
const T& get(const T& t) {
  static_assert(n == 0, "Don't be stupid.");
  return t;
}

// template<size_t n, class T, class U, class... Args>
// inline typename std::tuple_element<n, std::tuple<T, U, Args...>>::type&&
//     get(T&& t, U&& u, Args&&... args) {
//   return std::details::get_helper<n>()(t, u, args...);
//   // return std::get<n>(std::forward_as_tuple(t, u, args...));
// }

}

#endif  // VARIADIC_UTILS_H_
