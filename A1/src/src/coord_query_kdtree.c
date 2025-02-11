#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "record.h"
#include "coord_query.h"

const int k = 2;

struct index_record {
    int64_t osm_id;
    const struct record *record;
};

struct kdtree_data {
  struct kdtree_data *left, *right;
  int* point[k];
  struct record *rs;
  int n;
};

struct kdtree_data* newNode(struct record *rs, int n) {
    struct kdtree_data *temp = calloc(n ,sizeof(struct kdtree_data));
    //struct kdtree_data* temp = new struct kdtree_data

    for (int i=0; i < (k); i++)
       temp -> point[i] = &rs[i];
  
    temp -> left = temp -> right = NULL;
    return temp;
}

struct kdtree_data *insertRec(kdtree_data *root, int point[], unsigned depth) {
    if (root == NULL) // Tree is empty?
       return newNode(point);
  
    unsigned cd = depth % k; // Calculate current dimension (cd) of comparison
    // Compare the new point with root on current dimension 'cd', and decide the left or right subtree
    if (point[cd] < (root->point[cd]))
        root->left  = insertRec(root->left, point, depth + 1);
    else
        root->right = insertRec(root->right, point, depth + 1);
  
    return root;
}

struct kdtree_data* insert(struct kdtree_data *root, int point[]) {
    return insertRec(root, point, 0); 
}

bool arePointsSame(int point1[], int point2[]) {
    for (int i = 0; i < k; ++i) // Compare individual point values
        if (point1[i] != point2[i])
            return false;
  
    return true;
}

bool searchRec(struct kdtree_data* root, int point[], unsigned depth) {
    if (root == NULL) // Base cases
        return false;
    if (arePointsSame(root->point, point))
        return true;
 
    unsigned cd = depth % k;// Current dimension is computed using current depth and total dimensions (k)
  
    if (point[cd] < root->point[cd]) // Compare point with root with respect to cd (Current dimension)
        return searchRec(root->left, point, depth + 1);
  
    return searchRec(root->right, point, depth + 1);
}
  
bool search(struct kdtree_data* root, int point[]) { // Searches a Point in the K D tree. It mainly uses searchRec()
    return searchRec(root, point, 0); // Pass current depth as 0
}

struct kdtree_data* mk_kdtree(struct record* rs, int n) {
  struct kdtree_data *data = malloc(sizeof(struct kdtree_data));
  data -> rs = rs;
  data -> n = n;
  return data;
}

void free_kdtree(struct kdtree_data* data) {
  free(data);      // Free the allocated memory, to prevent memory leak
}

const struct record* lookup_kdtree(struct kdtree_data *data, double lon, double lat) {
	double lowestDist = 1000000;                 // Starts with an arbritrary high number
	struct record *lowestData = &data -> rs[1];  // lowestData define as the first record
  	for (int i = 0; i < (data -> n); i++) {      // For loop to go through all the records
		// The difference between the inputted lat and lon with the current lat and lon
        double currentDist = sqrt( pow(lon - data -> rs[i].lon, 2) + pow(lat - data -> rs[i].lat, 2));
		if (currentDist < lowestDist) {  // Checks if the difference is less than lowestDist
			lowestData = &data -> rs[i]; // Change the lowestData to be the current record
			lowestDist = currentDist;    // lowestDIst will become smaller and smaller
		}
	}
	return lowestData;     // Returns a record where the difference between the inputted lat and lon 
}                          // and the lat and lon for a place in the records is the smallest

int main(int argc, char** argv) {
  return coord_query_loop(argc, argv,
                          (mk_index_fn)mk_kdtree,
                          (free_index_fn)free_kdtree,
                          (lookup_fn)lookup_kdtree);
}
