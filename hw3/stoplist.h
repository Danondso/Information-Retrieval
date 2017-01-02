#include <fstream>
#include "hashtable.h"


class stoplist{


public:
 void ReadWordList(string file);
 stoplist();

private:
 hashtable table;

};
