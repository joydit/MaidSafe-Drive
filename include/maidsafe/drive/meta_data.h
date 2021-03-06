/*  Copyright 2011 MaidSafe.net limited

    This MaidSafe Software is licensed to you under (1) the MaidSafe.net Commercial License,
    version 1.0 or later, or (2) The General Public License (GPL), version 3, depending on which
    licence you accepted on initial access to the Software (the "Licences").

    By contributing code to the MaidSafe Software, or to this project generally, you agree to be
    bound by the terms of the MaidSafe Contributor Agreement, version 1.0, found in the root
    directory of this project at LICENSE, COPYING and CONTRIBUTOR respectively and also
    available at: http://www.maidsafe.net/licenses

    Unless required by applicable law or agreed to in writing, the MaidSafe Software distributed
    under the GPL Licence is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS
    OF ANY KIND, either express or implied.

    See the Licences for the specific language governing permissions and limitations relating to
    use of the MaidSafe Software.                                                                 */

#ifndef MAIDSAFE_DRIVE_META_DATA_H_
#define MAIDSAFE_DRIVE_META_DATA_H_

#ifdef MAIDSAFE_WIN32
#include <windows.h>
#else
#include <fuse/fuse.h>  // NOLINT
#include <sys/stat.h>  // NOLINT
#endif

#include <cstdint>
#include <memory>

#include "boost/filesystem/path.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

#include "maidsafe/common/config.h"
#include "maidsafe/encrypt/data_map.h"

#include "maidsafe/drive/config.h"

namespace maidsafe {

namespace drive {

namespace detail {

namespace protobuf { class MetaData; }

// Represents directory and file information
struct MetaData {
  MetaData();
  MetaData(const boost::filesystem::path& name, bool is_directory);
  explicit MetaData(const protobuf::MetaData& protobuf_meta_data);
  MetaData(MetaData&& other);
  MetaData& operator=(MetaData other);

  void ToProtobuf(protobuf::MetaData* protobuf_meta_data) const;

  boost::posix_time::ptime creation_posix_time() const;
  boost::posix_time::ptime last_write_posix_time() const;
  bool operator<(const MetaData& other) const;
  void UpdateLastModifiedTime();
  uint64_t GetAllocatedSize() const;

  boost::filesystem::path name;
#ifdef MAIDSAFE_WIN32
  uint64_t end_of_file;
  uint64_t allocation_size;
  DWORD attributes;
  FILETIME creation_time;
  FILETIME last_access_time;
  FILETIME last_write_time;
#else
  struct stat attributes;
  boost::filesystem::path link_to;
#endif
  std::unique_ptr<encrypt::DataMap> data_map;
  std::unique_ptr<DirectoryId> directory_id;

 private:
  MetaData(const MetaData& other) = delete;
};

void swap(MetaData& lhs, MetaData& rhs) MAIDSAFE_NOEXCEPT;

}  // namespace detail

}  // namespace drive

}  // namespace maidsafe

#endif  // MAIDSAFE_DRIVE_META_DATA_H_
