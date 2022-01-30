#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "list.h"

// the fill ration (alpha) will be bounded by these two values
#define ALPHA_LOW 0.25
#define ALPHA_HIGH 1

void free_list(List * list){
    free(list->arr);
    free(list);
}

/**
 * Prints the stats of the list
 */
void print_stats(List * list){
    printf("Size: %d\n", list->size);
    printf("Num Elem: %d\n", list->num_elem);
}

/**
 * Prints the stats and the list
 */
void print_list(List * list){
    print_stats(list);
    int i;
    for(i = 0; i < list->num_elem; i++){
        printf("[%d] = %d\n", i, list->arr[i]);
    }
}

/*
 * Copies the array from src to dest, upto length
 * length is assumed to be valid-- garbage may be copied on account of client's negligance.
 */
void copy_arr(int * dest, int * src, int length){
    int i;
    for(int i = 0; i < length; i++){
        dest[i] = src[i];
    }
}

/** 
 * Return is the alpha coefficient (the ration of fill to space)
 */
float get_alpha(List * list){
    return ((float) list->num_elem) / ((float)list->size);
}

/**
 * Return is truee iff the elem is in the list
 */
bool elem_in_list(List * list, int elem){
    int i = 0;
    for(i = 0; i < list->num_elem; i++){
        if(list->arr[i] == elem){
            return true;
        }
    }
    return false;
}

/**
 * Return is true iff elem is in any of the lists
 */
bool elem_in_list_of_lists(List ** lists, int num_lists, int elem){
    int i = 0;
    for(i = 0; i < num_lists; i++){
        if(elem_in_list(lists[i], elem)){
            return true;
        }
    }
    return false;
}


/**
 * Return is a new initialised List pointer.
 * the arr size is made to the first_size given -- if the first size is smaller than 1 -- size will be 1
 */
List * get_new_dynamic_list(unsigned int first_size){
    if(first_size < 1) first_size = 1;
    int * arr = (int *) malloc(first_size * sizeof(int));
    List * list = (List *) malloc(sizeof(List));
    list->arr = arr;
    list->size = first_size;
    list->num_elem = 0;
    return list;
}

/**
 * Removes the last element from the list
 * return is the last element removed or if attempted remove on empty list INFINITY
 * the list is reduced in size if the reduction brought the alpha to its lowbound
 */
int remove_last_elem(List * list){
    if(list->num_elem == 0) return (int) INFINITY;
    int removed_elem = list->arr[list->size - 1];
    list->num_elem--;

    if(get_alpha(list) <= ALPHA_LOW){
        //reduce
        int new_size = list->size / 2;
        int * new_arr = (int *) malloc(new_size * sizeof(int));
        copy_arr(new_arr, list->arr, new_size);
        int * tmp_arr = list->arr;
        list->arr = new_arr;
        list->size = new_size;
        free(tmp_arr);
    }
    return removed_elem;
}

/**
 * Inserts an element at the end of the list
 * An expansion will be made when the alpha hits the top bound
 */
void insert_last_elem(List * list, int elem){
   if(get_alpha(list) >= ALPHA_HIGH){
       //full -- expand
        int new_size = list->size * 2;
        int * new_arr = (int *) malloc(new_size * sizeof(int));
        copy_arr(new_arr, list->arr, list->size);
        int * tmp_arr = list->arr;
        list->arr = new_arr;
        list->size = new_size;
        free(tmp_arr);
   }
   list->arr[list->num_elem++] = elem;
}

/**
 * Return is the element at index 
 * if the index is invalid, the memory location is still accessed. A warning might be shown
 */
int get_elem(List * list, int index){
    if(!(0 <= index < list->num_elem)){
        printf("You are index should be out of bounds, but I will let you get some garbage or potentially die on your own. Leben Sie Wohl.\n");
    }
    return list->arr[index];
}

void test_dynamic_list(){
    List * list = get_new_dynamic_list(2);
    insert_last_elem(list, 2);
    insert_last_elem(list, 3);
    print_list(list);
    insert_last_elem(list, 4);
    print_list(list);
    printf("%d\n", get_elem(list, 1));
    printf("%d\n", get_elem(list, 2));
    remove_last_elem(list);
    print_list(list);
    remove_last_elem(list);
    print_list(list);
    remove_last_elem(list);
    print_list(list);
    remove_last_elem(list);
    remove_last_elem(list);
    print_list(list);
}