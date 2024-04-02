#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct numPair {
    int first;
    int second;
};  // To store i (first), j (second)

int main ()
{
    double time_spent = 0.0; // program time check

	FILE * fp = fopen("formula", "w") ;
	
    int i, j, b;
    int region[10][10];
    int max_region = 0;

    clock_t begin = clock();
    
    // i: 0~9   j: 0~9    b: 0,1 (white, black) -> both are false, then empty cell
	for (i = 0 ; i <= 9 ; i++)
		for (j = 0 ; j <= 9 ; j++)
            for(b = 0 ; b <= 1 ; b++)
    			fprintf(fp,"(declare-const p%d%d%d Bool)\n", i, j, b) ;
    // example p000 p001
    //           T    T  = contradiction
    //           T    F  = White
    //           F    T  = Black
    //           F    F  = empty cell

    // Each cell must be empty, or contain either a white or black circle. 
    // => white, black both must not be true at the same time.
    fprintf(fp,"; Q0\n") ;
    fprintf(fp,"(assert (and ") ;
    for (i = 0 ; i <= 9 ; i++) {
		fprintf(fp,"(and ") ;
		for (j = 0 ; j <= 9 ; j++){
			fprintf(fp,"(not (and p%d%d%d p%d%d%d))", i, j, 0, i, j, 1) ;
        }
    	fprintf(fp,")") ;
	}
	fprintf(fp,"))\n") ;
    
    char command;
    char fileName[128];

    printf("Do you want to type the puzzle? (y/n) > ");
    scanf("%c", &command);

    // puzzle input , pre-assigned
    if(command == 'y') {
        char buffer[5];
        printf("Type the nondango puzzle!\n");

        fprintf(fp,"; pre-assigned\n") ;
        fprintf(fp,"(assert (and ") ;
        for(int i = 0; i < 10; i++) {
            for(int j = 0; j < 10; j++) {
                scanf("%s", buffer);
                char* ptr = strchr(buffer, 'W');
                if (ptr){
                    *ptr = 0;
                    region[i][j] = atoi(buffer);
                    fprintf(fp,"(or p%d%d%d p%d%d%d)", i, j, 0, i, j, 1) ;
                }
                else{
                    region[i][j] = atoi(buffer);
                    fprintf(fp,"(not (or p%d%d%d p%d%d%d))", i, j, 0, i, j, 1) ;
                }
                if(region[i][j] > max_region) max_region = region[i][j];                    
            }
        }
        fprintf(fp,"))\n") ;

    } else if(command == 'n') {
        printf("Write down the file name of Nondango puzzle. > ");
        scanf("%s", fileName);

        FILE * nondango = fopen(fileName, "r");
        char num[5];
        fprintf(fp,"; pre-assigned\n") ;
        fprintf(fp,"(assert (and ") ;
        for(int i = 0; i < 10; i++) {
            for(int j = 0; j < 10; j++) {
                fscanf(nondango, "%s", num);
                // printf("%s\n", num);
                char* ptr = strchr(num, 'W');
                if (ptr){
                    *ptr = 0;
                    region[i][j] = atoi(num);
                }
                else{
                    region[i][j] = atoi(num);
                    fprintf(fp,"(not (or p%d%d%d p%d%d%d))", i, j, 0, i, j, 1) ;
                }
                if(region[i][j] > max_region) max_region = region[i][j];                    
            }
        }
        fprintf(fp,"))\n") ;
    } else {
        return 0;
    }
    
    // There must be no three circles of the same color placing on three consecutive cells 
    // horizontally (regardless of region)
    fprintf(fp,"; Q1\n") ;
    fprintf(fp,"(assert (and ") ;
    for (i = 0 ; i <= 9 ; i++) {
		fprintf(fp,"(and ") ;
		for (j = 0 ; j <= 7 ; j++){
            fprintf(fp,"(and ") ;
            for (b = 0 ; b <= 1 ; b++){
			    fprintf(fp,"(not (and p%d%d%d p%d%d%d p%d%d%d))", i, j, b, i, j+1, b, i, j+2, b) ;
            }
            fprintf(fp,")") ;
        }
    	fprintf(fp,")") ;
	}
	fprintf(fp,"))\n") ;

    // There must be no three circles of the same color placing on three consecutive cells 
    // vertically (regardless of region)
    fprintf(fp,"; Q2\n") ;
    fprintf(fp,"(assert (and ") ;
    for (i = 0 ; i <= 7 ; i++) {
		fprintf(fp,"(and ") ;
		for (j = 0 ; j <= 9 ; j++){
            fprintf(fp,"(and ") ;
            for (b = 0 ; b <= 1 ; b++){
			    fprintf(fp,"(not (and p%d%d%d p%d%d%d p%d%d%d))", i, j, b, i+1, j, b, i+2, j, b) ;
            }
            fprintf(fp,")") ;
        }
    	fprintf(fp,")") ;
	}
	fprintf(fp,"))\n") ;
    
    // There must be no three circles of the same color placing on three consecutive cells 
    // diagonally to the right (regardless of region)
    fprintf(fp,"; Q3\n") ;
    fprintf(fp,"(assert (and ") ;
    for (i = 0 ; i <= 7 ; i++) {
		fprintf(fp,"(and ") ;
		for (j = 0 ; j <= 7 ; j++){
            fprintf(fp,"(and ") ;
            for (b = 0 ; b <= 1 ; b++){
			    fprintf(fp,"(not (and p%d%d%d p%d%d%d p%d%d%d))", i, j, b, i+1, j+1, b, i+2, j+2, b) ;
            }
            fprintf(fp,")") ;
        }
    	fprintf(fp,")") ;
	}
	fprintf(fp,"))\n") ;

    // There must be no three circles of the same color placing on three consecutive cells 
    // diagonally to the left (regardless of region)
    fprintf(fp,"; Q4\n") ;
    fprintf(fp,"(assert (and ") ;
    for (i = 0 ; i <= 7 ; i++) {
		fprintf(fp,"(and ") ;
		for (j = 2 ; j <= 9 ; j++){
            fprintf(fp,"(and ") ;
            for (b = 0 ; b <= 1 ; b++){
			    fprintf(fp,"(not (and p%d%d%d p%d%d%d p%d%d%d))", i, j, b, i+1, j-1, b, i+2, j-2, b) ;
            }
            fprintf(fp,")") ;
        }
    	fprintf(fp,")") ;
	}
	fprintf(fp,"))\n") ;

    
    int region_count; // In loop, to count the number of cells in each region
    int roop_region =0; // check how many are stored according to the region in the loop to rearrange them in order of region
    int region_cell_num[50]; // declare to check how many cells are in each region
    struct numPair region_order[100]; // arrange i, j in the order of region
    
    // Each region must include at least one black circle
    fprintf(fp,"; Q5\n") ;
    fprintf(fp,"(assert (and ") ;
    for(int m = 1; m <= max_region; m++){
        fprintf(fp,"(or") ;
        region_count = 0;
        for(i = 0; i <= 9; i++){
            for(j = 0; j <= 9; j++){
                if(region[i][j] == m){
                    region_order[roop_region].first = i;
                    region_order[roop_region].second = j;
                    roop_region++;
                    fprintf(fp," p%d%d%d", i, j, 1) ;
                    region_count++;
                }
            }
        }
        fprintf(fp,")") ;
        region_cell_num[m-1] = region_count;
    }
    fprintf(fp,"))\n") ;

    // In the same region, there must be no two black circles.
    fprintf(fp,"; Q6\n");
    fprintf(fp,"(assert (and ") ;
    int t = 0; // In the region_order array, when a condition is completed for a region, the index is increased by that number when it is passed to the region, and the declaration is made to repeat the next region.
    for(int m = 0; m < max_region; m++){  // For one region, write a condition that two cannot be black at the same time for the number of cases where all two can be pulled out.
        for(int j = 0 ; j < region_cell_num[m] - 1 ; j++){ 
            for(int k = j+1 ; k < region_cell_num[m] ; k++){
                fprintf(fp,"(not (and p%d%d%d p%d%d%d))", region_order[j+t].first, region_order[j+t].second, 1, region_order[k+t].first, region_order[k+t].second, 1) ;
            }
        }
        t += region_cell_num[m];
    }
    fprintf(fp,"))\n") ;

    fprintf(fp,"(check-sat)\n(get-model)\n") ;
    
    fclose(fp) ;

    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    
    // formula

    FILE * fin = popen("z3 formula", "r") ;
	char buf[128] ;
	char buf1[128] ;
	char buf2[128] ;
	fscanf(fin, "%s %s", buf, buf1) ;
    
    int nondango[10][10];
    for(int i=0; i<10; i++)
        for(int j=0; j<10; j++)
            nondango[i][j] = -1;  // For the output portion, set the default value to -1 instead of 0 or 1.

    int nondango_area[10][10];

    if(strcmp(buf, "unsat") == 0){
        printf("This puzzle has no solution.\n");
        return 0;
    }

	while (!feof(fin)) {
		fscanf(fin, "%s", buf) ; //printf("%s ", buf) ;
		fscanf(fin, "%s", buf1) ; //printf("%s ", buf1) ;
		fscanf(fin, "%s", buf) ; //printf("%s ", buf) ;
		fscanf(fin, "%s", buf) ; //printf("%s ", buf) ;
		fscanf(fin, "%s", buf2) ; //printf("%s\n", buf2) ;
        if(strcmp(buf2, "true)") == 0) {
            int i = buf1[1] - '0';
            int j = buf1[2] - '0';
            int n = buf1[3] - '0';
            nondango[i][j] = n;
        }
	}

    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            switch (nondango[i][j])
            {
            case 0 :
                printf("W ");
                break;
            case 1 :
                printf("B ");
                break;                
                 
            default:
                printf("X ");
                break;
            }
        }
        printf("\n");
    }

    printf("\nThe elapsed time is %f seconds\n", time_spent);
    
    return 0;
}
