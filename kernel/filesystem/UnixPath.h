/**
 *   @file: UnixPath.h
 *
 *   @date: Aug 16, 2017
 * @author: Mateusz Midor
 */

#ifndef SRC_FILESYSTEM_UNIXPATH_H_
#define SRC_FILESYSTEM_UNIXPATH_H_

#include "kstd.h"

namespace filesystem {

/**
 * @brief   This class represents unix-like path in filesystem eg. /home/mateusz/Download
 *          It is always in normalized for ie. no /../ in the path
 */
class UnixPath {
public:
    UnixPath(const char path[]);
    UnixPath(const kstd::string& path = "");
    bool is_valid_absolute_path() const;
    bool is_root_path() const;
    operator kstd::string() const;
    kstd::string extract_directory() const;
    kstd::string extract_file_name() const;

private:
    kstd::string normalize(const kstd::string& path) const;
    kstd::string path;

};

} /* namespace filesystem */

#endif /* SRC_FILESYSTEM_UNIXPATH_H_ */