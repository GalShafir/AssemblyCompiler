#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "global_definitions.h"
#include "utility_functions.h"


/**
 * Open a File
 *
 * This function opens a file with the specified name and access mode.
 *
 * @param fileName - The name of the file to open.
 * @param accessMode - The access mode for opening the file.
 * @return A file pointer or NULL if the file cannot be opened.
 */
 
FILE* openFile(const char * fileName, const char * accessMode) {
    
    FILE *file = NULL;

    /* Open the file for reading and check for errors */

    file = fopen(fileName, accessMode);
    if (file == NULL) {
        perror(FILE_OPEN_ERROR);
        return NULL;
    }

    return file;
}

/**
 * Remove File Extension from the Input File Name
 *
 * This function takes an input file name and removes the file extension (if exists).
 * It returns a new dynamically allocated string containing the modified file name.
 *
 * @param inputFileName - The input file name to process.
 * @return char* - A dynamically allocated string containing the file name without the extension.
 */

char *removeFileExtension(const char *inputFileName) {

    const char *dotPosition;
    size_t length;
    char *outputFileName;

    /* Find the last occurrence of '.' in the input file name */
    dotPosition = strrchr(inputFileName, '.');
    if (dotPosition != NULL) {
        /* Calculate the length of the file name without the extension */
        size_t length = dotPosition - inputFileName;

        /* Allocate memory for the new string */
        outputFileName = (char *)malloc(length + 1);

        /* Check for memory allocation errors */
        if (outputFileName == NULL) {
            perror(MEMORY_ALLOCATION_ERROR);
            exit(EXIT_FAILURE);
        }

        /* Copy the file name without the extension */
        strncpy(outputFileName, inputFileName, length);

        /* Null-terminate the string */
        outputFileName[length] = '\0';

        return outputFileName;
    } else {
        /* No dot found, return a copy of the entire input file name */
        length = strlen(inputFileName);

        /* Allocate memory for the new string */
        outputFileName = (char *)malloc(length + 1);

        /* Check for memory allocation errors */
        if (outputFileName == NULL) {
            perror(MEMORY_ALLOCATION_ERROR);
            exit(EXIT_FAILURE);
        }

        /* Copy the entire input file name */
        strcpy(outputFileName, inputFileName);

        return outputFileName;
    }
}

/**
 * Remove Whitespace Characters from the Input String
 *
 * This function removes whitespace characters from the input string.
 *
 * @param inputString - The input string to be modified.
 */

void removeWhiteSpaces(char *inputString) {
    int i, j = 0;
    int length = strlen(inputString);

    /* Iterate through the characters of the string */
    for (i = 0; i < length; i++) {
        /* If the current character is not a whitespace, copy it to the new position */
        if (inputString[i] != ' ' && inputString[i] != '\t' && inputString[i] != '\n' && inputString[i] != '\r') {
            inputString[j] = inputString[i];
            j++;
        }
    }

    /* Null-terminate the modified string */
    inputString[j] = '\0';
}

/**
 * Remove Leading Spaces from the Input String
 * @param str - The input string to be modified.
 */
void removeLeadingSpaces(char *str) {

    int i = 0;
    int j = 0;

    if (str == NULL || str[0] == '\0') {
        /* Handle invalid or empty string */
        return;
    }

    /* Skip leading spaces or tabs */
    while (isspace(str[i])) {
        i++;
    }

    /* Move non-space characters to the beginning of the string */
    while (str[i] != '\0') {
        str[j++] = str[i++];
    }

    /* Null-terminate the modified string */
    str[j] = '\0';
}


/**
 * Split inputString into an array of strings based on the specified separators.
 *
 * @param inputString - The input string to be split.
 * @param separators - The characters used as separators.
 * @param count - Pointer to an integer to store the number of strings in the result.
 * @return An array of strings or NULL if memory allocation fails.
 */
char **splitString(const char *inputString, const char *separators, int *count) {
    
    int length = strlen(inputString);
    char buffer[MAX_LINE_LENGTH + 2];
    int i = 0, j, k;

    /* Marks the starting index of a word */
    int startIndex; 
    int stringIndex = 0;
    int toAllocate; 

    char *stringCopy;
    char **resultStrings;


    /* Allocate memory for the copy of the input string */
    stringCopy = (char *)malloc((length + 1) * sizeof(char));
    if (stringCopy == NULL) {
        /* Failed to allocate memory for the copy */
        perror(MEMORY_ALLOCATION_ERROR);
        exit(EXIT_FAILURE);
    }

    strncpy(stringCopy, inputString, length + 1);

    /* Ignore the newline character in the last word */
    if (length > 0 && stringCopy[length - 1] == '\n') {
        stringCopy[length - 1] = '\0';
        length--;
    }

    /* Count the number of strings */
    *count = 0;
    while (i < length) {
        /* Skip the separators */
        while (i < length && strchr(separators, stringCopy[i]) != NULL) {
            i++;
        }
        /* Mark the starting index of a word */
        startIndex = i;
        while (i < length && strchr(separators, stringCopy[i]) == NULL) {
            i++;
        }
        /* If a word was found, increment the count */
        if (i > startIndex) {
            (*count)++;
        }
    }

    /* Allocate memory for the array of strings */
    resultStrings = (char **)malloc((*count) * sizeof(char *));
    if (resultStrings == NULL) {
        /* Failed to allocate memory for the array */
        free(stringCopy);
        return NULL;
    }

    /* Split the input string into strings */
    i = 0;
    while (i < length) {
        /* Skip the separators */
        while (i < length && strchr(separators, stringCopy[i]) != NULL) {
            i++;
        }
        /* Mark the starting index of a word */
        j = 0;
        while (i < length && strchr(separators, stringCopy[i]) == NULL) {

            /* Copy the word into the buffer */
            buffer[j] = stringCopy[i];
            i++;
            j++;

        }
        /* If a word was found, copy it into the result array */
        if (j > 0) {

            buffer[j] = '\0';

            /* Allocate memory for the string */
            toAllocate = (strlen(buffer) + 1) * sizeof(char);
            resultStrings[stringIndex] = (char *)malloc(toAllocate);

            if (resultStrings[stringIndex] == NULL) {
                /* Failed to allocate memory for a string */
                free(stringCopy);
                for (k = 0; k < stringIndex; k++) {
                    free(resultStrings[k]);
                }
                free(resultStrings);
                return NULL;
            }
            /* Copy the string into the result array */
            strcpy(resultStrings[stringIndex], buffer);
            
            stringIndex++;
        }
    }

    /* Free the copy of the input string */
    free(stringCopy);

    /* Return the array of strings */
    return resultStrings;
}

/**
 * Free the memory allocated for the array of strings.
 *
 * @param splittedStrings - The array of strings to be freed.
 * @param count - The number of strings in the array.
 */
void freeStringArray(char **splittedStrings, int count) {
    int i = 0;
    for (i = 0; i < count; i++) {
        free(splittedStrings[i]);
    }
    free(splittedStrings);
}


/**
 * Print the array of strings.
 *
 * @param strings - The array of strings to be printed.
 * @param count - The number of strings in the array.
 */
void printStringArray(char **strings, int count) {
    int i = 0;

    printf("\nString Array:\n");
    printf("-------------------\n");

    for (i = 0; i < count; i++) {
        printf("%s\n", strings[i]);
    }

    printf("-------------------\n");

}

/**
 * removeSubstring - Remove a specified substring from a given string
 * @param str - The input string
 * @param subStr - The substring to be removed
 */
void removeSubstring(char *str, const char *subStr) {
    /* Find the first occurrence of the substring in the string */
    char *found = strstr(str, subStr);

    /* If the substring is found */
    while (found != NULL) {
        /* Calculate the length of the substring */
        size_t subStrLen = strlen(subStr);

        /* Move characters following the substring to overwrite it */
        memmove(found, found + subStrLen, strlen(found + subStrLen) + 1);

        /* Search for the next occurrence of the substring */
        found = strstr(str, subStr);
    }
}
/**
 * removeLastCharacter - Remove the last character from a given string
 * @param str - The input string
 */
void removeLastCharacter(char *str) {
    size_t length = strlen(str);

    if (str == NULL || str[0] == '\0') {
        /* Handle invalid or empty string */
        return;
    }


    /* Check if the string has at least one character */
    if (length > 0) {
        /* Replace the last character with the null terminator */
        str[length - 1] = '\0';
    }
}

/**
 * isValidInteger - Check if a given string represents a valid integer.
 * @param str - The string to be checked.
 * @return True if valid, False otherwise.
 */
bool isValidInteger(const char *str) {
    if (str == NULL || *str == '\0') {
        /* Empty string is not a valid integer */
        return False;
    }

    /* Skip leading whitespaces */
    while (isspace(*str)) {
        str++;
    }

    /* Check for optional sign */
    if (*str == '+' || *str == '-') {
        str++;
    }

    /* Check if the remaining characters are digits */
    while (isdigit(*str)) {
        str++;
    }

    /* Check if the string has been fully processed (no trailing characters) */
    if(*str == '\0'){
        return True;
    } else {
        return False;
    }
}

/* Function to convert integer to string */
char* intToString(int num) {
    /* Allocate memory for the string representation */
    char* str = (char*)malloc(12); /* 12 is enough for INT_MIN */

    if (str == NULL) {
        /* Handle memory allocation failure */
        perror(MEMORY_ALLOCATION_ERROR);
        exit(EXIT_FAILURE);
    }

    /* Use sprintf to convert int to string */
    sprintf(str, "%d", num);

    return str;
}

/* Function to convert string to integer */
int stringToInt(const char* str) {
    /* Use strtol to convert string to int */
    return strtol(str, NULL, 10);
}

int countCharacters(const char *str) {
    int count = 0;

    /* Iterate through the string until the null terminator is reached */
    while (str[count] != '\0') {
        count++; /* Increment count for each character encountered */
    }

    return count;
}



unsigned long hash_function(char *str)
{
    unsigned long i = 0;
    int j;

    for (j = 0; str[j]; j++)
        i += str[j];

    return i % HT_CAPACITY;
}


LinkedList *allocate_list()
{
    /* Allocates memory for a LinkedList pointer. */
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    return list;
}

LinkedList *linkedlist_insert(LinkedList *list, Ht_item *item)
{
    LinkedList *head;
    LinkedList *temp;
    LinkedList *node;

    /* Inserts the item onto the LinkedList. */
    if (!list)
    {
        head = allocate_list();
        head->item = item;
        head->next = NULL;
        list = head;
        return list;
    }
    else if (list->next == NULL)
    {
        LinkedList *node = allocate_list();
        node->item = item;
        node->next = NULL;
        list->next = node;
        return list;
    }

    temp = list;

    while (temp->next->next)
    {
        temp = temp->next;
    }

    node = allocate_list();
    node->item = item;
    node->next = NULL;
    temp->next = node;
    return list;
}

Ht_item *linkedlist_remove(LinkedList *list)
{
    LinkedList *node;
    LinkedList *temp;
    Ht_item *it = NULL;

    /* Removes the head from the LinkedList.
       Returns the item of the popped element. */
    if (!list)
        return NULL;

    if (!list->next)
        return NULL;

    node = list->next;
    temp = list;
    temp->next = NULL;
    list = node;
    
    memcpy(temp->item, it, sizeof(Ht_item));
    free(temp->item->key);
    free(temp->item->value);
    free(temp->item->type);
    free(temp->item->address);
    free(temp->item->memorySize);
    free(temp->item->order);
    free(temp->item);
    free(temp);
    return it;
}

void free_linkedlist(LinkedList *list)
{
    LinkedList *temp = list;

    while (list)
    {
        temp = list;
        list = list->next;
        free(temp->item->key);
        free(temp->item->value);
        free(temp->item->type);
        free(temp->item->address);
        free(temp->item->memorySize);
        free(temp->item->order);
        free(temp->item);
        free(temp);
    }
}

LinkedList **create_overflow_buckets(HashTable *table)
{
    /* Create the overflow buckets; an array of LinkedLists. */
    LinkedList **buckets = (LinkedList **)calloc(table->size, sizeof(LinkedList *));
    int i;

    for (i = 0; i < table->size; i++)
        buckets[i] = NULL;

    return buckets;
}

void free_overflow_buckets(HashTable *table)
{
    /* Free all the overflow bucket lists. */
    LinkedList **buckets = table->overflow_buckets;
    int i;

    for (i = 0; i < table->size; i++)
        free_linkedlist(buckets[i]);

    free(buckets);
}

Ht_item *create_item(char *key, char *value, char *type, char *address, char *memorySize, char *order)
{
    /* Creates a pointer to a new HashTable item. */
    Ht_item *item = (Ht_item *)malloc(sizeof(Ht_item));
    item->key = (char *)malloc(strlen(key) + 1);
    item->value = (char *)malloc(strlen(value) + 1);
    item->type = (char *)malloc(strlen(type) + 1);
    item->address = (char *)malloc(strlen(address) + 1);
    item->memorySize = (char *)malloc(strlen(memorySize) + 1);
    item->order = (char *)malloc(strlen(order) + 1);
    strcpy(item->key, key);
    strcpy(item->value, value);
    strcpy(item->type, type);
    strcpy(item->address, address);
    strcpy(item->memorySize, memorySize);
    strcpy(item->order, order);
    return item;
}

HashTable *create_table(int size)
{
    int i;

    /* Creates a new HashTable. */
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item **)calloc(table->size, sizeof(Ht_item *));

    for (i = 0; i < table->size; i++)
        table->items[i] = NULL;

    table->overflow_buckets = create_overflow_buckets(table);

    return table;
}

void free_item(Ht_item *item)
{
    /* Frees an item. */
    free(item->key);
    free(item->value);
    free(item->type);
    free(item->address);
    free(item->memorySize);
    free(item->order);
    free(item);
}

void free_table(HashTable *table)
{
    int i;
    /* Frees the table. */
    for (i = 0; i < table->size; i++)
    {
        Ht_item *item = table->items[i];

        if (item != NULL)
            free_item(item);
    }

    /* Free the overflow bucket lists and its items. */
    free_overflow_buckets(table);
    free(table->items);
    free(table);
}

void handle_collision(HashTable *table, unsigned long index, Ht_item *item)
{
    LinkedList *head = table->overflow_buckets[index];

    if (head == NULL)
    {
        /* Creates the list. */
        head = allocate_list();
        head->item = item;
        table->overflow_buckets[index] = head;
        return;
    }
    else
    {
        /* Insert to the list. */
        table->overflow_buckets[index] = linkedlist_insert(head, item);
        return;
    }
}

void ht_insert(HashTable *table, char *key, char *value, char *type, char *address, char *memorySize, char *order)
{
    /* Creates the item. */
    Ht_item *item = create_item(key, value, type, address, memorySize, order);

    /* Computes the index. */
    int index = hash_function(key);

    Ht_item *current_item = table->items[index];

    if (current_item == NULL)
    {
        /* Key does not exist. */
        if (table->count == table->size)
        {
            /* HashTable is full. */
            printf("Insert Error: Hash Table is full\n");
            free_item(item);
            return;
        }

        /* Insert directly. */
        table->items[index] = item;
        table->count++;
    }
    else
    {
        /* Scenario 1: Update the value. */
        if (strcmp(current_item->key, key) == 0)
        {
            strcpy(table->items[index]->value, value);
            strcpy(table->items[index]->type, type);
            strcpy(table->items[index]->address, address);
            strcpy(table->items[index]->memorySize, memorySize);
            strcpy(table->items[index]->order, order);
            return;
        }
        else
        {
            /* Scenario 2: Handle the collision. */
            handle_collision(table, index, item);
            return;
        }
    }
}

char *ht_search(HashTable *table, char *key)
{
    /* Searches for the key in the HashTable.
       Returns NULL if it doesn't exist. */
    int index = hash_function(key);
    Ht_item *item = table->items[index];
    LinkedList *head = table->overflow_buckets[index];

    /* Provide only non-NULL values. */
    if (item != NULL)
    {
        if (strcmp(item->key, key) == 0)
            return item->value;

        if (head == NULL)
            return NULL;

        item = head->item;
        head = head->next;
    }

    return NULL;
}

char *ht_get_type(HashTable *table, char *key)
{
    /* Searches for the key in the HashTable.
       Returns NULL if it doesn't exist. */
    int index = hash_function(key);
    Ht_item *item = table->items[index];
    LinkedList *head = table->overflow_buckets[index];

    /* Provide only non-NULL types. */
    if (item != NULL)
    {
        if (strcmp(item->key, key) == 0)
            return item->type;

        if (head == NULL)
            return NULL;

        item = head->item;
        head = head->next;
    }

    return NULL;
}

char *ht_get_memory_address(HashTable *table, char *key)
{
    /* Searches for the key in the HashTable.
       Returns NULL if it doesn't exist. */
    int index = hash_function(key);
    Ht_item *item = table->items[index];
    LinkedList *head = table->overflow_buckets[index];

    /* Provide only non-NULL types. */
    if (item != NULL)
    {
        if (strcmp(item->key, key) == 0)
            return item->address;

        if (head == NULL)
            return NULL;

        item = head->item;
        head = head->next;
    }

    return NULL;
}

char *ht_get_memory_size(HashTable *table, char *key)
{
    /* Searches for the key in the HashTable.
       Returns NULL if it doesn't exist. */
    int index = hash_function(key);
    Ht_item *item = table->items[index];
    LinkedList *head = table->overflow_buckets[index];

    /* Provide only non-NULL types. */
    if (item != NULL)
    {
        if (strcmp(item->key, key) == 0)
            return item->memorySize;

        if (head == NULL)
            return NULL;

        item = head->item;
        head = head->next;
    }

    return NULL;
}


void ht_delete(HashTable *table, char *key)
{
    /* Deletes an item from the table. */
    int index = hash_function(key);
    Ht_item *item = table->items[index];
    LinkedList *head = table->overflow_buckets[index];

    LinkedList *node;
    LinkedList *curr;
    LinkedList *prev;

    if (item == NULL)
    {
        /* Does not exist. */
        return;
    }
    else
    {
        if (head == NULL && strcmp(item->key, key) == 0)
        {
            /* Collision chain does not exist.
               Remove the item.
               Set table index to NULL. */
            table->items[index] = NULL;
            free_item(item);
            table->count--;
            return;
        }
        else if (head != NULL)
        {
            /* Collision chain exists. */
            if (strcmp(item->key, key) == 0)
            {
                /* Remove this item.
                   Set the head of the list as the new item. */
                free_item(item);
                node = head;
                head = head->next;
                node->next = NULL;
                table->items[index] = create_item(node->item->key, node->item->value, node->item->type, node->item->address, node->item->memorySize, node->item->order);
                free_linkedlist(node);
                table->overflow_buckets[index] = head;
                return;
            }

            curr = head;
            prev = NULL;

            while (curr)
            {
                if (strcmp(curr->item->key, key) == 0)
                {
                    if (prev == NULL)
                    {
                        /* First element of the chain.
                           Remove the chain. */
                        free_linkedlist(head);
                        table->overflow_buckets[index] = NULL;
                        return;
                    }
                    else
                    {
                        /* This is somewhere in the chain. */
                        prev->next = curr->next;
                        curr->next = NULL;
                        free_linkedlist(curr);
                        table->overflow_buckets[index] = head;
                        return;
                    }
                }

                curr = curr->next;
                prev = curr;
            }
        }
    }
}

void print_search(HashTable *table, char *key)
{
    char *val;

    if ((val = ht_search(table, key)) == NULL)
    {
        printf("Key:%s does not exist\n", key);
        return;
    }
    else
    {
        printf("Key:%s, Value:%s\n", key, val);
    }
}

void print_type(HashTable *table, char *key)
{
    char *type;

    if ((type = ht_get_type(table, key)) == NULL)
    {
        printf("Key:%s does not exist\n", key);
        return;
    }
    else
    {
        printf("Key:%s, Type:%s\n", key, type);
    }
}

void print_memory_address(HashTable *table, char *key)
{
    char *address;

    if ((address = ht_get_memory_address(table, key)) == NULL)
    {
        printf("Key:%s does not exist\n", key);
        return;
    }
    else
    {
        printf("Key:%s, Memory Address:%s\n", key, address);
    }
}

void print_memory_size(HashTable *table, char *key)
{
    char *memorySize;

    if ((memorySize = ht_get_memory_size(table, key)) == NULL)
    {
        printf("Key:%s does not exist\n", key);
        return;
    }
    else
    {
        printf("Key:%s, Memory Size:%s\n", key, memorySize);
    }
}

char * return_search(HashTable *table, char *key)
{
    char *val;

    if ((val = ht_search(table, key)) == NULL)
    {
        return "Empty";
    }
    else
    {
        return "Value";
    }
}

bool existsInHash(HashTable *table, char *key)
{
    if (ht_search(table, key) == NULL)
    {
        return False;
    }
    else
    {
        return True;
    }
}

void print_table(HashTable *table)
{
    int i;
    printf("\nHash Table\n-------------------\n");

    for (i = 0; i < table -> size; i++)
    {
        if (table -> items[i])
        {
            printf("Index:%d, Key:%s, Value:%s, Type:%s, Address:%s, Memory Size:%s, Order:%s\n", i, table -> items[i] -> key, table -> items[i] -> value, table -> items[i] -> type, table -> items[i] -> address, table -> items[i] -> memorySize, table -> items[i] -> order);
        }
    }

    printf("-------------------\n\n");
}






