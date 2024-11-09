#include "parser.h"

#include <zip.h>

/*1. Determine the publication card hash

   1. Query the SQLite `Publication` table
   2. Create a list with the `MepsLanguageIndex`, `Symbol`, `Year` fields
   3. If the `IssueTagNumber` field is not zero, add it to the end of the list
   4. Join the list with underscores to one string, for example for
w_S_202206.jwpub, this would be `1_w22_2022_20220600`
   5. Calculate the SHA 256 hash of that string
   6. Calculate the bitwise XOR with
`11cbb5587e32846d4c26790c633da289f66fe5842a3a585ce1bc3a294af5ada7` [CyberChef
example
1](https://gchq.github.io/CyberChef/#recipe=SHA2('256',64,160)From_Hex('Auto')XOR(%7B'option':'Hex','string':'11cbb5587e32846d4c26790c633da289f66fe5842a3a585ce1bc3a294af5ada7'%7D,'Standard',false)To_Hex('None',0)&input=MV93MjJfMjAyMl8yMDIyMDYwMA)

2. Decrypt the text

   1. Query a row from the `Document`, `BibleChapter` or `BibleVerse` table
   2. Read the encoded `Content` field
   3. Run AES-128-CBC, use the first 16 bytes of the hash as AES Key, and the
last 16 bytes as Initialization Vector (IV)
   4. Run Zlib Inflate
      [CyberChef example
2](https://gchq.github.io/CyberChef/#recipe=AES_Decrypt(%7B'option':'Hex','string':'909fd5b41ddd8a75ac39c69604828a7d'%7D,%7B'option':'Hex','string':'3bc2c616d0ca2cff6dc4c0d7263a2327'%7D,'CBC','Hex','Raw',%7B'option':'Hex','string':''%7D,%7B'option':'Hex','string':''%7D)Zlib_Inflate(0,0,'Adaptive',false,false)&input=ZjZhZmMwMTEzZmRiMzY4MDE4ZmEzYmEwZDUwNjJlZWFmNGJlNzVhY2Q2NDJkNzM0YTQ2N2M2OTNjODIyMWM2NzM4ODMwYzE0NDQwMjVkZWQ2ZTZmNGZiNjBjZjgyYzcwYWUyYTY5M2EyYTg3NjQ5NWVlMGMxZTU5MDE1NDcyODM0OTMyMGI1OWY2NDAwNzRjMDgzMzYxN2RhYzI5N2ZjZmI1NTY4ODhhMDgzOTAyZGZhYTgyZDNmNmY1MjZhMjI2NTJkZTlhYmEzZjU4NjRkM2RlOTQzMGU2N2Q4NmViNzQwYmUzM2MyNmFjMWVjMmZmNDg0MGE1ODRkYjdhMmMyM2I3Nzc5Y2FlZDNhZGZhZjQ5MzJhZWJhMjM4MDUxNTgzNjRkMjBhNGIxMGU3Njg3YjU4NzBjNDMzZWZmOTk2ZmJjMGI5NGRkYzRkMTI0YjA0NjU2NTUxMjVkZGNmNzI4ZjNlYTgxMDk5YjZkNGM1YjM5NTczM2U5OTZjNjE0YzgyMTA4MTZkNDc3MzViMjI4YzU0MWJiNGJhMTBkMDBlODVkZmVlODlmN2YzOTQ0Y2NmOTk2YWJmMDJkY2QyZTdkMzJlOTU0MGY3ZTVkMDUzMjE2OWFjZTM5ODk5MWI2YzMzYTlmZDdhYmUzMjMyYTU4ODIwYWZhNDFmOTdmMTU2YWI4YTExMzEzMjI5MjhkMDNhOWM4OTE4NGEzOTE5YTQ3NGM2YjM1MWM4MDg2NDkwYWM0ZDYyYTI3MDU2YWMzN2M2NTFmZjYxYmExMmJmZDRhNzk0YmQxNjNlNmM3MjRhYmY2ZjMxMTc1OTQyMDE4OWM3NjNkZGJjMGQxMzQ2Y2Q1ZDUyYjJiNzExYTg1MGY0NWI3MTk5NDIzNDIwNDRhZDVhYjMwMmY1ZGQzMWJjZjlhMjcwZjM4YTlmNmJlMWFjZDlkYzMxYTczYjE1YzBlNDcwNmYyYThjNDE3NmFiNzg1ODI4NDMyMjUwNzM0MzIxMzY1MWRhMTk5ODg5NTMyYzJiOTQyYTkwOTRjMDVlOWY2OGRjM2ExNmU0NTUzNTE4MTA4MzQzODk2YmRhZTA2ZTVmYjIxZDYzMzRjYjk4NmVmMzlkN2Q1MzQ3YTQwNWU3YzkzMGYzYTVlN2U1ZWM1MWY5MDk4OTNkYjg1ZGZlZjUzYTM3ODBhZWQ3OGNkNjRjM2EzMTdjNWM5MDkwZjYyYjNhMGVkMjZiODBlYWMxNzMxMmEzMDkxMzA0MTIxYmM1Mzc4ZTU3MGQ5MjVjMjcwMmYwNmY3ZWJlNTYwNGRmNDRmMjQ0YjA5MjYxODkyYTE0OTIyYzkxMTBhZTIyY2FkMzg1NmM3MDQ1NDlmMmYwNTVmNGI1Mjg0MDQwYzk5Y2I4ZGY2Y2Q4N2Q5NjA1MmIzYTJjY2IxYmE1ZWMxNTMyOWY2ZThlNmM2NjhjMjkyMjViMWMxN2YxYTVhN2MzZmQ0YmVmODRiMzYyYjJkOTg3NGUyMTIyNDUzYzUyMDA3OTFkOTFhYmZjMzU0YzkxMWJlMTY4NmFmNmIzYTJmMjBjZTc2MzBlZjRiMzJjYWExYzcyZTA2NzhlNDcwMTk2NTYzZTZlNTgxZTNjYjAwOTRlOGYyMWIyZTUxZWZjYTNlNDdkZDExN2YzNGFmZWJhNmI2ODJjY2FkZjhkM2Q2ZjE5MDVlNzIxN2JkZTVjMTU3ZThiMmExOWYyYWFiZGEwZTM3OGZkODA3MjgwNGVmNWJhN2ZjMTQzOTg1NmFjNDVkYjY1MDY1OTBkMDI0Zjc5YjY0Njk1ZWJiYTYyN2U2YTdmOTkzYzZlMmY3NDdhZGQ0MmYyOTQyMGVjMzc5NmJjNWU5M2QzNzAwNTg0NzM4ZDI2YmE3ODUzZTVlNjgzMmU0YzQ5NDM1MGI5MThjMTlmZTI1MmY1)

*/

void open_file(gchar *path) {
  zip_error_t error;
  zip_source_t *source = zip_source_file_create(path, 0, -1, &error);
  if (source == NULL) {
    printf("Error creating zip source: %s\n", zip_error_strerror(&error));
  } else {
    open_zip_source(source, "contents");
    zip_source_t *source =
        zip_source_buffer_create(inner_contents, stat.size, 1, &err);
  }
}

static void open_zip_source(zip_source_t *source, const char *filename) {
  char *contents = NULL;
  zip_error_t error;
  zip_t *archive = zip_open_from_source(source, ZIP_RDONLY, &error);
  if (archive == NULL) {
    printf("Error opening zip archive: %s\n", zip_error_strerror(&error));
    zip_source_free(source);
  } else {
    zip_int64_t file_index = zip_name_locate(archive, filename, ZIP_FL_NOCASE);
    if (file_index < 0) {
      printf("File not found in zip archive: %s\n", filename);
      zip_close(archive);
    } else {
      zip_file_t *file = zip_fopen_index(archive, file_index, 0);
      if (file == NULL) {
        printf("Error opening file inside zip archive\n");
        zip_close(archive);
      } else {
        struct zip_stat stat;
        zip_stat_index(archive, file_index, 0, &stat);
        contents = malloc(stat.size);
        if (contents == NULL) {
          printf("Error allocating memory for inner zip file contents\n");
          zip_fclose(inner_file);
          zip_close(outer_archive);
          return 1;
        }
      }
    }
  }
}