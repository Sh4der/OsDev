/**
 *   @file: Optional.h
 *
 *   @date: Jan 3, 2018
 * @author: Mateusz Midor
 */

#ifndef MIDDLESPACE_CSTD_SRC_OPTIONAL_H_
#define MIDDLESPACE_CSTD_SRC_OPTIONAL_H_

#include "String.h"

namespace cstd {

// This Optional doesnt support "string" as "string" constructor indicates error value
template <class T>
class Optional {
public:
    Optional() : invalid(true) {}
    Optional(const string& error) : error_msg(error), invalid(true) {}
    Optional(const T& value) : value(value), invalid(false) {}
    Optional(T&& value) : value(std::move(value)), invalid(false) {}
    explicit operator bool()  const { return !invalid; }
    bool operator!() const { return invalid; }

    string  error_msg {};
    T       value {};
    T value_or(const T& fallback) { return (invalid) ? fallback : value; }
private:
    bool    invalid;
};

// forbidden
template <>
class Optional<string> {
};

} /* namespace cstd */

#endif /* MIDDLESPACE_CSTD_SRC_OPTIONAL_H_ */
