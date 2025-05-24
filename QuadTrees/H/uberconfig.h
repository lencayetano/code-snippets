#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <mysql.h>


#define MAX_CONF_LEN     100
#define QUERYLENGTH      2048  /* Note: Consider adjusting length */
#define MAX_ID_LENGTH    256   /* Define a maximum length */
#define MAX_ADDR_LEN     1024  /* Define a maximum length */
#define MAX_DRIVERS      4     /* Maximum number of drivers to return to rider client */

typedef struct Location {
    double latitude;
    double longitude;
    double accuracy;
    char driverID[MAX_ID_LENGTH];
    char address[MAX_ADDR_LEN];
} Location;

typedef struct ListOfDrivers {
    int size;
    struct Location *arrayptr[4];
} ListOfDrivers;

bool start_driverclients(MYSQL *mysql_conn, FILE *logFile);
bool driver_client(MYSQL *mysql_conn, FILE *logFile, char *driverid);
bool start_riderclients(MYSQL *mysql_conn, FILE *logFile);
bool start_riderclients_rest_CSharp(MYSQL* mysql_conn, FILE* logFile);
bool start_riderclients_rest_py(MYSQL* mysql_conn, FILE* logFile);
bool rider_client(MYSQL *mysql_conn, FILE *logFile, char *riderid);
bool rider_client_rest_CSharp(MYSQL* mysql_conn, FILE* logFile, char* riderid);
bool rider_client_rest_py(MYSQL* mysql_conn, FILE* logFile, char* riderid);
ListOfDrivers *GetListOfDrivers(const char *url);
ListOfDrivers* GetDriver(const char *url);
bool InitQuad(FILE *logFile);
bool ReleaseQuad(FILE *logFile);
bool OptimizeQuad(FILE *logFile);
void CallRestfulService(void);
bool PrintQuad(FILE * logFile);
double round_to_decimal(double value, int decimals);
void TestPolyPersistence(FILE* logFile);