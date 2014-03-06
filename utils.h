#ifndef UTILS_H_
#define UTILS_H_

constexpr bool any() { return false; }

constexpr bool all() { return true; }

template<class T, class... Args>
constexpr bool any(T&& t, Args&&... args) {
  return t || any(std::forward<T>(args)...);
}

template<class T, class... Args>
constexpr bool all(T&& t, Args&&... args) {
  return t && all(std::forward<T>(args)...);
}

template<bool C, class U = void>
using enable_if = typename std::enable_if<C, U>::type;


#endif  // UTILS_H_
