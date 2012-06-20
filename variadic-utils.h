#ifndef VARIADIC_UTILS_H_
#define VARIADIC_UTILS_H_

#include <type_traits>
#include "tuple-utils.h"

namespace std {

// namespace details {

// template<size_t n, class T, class... Args>
// struct variadic_element_helper {
//   using type = typename variadic_element_helper<n-1, Args...>::type;
// };

// template<class T, class... Args>
// struct variadic_element_helper<0, T, Args...> {
//   using type = T;
// };

// }

template<size_t n, class... Args>
using variadic_element = typename std::tuple_element<n, std::tuple<Args...>>::type;
// using variadic_element = typename std::details::variadic_element_helper<n, Args...>::type;

namespace details {

template<size_t n>
struct get_helper {
  template<class T, class... Args>
  // inline typename std::tuple_element<n, std::tuple<T, Args...>>::type&&
  inline variadic_element<n, T, Args...>&&
      operator()(T&& t, Args&&... args) {
    return get_helper<n-1>()(std::forward<Args>(args)...);
  }
};

template<>
struct get_helper<0> {
  template<class T, class... Args>
  inline T&& operator()(T&& t, Args&&... args) {
    return std::forward<T>(t);
  }
};

}

// TODO: Why do I have this structure for get? couldn't I just have the one?
template<size_t n, class... Args>
inline variadic_element<n, Args...>&& get(Args&&... args) {
  return std::details::get_helper<n>()(std::forward<Args>(args)...);
}

// template<size_t n, class T, class U, class... Args>
// // inline typename std::tuple_element<n, std::tuple<T, U, Args...>>::type&&
// inline variadic_element<n, T, U, Args...>&&
//     get(T&& t, U&& u, Args&&... args) {
//   return std::details::get_helper<n>()(std::forward<T>(t), std::forward<U>(u), std::forward<Args>(args)...);
// }

// template<size_t n, class T>
// T&& get(T&& t) {
//   static_assert(n == 0, "Don't be stupid.");
//   return std::forward<T>(t);
// }

}

#endif  // VARIADIC_UTILS_H_
