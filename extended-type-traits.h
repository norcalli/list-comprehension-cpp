#ifndef EXTENDED_TYPE_TRAITS_H_
#define EXTENDED_TYPE_TRAITS_H_

template<class Function, class... Args>
struct is_callable {
  // template<class... Params>
  // static char test(int, decltype(std::declval<Function>()(std::declval<Params>()...))* = 0); // Not everything has a pointer...?
  template<class... Params, class = decltype(std::declval<Function>()(std::declval<Params>()...))>
  static char test(int);

  template<class... Params>
  static int test(...);

  static const bool value = sizeof(test<Args...>(0)) == 1;
};

#endif  // EXTENDED_TYPE_TRAITS_H_
