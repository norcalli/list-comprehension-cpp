#ifndef STRING_UTILS_H_
#define STRING_UTILS_H_

std::string join(const std::string& j, std::initializer_list<std::string> array) {
  std::string result;
  auto it = std::begin(array), end = std::end(array);
  result += *it;
  while (++it != end) {
    result += j;
    result += *it;
  }
  return result;
}

#endif  // STRING_UTILS_H_
