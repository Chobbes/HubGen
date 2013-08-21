/* Copyright (C) 2013 Calvin Beck

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation files
  (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <mux_pipe.h>

#include "pipes.h"
#include "codegen.h"


void usage(char *program_name)
{
    fprintf(stderr, "Usage: %s <input file> <output file>\n", program_name);
}


int main(int argc, char *argv[])
{
    if (3 != argc) {
	fprintf(stderr, "Invalid number of arguments!\n");
	usage(argv[0]);

	return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file = fopen(argv[2], "w");

    if (NULL == input_file) {
	fprintf(stderr, "No such file: \"%s\"\n", argv[1]);
	usage(argv[0]);

	return 1;
    }

    if (NULL == output_file) {
	fprintf(stderr, "Problem with output file: \"%s\"\n", argv[2]);
	usage(argv[0]);

	return 1;
    }

    size_t total_pipes;
    MuxPipe *pipes = load_pipes(input_file, &total_pipes);

    if (NULL == pipes) {
	fprintf(stderr, "Error parsing file: %s\n", argv[1]);
	usage(argv[0]);

	return 1;
    }

    fclose(input_file);
    printf("Loaded %zd pipes...\n", total_pipes);

    write_arduino_code(output_file, pipes, total_pipes);
    printf("Done. Wrote to \"%s\"\n", argv[2]);

    fclose(output_file);
    free(pipes);
    return 0;
}
