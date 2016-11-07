/*changecase.h */

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */


#ifndef  CASECHANGE_H
#define  CASECHANGE_H

#include <stdlib.h>
#include "dstring.h"
#include "dstringarray.h"

/* an enumerated integer type */
typedef enum { NONE, LOWER, SENTENCE, TITLE, UPPER } casechange_t;

void no_change(FILE* f);
size_t changecase_files_ds( dstringarray dsa , casechange_t change );
bool   valid_filename ( const char* filename );
size_t longest_name(dstringarray dsa);
dstring format_for_length( dstring a , dstring b, size_t len );


#endif

/*END*/
