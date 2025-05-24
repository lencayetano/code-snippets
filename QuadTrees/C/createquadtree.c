#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <uberconfig.h>
#include <quadtree.h>
#include <logging.h>
#include <mysqlint.h>

/* #define DEBUG */ 
/*------------------------------------------------------------------------------
 * Function: CreateQuadNode()
 * Purpose:  Create a quadtree node
 * Inputs:   1) Pointer to log file
 *           2) Minimum latitude
 *           3) Maximum Latitude
 *           4) Minimum Longitude
 *           5) Maximum Longitude
 * Output:   1) Pointer to QuadTree Node
 * Called from: InitQuadTree(), SubdivideNode()
 * Notes:
 * --------------------------------------------------------------------------*/
GeoQuadTreeNode *CreateQuadNode(FILE *logFile, double minLat, double maxLat,
    double minLon, double maxLon)
{
    GeoQuadTreeNode* node = (GeoQuadTreeNode*)malloc(sizeof(GeoQuadTreeNode));
    if (node == NULL) {
        fprintf(logFile, "CreateQuadNode() : Cannot allocate node!\n");
        return NULL;
    }
    node->minLat = minLat;
    node->maxLat = maxLat;
    node->minLon = minLon;
    node->maxLon = maxLon;

    /* Pre-allocate the locations array */
    node->locationCount = 0;
    node->locations = (Location **)malloc(MAX_LOCATIONS_PER_NODE * sizeof(Location*));
    if (node->locations == NULL) {
        fprintf(logFile, "InsertLocation(): Memory allocation failed for locations!\n");
        free(node);
        return NULL;
    }

    for (int i = 0; i < MAX_LOCATIONS_PER_NODE; ++i) {
        node->locations[i] = NULL; /* Initialize all to NULL */
    }

    /* We initialize the child nodes to NULL */
    node->numChildren = 0;
    for (int i = 0; i < MAX_CHILD_NODES; i++) {
        node->children[i] = NULL;
    }
    return node;
}

/*------------------------------------------------------------------------------
 * Function: InitQuadTree()
 * Purpose:  Initialize quadtree
 * Input(s): 1) Pointer to log file
 * Output:   1) Root node of Quad Tree is created
 * Called from: This is called during initialization to create the initial
 *           Quad Tree.
 * Notes:
 *----------------------------------------------------------------------------*/
GeoQuadTreeNode *InitQuadTree(FILE* logFile)
{
    GeoQuadTreeNode *node = CreateQuadNode(logFile, -90, 90, -180, 180);
    node->locationCount = 0;
    node->locations = (Location**)malloc(MAX_LOCATIONS_PER_NODE * sizeof(Location*));
    if (node->locations == NULL) {
        fprintf(logFile, "InitQuadTree(): Memory allocation failed for locations!\n");
        return NULL;
    }
    for (int i = 0; i < MAX_LOCATIONS_PER_NODE; ++i) {
        node->locations[i] = NULL; /* Initialize all to NULL */
    }
    return node;
}
