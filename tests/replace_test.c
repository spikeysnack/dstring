/*replace_test.*/


/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */

#include "dstring.h"
#include "dstringarray.h"
//#include "dstring_utils.h"
#include "key_utils.h"


/* test replace functions */

#define P(S)   printf("(%s)\n", S )

#define PDS(DS)   printf("(%s) %zu :%zu \n", DS, dstlen(DS), dstfree(DS) )

void verheader( dstring ver )
{
  system("clear");
  printf("=============================================\n");
  printf("dstring version:\t %s\n", ver );  
  printf("=============================================\n\n");
}





int main( int argc, char** argv, char** env )
{

  verheader( dstring_version() );

  P("==APPEND TEST==");
  dstring d = new_dstring_init( "ABCDE" , 16 );
  PDS(d);

  d = dstring_addmem( d , 100000);
  P("add 100k memory:\t");
  PDS(d);
  
  d = dstring_append( d, "FGHIJ");
  PDS(d);


  askforkey("press any key");

  verheader( dstring_version() );


  P("==TRUNCATE/SLICE TESTS==");

  dstring e = dstring_truncate(d , 5);
  PDS(e);

  dstring f = dstring_slice(d , 5, 10);
  PDS(f);

  askforkey("press any key");
  verheader( dstring_version() );

 
 P("==REMOVE/REPLACE TESTS==");

  dstring g = new_dstring("This is a strange, weird, sentence." );
  PDS(g);

  dstring h = new_dstring ("strange");
  PDS(h);

  P("\tremove strange ");
  dstring i = remove_word ( g , h );
  PDS(i);


  P("\tremove This ");
  dstring j = remove_word ( i , "This" );
  PDS(j);



  P("\treplace \"a strange\" with \"not a\" ");
  dstring k = replace_word ( g , "a strange,", "not a" );
  PDS(k);

  askforkey("press any key");
  verheader( dstring_version() );
  P("==CAPS/TITLECASE TESTS==");

  dstring l = all_caps ( g );
  PDS(l);

  dstring m = uncapitalize( g );
  PDS(m);  

  dstring n = title_case ( g );
  PDS(n);  

  askforkey("press any key");
  verheader( dstring_version() );
  P("==CHOMP TEST==");

  dstring o = new_dstring_init(" This \r string has \n \t problems... , \nlots.\n" , 100);
  PDS(o);  

  dstring p = chomp(o);
  PDS(p); 

  P("\n==QUOTE TEST==");
  dstring q = quote(g);
  PDS(q);
 
  dstring r = unquote(q);
  PDS(r); 

  askforkey("press any key");
  verheader( dstring_version() );
  P("===TRIM TESTS===");

  dstring s = new_dstring("junk at both ends junk");
  PDS(s); 

  dstring t = ltrim(s , "junk" );
  PDS(t); 

  dstring u = rtrim(s , "junk" );
  PDS(u); 

  dstring v = trim(s , "junk" );
  PDS(v); 

  askforkey("press any key");
  verheader( dstring_version() );

  P("===FIND/TRIM/SLICE TESTS===");

  dstring w = new_dstring(" A B C D E F G H I J K L M N O P Q R S T U V W X Y Z 0 1 2 3 4 5 6 7 8 9");
  PDS(w); 
  dstring x =  find ( w, "N O ");
  PDS(x); 

  dstring y =  dstring_slice( x , 5 , 16 );
  PDS(y); 

  dstring z =  dstring_after( w, "Y Z ");
  PDS(z); 


  dstring a1 =  dstring_before( w, "J K L");
  PDS(a1); 


  P("===FREE MEMORY===");

  P("add all to an array and free the array");
  dstringarray dsa = dstringarray_new_init(24 , d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, a1);

  DSARRAY_PRINT(dsa) ;

  DSARRAY_FREE(dsa); /* frees all members -- then self */

  if (dsa) 
    printf("I still see dsa\n");
  else 
    printf("ALL freed\n");


  P("===DONE===");

  return 0;
} //main




/*END*/


