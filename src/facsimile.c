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

#include "facsimile.h"
#include <stdlib.h>
#include <mux_pipe.h>


static const char *indent = "    ";


static void write_indents(FILE *out_file, int level)
{
    for (int amount = 0; amount < level; ++amount) {
	fprintf(out_file "%s", indent);
    }
}


static void out_name(FILE *out_file, int out_pin)
{
    fprintf(out_file, "out%d", out_pin);
}


static void out_chan(FILE *out_file, int out_pin)
{
    fprintf(out_file, "out%d_chan", out_pin);
}


static void in_name(FILE *out_file, int in_pin)
{
    fprintf(out_file, "in%d", in_pin);
}


/* Write all of the inputs for a certain channel to a lovely or'd expression */
static void write_channel_inputs(FILE *out_file,
				 MuxPipe *pipes,
				 size_t total_pipes,
				 int out_pin,
				 int channel)
{
    size_t num_ins = 0; /* Number of inputs that we have written */

    for (size_t index = 0; index < total_pipes; ++index) {
	MuxPipe current = pipes[index];

	if (out_pin == current.out_pin && channel == current.channel) {
	    if (0 != num_ins) {
		/* This is not the first one - we need an or! */
		fprintf(out_file, " || ");
	    }

	    in_name(out_file, current.in_pin);
	}
    }
}


void write_facsimile_code(FILE *out_file, MuxPipe *pipes, size_t total_pipes)
{
    fprintf(out_file, "unit hub {\n");
    fprintf(out_file, "\n}\n");
}
