#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "csv.h"
#include "tid_list.h"
#include "dirwalk.h"
#include "quicksort.h"

int get_field_index(char *field_name);

int main(int argc, char **argv)
{
	int c;
	int field_index;
	char *field_name;
	char *input_dir;
	char *output_dir;
	int verbosity;
	struct csv table;
	struct tid_list tids;
	struct dirwalk_args dwargs;
	struct quicksort_args qsargs;
	field_index = -1;
	input_dir = ".";
	output_dir = ".";
	verbosity = 2;
	while ((c = getopt(argc, argv, "c:d:o:sqv")) != -1) {
		switch (c) {
		case 'c':
			field_name = optarg;
			field_index = get_field_index(optarg);
			break;
		case 'd':
			input_dir = optarg;
			break;
		case 'o':
			output_dir = optarg;
			break;
		case 's':
			verbosity = 0;
			break;
		case 'q':
			verbosity = 1;
			break;
		case 'v':
			verbosity = 2;
		/* we will let getopt() deal with errors */
		}
	}
	if (field_index == -1) {
		fputs("please provide a field by which to sort\n", stderr);
	} else {
		initialize_csv(&table);
		initialize_tid_list(&tids);
		dwargs.table = &table;
		dwargs.tids = &tids;
		dwargs.dir = input_dir;
		dirwalk((void *) &dwargs);
		qsargs.matrix = table.matrix;
		qsargs.tids = &tids;
		qsargs.field_index = field_index;
		qsargs.low = 0;
		qsargs.high = table.num_rows;
		quicksort((void *) &qsargs);
		//serial_quicksort(table.matrix, field_index, 0, table.num_rows);
		print_csv(&table, field_name, output_dir);
		if (verbosity)
			print_tid_list(&tids, verbosity);
		free_csv(&table);
		free_tid_list(&tids);
	}
	return 0;
}
