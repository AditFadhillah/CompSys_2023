#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "record.h"
#include "coord_query.h"

struct naive_data {
	struct record *rs;
	int n;
};

struct naive_data* mk_naive(struct record* rs, int n) {
	struct naive_data *data = malloc(sizeof(struct naive_data));
	data -> rs = rs;
	data -> n = n;
	return data;
}

void free_naive(struct naive_data* data) {
  	free(data); // Free the allocated memory, to prevent memory leak
}

const struct record* lookup_naive(struct naive_data *data, double lon, double lat) {
	double lowestDist = 1000000;          // Starts with an arbritrary high number
	struct record *lowestData = &data -> rs[1]; // lowestData define as the first record
  	for (int i = 0; i < (data -> n); i++) {     // For loop to go through all the records
		// The difference between the inputted lat and lon with the current lat and lon
    double currentDist = sqrt( pow(lon - data -> rs[i].lon, 2) + pow(lat - data -> rs[i].lat, 2));
		if (currentDist < lowestDist) {         // Checks if the difference is less than lowestDist
			lowestData = &data -> rs[i];        // Change the lowestData to be the current record
			lowestDist = currentDist;           // lowestDIst will become smaller and smaller
		}
	}
	return lowestData;       // Returns a record where the difference between the inputted lat and lon 
}                            // and the lat and lon for a place in the records is the smallest

int main(int argc, char** argv) {
  	return coord_query_loop(argc, argv,
                          (mk_index_fn)mk_naive,
                          (free_index_fn)free_naive,
                          (lookup_fn)lookup_naive);
}
