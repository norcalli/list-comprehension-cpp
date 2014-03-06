#ifndef STATS_H_
#define STATS_H_

#include <cmath>

template<class T>
struct encapsulation {
  encapsulation(T& v) : value(v) {}

  inline void operator()(const T& v) {
    value = std::move(v);
  }

  inline const T& operator()() const {
    return value;
  }
 private:
  T& value;
};

template<class T>
struct const_encapsulation {
  const_encapsulation(const T& v) : value(v) {}

  inline const T& operator()() const {
    return value;
  }
  
 private:
  const T& value;
};

// template<class T>
// const_encapsulation<T> encapsulate(const T& value) {
//   return const_encapsulation<T>(value);
// }

// template<class T>
// encapsulation<T> encapsulate(T& value) {
//   return encapsulation<T>(value);
// }

// TODO: Cache values by adding `bool cached` member and static values to each function which calculates values?
// Set cached = false whenever Push is called.

class Statistics {
 public:
  using type = double;

  Statistics() = default;

  template<class T>
  inline void PushArray(const T& array) {
    for (auto& e : array)
      Push(e);
  }

  template<class T>
  inline void Push(std::initializer_list<T> array) {
    PushArray(array);
  }

  template<class T, size_t n>
  inline void Push(const std::array<T, n>& array) {
    PushArray(array);
  }

  template<class T>
  inline void Push(const T& e) {
    sum_of_squares_ += e*e, sum_ += e, ++count_;
  }

  template<class T>
  inline void operator()(const T& v) {
    Push(v);
  }

  inline void Reset() {
    sum_of_squares_ = 0, sum_ = 0, count_ = 0;
  }

  inline type Sum() const { return sum_; }
  inline type SumOfSquares() const { return sum_of_squares_; }
  inline type Count() const { return count_; }

  inline type Mean() const {
    return sum_ / count_;
  }

  inline type Variance() const {
    return (sum_of_squares_ - sum_*sum_ / count_) / count_;
  }

  inline type SampleVariance() const {
    // Bessel's Correction.
    return Variance() * count_ / (count_ - 1);
    // return (sum_of_squares_ - sum_*sum_) / (count_ - 1);
  }

  inline type StandardDeviation() const {
    return std::sqrt(Variance());
  }

 private:
  type sum_of_squares_ = 0.0, sum_ = 0.0;
  size_t count_ = 0;
};

std::ostream& operator<<(std::ostream& stream, const Statistics& stats) {
  return stream << "sum(" << stats.Sum() << "), N(" << stats.Count() << "), sigma(" << stats.StandardDeviation() << "), mu(" << stats.Mean() << ')';
}

#endif  // STATS_H_
