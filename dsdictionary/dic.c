/*dic.c*/

#include "dsdictionary.h"

/* basic dsdictionary functions test program*/

void dictest1(void);
void dictest2(void);

int main( int argc, char** argv, char** env)
{

  dictest1(); 
  dictest2(); 

  return 0;
}

/*END*/


void dictest1(void)
{
  /* create a new dictionary object */
  dsdictionary mydic = new_dic();

  /* create some entries */
  dic_entry dsde1 = new_dic_entry( "dog"  , "bone");
  dic_entry dsde2 = new_dic_entry( "cat" , "scratch");
  dic_entry dsde3 = new_dic_entry( "fish" , "swim");
  dic_entry dsde4 = new_dic_entry( "bird"    , "peck"  );
  dic_entry dsde5 = new_dic_entry( "cow"    , "pie"  );

  /* insert into mydic */
  add_entry( mydic , dsde1 );
  add_entry( mydic , dsde2 );
  add_entry( mydic , dsde3 );
  add_entry( mydic , dsde4 );
  add_entry( mydic , dsde5 );

  /* search for an entry in mydic */  
  PN("search key: fish");
  dstring d1 = val_for_key(mydic , "fish");

  P("fish:\t");
  PN(d1); 
 
  DSFREE(d1);

  /* output the entire contents of mydic */
  PN("DIC DUMP");
  dump_dic(mydic);

  delete_dic(mydic);

}

void dictest2(void)
{

  dsdictionary mydic = new_dic();
  dic_entry dsde1 = new_dic_entry( "car"  , "Chevrolet");
  dic_entry dsde2 = new_dic_entry( "car" , "Cadillac");
  dic_entry dsde3 = new_dic_entry( "bike" , "Schwinn");
  dic_entry dsde4 = new_dic_entry( "car"    , "Ford"  );
  dic_entry dsde5 = new_dic_entry( "bike"    , "Supertuff"  );
  dic_entry dsde6 = new_dic_entry( "other"    , "skateboard"  );

  add_entry( mydic , dsde1 );
  add_entry( mydic , dsde2 );
  add_entry( mydic , dsde3 );
  add_entry( mydic , dsde4 );
  add_entry( mydic , dsde5 );
  add_entry( mydic , dsde6 );

  /* find all entries under a key in mydic */
  dstringlist cars = all_vals_for_key(mydic , "car");
  dstringlist bikes = all_vals_for_key(mydic , "bike");
  
  PN("Search for all vals");

  P("\n");
  P("car:\t");
  print_dstringlist(cars);
  P("\n");

  P("\n");
  P("bike:\t");
  print_dstringlist(bikes);
  P("\n");

  PN("DIC DUMP");
  dump_dic(mydic);

  /* clean up */
  DSLISTFREE(bikes);
  DSLISTFREE(cars);

  delete_dic(mydic);

} // main

/*END*/

