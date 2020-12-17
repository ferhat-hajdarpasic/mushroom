#include <stdio.h>
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */
#include "cJSON.h"

const int ROWS=28;
const int COLS=48;


void parse(char *json_string) {
    char PANEL[ROWS][COLS];
    int rowIndex, colIndex;
    cJSON *root = cJSON_Parse(json_string);

    // cJSON_bool b = cJSON_HasObjectItem(root, "grid");
    cJSON *grid = cJSON_GetObjectItem(root, "grid");

    // int actualRows = cJSON_GetArraySize(grid);
    for (rowIndex = 0; rowIndex < ROWS; rowIndex++) {
        cJSON *row = cJSON_GetArrayItem(grid, rowIndex);
        // int actualColumns = cJSON_GetArraySize(row);
        for (colIndex = 0; colIndex < COLS; colIndex++) {
            cJSON *cell = cJSON_GetArrayItem(row, colIndex);
            char value = cell->valueint;
            PANEL[rowIndex][colIndex] = (char)value;
            putchar('0' + PANEL[rowIndex][colIndex]);
        }
        putchar('\n' + PANEL[rowIndex][colIndex]);
    }
}

extern int http_get();

int main(int argc, char **argv) {
    // char *HTTPS_GET_METHOD_BODY = "{\"_id\":\"5fc87af004e4d30008c2d796\",\"name\":\"message1\",\"line1\":\"Fred\",\"line2\":\"Here \",\"line3\":\"Comes\",\"line4\":\"Again\",\"grid\":[[0,23,34,0,3],[56,0,78,0,3],[77,0,66,1,4],[1,0,9,7,5]]}";

    char HTTPS_GET_METHOD_BODY[BUFSIZ];
 
    ssize_t mess_index = http_get(HTTPS_GET_METHOD_BODY);
    write(STDOUT_FILENO, HTTPS_GET_METHOD_BODY, mess_index);
    // parse(HTTPS_GET_METHOD_BODY);


    mess_index++;
    int array2= sizeof( HTTPS_GET_METHOD_BODY);
  char* test_string = (char*)malloc(sizeof(char) * (mess_index+1));
  memset(test_string, 0x00, (mess_index+1));
  memcpy(test_string, &HTTPS_GET_METHOD_BODY, mess_index);
  parse(test_string);


    return 0;
}

