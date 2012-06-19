#ifndef TUPLE_UTILS_H_
#define TUPLE_UTILS_H_

#include <type_traits>

template<int ...>
struct tuple_indices { };

template<int N, int... S>
struct gens : gens<N-1, N-1, S...> { };

template<int... S>
struct gens<0, S...> {
  using type = tuple_indices<S...>;
};

// TODO: This is a workaround for clang because sizeof... reports inaccurately, I believe that
// gcc accepts
// template<class... Args>
// using make_tuple_indices = typename gens<sizeof...(Args)>::type;
template<class... Args>
using make_tuple_indices = typename gens<std::tuple_size<std::tuple<Args...>>::value>::type;

// template<class T>
// using make_tuple_indices = typename gens<std::tuple_size<T>::value>::type;

template<class T>
struct is_tuple_helper : std::false_type {};

template<class... Args>
struct is_tuple_helper<std::tuple<Args...>> : std::true_type {};

template<class T>
using is_tuple = is_tuple_helper<std::remove_reference<T>>;

template<class T, class U = T>
using Tuple = typename std::enable_if<is_tuple<T>::value, U>::type;

#endif  // TUPLE_UTILS_H_
