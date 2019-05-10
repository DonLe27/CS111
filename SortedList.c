#include <stdlib.h>
#include "SortedList.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>


void SortedList_insert(SortedList_t *list, SortedListElement_t *element)
{
  //Make new node for insertion instead of using array directly (accessing array is global)
 SortedListElement_t * newNode = (SortedListElement_t *) malloc(sizeof(SortedListElement_t));
  if (newNode == NULL)
    {
      list->key = "Insertion error"; //let driver program know error happened
      return;
    }
  newNode->key = element -> key;
  SortedList_t *ptr = list;
  if ((ptr -> next)->key == NULL){
    list -> next = newNode;
    list -> prev = newNode;
    if (opt_yield & INSERT_YIELD)
      {
	sched_yield();
      }
    newNode -> next = list;
    newNode -> prev = list;
    return;
  }

  SortedList_t *last = list;
  ptr = last->next;
  if (last->key != NULL) //  *list is not the head
    return;
  while (ptr != list) {
    if (newNode == ptr) //Already in the list
      return;
    if (ptr == ptr->next)
      {
        list->key="Loop error";
        return;
      }
    if (ptr->key > newNode->key)
      break;
    last = ptr;
    if (opt_yield & INSERT_YIELD)
      sched_yield();
    ptr = ptr->next;
  }
  newNode->next = last->next;
  newNode->prev = last;
  last->next = newNode;
  newNode->next->prev = newNode;
}
int SortedList_delete( SortedListElement_t *element)
{
  //Check for validity
  if ((element->prev->next != element) || (element->next->prev != element))
    return 1;
  else
    {
      element->prev->next = element->next;
      if(opt_yield & DELETE_YIELD)
	sched_yield();
      element->next->prev = element->prev;
      //Don't actually delete the element since we free it later
      return 0;
    }
}

int SortedList_length(SortedList_t *list)
{
  int count = 0;
  if (list->next == NULL || list->next == list) //List is empty
    return 0;
  SortedListElement_t * ptr = list->next; //Start at beginning
  while (ptr->key != NULL)
    {
      if ((ptr->prev->next != ptr) || (ptr->next->prev != ptr))
	return -1;
      count++;
      if(opt_yield & LOOKUP_YIELD)
	sched_yield();
      ptr=ptr->next;
    }
  return count;
}
SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key)
{
  if (list->next == NULL) //List is empty
    return NULL;
  SortedListElement_t * ptr = list->next; //Start at beginning
  while (ptr->key != NULL)
    {
      if(ptr->key == key)
	return ptr;
      if(opt_yield & LOOKUP_YIELD)
	sched_yield();
      ptr=ptr->next;
    }
  return NULL; //Didn't find it
}
/*
void random_key(SortedListElement_t *element)
{
  char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char *temp = malloc(length * sizeof(char));
  int i = 0;
  for (; i < length; i++)
    {
      int index = abs(rand() % (strlen(charset)-1));
      temp[i] = charset[index];
    }
  temp[length-1] = '\0';
  element->key = temp;
}

//TEST SCRIPT FOR LINKED LIST

int main()
{
  SortedListElement_t *headElement = malloc(sizeof(SortedListElement_t));
  headElement->next = NULL;
  headElement->prev = NULL;
  headElement->key = NULL;
  //Make array of elements
  int numElements = 8;
  SortedListElement_t *elementArray = malloc(sizeof(SortedListElement_t) * numElements);

  int i = 0;
  for (; i < numElements; i++)
    {
      //The elements will be intialized randomly
      elementArray[i].next = NULL;
      elementArray[i].prev = NULL;
      random_key(&elementArray[i]);
    }
  int iterations = 0;
  for (; iterations < 3 ; iterations++)
    {
      i = 0;
      for (; i < numElements; i++)
	{
	  SortedList_insert(headElement, &elementArray[i]);
	}
      
      SortedListElement_t *ptr = headElement->next;
      while (ptr->key != NULL)
	{
	  printf(ptr->key);
	  ptr=ptr->next;
	}
      
      int j = 0;
      for (; j < numElements; j++)
	{
	  if (SortedList_lookup(headElement, elementArray[j].key) ==NULL)
	    printf("couldnt find a lookup");
	}
      int count = 0;
      count = SortedList_length(headElement);
      if (count == -1)
	printf("error found while counting");
      printf("count: %d", count);
      j = 0;
      for (; j < numElements; j++)
	{
	  if (SortedList_delete(&elementArray[j]) == 1)
	    printf("corrupt found when deleting");
	}
      count = SortedList_length(headElement);
      printf("count should be 0: %d", count);
    }
  free(elementArray);
  free(headElement);

}
*/
