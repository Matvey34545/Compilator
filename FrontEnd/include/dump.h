#ifndef DUMP
#define DUMP

#include "bynar_tree.h"
#include "syntax.h"
#include "split_token.h"

ErrorFrontEnd dump(const BynarTree *tree, const char *filename, const char *png_file);

#endif
