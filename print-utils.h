#ifndef PRINT_UTILS_H_
#define PRINT_UTILS_H_

template<std::size_t N>
struct TuplePrinter {
  template<class Tuple>
  static void print(std::ostream& stream, const Tuple& t) {
    TuplePrinter<N-1>::print(stream, t);
    stream << ", " << std::get<N-1>(t);
  }
};

template<>
struct TuplePrinter<1> {
  template<class Tuple>
  static void print(std::ostream& stream, const Tuple& t) {
    stream << std::get<0>(t);
  }
};

template<class... Args>
std::ostream& operator<<(std::ostream& stream, std::tuple<Args...>&& v) {
  // using tuple_type = std::remove_reference<decltype(v)>::type;
  // constexpr auto tuple_size = std::tuple_size<tuple_type>::value;
  stream << '(';
  TuplePrinter<sizeof...(Args)>::print(stream, v);
  return stream << ')';
}


#endif  // PRINT_UTILS_H_
