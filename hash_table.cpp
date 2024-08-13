/*
Student Name: Nathan Barr
Date: 11/24/2019
=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.
FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.
===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
e.g. "HashTable", or "HashTableEntry"
2. Variable names with a preceding underscore "_" will not be called directly.
e.g. "_HashTable", "_HashTableEntry"
Recall that in C, we have to type "struct" together with the name of the struct
in order to initialize a new variable. To avoid this, in hash_table.h
we use typedef to provide new "nicknames" for "struct _HashTable" and
"struct _HashTableEntry". As a result, we can create new struct variables
by just using:
- "HashTable myNewTable;"
or
- "HashTableEntry myNewHashTableEntry;"
The preceding underscore "_" simply provides a distinction between the names
of the actual struct defition and the "nicknames" that we use to initialize
new structs.
[See Hidden Definitions section for more information.]
3. Functions, their local variables and arguments are named with camel case, where
the first letter is lower-case.
e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
It also has a local variable called "newTable".
4. The name of a struct member is divided by using underscores "_". This serves
as a distinction between function local variables and struct members.
e.g. "num_buckets" is a member of "HashTable".
*/

/****************************************************************************
* Include the Public Interface
*
* By including the public interface at the top of the file, the compiler can
* enforce that the function declarations in the the header are not in
* conflict with the definitions in the file. This is not a guarantee of
* correctness, but it is better than nothing!
***************************************************************************/
#include "hash_table.h"


/****************************************************************************
* Include other private dependencies
*
* These other modules are used in the implementation of the hash table module,
* but are not required by users of the hash table.
***************************************************************************/
#include <stdlib.h>   // For malloc and free
#include <stdio.h>    // For printf


/****************************************************************************
* Hidden Definitions
*
* These definitions are not available outside of this file. However, because
* the are forward declared in hash_table.h, the type names are
* available everywhere and user code can hold pointers to these structs.
***************************************************************************/
/**
* This structure represents an hash table.
* Use "HashTable" instead when you are creating a new variable. [See top comments]
*/
struct _HashTable {
  /** The array of pointers to the head of a singly linked list, whose nodes
  are HashTableEntry objects */
  HashTableEntry** buckets;

  /** The hash function pointer */
  HashFunction hash;

  /** The number of buckets in the hash table */
  unsigned int num_buckets;
};

/**
* This structure represents a hash table entry.
* Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
*/
struct _HashTableEntry {
  /** The key for the hash table entry */
  unsigned int key;

  /** The value associated with this hash table entry */
  void* value;

  /**
  * A pointer pointing to the next hash table entry
  * NULL means there is no next entry (i.e. this is the tail)
  */
  HashTableEntry* next;
};


/****************************************************************************
* Private Functions
*
* These functions are not available outside of this file, since they are not
* declared in hash_table.h.
***************************************************************************/
/**
* createHashTableEntry
*
* Helper function that creates a hash table entry by allocating memory for it on
* the heap. It initializes the entry with key and value, initialize pointer to
* the next entry as NULL, and return the pointer to this hash table entry.
*
* @param key The key corresponds to the hash table entry
* @param value The value stored in the hash table entry
* @return The pointer to the hash table entry
*/
static HashTableEntry* createHashTableEntry(unsigned int key, void *value) {

  HashTableEntry* newEntry = (HashTableEntry*)malloc(sizeof(HashTableEntry));
  newEntry ->key = key;
  newEntry ->value = value;
  newEntry ->next = NULL;

  return newEntry;
}

/**
* findItem
*
* Helper function that checks whether there exists the hash table entry that
* contains a specific key.
*
* @param hashTable The pointer to the hash table.
* @param key The key corresponds to the hash table entry
* @return The pointer to the hash table entry, or NULL if key does not exist
*/
 static HashTableEntry* findItem(HashTable* hashTable, unsigned int key) {

  //get which bucket
  int bucketIndex = hashTable -> hash(key);

  /**
  * set thisNode to head of bucket we are looking in
  */
  HashTableEntry *thisNode = hashTable->buckets[bucketIndex];

  while(thisNode) //while not null
  {
    if(thisNode -> key == key) return thisNode; //if equal, return that entry
    thisNode = thisNode->next;//if not, get next entry
  }
  return thisNode; // return value, null if not found
}


/****************************************************************************
* Public Interface Functions
*
* These functions implement the public interface as specified in the header
* file, and make use of the private functions and hidden definitions in the
* above sections.
****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable* createHashTable(HashFunction hashFunction, unsigned int numBuckets) {
  // The hash table has to contain at least one bucket. Exit gracefully if
  // this condition is not met.
  if (numBuckets==0) {
    printf("Hash table has to contain at least 1 bucket...\n");
    exit(1);
  }

  // Allocate memory for the new HashTable struct on heap.
  HashTable* newTable = (HashTable*)malloc(sizeof(HashTable));

  // Initialize the components of the new HashTable struct.
  newTable->hash = hashFunction;
  newTable->num_buckets = numBuckets;
  newTable->buckets = (HashTableEntry**)malloc(numBuckets*sizeof(HashTableEntry*));

  // As the new buckets are empty, init each bucket as NULL.
  unsigned int i;
  for (i=0; i<numBuckets; ++i) {
    newTable->buckets[i] = NULL;
  }

  // Return the new HashTable struct.
  return newTable;
}


void destroyHashTable(HashTable* hashTable) {

  int i;
  //iterate through all all buckets, if 4 buckets, final value of i is 3
  for (i = 0; i< (hashTable -> num_buckets); i++)
  {
    //if bucket's head entry is not null, delete all entries
    if(hashTable -> buckets[i])
    {
      //keep track of currentNode
      HashTableEntry* thisNode = hashTable -> buckets[i];
      //keep track of nextNode so can iterate until there is only
      //one entry left in bucket
      HashTableEntry *nextNode = thisNode -> next;

      while(nextNode)
      {
        //destroy thisNode and all its variables
        free(thisNode -> value);
        free(thisNode);
        //update thisNode and nextNode
        thisNode = nextNode;
        nextNode = nextNode ->next;
      }
      //delete the final remaining entry, the tail
      free(thisNode);
    }
  }
  //destroy hashTable
  free(hashTable);
  return;
}

void* insertItem(HashTable* hashTable, unsigned int key, void* value) {

  //get which bucket to insert into
  int bucketIndex = hashTable -> hash(key);

  /**
  * If there exists entry with passed in key, change its value
  */
  HashTableEntry *existNode = findItem(hashTable,key);
  if(existNode)
  {
    //make copy of exisiting value
    void* oldValue;
    oldValue = existNode -> value;
    //update value
    existNode -> value = value;
    //return value that is replaced
    return oldValue;
  }

  //create new entry with specified key and value
  HashTableEntry *thisNode = createHashTableEntry(key,value); //create new entry

  //Edge case to check if malloc fails in createHashTableEntry, return NULL
  if(!thisNode) return NULL;

  /**
  * Insert at head
  * 1) change head of bucket to new entry
  * 2) make new entry/head point to the changed head
  */
  thisNode -> next = hashTable->buckets[bucketIndex];
  hashTable->buckets[bucketIndex] = thisNode;
  return NULL;
}


void* getItem(HashTable* hashTable, unsigned int key) {

  //check if entry/node exists by calling findItem()
  HashTableEntry* existNode = findItem(hashTable,key);
  if(existNode)
  {
    return existNode -> value;
  }
  return NULL;
}

void* removeItem(HashTable* hashTable, unsigned int key) {

  //get which bucket
  int bucketIndex = hashTable -> hash(key);

  //get head of bucket
  HashTableEntry *thisNode = hashTable -> buckets[bucketIndex];

  //remove head
  if(thisNode && thisNode -> key == key)
  {
    //get value of entry to be removed
    void* removedEntryVal = thisNode -> value;
    // update head
    hashTable -> buckets[bucketIndex] = thisNode ->next;
    //remove head entry
    free(thisNode);
    //return value of removed entry
    return removedEntryVal;
  }

  //remove node other than head
  while(thisNode && thisNode -> next)
  {
    //check if next entry is one to remove
    if(thisNode -> next -> key == key)
    {
      //save entry to be removed
      HashTableEntry* entryToRemove = thisNode -> next;
      //save value of entry to be removed
      void* removedEntryVal = entryToRemove -> value;
      //remove entry from list by pointing around it and freeing it
      thisNode -> next = thisNode -> next -> next;
      free(entryToRemove);
      //return value of removed entry
      return removedEntryVal;
    }
    //if not, get next entry
    thisNode = thisNode -> next;
  }
  //if entry not in table, return NULL
  return NULL;
}


void deleteItem(HashTable* hashTable, unsigned int key) {
  //get which bucket
  int bucketIndex = hashTable -> hash(key);

  //get head of bucket
  HashTableEntry *thisNode = hashTable -> buckets[bucketIndex];
  if (findItem(hashTable,key) == NULL) return;

  //delete head
  if (thisNode && thisNode ->key == key)
  {
    //update the head
    hashTable -> buckets[bucketIndex] = thisNode -> next;
    //free value of head
    free(thisNode -> value);
    //free head entry
    free(thisNode);
    return;
  }

  //delete node other than head
  while(thisNode && thisNode -> next)
  {
    //check if next entry is one to delete
    if(thisNode -> next -> key == key)
    {
      //save entry to be deleted
      HashTableEntry* entryToDelete = thisNode -> next;
      //remove entry from list by pointing around it and freeing it
      thisNode -> next = thisNode -> next -> next;
      //free value
      free(entryToDelete -> value);
      //free entry
      free(entryToDelete);
      return;
    }
    //if not, get next entry
    thisNode = thisNode -> next;
  }
}