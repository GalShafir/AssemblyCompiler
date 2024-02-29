/* ------------------------------------ Structures ------------------------------------ */

/* Defines the HashTable item. */
typedef struct Ht_item
{
    char *key;
    char *value;
    char *type;
    char *address;
    char *memorySize;
    char *order;

} Ht_item;

/* Defines the LinkedList. */
typedef struct LinkedList
{
    Ht_item *item;
    struct LinkedList *next;
} LinkedList;

/* Defines the HashTable. */
typedef struct HashTable
{
    /* Contains an array of pointers to items. */
    Ht_item **items;
    LinkedList **overflow_buckets;
    int size;
    int count;
} HashTable;


/* ------------------------------------ Functions ------------------------------------ */

FILE* openFile(const char * fileName, const char * accessMode);
char *removeFileExtension(const char *inputFileName);
void removeWhiteSpaces(char *inputString);
void removeLeadingSpaces(char *str);
char **splitString(const char *inputString, const char *separators, int *count);
void freeStringArray(char **splittedStrings, int count);
void printStringArray(char **strings, int count);
void removeSubstring(char *str, const char *subStr);
void removeLastCharacter(char *str);
char* intToString(int num);
int stringToInt(const char* str);
LinkedList *allocate_list();
LinkedList *linkedlist_insert(LinkedList *list, Ht_item *item);
Ht_item *linkedlist_remove(LinkedList *list);
void free_linkedlist(LinkedList *list);
LinkedList **create_overflow_buckets(HashTable *table);
void free_overflow_buckets(HashTable *table);
Ht_item *create_item(char *key, char *value, char *type, char *address, char *memorySize, char *order);
HashTable *create_table(int size);
void free_item(Ht_item *item);
void free_table(HashTable *table);
void handle_collision(HashTable *table, unsigned long index, Ht_item *item);
void ht_insert(HashTable *table, char *key, char *value, char *type, char *address, char *memorySize, char *order);
char *ht_search(HashTable *table, char *key);
char *ht_get_type(HashTable *table, char *key);
char *ht_get_memory_address(HashTable *table, char *key);
char *ht_get_memory_size(HashTable *table, char *key);
void ht_delete(HashTable *table, char *key);
void print_search(HashTable *table, char *key);
void print_type(HashTable *table, char *key);
void print_memory_address(HashTable *table, char *key);
void print_memory_size(HashTable *table, char *key);
char * return_search(HashTable *table, char *key);
void print_table(HashTable *table);
bool existsInHash(HashTable *table, char *key);

