/**
 *   @file: Conversions.h
 *
 *   @date: Jan 3, 2018
 * @author: Mateusz Midor
 */

#ifndef USER_USTD_SRC_CONVERSIONS_H_
#define USER_USTD_SRC_CONVERSIONS_H_

#include "types.h"
#include "String.h"

namespace cstd {
namespace conversions {

string int_to_string(s64 num, u8 base = 10);
string double_to_string(double num, u8 max_frac_digits = 10);
s64 string_to_int(const string& str);
double string_to_double(const string& str);

} /* namespace conversions */
} /* namespace cstd */

#endif /* USER_USTD_SRC_CONVERSIONS_H_ */
