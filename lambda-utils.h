#ifndef LAMBDA_UTILS_H_
#define LAMBDA_UTILS_H_

#include "utils.h"
#include "extended-type-traits.h"
// TODO: Better way to tell if it is derived from a lambda (possible `is_base_of` `is_convertible`).

namespace lambda {

namespace templates {

template<class Param, class Function>
struct phoenix_unary;

template<class Left, class Right, class Function>
struct phoenix_binary;

template<class Function, class... Args>
struct phoenix_nary;

}

template<class T>
struct Lambda;

template<size_t n>
struct placeholder;

template<class T>
struct Constant;

template<class T>
struct is_lambda_helper : std::false_type {};

template<class T>
struct is_lambda_helper<lambda::Lambda<T>> : std::true_type {};

template<size_t n>
struct is_lambda_helper<lambda::placeholder<n>> : std::true_type {};

template<class T>
struct is_lambda_helper<lambda::Constant<T>> : std::true_type {};

template<class T>
using is_lambda = is_lambda_helper<typename std::remove_cv<typename std::remove_reference<T>::type>::type>;

template<class... Args>
struct any_lambdas : public std::integral_constant<bool, any(lambda::is_lambda<Args>::value...)> {};

// template<class Function, class Template, class... Args>
// using LambdaResult = typename std::enable_if<lambda::any_lambdas<Args...>, lambda::Lambda<Template<Args..., Function> > >

// TODO: Reorder template parameters to phoenix's to make Function the first one.
template<class Function, class... Args>
using BinaryLambda = typename std::enable_if<lambda::any_lambdas<Args...>::value, lambda::Lambda<lambda::templates::phoenix_binary<Args..., Function> > >::type;

template<class Function, class... Args>
using UnaryLambda = typename std::enable_if<lambda::any_lambdas<Args...>::value, lambda::Lambda<lambda::templates::phoenix_unary<Args..., Function> > >::type;

template<class Function, class... Args>
using NaryLambda = typename std::enable_if<lambda::any_lambdas<Args...>::value, lambda::Lambda<lambda::templates::phoenix_nary<Function, Args...> > >::type;

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

// template<class Function, class T, class... Args>
// auto eval(Function fn, T&& t, Args&&... args) -> decltype(fn(t, args...)) {
// // auto eval(lambda::Lambda<Function>& fn, Args&&... args) -> decltype(fn(args...)) {
//   // return fn(args...);
//   return fn(std::forward<T>(t), std::forward<Args>(args)...);
// }

// TODO: Functions other than lambdas shouldn't be evaluated right?
// template<class Function, class... Args, typename std::enable_if<lambda::is_lambda<Function>::value || is_callable<Function, Args...>::value, int>::type = 0>
template<class Function, class... Args, typename std::enable_if<is_callable<Function, Args...>::value, int>::type = 0>
auto eval(Function fn, Args&&... args) -> decltype(fn(args...)) {
// auto eval(lambda::Lambda<Function>& fn, Args&&... args) -> decltype(fn(args...)) {
  // return fn(args...);
  return fn(std::forward<Args>(args)...);
}

// template<class Function, class... Args, class = typename std::enable_if<lambda::is_lambda<Function>::value>::type>
// auto eval(Function fn, Args&&... args) -> decltype(fn(args...)) {
// // auto eval(lambda::Lambda<Function>& fn, Args&&... args) -> decltype(fn(args...)) {
//   // return fn(args...);
//   return fn(std::forward<Args>(args)...);
// }

// template<class Function, class... Args, typename std::enable_if<lambda::is_lambda<Function>::value, int>::type = 0>
// auto eval(Function fn, Args&&... args) -> decltype(fn(args...)) {
//   std::cout << is_callable<Function, Args...>::value << std::endl;
//   return fn(std::forward<Args>(args)...);
// }

// template<class Function>
// auto eval(Function fn) -> decltype(fn()) {
// // auto eval(lambda::Lambda<Function>& fn, Args&&... args) -> decltype(fn(args...)) {
//   // return fn(args...);
//   return fn();
// }

// template<class T, class... Args, class = typename std::enable_if<!lambda::is_lambda<T>::value>::type>
template<class T, class... Args, typename std::enable_if<!is_callable<T>::value, int>::type = 0>
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
