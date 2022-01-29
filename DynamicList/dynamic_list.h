#ifndef DYNAMIC_LIST
#define DYNAMIC_LIST

void free_list(List * list);

/**
 * Prints the stats of the list
 */
void print_stats(List * list);

/**
 * Prints the stats and the list
 */
void print_list(List * list);

/** 
 * Return is the alpha coefficient (the ration of fill to space)
 */
float get_alpha(List * list);

/**
 * Return is a new initialised List pointer.
 * the arr size is made to the first_size given -- if the first size is smaller than 1 -- size will be 1
 */
List * get_new_dynamic_list(unsigned int first_size);

/**
 * Removes the last element from the list
 * return is the last element removed or if attempted remove on empty list INFINITY
 * the list is reduced in size if the reduction brought the alpha to its lowbound
 */
int remove_last_elem(List * list);

/**
 * Inserts an element at the end of the list
 * An expansion will be made when the alpha hits the top bound
 */
void insert_last_elem(List * list, int elem);

/**
 * Return is the element at index 
 * if the index is invalid, the memory location is still accessed. A warning might be shown
 */
int get_elem(List * list, int index);


#endif