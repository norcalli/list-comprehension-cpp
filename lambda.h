#ifndef LAMBDA_H_
#define LAMBDA_H_

#include <string>
#include "variadic-utils.h"
#include "lambda-utils.h"
#include "tuple-utils.h"

namespace lambda {

// template<size_t n>
// struct placeholder {
//   placeholder() = default;

//   template<class... Args>
//   inline auto operator()(std::tuple<Args...>&& t) -> decltype(std::get<n>(t)) {
//     return std::get<n>(t);
//   }

//   template<class... Args>
//   inline auto operator()(std::tuple<Args...>&& t) const -> decltype(std::get<n>(t)) {
//     return std::get<n>(t);
//   }

//   template<class... Args>
//   inline auto operator()(Args&&... args) -> decltype(std::get<n>(args...)) {
//     return std::get<n>(args...);
//   }

//   template<class... Args>
//   inline auto operator()(Args&&... args) const -> decltype(std::get<n>(args...)) {
//     return std::get<n>(args...);
//   }
// };

namespace templates {

template<class Param, class Function>
struct phoenix_unary {
  phoenix_unary(const Param& p, Function f = Function()) : param(p), fn(f) {}

  template<class... Args>
  inline auto operator()(Args&&... args)
      -> decltype(std::declval<Function>()(std::declval<Param>()(std::forward<Args>(args)...))) {
    return fn(param(std::forward<Args>(args)...));
  }

  template<class... Args>
  inline auto operator()(Args&&... args) const
      -> decltype(std::declval<Function>()(std::declval<Param>()(std::forward<Args>(args)...))) {
    return fn(param(std::forward<Args>(args)...));
  }

  Param param;
  Function fn;
};

template<class Function, class... Params>
struct lambda_return_type {
  template<class... Args>
  using type = decltype(std::declval<Function>()(eval(std::declval<Params>(), std::declval<Args>()...)...));
};

template<class Left, class Right, class Function>
struct phoenix_binary {
  phoenix_binary(Left&& l, Right&& r, Function f = Function()) : left(l), right(r), fn(f) {}

  template<class... Args>
  inline auto operator()(Args&&... args)
      // -> decltype(std::declval<Function>()(std::declval<Left>()(std::forward<Args>(args)...), std::declval<Right>()(std::forward<Args>(args)...))) {
      -> typename lambda_return_type<Function, Left, Right>::template type<Args...> {
    return fn(left(std::forward<Args>(args)...), right(std::forward<Args>(args)...));
    // return fn(eval(left, std::forward<Args>(args)...), eval(right, std::forward<Args>(args)...));
  }

  template<class... Args>
  inline auto operator()(Args&&... args) const
      -> decltype(std::declval<Function>()(std::declval<Left>()(std::forward<Args>(args)...), std::declval<Right>()(std::forward<Args>(args)...))) {
    return fn(left(std::forward<Args>(args)...), right(std::forward<Args>(args)...));
    // return fn(eval(left, std::forward<Args>(args)...), eval(right, std::forward<Args>(args)...));
  }

  Left left;
  Right right;
  Function fn;
};

template<class Expression, class Left, class Function>
struct phoenix_binary_right {
  phoenix_binary_right(const Expression& e, const Left& l, Function f = Function()) : expr(e), left(l), fn(f) {}

  template<class... Args>
  inline auto operator()(Args&&... args)
      -> decltype(std::declval<Function>()(std::declval<Left>(), std::declval<Expression>()(args...))) {
    return fn(left, expr(args...));
  }

  template<class... Args>
  inline auto operator()(Args&&... args) const
      -> decltype(std::declval<Function>()(std::declval<Left>(), std::declval<Expression>()(args...))) {
    return fn(left, expr(args...));
  }

  Expression expr;
  Function fn;
  Left left;
};

template<class Expression, class Right, class Function>
struct phoenix_binary_left {
  phoenix_binary_left(const Expression& e, const Right& r, Function f = Function()) : expr(e), right(r), fn(f) {}

  template<class... Args>
  inline auto operator()(Args&&... args)
      -> decltype(std::declval<Function>()(std::declval<Expression>()(args...), std::declval<Right>())) {
    return fn(expr(args...), right);
  }

  template<class... Args>
  inline auto operator()(Args&&... args) const
      -> decltype(std::declval<Function>()(std::declval<Expression>()(args...), std::declval<Right>())) {
    return fn(expr(args...), right);
  }

  Expression expr;
  Function fn;
  Right right;
};

template<class Func, class... Params>
struct phoenix_nary {
  using Parameters = std::tuple<typename std::decay<Params>::type...>;
  using Function = typename std::decay<Func>::type;
  using indices = typename gens<sizeof...(Params)>::type;

  // template<class... Args>
  // phoenix_nary(Args&&... ps) : params(std::forward<Args>(ps)...) {}

  // phoenix_nary(const Params&... ps) : params(ps...) {}
  phoenix_nary(Params&&... ps) : params(std::forward<Params>(ps)...) {}

  // phoenix_nary(Function f, Params&&... ps) : fn(f), phoenix_nary(ps...) {}
  // phoenix_nary(Params&&... ps, Function f = Function()) : fn(f), params(std::forward_as_tuple(ps...)) {}

  template<int... idx, class... Args>
  inline auto apply(tuple_indices<idx...>, Args&&... args)
      -> decltype(std::declval<Function>()(eval(std::declval<Params>(), args...)...)) {
    return fn(eval(std::get<idx>(params), args...)...);
  }

  template<int... idx, class... Args>
  inline auto apply(tuple_indices<idx...>, Args&&... args) const
      -> decltype(std::declval<Function>()(eval(std::declval<Params>(), args...)...)) {
    return fn(eval(std::get<idx>(params), args...)...);
  }

  template<class... Args>
  inline auto operator()(Args&&... args)
      -> decltype(std::declval<Function>()(eval(std::declval<Params>(), args...)...)) {
      // -> decltype(apply(indices(), args...)) {
    return apply(indices(), std::forward<Args>(args)...);
  }

  template<class... Args>
  inline auto operator()(Args&&... args) const
      -> decltype(std::declval<Function>()(eval(std::declval<Params>(), args...)...)) {
      // -> decltype(apply(indices(), args...)) {
    return apply(indices(), std::forward<Args>(args)...);
  }

  Parameters params;
  Function fn = Function();
};

}

#define BINARY_ASSIGNMENT(name, op)                                     \
  struct name {                                                         \
    template<class T, class U>                                          \
    inline auto operator()(T& t, const U& u) const -> decltype(t op u) { return t op u; } \
    std::string str(const std::string& l, const std::string& r) const { return l + " " #op " " + r; } \
  };                                                                    \

BINARY_ASSIGNMENT(Assign, =);
// BINARY_ASSIGNMENT(AddAssign, +=);
// BINARY_ASSIGNMENT(SubAssign, -=);
// BINARY_ASSIGNMENT(MulAssign, *=);
// BINARY_ASSIGNMENT(DivAssign, /=);

#undef BINARY_ASSIGNMENT

// TODO: If I wrap the `t` in parenthesis, I can support unary functions too.
// or I can add a separate macro for n-ary functions with variadic templates.
// Things to worry about: Creating an rvalue reference accidentally?

// #define UNARY_FUNCTION(name, op, mod, incr)                             \
//   struct name {                                                         \
//     template<class T>                                                   \
//     inline auto operator()(mod T& t) const -> decltype(op t incr) { return op t incr; } \
//   };

// #define UNARY_FUNCTION(name, op, incr)                                  \
//   struct name {                                                         \
//     template<class T>                                                   \
//     inline auto operator()(T& t) const -> decltype(op t incr) { return op t incr; } \
//     template<class T>                                                   \
//     inline auto operator()(const T& t) const -> decltype(op t incr) { return op t incr; } \
//   };

#define UNARY_FUNCTION(name, op)                                        \
  struct name {                                                         \
    template<class T>                                                   \
    inline auto operator()(T& t) const -> decltype(op (t)) { return op (t); } \
    template<class T>                                                   \
    inline auto operator()(const T& t) const -> decltype(op (t)) { return op (t); } \
    std::string str(const std::string& p) const { return #op "(" + p + ")"; } \
  };

#define UNARY_OPERATOR_SUFFIX(name, op)                                 \
  struct name {                                                         \
    template<class T>                                                   \
    inline auto operator()(T& t) const -> decltype((t) op) { return (t) op; } \
    template<class T>                                                   \
    inline auto operator()(const T& t) const -> decltype((t) op) { return (t) op; } \
    std::string str(const std::string& p) const { return std::string("(") + p + ")" #op; } \
  };

#define NARY_FUNCTION(name, fn)                                         \
  struct name {                                                         \
    template<class... Args>                                             \
    inline auto operator()(Args&&... args) const -> decltype(fn(args...)) { return fn(args...); } \
    std::string str() const { return #fn; }                             \
  };

#define NARY_LAMBDA(name, op)                                   \
  template<class... Args>                                       \
  inline lambda::Lambda<lambda::templates::phoenix_nary<name, Args...>> \
      op(Args&&... args) {                                      \
    using type = lambda::templates::phoenix_nary<name, Args...>;        \
    return make_lambda(type(std::forward<Args>(args)...));      \
  }

#define NARY_FUNCTION_LAMBDA(name, fn) \
  NARY_FUNCTION(name, fn);             \
  NARY_LAMBDA(name, fn);

// UNARY_FUNCTION(Negative, -, const, );
// UNARY_FUNCTION(Positve, +, const, );
// UNARY_FUNCTION(Preincrement, ++, , );
// UNARY_FUNCTION(Postincrement, , , ++);
// UNARY_FUNCTION(Bitflip, ~, const, );
// UNARY_FUNCTION(Not, !, const, );
// UNARY_FUNCTION(Predecrement, --, , );
// UNARY_FUNCTION(Postdecrement, , , --);
// UNARY_FUNCTION(AddressOf, &, const, );
// UNARY_FUNCTION(Dereference, *, const, );

UNARY_FUNCTION(Negative, -);
UNARY_FUNCTION(Positve, +);
UNARY_FUNCTION(Preincrement, ++);
UNARY_FUNCTION(Bitflip, ~);
UNARY_FUNCTION(Not, !);
UNARY_FUNCTION(Predecrement, --);
UNARY_FUNCTION(AddressOf, &);
UNARY_FUNCTION(Dereference, *);

UNARY_OPERATOR_SUFFIX(Postincrement, ++);
UNARY_OPERATOR_SUFFIX(Postdecrement, --);


#define UNARY_MEMBER(name, op, incr)                                    \
  inline Lambda<templates::phoenix_unary<const Lambda<Function>&, name>> \
      operator op(incr) const {                                         \
    using type = templates::phoenix_unary<const Lambda<Function>&, name>; \
    return make_lambda(type(*this));                                    \
  }

// TODO: Possible to generalize for postincrement?
#define LAMBDA_GLOBAL_UNARY_FUNCTION(name, op)                          \
  template<class Function>                                              \
  inline lambda::Lambda<lambda::templates::phoenix_unary<const lambda::Lambda<Function>&, name>> \
      op(const lambda::Lambda<Function>& l) {                           \
    using type = lambda::templates::phoenix_unary<const lambda::Lambda<Function>&, name>; \
    return make_lambda(type(l));                                        \
  }

#define LAMBDA_GLOBAL_UNARY_OPERATOR(name, op) LAMBDA_GLOBAL_UNARY_FUNCTION(name, operator op)

#define LAMBDA_GLOBAL_UNARY_OPERATOR_SUFFIX(name, op)                   \
  template<class Function>                                              \
  inline lambda::Lambda<lambda::templates::phoenix_unary<const lambda::Lambda<Function>&, name>> \
      operator op(const lambda::Lambda<Function>& l, int) {             \
    using type = lambda::templates::phoenix_unary<const lambda::Lambda<Function>&, name>; \
    return make_lambda(type(l));                                        \
  }

// template<class T>                             \
// Lambda<name> operator op()(const T& t) {      \
//   return Lambda<name>                         \
// }                                             \

#define BINARY_MEMBER(name, op)                                         \
  template<class T, class = typename std::enable_if<!is_lambda<T>::value>::type> \
  inline Lambda<templates::phoenix_binary_left<const Lambda<Function>&, T, name>> \
      operator op(const T& t) const {                                   \
    using type = templates::phoenix_binary_left<const Lambda<Function>&, T, name>; \
    return make_lambda(type(*this, t));                                 \
  }                                                                     \
  template<class Alpha>                                                 \
  inline Lambda<templates::phoenix_binary<const Lambda<Function>&, const Lambda<Alpha>&, name>> \
      operator op(const Lambda<Alpha>& r) const {                       \
    using type = templates::phoenix_binary<const Lambda<Function>&, const Lambda<Alpha>&, name>; \
    return make_lambda(type(*this, r));                                 \
  }                                                                     \


template<class Func>
struct Lambda {
  using Function = typename std::decay<Func>::type;
  // Lambda() = default;
  // Lambda(Function fn) : f(fn) {}
  Lambda(Lambda&&) = default;
  Lambda(const Lambda&) = default;
  // Lambda& operator=(Lambda&&) = default;
  // Lambda& operator=(const Lambda&) = default;

  Lambda(Function fn = Function()) : f(fn) {}
  
  template<class... Args>
  inline auto operator()(Args&&... args) -> decltype(std::declval<Function>()(args...)) {
    return f(args...);
  }

  template<class... Args>
  inline auto operator()(Args&&... args) const -> decltype(std::declval<Function>()(args...)) {
    return f(args...);
  }

  // Lambda& operator=(const Lambda& other) {
  //   f = other.f;
  //   return *this;
  // }
  // Lambda(const Lambda& other) : f(other.f) {}

  BINARY_MEMBER(Assign, =);

  // Function f = Function();
  Function f;
};

template<size_t n>
struct placeholder : public Lambda<Get<n>> {
  using Function = Get<n>;

  placeholder() = default;
  // placeholder(placeholder&&) = default;
  // placeholder(const placeholder&) = default;

  BINARY_MEMBER(Assign, =);
  // BINARY_MEMBER(AddAssign, +=);
  // BINARY_MEMBER(SubAssign, -=);
  // BINARY_MEMBER(MulAssign, *=);
  // BINARY_MEMBER(DivAssign, /=);

  // TODO: Rvalues shouldn't have addressof operator, but operator= and the like could support it,
  // so should this be under the 

};

template<class Function>
inline Lambda<Function> make_lambda(Function fn) {
  return Lambda<Function>(fn);
}

struct Identity {
  template<class T>
  inline T& operator()(T& t) { return t; }

  template<class T>
  inline T&& operator()(T&& t) { return t; }

  template<class T>
  inline const T& operator()(const T& t) { return t; }
};

#define WRAPPER(suffix, op)                                     \
  template<class T>                                             \
  struct Wrapper ## suffix {                                    \
    Wrapper ## suffix(T op tt) : t(tt) {}                       \
    template<class... Args>                                     \
    inline T op operator()(Args&&...) { return t; }             \
    template<class... Args>                                     \
    inline const T& operator()(Args&&...) const { return t; }   \
    T op t;                                                     \
  };                                                            \

WRAPPER(Reference, &);
WRAPPER(ConstRef, const &);
WRAPPER(RValue, &&);

#undef WRAPPER

// template<class T>
// struct Wrapper {
//   Wrapper(const T& tt) : t(tt) {}
//   Wrapper(T&& tt) : t(std::move(tt)) {}

//   template<class... Args>
//   inline T& operator()(Args&&...) { return t; }

//   template<class... Args>
//   inline const T& operator()(Args&&...) const { return t; }

//   T t;
// };

template<class T, class Enable = void>
struct wrapper_type { };

template<class T>
struct wrapper_type<T, typename std::enable_if<std::is_rvalue_reference<T>::value>::type> {
  using type = WrapperRValue<typename std::remove_reference<T>::type>;
};

template<class T>
struct wrapper_type<T, typename std::enable_if<std::is_const<T>::value
                                               && std::is_lvalue_reference<T>::value>::type> {
  using type = WrapperConstRef<typename std::remove_reference<T>::type>;
};

template<class T>
struct wrapper_type<T, typename std::enable_if<!std::is_const<T>::value
                                               && std::is_lvalue_reference<T>::value>::type> {
  using type = WrapperReference<typename std::remove_reference<T>::type>;
};

template<class T>
WrapperRValue<T> wrap(T&& t) {
  return WrapperRValue<T>(t);
}

template<class T>
WrapperReference<T> wrap(T& t) {
  return WrapperReference<T>(t);
}

template<class T>
WrapperConstRef<T> wrap(const T& t) {
  return WrapperConstRef<T>(t);
}

// TODO: Eventually make Constant inherit from Lambda<wrapper> and initialize it with a value initialized from the constructor.

// TODO: optimize this for references (rvalue and lvalue) so that you don't do the copy in the initialization.
template<class T>
struct Constant {
  Constant(T&& tt) : t(tt) {}
  Constant(const T& tt) : t(tt) {}
  template<class... Args>
  inline T& operator()(Args&&...) {
    return t;
  }
  T t;
};

const placeholder<0> _1;
const placeholder<1> _2;
const placeholder<2> _3;
const placeholder<3> _4;
const placeholder<4> _5;
const placeholder<5> _6;

}

// template<class Expression, class Right>
// struct phoenix_less {
//   phoenix_less(const Expression& e, const Right& r) : expr(e), right(r) {}

//   template<class... Args>
//   inline bool operator()(Args&&... args) {
//     return expr(args...) < right;
//   }

//   Expression expr;
//   Right right;
// };

// template<class Expression, class Right>
// struct phoenix_less : public phoenix_binary_left<Expression, Right, Less> {
//   phoenix_less(const Expression& e, const Right& r)
//       : phoenix_binary_left<Expression, Right, Less>(e, r) {}
// };

// template<size_t n, class T>
// lambda::Lambda<phoenix_less<const lambda::placeholder<n>&, T> > operator<(const lambda::placeholder<n>& p, const T& t) {
//   using type = phoenix_less<const lambda::placeholder<n>&, T>;
//   return lambda::make_lambda(type(p, t));
// }

// template<class Function, class T>
// lambda::Lambda<phoenix_less<const lambda::Lambda<Function>&, T>> operator<(const lambda::Lambda<Function>& f, const T& t) {
//   using type = phoenix_less<const lambda::Lambda<Function>&, T>;
//   return lambda::make_lambda(type(f, t));
// }

// struct Less {
//   template<class T, class U>
//   inline bool operator()(const T& t, const U& u) { return t < u; }
// };

// struct LessEqual {
//   template<class T, class U>
//   inline bool operator()(const T& t, const U& u) { return t <= u; }
// };

// struct Greater {
//   template<class T, class U>
//   inline bool operator()(const T& t, const U& u) { return t > u; }
// };

// struct GreaterEqual {
//   template<class T, class U>
//   inline bool operator()(const T& t, const U& u) { return t >= u; }
// };

// struct Equal {
//   template<class T, class U>
//   inline bool operator()(const T& t, const U& u) { return t == u; }
// };

// struct NotEqual {
//   template<class T, class U>
//   inline bool operator()(const T& t, const U& u) { return t != u; }
// };

// template<class Function, class T>
// inline lambda::Lambda<phoenix_binary_left<const lambda::Lambda<Function>&, T, Less>>
// operator<(const lambda::Lambda<Function>& f, const T& t) {
//   using type = phoenix_binary_left<const lambda::Lambda<Function>&, T, Less>;
//   return lambda::make_lambda(type(f, t));
// }

// FIXME: The way that phoenix_left and right store values (efficiently dealing with references)
// and fix the ambiguity.
// OR
// TODO: Remove the 'left' and 'right' and replace with a decent 'constant' interface that properly
// and efficiently captures and wraps variable references (both r and l values)

// inline typename std::enable_if<!is_lambda<T>::value, lambda::Lambda<phoenix_binary_right<const lambda::Lambda<Function>&, T, name>>>::type
// template<class Function, class T, class = typename std::enable_if<!is_lambda<T>::value>::type> \
// template<class Function, class T>


#define BINARY_FUNCTION_TEMPLATE(name, fn, title)                       \
  struct name {                                                         \
    template<class T, class U>                                          \
    inline auto operator()(const T& t, const U& u) const -> decltype(fn) { return fn; } \
    template<class T, class U>                                          \
    inline auto operator()(T& t, const U& u) const -> decltype(fn) { return fn; } \
    std::string str(const std::string& l, const std::string& r) const { return title; } \
  };                                                                    \

#define BINARY_OPERATOR(name, op) BINARY_FUNCTION_TEMPLATE(name, t op u, l + " " #op " " + r)
#define BINARY_FUNCTION(name, op) BINARY_FUNCTION_TEMPLATE(name, op(t, u), #op "(" + l + ", " + r + ")")
// #define BINARY_OPERATOR(name, op)                                       \
//   struct name {                                                         \
//     template<class T, class U>                                          \
//     inline auto operator()(const T& t, const U& u) const -> decltype(t op u) { return t op u; } \
//     template<class T, class U>                                          \
//     inline auto operator()(T& t, const U& u) const -> decltype(t op u) { return t op u; } \
//   };                                                                    \

// #define BINARY_FUNCTION(name, op)                                       \
//   struct name {                                                         \
//     template<class T, class U>                                          \
//     inline auto operator()(const T& t, const U& u) const -> decltype(op(t, u)) { return op(t, u); } \
//     template<class T, class U>                                          \
//     inline auto operator()(T& t, const U& u) const -> decltype(op(t, u)) { return op(t, u); } \
//   };                                                                    \

#define BINARY_LAMBDA(name, op)                                         \
  template<class Function, class T, class = typename std::enable_if<!lambda::is_lambda<T>::value>::type> \
  inline lambda::Lambda<lambda::templates::phoenix_binary_left<const lambda::Lambda<Function>&, T, name>> \
      op(const lambda::Lambda<Function>& f, const T& t) {               \
    using type = lambda::templates::phoenix_binary_left<const lambda::Lambda<Function>&, T, name>; \
    return lambda::make_lambda(type(f, t));                             \
  }                                                                     \
  template<class Function, class T, class = typename std::enable_if<!lambda::is_lambda<T>::value>::type> \
  inline lambda::Lambda<lambda::templates::phoenix_binary_right<const lambda::Lambda<Function>&, T, name>> \
      op(const T& t, const lambda::Lambda<Function>& f) {               \
    using type = lambda::templates::phoenix_binary_right<const lambda::Lambda<Function>&, T, name>; \
    return lambda::make_lambda(type(f, t));                             \
  }                                                                     \
  template<class Alpha, class Beta>                                     \
  inline lambda::Lambda<lambda::templates::phoenix_binary<const lambda::Lambda<Alpha>&, \
                                                          const lambda::Lambda<Beta>&, name>> \
      op(const lambda::Lambda<Alpha>& l, const lambda::Lambda<Beta>& r) { \
    using type = lambda::templates::phoenix_binary<const lambda::Lambda<Alpha>&, \
                                                   const lambda::Lambda<Beta>&, name>; \
    return lambda::make_lambda(type(l, r));                             \
    }                                                                   \

#define BINARY_OPERATOR_LAMBDA(name, op)        \
  BINARY_OPERATOR(name, op);                    \
  BINARY_LAMBDA(name, operator op);

#define BINARY_FUNCTION_LAMBDA(name, op)        \
  BINARY_FUNCTION(name, op);                    \
  BINARY_LAMBDA(name, op);

// Comparison.
BINARY_OPERATOR_LAMBDA(Less, <);
BINARY_OPERATOR_LAMBDA(Greater, >);
BINARY_OPERATOR_LAMBDA(LessEqual, <=);
BINARY_OPERATOR_LAMBDA(GreaterEqual, >=);
BINARY_OPERATOR_LAMBDA(Equal, ==);
BINARY_OPERATOR_LAMBDA(NotEqual, !=);

// Arithmetic.
BINARY_OPERATOR_LAMBDA(Add, +);
BINARY_OPERATOR_LAMBDA(Subtract, -);
BINARY_OPERATOR_LAMBDA(Multiply, *);
BINARY_OPERATOR_LAMBDA(Divide, /);
BINARY_OPERATOR_LAMBDA(Modulo, %);

// Bit operations.
BINARY_OPERATOR_LAMBDA(LeftShift, <<);
BINARY_OPERATOR_LAMBDA(RightShift, >>);
BINARY_OPERATOR_LAMBDA(Xor, ^);
BINARY_OPERATOR_LAMBDA(BitAnd, &);
BINARY_OPERATOR_LAMBDA(BitOr, |);

// Arithmetic assignment.
BINARY_OPERATOR_LAMBDA(AddAssign, +=);
BINARY_OPERATOR_LAMBDA(SubAssign, -=);
BINARY_OPERATOR_LAMBDA(MulAssign, *=);
BINARY_OPERATOR_LAMBDA(DivAssign, /=);
BINARY_OPERATOR_LAMBDA(ModAssign, %=);

// Bit assignment.
BINARY_OPERATOR_LAMBDA(LSAssign, <<=);
BINARY_OPERATOR_LAMBDA(RSAssign, >>=);
BINARY_OPERATOR_LAMBDA(XorAssign, ^=);
BINARY_OPERATOR_LAMBDA(AndAssign, &=);
BINARY_OPERATOR_LAMBDA(OrAssign, |=);

// Logical.
BINARY_OPERATOR_LAMBDA(LogicalAnd, &&);
BINARY_OPERATOR_LAMBDA(LogicalOr, ||);

// BINARY_FUNCTION_LAMBDA(Evaluator, Evaluate);

// TODO: Can move the declarations of functors out of lambda namespace
// can also move them into LAMBDA_GLOBAL_UNARY_OPERATOR(_SUFFIX) definition.

LAMBDA_GLOBAL_UNARY_OPERATOR(lambda::Negative, -);
LAMBDA_GLOBAL_UNARY_OPERATOR(lambda::Positve, +);
LAMBDA_GLOBAL_UNARY_OPERATOR(lambda::Preincrement, ++);
LAMBDA_GLOBAL_UNARY_OPERATOR(lambda::Bitflip, ~);
LAMBDA_GLOBAL_UNARY_OPERATOR(lambda::Not, !);
LAMBDA_GLOBAL_UNARY_OPERATOR(lambda::Predecrement, --);
LAMBDA_GLOBAL_UNARY_OPERATOR(lambda::AddressOf, &);
LAMBDA_GLOBAL_UNARY_OPERATOR(lambda::Dereference, *);

LAMBDA_GLOBAL_UNARY_OPERATOR_SUFFIX(lambda::Postdecrement, --);
LAMBDA_GLOBAL_UNARY_OPERATOR_SUFFIX(lambda::Postincrement, ++);

#include <cmath>


// for f in sqrt cos sin tan atan exp log log10 log2; do echo "UNARY_FUNCTION($f, std::$f);"; done | sed -e "s/\([^ ]\+\)\./\u\1\./g"
namespace functors {
UNARY_FUNCTION(Sqrt, std::sqrt);
UNARY_FUNCTION(Cos, std::cos);
UNARY_FUNCTION(Sin, std::sin);
UNARY_FUNCTION(Tan, std::tan);
UNARY_FUNCTION(Atan, std::atan);
UNARY_FUNCTION(Exp, std::exp);
UNARY_FUNCTION(Log, std::log);
UNARY_FUNCTION(Log10, std::log10);
UNARY_FUNCTION(Log2, std::log2);
}

// for f in sqrt cos sin tan atan exp log log10 log2; do echo "LAMBDA_GLOBAL_UNARY_FUNCTION(functors::$f, $f);"; done
namespace std {
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Sqrt, sqrt);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Cos, cos);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Sin, sin);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Tan, tan);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Atan, atan);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Exp, exp);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Log, log);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Log10, log10);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Log2, log2);

}

// TODO: Double check support for assignment and modifying operators.
// TODO: To support assignment, the Args&& passed must be capable of supporting lvalue reference.

#undef BINARY_OPERATOR_LAMBDA

#endif  // LAMBDA_H_
