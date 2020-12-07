#include <stdio.h>
#include "cJSON.h"

const int ROWS=4;
const int COLS=5;

char GRID[ROWS][COLS];

void parse(char *json_string) {
    int rowIndex, colIndex;
    cJSON *root = cJSON_Parse(json_string);

    cJSON_bool b = cJSON_HasObjectItem(root, "grid");
    cJSON *grid = cJSON_GetObjectItem(root, "grid");

    int actualRows = cJSON_GetArraySize(grid);
    for (rowIndex = 0; rowIndex < 4; rowIndex++) {
        cJSON *row = cJSON_GetArrayItem(grid, rowIndex);
        int actualColumns = cJSON_GetArraySize(row);
        for (colIndex = 0; colIndex < 5; colIndex++) {
            cJSON *cell = cJSON_GetArrayItem(row, colIndex);
            char value = cell->valueint;
            GRID[rowIndex][colIndex] = (char)value;
        }
    }
}

int main(int argc, char **argv) {
    char *json_string = "{\"_id\":\"5fc87af004e4d30008c2d796\",\"name\":\"message1\",\"line1\":\"Fred\",\"line2\":\"Here \",\"line3\":\"Comes\",\"line4\":\"Again\",\"grid\":[[0,23,34,0,3],[56,0,78,0,3],[77,0,66,1,4],[1,0,9,7,5]]}";
    parse(json_string);
    return 0;
}

