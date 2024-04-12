#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 10

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// Node for chaining
struct Node
{
    struct RecordType data;
    struct Node *next;
};

// HashType
struct HashType
{
    struct Node *head;
};

// Compute the hash function
int hash(int x)
{
    return x % HASH_SIZE; // Simple modulo hashing for demonstration
}

// Parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);

        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }

        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// Display records in the hash structure
// Skip the indices which are free
// The output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;
    struct Node *temp;

    for (i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        temp = pHashArray[i].head;
        while (temp != NULL)
        {
            printf("%d %c %d -> ", temp->data.id, temp->data.name, temp->data.order);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

// Insert a record into the hash table
void insertRecord(struct HashType *pHashArray, struct RecordType record)
{
    int index = hash(record.id);
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed for new record\n");
        exit(-1);
    }
    newNode->data = record;
    newNode->next = pHashArray[index].head;
    pHashArray[index].head = newNode;
}

int main(void)
{
    struct RecordType *pRecords;
    struct HashType hashArray[HASH_SIZE] = {NULL}; // Initialize all heads to NULL
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Insert records into hash table
    for (int i = 0; i < recordSz; i++)
    {
        insertRecord(hashArray, pRecords[i]);
    }

    // Display records in the hash structure
    displayRecordsInHash(hashArray, HASH_SIZE);

    // Free dynamically allocated memory
    free(pRecords);

    return 0;
}
