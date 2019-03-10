#include <stdio.h>
#include <string.h>
#include "find.h"

int main(int argc, char* argv[]) {
	save_find_result_to_file(".", "*.txt", "tempfile.txt");
	return 0;
}
