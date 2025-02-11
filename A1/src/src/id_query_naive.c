#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "record.h"
#include "id_query.h"

struct naive_data {
	struct record *rs;
	int n;
};

struct naive_data* mk_naive(struct record* rs, int n) {
	struct naive_data *data = malloc(sizeof(struct naive_data));
	data -> rs = rs;   // data stores all the records
	data -> n = n;     // n is the number of records
	return data;
}

void free_naive(struct naive_data* data) {
  	free(data);     // Free the allocated memory, to prevent memory leak
}

const struct record* lookup_naive(struct naive_data *data, int64_t needle) {
	for (int i = 1; i < (data -> n); i++) {      // For loop to go through all the record
		if (data -> rs[i].osm_id == needle) {      // Compares the current osm_id with needle osm_id
			return &data -> rs[i];      // Returns the record pointer where current osm_id == needle
		};
	}
	return NULL;
}

int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_naive,
                    (free_index_fn)free_naive,
                    (lookup_fn)lookup_naive);
}
