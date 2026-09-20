/*
//small file to pre-compute a table of Sines, to be exported to a file
//first argument being total number of angles to be computed (Always ranges from 0 to 180, however the increment on each step is what varies)
//second argument is the number of itens for each line (automatic linebreaks)
//third argument, if passed, sets an integer number to multiply by the table, such that index 0 is 0, and the last index is equal to the number here passed. the entire table will be changed from decimals to integers when this is passed, thus creating a new file
//fourth argument, if passed, sets a specific integer value to be treated as "PI/2" (this value is equal to the last index of the table). If omitted, but 3rd argument is passed, this value is equal to (long)(the 3rd argument. * (PI / 2) )
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.141592653589793
#define DEFAULT_LINEBREAK 15

/*this value can be either of the following types:  | double | float | long double | */
const char tableType[] = "double";

long double degToRad(long double deg){
    return deg * PI / 180;
}

long double* initSinTable(long sinSampleSize){
    long double granularity = 90.0 / sinSampleSize;
    long double* SinTable = malloc(sizeof(long double)*sinSampleSize );
    for (int idx = 0; idx < sinSampleSize; idx++){
        long double idx_sin = sinl( degToRad(granularity * idx) );
        SinTable[idx] = idx_sin;
    }
    return SinTable;
}
/*
int printSin(unsigned long sinSampleSize, unsigned long maxNumPerLine){
    long double* SinTable = initSinTable(sinSampleSize);
    printf("long double SinTable[%u] = {\n", sinSampleSize);
    for (int idx = 0; idx < sinSampleSize; idx++ )
    {
        printf("\t"); if (SinTable[idx] >= 0.0) printf(" ");
        printf("%.16Lf", SinTable[idx]);
        if (idx < sinSampleSize-1)printf(","); else break;
        printf("%c", ( ((idx+1)%maxNumPerLine) == 0) ? "\n"[0] : " "[0]);
    }
    printf("\n};\n");
    free(SinTable);
    return 0;
}
*/


int printSin_toFile(unsigned long sinSampleSize, unsigned long maxNumPerLine){
    long double* SinTable = initSinTable(sinSampleSize);
    if (SinTable == NULL){
        printf("Could not allocate memory for the table; try passing a lower sample count!\n");
        return -1;
    }
    FILE* outp = fopen("sin_table.h", "w");
    fprintf(outp, "#ifndef SIN_TABLE\n#define SIN_TABLE\n#define SIN_TABLE_SIZE %u\n#define PI %.20lf\n#define LOWEST_ANGLE_DELTA %.32Lf\n\nstatic %s SinTable_static[%u] = {\n", sinSampleSize, PI, (long double)90.0 / sinSampleSize, tableType, sinSampleSize+1);
    for (unsigned long idx = 0; idx < sinSampleSize; idx++ )
    {
        fprintf(outp, "\t"); if (SinTable[idx] >= 0.0) fprintf(outp, " ");
        fprintf(outp, "%.64Lf,", SinTable[idx]);
        fprintf(outp, "%c", ( ((idx+1)%maxNumPerLine) == 0) ? "\n"[0] : " "[0]);
    }
    fprintf(outp, "1.0");
    fprintf(outp, "\n};\n%s* SinTable = &(SinTable_static[0]);\n\nstatic %s negInf = -1.0/0.0;\nstatic %s posInf = 1.0/0.0;\n\n", tableType, tableType, tableType);
    fprintf(outp, "%s sin_table(const %s angle){\n\tunsigned long index, div, trueIndex;\n\tif(angle != angle || angle == posInf || angle == negInf) return 0.0;\n\tindex =  (unsigned long)(SIN_TABLE_SIZE * angle  * 2.0 / (PI));\n\tdiv = index / SIN_TABLE_SIZE;\n\ttrueIndex = index - div * SIN_TABLE_SIZE;\n\tswitch ( div & 0b11 ){\n\t\tcase 0: return  SinTable[trueIndex];\n\t\tcase 1: return  SinTable[SIN_TABLE_SIZE - trueIndex];\n\t\tcase 2: return -SinTable[trueIndex];\n\t\tcase 3: return -SinTable[SIN_TABLE_SIZE - trueIndex];\n\t\tdefault: return 0.0;\n\t}\n}\n", tableType, tableType);
    fprintf(outp, "\n#endif");
    free(SinTable);
    fclose(outp);
    return 0;
}
/*
int printSinInt(unsigned int sinSampleSize, unsigned int maxNumPerLine, unsigned int maximumVal){
    long double* SinTable = initSinTable(sinSampleSize);
    printf("int SinTableInt[%u] = {\n", sinSampleSize);
    for (int idx = 0; idx < sinSampleSize; idx++ )
    {
        printf("\t"); if (SinTable[idx] >= 0.0) printf(" ");
        printf("%u", (unsigned int)(SinTable[idx] * maximumVal) );
        if (idx < sinSampleSize-1)printf(","); else break;
        printf("%c", ( ((idx+1)%maxNumPerLine) == 0) ? "\n"[0] : " "[0]);
    }
    printf("\n};\n//MAX = %u\n", maximumVal);
    free(SinTable);
    return 0;
}
*/
int printSinInt_toFile(unsigned long sinSampleSize, unsigned long maxNumPerLine, unsigned long maximumVal, unsigned long piOver2){
    long double* SinTable = initSinTable(sinSampleSize);
    if (SinTable == NULL){
        printf("Could not allocate memory for the table; try passing a lower sample count!\n");
        return -1;
    }
    FILE* outp = fopen("sin_table_int.h", "w");
    fprintf(outp, "#ifndef SIN_TABLE_INT\n#define SIN_TABLE_INT\n#define SIN_TABLE_INT_SIZE %u\n#define PI_DIV2_INT %u\n\nstatic long SinTable_static[%u] = {\n", sinSampleSize, piOver2, sinSampleSize+1);
    for (unsigned long idx = 0; idx < sinSampleSize; idx++ )
    {
        fprintf(outp, "\t"); if (SinTable[idx] >= 0.0) fprintf(outp, " ");
        fprintf(outp, "%u,", (long)(SinTable[idx] * maximumVal) );
        fprintf(outp, "%c", ( ((idx+1)%maxNumPerLine) == 0) ? "\n"[0] : " "[0]);
    }
    fprintf(outp, "%u", maximumVal);
    fprintf(outp, "\n};\nlong* SinTable = &(SinTable_static[0]);\n\n" );
    fprintf(outp, "long sin_table_int(const long angle){\n\tunsigned long trueIndex, div;\n\ttrueIndex = (angle * SIN_TABLE_INT_SIZE);\n\tdiv = angle / (PI_DIV2_INT); \n\ttrueIndex %= PI_DIV2_INT;\n\tswitch ( div & 0b11 ){\n\t\tcase 0: return  SinTable[trueIndex];\n\t\tcase 1: return  SinTable[SIN_TABLE_INT_SIZE - trueIndex];\n\t\tcase 2: return -SinTable[trueIndex];\n\t\tcase 3: return -SinTable[SIN_TABLE_INT_SIZE - trueIndex];\n\t\tdefault: return 0;\n\t}\n}\n");
    fprintf(outp, "\n#endif");
    free(SinTable);
    fclose(outp);
    return 0;
}


int main(int argc, char* argv[]){
    if (argc > 5) {return 1;}

    unsigned long sinSampleSize = strtol(argv[1], NULL, 10);
    unsigned long maxNumPerLine = (argc > 2) ? strtol(argv[2], NULL, 10) : DEFAULT_LINEBREAK;
    if (argc > 3){
        unsigned long maximumVal = strtol(argv[3], NULL, 10);
        unsigned long piOver2 = argc == 5 ? strtol(argv[4], NULL, 10) : (long)( (double)maximumVal * (PI / 2));
        return printSinInt_toFile(sinSampleSize, maxNumPerLine, maximumVal, piOver2);
    }
    else if (argc == 3) return printSin_toFile(sinSampleSize, maxNumPerLine);
    return 2;
}

