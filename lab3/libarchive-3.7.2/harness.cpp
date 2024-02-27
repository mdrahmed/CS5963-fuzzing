#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <iostream>
#include <fstream>

#include "archive.h"
#include "archive_entry.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *buf, size_t len) {
  struct archive *a = archive_read_new();

  archive_read_support_filter_all(a);
  archive_read_support_format_all(a);
  archive_read_support_format_empty(a);
  archive_read_support_format_raw(a);
  archive_read_support_format_gnutar(a);

  if (ARCHIVE_OK != archive_read_set_options(a, "zip:ignorecrc32,tar:read_concatenated_archives,tar:mac-ext")) {
    return 0;
  }

  if (ARCHIVE_OK != archive_read_open_memory(a, buf, len)) {
    archive_read_free(a);
    return 0;
  }

  archive_read_add_passphrase(a, "secret");

  while(1) {
    std::vector<uint8_t> data_buffer(getpagesize(), 0);
    struct archive_entry *entry;
    int ret = archive_read_next_header(a, &entry);
    if (ret == ARCHIVE_EOF || ret == ARCHIVE_FATAL)
      break;
    if (ret == ARCHIVE_RETRY)
      continue;

    // I added following,
    (void)archive_entry_unset_atime(entry);
    (void)archive_entry_xattr_clear(entry);
    (void)archive_entry_xattr_count(entry);
    (void)archive_entry_unset_birthtime(entry);
    (void)archive_entry_nlink(entry);
    (void)archive_entry_mtime_is_set(entry);
    (void)archive_entry_mode(entry);
    (void)archive_entry_ino64(entry);
    (void)archive_entry_hardlink(entry);
    (void)archive_entry_hardlink_w(entry);
    (void)archive_entry_fflags_text(entry);
    (void)archive_entry_filetype(entry);
    (void)archive_entry_gid(entry);
    (void)archive_entry_gname(entry);
    (void)archive_entry_gname_w(entry);
    (void)archive_entry_dev(entry);
    (void)archive_entry_dev_is_set(entry);
    (void)archive_entry_devminor(entry);
    (void)archive_entry_devmajor(entry);

    // Original functions
    (void)archive_entry_pathname(entry);
    (void)archive_entry_pathname_utf8(entry);
    (void)archive_entry_pathname_w(entry);

    (void)archive_entry_atime(entry);
    (void)archive_entry_birthtime(entry);
    (void)archive_entry_ctime(entry);
    (void)archive_entry_dev(entry);
    (void)archive_entry_digest(entry, ARCHIVE_ENTRY_DIGEST_SHA1);
    (void)archive_entry_filetype(entry);
    (void)archive_entry_gid(entry);
    (void)archive_entry_is_data_encrypted(entry);
    (void)archive_entry_is_encrypted(entry);
    (void)archive_entry_is_metadata_encrypted(entry);
    (void)archive_entry_mode(entry);
    (void)archive_entry_mtime(entry);
    (void)archive_entry_size(entry);
    (void)archive_entry_uid(entry);

    ssize_t r;
    while ((r = archive_read_data(a, data_buffer.data(),
            data_buffer.size())) > 0)
      ;
    if (r == ARCHIVE_FATAL)
      break;
  }

  archive_read_has_encrypted_entries(a);
  archive_read_format_capabilities(a);
  archive_file_count(a);
  archive_seek_data(a, 0, SEEK_SET);

  archive_read_free(a);
  return 0;
}

#define SIZE 1000

//int main(){
//	const uint8_t input[SIZE] = {0};
//	ssize_t length;
//	length = read(STDIN_FILENO, (char*)input, SIZE);
//    printf("Input is: ", input);
//	LLVMFuzzerTestOneInput(input, length);	
//}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    const char* filename = argv[1];
    std::ifstream input_file(filename, std::ios::binary);
    if (!input_file) {
        std::cerr << "Error: Unable to open input file: " << filename << '\n';
        return 1;
    }

    // Determine the size of the file
    input_file.seekg(0, std::ios::end);
    std::streamsize length = input_file.tellg();
    input_file.seekg(0, std::ios::beg);

    // Allocate a buffer to hold the file contents
    std::vector<uint8_t> input(length);

    // Read the file contents into the buffer
    if (!input_file.read(reinterpret_cast<char*>(input.data()), length)) {
        std::cerr << "Error: Failed to read input file: " << filename << '\n';
        return 1;
    }

    std::cout << "Input is: " << filename << '\n';
    LLVMFuzzerTestOneInput(input.data(), length);

    return 0;
}
