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

#include <stdio.h>
#include <stdlib.h>
#include <mux_pipe.h>


static const char *indent = "    ";


static void write_indents(FILE *out_file, int level)
{
    for (int amount = 0; amount < level; ++amount) {
	fprintf(out_file, "%s", indent);
    }
}


static void out_name(FILE *out_file, int out_pin)
{
    fprintf(out_file, "out%d", out_pin);
}


static void out_channel(FILE *out_file, int out_pin)
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


static int int_in_array(int number, int *array, size_t array_length)
{
    for (size_t count = 0; count < array_length; ++count) {
	int current = array[count];

	if (current == number) {
	    return 1;
	}
    }

    return 0;
}

static void write_output_decs(FILE *out_file,
			      MuxPipe *pipes,
			      size_t total_pipes)
{
    int outputs[total_pipes];
    size_t output_count = 0;

    /* Output declarations */
    for (size_t index = 0; index < total_pipes; ++index) {
	MuxPipe pipe = pipes[index];

	/* Check if we have already recorded the output */
	if (int_in_array(pipe.out_pin, outputs, output_count)) {
	    continue;
	}

	/* Write the output declaration */
	write_indents(out_file, 1);
	fprintf(out_file, "output ");
	out_name(out_file, pipe.out_pin);
	fprintf(out_file, " port %d = false;\n", pipe.out_pin);

	/* Write the channel declaration for the output */
	write_indents(out_file, 1);
	fprintf(out_file, "int ");
	out_channel(out_file, pipe.out_pin);
	fprintf(out_file, " = %d;\n\n", pipe.channel);

	/* Record in the outputs array */
	outputs[output_count] = pipe.out_pin;
	++output_count;
    }
}


static void write_input_decs(FILE *out_file, MuxPipe *pipes, size_t total_pipes)
{
    int inputs[total_pipes];
    size_t input_count = 0;

    /* Input declarations */
    for (size_t index = 0; index < total_pipes; ++index) {
	MuxPipe pipe = pipes[index];

	/* Check if we have already recorded the input */
	if (int_in_array(pipe.in_pin, inputs, input_count)) {
	    continue;
	}

	/* Write the output declaration */
	write_indents(out_file, 1);
	fprintf(out_file, "input ");
	in_name(out_file, pipe.in_pin);
	fprintf(out_file, " port %d;\n", pipe.in_pin);

	/* Record in the inputs array */
	inputs[input_count] = pipe.in_pin;
	++input_count;
    }
}


static void write_out_updates(FILE *out_file,
			      MuxPipe *pipes,
			      size_t total_pipes,
			      int out_pin)
{
    int channels[total_pipes];
    size_t channel_count = 0;

    for (size_t index = 0; index < total_pipes; ++index) {
	MuxPipe pipe = pipes[index];

	/* Check if we need to add something for this pipe. */
	if (pipe.out_pin != out_pin) {
	    continue;
	}
	else if (int_in_array(pipe.channel, channels, channel_count)) {
	    continue;
	}

	write_indents(out_file, 3);

	if (0 == channel_count) {
	    fprintf(out_file, "if (");
	}
	else {
	    fprintf(out_file, "else if (");
	}

	out_channel(out_file, out_pin);
	fprintf(out_file, " == %d) {\n", pipe.channel);

	/* Body of the if statement */
	write_indents(out_file, 4);
	out_name(out_file, out_pin);
	fprintf(out_file, " = ");
	write_channel_inputs(out_file,
			     pipes,
			     total_pipes,
			     out_pin,
			     pipe.channel);
	fprintf(out_file, ";\n");

	write_indents(out_file, 3);
	fprintf(out_file, "}\n");

	/* Now add to the array of channels that we have seen */
	channels[channel_count] = pipe.channel;
	++channel_count;
    }
}


static void write_phase(FILE *out_file, MuxPipe *pipes, size_t total_pipes)
{
    int outputs[total_pipes];
    size_t output_count = 0;

    write_indents(out_file, 2);
    fprintf(out_file, "phase update {\n");

    for (size_t index = 0; index < total_pipes; ++index) {
	MuxPipe pipe = pipes[index];

	if (int_in_array(pipe.out_pin, outputs, output_count)) {
	    continue;
	}

	/* Write the update statements for the output */
	write_out_updates(out_file, pipes, total_pipes, pipe.out_pin);
	fprintf(out_file, "\n");

	/* Add output to the array of outputs that we have seen */
	outputs[output_count] = pipe.out_pin;
	++output_count;
    }

    write_indents(out_file, 2);
    fprintf(out_file, "}\n");
}


static void write_process(FILE *out_file, MuxPipe *pipes, size_t total_pipes)
{
    write_indents(out_file, 1);
    fprintf(out_file, "process mux_update {\n");

    write_phase(out_file, pipes, total_pipes);

    write_indents(out_file, 1);
    fprintf(out_file, "}");
}


void write_facsimile_code(FILE *out_file, MuxPipe *pipes, size_t total_pipes)
{
    fprintf(out_file, "unit hub {\n");
    write_output_decs(out_file, pipes, total_pipes);
    fprintf(out_file, "\n");

    write_input_decs(out_file, pipes, total_pipes);
    fprintf(out_file, "\n");

    write_process(out_file, pipes, total_pipes);
    fprintf(out_file, "\n}\n");
}
