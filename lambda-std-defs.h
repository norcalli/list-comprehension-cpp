#ifndef LAMBDA_STD_DEFS_H_
#define LAMBDA_STD_DEFS_H_

#include <cmath>

/*
  unary="abs nan exp exp2 expm1 log log10 log1p log2 sqrt cbrt sin cos tan asin acos atan sinh cosh tanh asinh acosh atanh erf erfc lgamma tgamma ceil floor trunc round nearbyint rint fpclassify isfinite isinf isnan isnormal signbit"
  binary="fmod remainder fmax fmin fdim div hypot pow atan2 frexp ldexp modf scalbn ilogb logb nextafter nexttoward copysign"
  fnname=functors
  unaryfunctor=UNARY_FUNCTION
  binaryfunctor=BINARY_FUNCTION
  unaryfn=LAMBDA_GLOBAL_UNARY_FUNCTION
  binaryfn=BINARY_LAMBDA

  echo "namespace $fnname {";
  for f in $unary; do echo "$unaryfunctor($f, std::$f);"; done | sed -e "s/(\([a-z]\)/(\u\1/g"
  echo
  for f in $binary; do echo "$binaryfunctor($f, std::$f);"; done | sed -e "s/(\([a-z]\)/(\u\1/g"
  echo -e '}\n\nnamespace std {'
  for f in $unary; do echo "$unaryfn(functors::$f, $f);"; done | sed -e "s/::\([a-z]\)/::\u\1/g"
  echo
  for f in $binary; do echo "$binaryfn(functors::$f, $f);"; done | sed -e "s/::\([a-z]\)/::\u\1/g"
  echo '}'
*/

namespace functors {
UNARY_FUNCTION(Abs, std::abs);
UNARY_FUNCTION(Nan, std::nan);
UNARY_FUNCTION(Exp, std::exp);
UNARY_FUNCTION(Exp2, std::exp2);
UNARY_FUNCTION(Expm1, std::expm1);
UNARY_FUNCTION(Log, std::log);
UNARY_FUNCTION(Log10, std::log10);
UNARY_FUNCTION(Log1p, std::log1p);
UNARY_FUNCTION(Log2, std::log2);
UNARY_FUNCTION(Sqrt, std::sqrt);
UNARY_FUNCTION(Cbrt, std::cbrt);
UNARY_FUNCTION(Sin, std::sin);
UNARY_FUNCTION(Cos, std::cos);
UNARY_FUNCTION(Tan, std::tan);
UNARY_FUNCTION(Asin, std::asin);
UNARY_FUNCTION(Acos, std::acos);
UNARY_FUNCTION(Atan, std::atan);
UNARY_FUNCTION(Sinh, std::sinh);
UNARY_FUNCTION(Cosh, std::cosh);
UNARY_FUNCTION(Tanh, std::tanh);
UNARY_FUNCTION(Asinh, std::asinh);
UNARY_FUNCTION(Acosh, std::acosh);
UNARY_FUNCTION(Atanh, std::atanh);
UNARY_FUNCTION(Erf, std::erf);
UNARY_FUNCTION(Erfc, std::erfc);
UNARY_FUNCTION(Lgamma, std::lgamma);
UNARY_FUNCTION(Tgamma, std::tgamma);
UNARY_FUNCTION(Ceil, std::ceil);
UNARY_FUNCTION(Floor, std::floor);
UNARY_FUNCTION(Trunc, std::trunc);
UNARY_FUNCTION(Round, std::round);
UNARY_FUNCTION(Nearbyint, std::nearbyint);
UNARY_FUNCTION(Rint, std::rint);
UNARY_FUNCTION(Fpclassify, std::fpclassify);
UNARY_FUNCTION(Isfinite, std::isfinite);
UNARY_FUNCTION(Isinf, std::isinf);
UNARY_FUNCTION(Isnan, std::isnan);
UNARY_FUNCTION(Isnormal, std::isnormal);
UNARY_FUNCTION(Signbit, std::signbit);
BINARY_FUNCTION(Fmod, std::fmod);
BINARY_FUNCTION(Remainder, std::remainder);
BINARY_FUNCTION(Fmax, std::fmax);
BINARY_FUNCTION(Fmin, std::fmin);
BINARY_FUNCTION(Fdim, std::fdim);
BINARY_FUNCTION(Div, std::div);
BINARY_FUNCTION(Hypot, std::hypot);
BINARY_FUNCTION(Pow, std::pow);
BINARY_FUNCTION(Atan2, std::atan2);
BINARY_FUNCTION(Frexp, std::frexp);
BINARY_FUNCTION(Ldexp, std::ldexp);
BINARY_FUNCTION(Modf, std::modf);
BINARY_FUNCTION(Scalbn, std::scalbn);
BINARY_FUNCTION(Ilogb, std::ilogb);
BINARY_FUNCTION(Logb, std::logb);
BINARY_FUNCTION(Nextafter, std::nextafter);
BINARY_FUNCTION(Nexttoward, std::nexttoward);
BINARY_FUNCTION(Copysign, std::copysign);
}

namespace std {
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Abs, abs);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Nan, nan);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Exp, exp);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Exp2, exp2);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Expm1, expm1);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Log, log);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Log10, log10);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Log1p, log1p);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Log2, log2);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Sqrt, sqrt);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Cbrt, cbrt);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Sin, sin);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Cos, cos);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Tan, tan);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Asin, asin);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Acos, acos);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Atan, atan);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Sinh, sinh);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Cosh, cosh);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Tanh, tanh);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Asinh, asinh);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Acosh, acosh);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Atanh, atanh);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Erf, erf);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Erfc, erfc);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Lgamma, lgamma);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Tgamma, tgamma);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Ceil, ceil);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Floor, floor);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Trunc, trunc);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Round, round);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Nearbyint, nearbyint);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Rint, rint);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Fpclassify, fpclassify);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Isfinite, isfinite);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Isinf, isinf);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Isnan, isnan);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Isnormal, isnormal);
LAMBDA_GLOBAL_UNARY_FUNCTION(functors::Signbit, signbit);
BINARY_LAMBDA(functors::Fmod, fmod);
BINARY_LAMBDA(functors::Remainder, remainder);
BINARY_LAMBDA(functors::Fmax, fmax);
BINARY_LAMBDA(functors::Fmin, fmin);
BINARY_LAMBDA(functors::Fdim, fdim);
BINARY_LAMBDA(functors::Div, div);
BINARY_LAMBDA(functors::Hypot, hypot);
BINARY_LAMBDA(functors::Pow, pow);
BINARY_LAMBDA(functors::Atan2, atan2);
BINARY_LAMBDA(functors::Frexp, frexp);
BINARY_LAMBDA(functors::Ldexp, ldexp);
BINARY_LAMBDA(functors::Modf, modf);
BINARY_LAMBDA(functors::Scalbn, scalbn);
BINARY_LAMBDA(functors::Ilogb, ilogb);
BINARY_LAMBDA(functors::Logb, logb);
BINARY_LAMBDA(functors::Nextafter, nextafter);
BINARY_LAMBDA(functors::Nexttoward, nexttoward);
BINARY_LAMBDA(functors::Copysign, copysign);
}


#endif  // LAMBDA_STD_DEFS_H_
