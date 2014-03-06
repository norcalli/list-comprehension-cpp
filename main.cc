#include <iostream>
#include <functional>
#include <tuple>
#include "lambda.h"
#include "print-utils.h"
#include "variadic-utils.h"
#include "syntactic-sugar.h"

#include <cmath>

// #define DOUT(x) std::cout << #x " = " << (x) << std::endl;
#define DOUT(x) std::cout << #x " = " << (x) << '\n';

using namespace lambda;

std::string operator "" _s(const char* s, size_t n) { return std::string(s); }

struct Notifier {
  
};

struct Times2 {
  template<class T>
  T operator()(const T& t) {
    return t*2;
  }
};

//////////

template<class Function>
std::string str(const Lambda<Function>& fn);

template<class Arg, class Function>
std::string str(const lambda::templates::phoenix_unary<Arg, Function>& f);

template<class Left, class Right, class Function>
std::string str(const lambda::templates::phoenix_binary_left<Left, Right, Function>& f);

template<class Left, class Right, class Function>
std::string str(const lambda::templates::phoenix_binary_right<Left, Right, Function>& f);

template<class Left, class Right, class Function>
std::string str(const lambda::templates::phoenix_binary<Left, Right, Function>& f);

template<class Function, class... Args>
std::string str(const lambda::templates::phoenix_nary<Function, Args...>& f);

template<class T, class = enable_if<std::is_fundamental<T>::value> >
std::string str(const T& t) {
  return std::to_string(t);
}

template<class T, int = sizeof(decltype(std::declval<T>().str()))>
std::string str(const T& t) {
  return t.str();
}

template<size_t n>
std::string str(const lambda::placeholder<n>&) {
  return std::string("_") + char('1' + n);
}

template<size_t n>
std::string str(const lambda::Lambda<lambda::Get<n> >&) {
  return std::string("_") + char('1' + n);
}

template<class Arg, class Function>
std::string str(const lambda::templates::phoenix_unary<Arg, Function>& f) {
  return f.fn.str(str(f.param));
}

template<class Left, class Right, class Function>
std::string str(const lambda::templates::phoenix_binary_left<Left, Right, Function>& f) {
  return f.fn.str(str(f.expr), str(f.right));
}

template<class Left, class Right, class Function>
std::string str(const lambda::templates::phoenix_binary_right<Left, Right, Function>& f) {
  return f.fn.str(str(f.left), str(f.expr));
}

template<class Left, class Right, class Function>
std::string str(const lambda::templates::phoenix_binary<Left, Right, Function>& f) {
  return f.fn.str(str(f.left), str(f.right));
}

template<class Function, class... Args, int... idx>
std::string str_helper(const lambda::templates::phoenix_nary<Function, Args...>& f, tuple_indices<idx...>) {
  return f.fn.str(str(std::get<idx>(f.params))...);
}

template<class Function, class... Args>
std::string str(const lambda::templates::phoenix_nary<Function, Args...>& f) {
  return str_helper(f, make_tuple_indices<Args...>());
}

template<class Function>
std::string str(const Lambda<Function>& fn) {
  return str(fn.f);
}

// template<class T, class... Args, class = typename std::enable_if<!lambda::is_lambda<T>::value>::type>

struct Eval {
  template<class... Args>
  auto operator()(Args&&... args) -> decltype(eval(args...)) const {
    return eval(args...);
  }
};

template <class... t_args>
struct storage;

template <class t_arg>
struct storage<t_arg> {
  storage(t_arg t) : m_var(t) {}

  template<class t_func, class... tt_args>
  inline auto operator()(t_func fn, tt_args... p) -> decltype(fn(p..., std::declval<t_arg>())) {
    return fn(p..., m_var);
  }

  t_arg m_var;
};

template <class t_arg, class t_arg2, class... t_args>
struct storage<t_arg, t_arg2, t_args...> {
  storage(t_arg t, t_arg2 t2, t_args... p) : m_var(t), m_storage(t2, p...) {}

  template<class t_func, class... tt_args>
  inline auto operator()(t_func fn, tt_args... p)
      -> decltype(std::declval<storage<t_arg2, t_args...>>()(fn, p..., std::declval<t_arg>())) {
    return m_storage(fn, p..., m_var);
  }

  t_arg m_var;
  storage<t_arg2, t_args...> m_storage;
};

template<class... Args>
storage<Args...> make_storage(Args&&... args) {
  return storage<Args...>(args...);
}

template <class... t_args>
struct lambda_storage;

template <class t_arg>
struct lambda_storage<t_arg> {
  lambda_storage(t_arg t) : m_var(t) {}

  template<class Function, class Evaluator, class... tt_args>
  inline auto operator()(Function fn, const Evaluator& e, tt_args... p)
      -> decltype(fn(p..., e(std::declval<t_arg>()))) {
    return fn(p..., e(m_var));
  }

  t_arg m_var;
};

template <class t_arg, class t_arg2, class... Params>
struct lambda_storage<t_arg, t_arg2, Params...> {
  lambda_storage(t_arg t, t_arg2 t2, Params... p) : m_var(t), m_storage(t2, p...) {}

  template<class Function, class Evaluator, class... Args>
  inline auto operator()(Function fn, const Evaluator& e, Args... args)
      -> decltype(std::declval<lambda_storage<t_arg2, Params...>>()(fn, e, args..., e(std::declval<t_arg>()))) {
    return m_storage(fn, e, args..., e(m_var));
  }

  t_arg m_var;
  lambda_storage<t_arg2, Params...> m_storage;
};

// TODO: Possible to generalize for postincrement?

// template<class Function, class... Args>
// auto Evaluate(Function fn, Args&&... args) -> decltype(fn(args...)) {
//   return fn(args...);
// }

// std::string join(const std::string& j, std::initializer_list<std::string> array) {
//   std::string result;
//   auto it = std::begin(array), end = std::end(array);
//   result += *it;
//   while (++it != end) {
//     result += j;
//     result += *it;
//   }
//   return result;
// }

struct Evaluator {
  template<class Function, class... Args>
  auto operator()(Function fn, Args&&... args) -> decltype(fn(args...)) {
    return fn(args...);
  }
  template<class... Strings>
  std::string str(Strings... strings) const {
    return std::string("lambda::bind(") + join(", ", {strings...}) + ")";
  }
};

struct threesum {
  template<class A, class B, class C>
  auto operator()(A a, B b, C c) -> decltype(a+b+c) {
    return a + b + c;
  }
};

NARY_LAMBDA(threesum, sum3);
NARY_LAMBDA(Evaluator, Evaluate);

// TODO: Binary function evaluation.

template<class Name, class... Args>
lambda::templates::phoenix_nary<Name, Args...> make_phoenix(Args&&... args) {
  return lambda::templates::phoenix_nary<Name, Args...>(std::forward<Args>(args)...);
}

template<class T>
std::ostream& operator<<(std::ostream& stream, const Lambda<T>& l) {
  return stream << str(l);
}

template<class T>
auto identity(T&& t) -> decltype(std::forward<T>(t)) {
  return std::forward<T>(t);
}

template<class T>
void ido(T&& t) {
  t = 123;
}


template<class... Args>
bool lambdas_test(Args&&...) {
  return any_lambdas<Args...>::value;
}

// TODO: Rename MACROS as NARY_WRAPPER when they include the functor definition.
// TODO: Adapters and wrappers, wrapper + adapter = nary_lambda

// TODO: Epsilon signalling tag that is just the identity function, turns whatever it interacts with into a lambda

#include <cstdlib>

std::string adjectives[] = {"stupid", "great", "lame"};

// TODO: rename Evaluate to lambda::bind
// TODO: add operator[] support
// TODO: Update all macros and functions to accept rvalues and std::forward them.
#include <random>

namespace std {

template<class T>
auto size(T&& t) -> decltype(t.size()) {
  return t.size();
}

// template<class T, size_t n>
// size_t size(T array[n]) {
//   return n;
// }

template<class T, size_t n>
constexpr size_t size(T&&) {
  return 1;
}

template<class T>
constexpr auto size(T&& array) -> decltype(sizeof(array)/sizeof(array[0])) {
  return sizeof(array)/sizeof(array[0]);
}

}

// TODO: Generator is iterator that calls generator function.
// TODO: Implement yield ideal.
// TODO: List comprehension syntax.
// TODO: bool conversion operator?
// TODO: Template conversion operator for lambdas?

#include "stats.h"

// Possible syntaxes:
// list = [x*x for x in list if x < 3]
// To only apply list comprehension when it is used, can add implicit conversion
// operator which only converts to iterables.
// Apply filter and map to pair of iterators or iterable object.
// list = list | map(_1*_1) | if(_1 < 3)
// list = map(_1*_1) | list | if(_1 < 3)
// map(_1*_1) | if(_1 < 3) | in({0, 1, 2, 3, 4, 5})

struct MapFilter {
  
};

#include "extended-type-traits.h"

template<class Function, class... Args>
bool CallableTest(Function, Args...) {
  return is_callable<Function, Args...>::value;
}

int main(int argc, char *argv[]) {
  const auto pi = atan(1)*4;

  std::random_device rd;
  std::default_random_engine re(rd());
  std::uniform_int_distribution<> dist{0, std::size(adjectives) - 1};

  auto rando = [&](){ return dist(re); };
  auto moo = std::bind(rando);
  auto boo = lambda::bind(rando);
  DOUT(CallableTest(rando, 4));
  DOUT(CallableTest(rando));
  DOUT(CallableTest(moo, 4));
  DOUT(CallableTest(boo));
  DOUT(is_callable<decltype(boo)>::value);
  DOUT(CallableTest(4, 5));
  DOUT(CallableTest(std::sin(_1), 5));
  DOUT(CallableTest(lambda::bind(functors::Sin(), pi/6), 5));
  DOUT(CallableTest(_1, 5));
  DOUT(CallableTest(_1*2, 5));
  DOUT(CallableTest(lambda::ref(std::cout), 5));
  DOUT(lambda::bind(functors::Sin(), pi/6)());
  DOUT(moo());
  DOUT(boo());                          // TODO: Figure out why you can't pass no arguments (to nary). Guess: eval is being called with a single argument, and just being returned (correct).
  DOUT(boo(4));
  srand(time(0));
  std::cout << std::boolalpha;
  // auto greet = lambda::ref(std::cout) << "Hello: " << _1 << ", nice to meet you. " << _1 << " is a " << lambda::ref(adjectives)[lambda::bind(rand) % 3] << " name.\n";
  auto greet = lambda::ref(std::cout) << "Hello: " << _1 << ", nice to meet you. " << _1 << " is a " << lambda::ref(adjectives)[lambda::bind(rando)] << " name\n";
  // auto greet = lambda::ref(std::cout) << "Hello: " << _1 << ", nice to meet you. " << _1 << " is a " << adjectives[rand() % 3] << " name\n";
  
  int ref_test = 9;
  DOUT(&ref_test);
  DOUT(&lambda::ref(ref_test)());
  DOUT(&ref_test == (&lambda::ref(ref_test))());
  DOUT(&ref_test == &lambda::ref(ref_test)());

  greet("john 117");
  greet("343 guilty spark");
  greet(80085);

  DOUT(lambdas_test(_1, 4));
  DOUT(lambdas_test(1, 4));
  DOUT(lambdas_test(_1, _2));
  
  int test = 8;
  DOUT(test == identity(test));
  DOUT(&test == &identity(test));
  identity(test) = 5;
  DOUT(&test == &identity(test));
  DOUT(identity(test));
  ido(test);
  ido(584);
  DOUT(test);
  auto p1 = make_phoenix<threesum>(_1, _2, _1*_2);

  DOUT(p1(1.1, 1.2));
  // auto p1 = phoenix_nary<threesum, double, double>(1.1, 1.2);
  // auto p2 = make_lambda(p1);
  // DOUT(p2(_1, _2, _1*_2));
 
  auto aa = sum3(_1, _2, _1*_2);
  DOUT(aa(1.1, 1.2));
  DOUT(1.1 + 1.2 + 1.1*1.2);
  auto asdf = std::make_tuple(_1, _2, _1*_2);
  DOUT(eval(5, 1));
  DOUT(eval(_1 * 2, 1));
  auto kk = std::bind(functors::Sqrt(), _1);
  auto ev = Evaluate(_1, _2);
  DOUT(ev);
  DOUT(ev(functors::Sqrt(), 5));
  auto t1 = [](double d) { return d * 2; };
  auto t2 = _1 * 2;
  auto t3 = Times2();

  auto t4 = std::sqrt(_1) + _1*4;
  auto t5 = 4.4 * _2;
  DOUT(t4);
  DOUT(t5);

  auto and_comp = 3 <= _1 && _1 <= 9;
  // TODO: investigate what happens when you reassign, and/or assign to a lambda
  // Do I want the expression that the lambda is pointing to to be replaced?
  // Analysis: Uses the assignment operator and returns an expression assigning to the current expression.
  auto t6 = t4 < t5;
  DOUT(and_comp);
  DOUT(t6);
  
#if 1
  // std::cout << t(t4, t5, t6, and_comp) << std::endl;
  for (int i = 0; i < 20; i++) {
    std::cout << t(t4(i,i), t5(i,i), t6(i,i), and_comp(i,i)) << std::endl;
    // std::cout << t4 << " = " << t4(i, i) << std::endl;
    // std::cout << t5 << " = " << t5(i, i) << std::endl;
    // std::cout << t6 << " = " << t6(i, i) << std::endl;
    // std::cout << and_comp << " = " << and_comp(i, i) << std::endl;
  }

  DOUT(t1(6));
  DOUT(t2(6));
  DOUT(t3(6));
  
  const lambda::placeholder<0> x;
  const lambda::placeholder<1> y;

  auto hypot = std::sqrt(x*x + y*y);
  auto hypot2 = std::hypot(x, y);
  DOUT(hypot(1, 1));
  DOUT(hypot2(1, 1));
  int alpha = 59;
  DOUT(alpha);
  DOUT(&(alpha));
  DOUT((&_1)(alpha));
  DOUT((&(_1 = 5))(alpha));
  DOUT((alpha = 4) = 2 * alpha);
  DOUT((alpha = 4) += 2 * alpha);
  DOUT(((_1 = 4) = 2*_1)(alpha));
  DOUT(((_1 = 4) += 2*_1)(alpha));
  DOUT(&_1);
  DOUT(&(_1 = 5));
  DOUT((_1 = 4) += 2*_1);
  DOUT((_1 = 4) = 2*_1);
  DOUT((_1 += 9)(alpha));
  DOUT((_1 * _2)(4, 5));
  DOUT(alpha);

  // auto expr = (_1 * 3) < 2;
  auto expr = _1 * (pi + _1 * (1 + _1));
  for (int i = 0; i < 10; ++i) {
    auto test = i/pi;
    DOUT(t(test, expr(test)));
  }
  DOUT((_1 < 2)(4));
  DOUT(std::get<2>(0, 1, 2, 3));
  int a = 2, b = 4, c = 5;
  DOUT(t(a,b));
  // t(a, b) = t(b+0, a);
  t(a, b) = t(b+0, a);
  DOUT(t(a,b));
  t(a, b, c) = t(a, a+b, a+b+c);
  DOUT(t(a,b,c));
#endif
  return 0;
}
