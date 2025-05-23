#pragma once

#define QUADTREE_ID "99999"
#define MAX_CHILD_NODES 4             /* Should not change, hence the term quad */
#define MAX_LOCATIONS_PER_NODE 4 /*20*/      /* Max number of nodes before subdivision */
#define DRIVER_RADIUS 5	              /* Configured radius in miles */

typedef struct GeoQuadTreeNode {

    double minLat;     /* Minimum latitude */
    double maxLat;     /* Maximum latitude */
    double minLon;     /* Minimum latitude */
    double maxLon;     /* Maximum latitude */
    /*----------------------------------------------------------------------
     * Here we define a dynamic array or linked list of locations
     * This could be pointers to Location structs
     *--------------------------------------------------------------------*/
    int locationCount;     /* Number of locations stored in this node */
    Location **locations;  /* We house the locations of drivers in this array */
                           /* Pointers to the 4 children */
    int numChildren;       /* Number of children */
    struct GeoQuadTreeNode *children[MAX_CHILD_NODES];  
    /*----------------------------------------------------------------------
     * Here we can specify additional data specific to mapping, e.g.,
     * road information, area descriptions. This can be pointers to other
     * structs representing these complex data types
     *---------------------------------------------------------------------*/
} GeoQuadTreeNode;


typedef struct DriverLocation {
    int locindex;
    GeoQuadTreeNode *drivernode;
} DriverLocation;

/* MemoryBufferReader structure for managing buffer read operations */
typedef struct {
    char *data;         /* Pointer to the buffer's data */
    size_t size;        /* Size of the buffer */
    size_t position;    /* Current read position in the buffer */
} MemoryBufferReader;

typedef struct {
    char *data;        /* Pointer to the buffer's data */
    size_t size;       /* Current size of the buffer */
    size_t capacity;   /* Current capacity of the buffer */
} MemoryBuffer;

GeoQuadTreeNode *CreateQuadNode(FILE *logFile, double minLat, double minLon,
                                double maxLat, double maxLon);
GeoQuadTreeNode *InitQuadTree(FILE *logFile);
GeoQuadTreeNode *DeserializeNode(FILE *logFile, MemoryBufferReader *reader);

bool SerializeNode(FILE *logFile, MemoryBuffer *buffer, GeoQuadTreeNode *node);
void SubdivideNode(FILE *logFile, GeoQuadTreeNode* node);
void FreeQuadTree(FILE* logFile, GeoQuadTreeNode* node);
void PrintQuadTree(FILE *logFIle, GeoQuadTreeNode *node);
void InsertLocationIntoChildren(FILE *logFile, GeoQuadTreeNode *node, Location *location);
void InsertQuadLocation(FILE *logfile, GeoQuadTreeNode *node, Location *location);
bool IsLocationInNode(GeoQuadTreeNode *node, Location *location);
void InitMemoryBufferReader(MemoryBufferReader *reader, char *data, size_t size);
bool ReadFromBuffer(MemoryBufferReader *reader, void *destination, size_t dataSize);
void InitMemoryBuffer(MemoryBuffer *buffer);
bool AppendToBuffer(FILE *logFile, MemoryBuffer *buffer, const void *data, size_t dataSize);
char *ReadFromMemcached(FILE *logFIle, const char* key, size_t * value_length);
void WriteToMemcached(FILE *logFile, const char *key, char *serializedqtree, size_t qtree_size);
bool write_quadtree_to_mysql(MYSQL* conn, FILE* logFile, const char* serialized_tree,
    size_t size, const char* tree_id);
char *read_quadtree_from_mysql(MYSQL *conn, FILE *logFile, const char *tree_id, size_t *sertree_size);
int UpdateDriverLocations(FILE* logFile);
void TestQuadTree(FILE* logFile, GeoQuadTreeNode* rootQuad);
void UpdateDriverLocation(FILE* logFile,Location* newLocation,GeoQuadTreeNode* root,int* err);

int FindDriversWithinRadius(struct soap* soap, GeoQuadTreeNode* node, Location* target,
    double radius, FILE* logFile, struct ListOfDrivers **results, int* count);
bool OptimizeQuadTree(GeoQuadTreeNode* node);
void SerializeWriteQuadDB(FILE* logFile, GeoQuadTreeNode* rootQuad);
GeoQuadTreeNode* ReadDeserializeQuadDB(FILE* logFile);
bool DeleteLocationByDriverID(FILE* logFile, GeoQuadTreeNode* node, const char* driverID);

bool FindDriverID(FILE * logFile,GeoQuadTreeNode * node,const char* driverID, DriverLocation * driverloc);

void RemoveLocationByIndex(GeoQuadTreeNode* node, int index);