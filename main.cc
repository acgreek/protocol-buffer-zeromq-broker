#include <iostream>
#include <stdlib.h>
#include <getopt.h>
void usage (char * name) {
	std::cout << "Usage: " << name << " [OPTIONS]" << std::endl;
	exit(EXIT_FAILURE);
}
int main (int argc, char * argv[]) {
	int options;
	while (-1 != (options = getopt(argc, argv, "h"))) {

		switch (options) {
			default:
			case 'h':
				usage(argv[0]);
		}
	}
	return EXIT_SUCCESS;
}
