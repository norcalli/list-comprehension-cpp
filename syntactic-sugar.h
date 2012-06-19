#ifndef SYNTACTIC_SUGAR_H_
#define SYNTACTIC_SUGAR_H_

template<class T, class... Args>
std::tuple<T, Args...> operator,(const T& t, std::tuple<Args...>&& tuple) {
  return std::tuple_cat(std::make_tuple(t), tuple);
}

template<class T, class... Args>
std::tuple<T&, Args...> operator,(T& t, std::tuple<Args...>&& tuple) {
  return std::tuple_cat(std::tie(t), tuple);
}

template<class T, class U>
auto operator,(T& t, U& u) -> decltype(std::tie(t, u)) {
  return std::tie(t, u);
}

template<class T, class U>
auto operator,(const T& t, const U& u) -> decltype(std::make_tuple(t, u)) {
  return std::make_tuple(t, u);
}


// template<class T, class... Args>
// std::tuple<T, Args...> operator,(T&& t, Args&&... args) {
//   return std::make_tuple(t, args...);
// }

// template<class T, class... Args>
// std::tuple<T, Args...> operator,(T& t, Args&... args) {
//   return std::tie(t, args...);
// }

template<class... Args>
auto t(Args&&... args) -> decltype(std::make_tuple(std::forward<Args>(args)...)) {
  return std::make_tuple(std::forward<Args>(args)...);
}

#endif  // SYNTACTIC_SUGAR_H_
