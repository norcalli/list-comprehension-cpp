#ifndef LAMBDA_UTILS_H_
#define LAMBDA_UTILS_H_

// TODO: Better way to tell if it is derived from a lambda (possible `is_base_of` `is_convertible`).

namespace lambda {
template<class T>
struct Lambda;

template<size_t n>
struct placeholder;

template<class T>
struct is_lambda_helper : std::false_type {};

template<class T>
struct is_lambda_helper<lambda::Lambda<T>> : std::true_type {};

template<size_t n>
struct is_lambda_helper<lambda::placeholder<n>> : std::true_type {};

template<class T>
using is_lambda = is_lambda_helper<typename std::remove_cv<typename std::remove_reference<T>::type>::type>;


template<size_t n>
struct Get {
  template<class... Args>
  inline auto operator()(Args&&... args) -> decltype(std::get<n>(args...)) {
    return std::get<n>(args...);
  }

  template<class... Args>
  inline auto operator()(Args&&... args) const -> decltype(std::get<n>(args...)) {
    return std::get<n>(args...);
  }
};

// TODO: `eval` can automatically support placeholders without the need for placeholders to actually inherit from `Lambda` by using it as a tag, so when eval runs into a placeholder, it automatically returns element `n` from the args passed to eval.

template<class Function, class... Args>
auto eval(Function fn, Args&&... args) -> decltype(fn(args...)) {
// auto eval(lambda::Lambda<Function>& fn, Args&&... args) -> decltype(fn(args...)) {
  // return fn(args...);
  return fn(std::forward<Args>(args)...);
}

template<class T, class... Args, class = typename std::enable_if<!lambda::is_lambda<T>::value>::type>
// auto eval(const T& t, Args&&... args) -> decltype(t) {
auto eval(T&& t, Args&&...) -> decltype(std::forward<T>(t)) {
  return std::forward<T>(t);
}

// template<class T, class... Args, class = typename std::enable_if<!lambda::is_lambda<T>::value>::type>
// auto eval(T& t, Args&&... args) -> decltype(t) {
//   return t;
// }

template<size_t n, class... Args>
typename std::tuple_element<n, std::tuple<Args...> >::type&&
eval(const lambda::placeholder<n>&, Args&&... args) {
  return std::get<n>(std::forward<Args>(args)...);
}

template<size_t n, class... Args>
typename std::tuple_element<n, std::tuple<Args...> >::type&&
eval(const lambda::Lambda<Get<n> >&, Args&&... args) {
  return std::get<n>(std::forward<Args>(args)...);
}

}

#endif  // LAMBDA_UTILS_H_
