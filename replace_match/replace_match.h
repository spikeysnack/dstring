/*replace_match.h*/

#ifndef REPLACE_MATCH_H
#define REPLACE_MATCH_H


#include "dstring.h"
#include "dstringlist.h"
#include "dstringarray.h"


dstringarray read_lines_from_file( const cstring filename );

bool write_lines_to_file(const cstring filename, dstringarray lines);

dstring read_a_line(FILE* f);

bool write_a_line(FILE*f, dstring line);

#endif



