#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Define the Pilot Overlay data arrays
char* SV_L1_Pilot_Overlay_r0[] = {
    "",          // Index 0 is unused in Python, similar handling in C
    "0110111011",
    "0111101000",
    "1100000001",
    // Add remaining entries...
};

char* SV_L1_Pilot_Overlay_r1[] = {
    "",          // Index 0 is unused in Python, similar handling in C
    "0100110000",
    "0110000010",
    "1110010001",
    // Add remaining entries...
};

void shift(int *reg, int feedback, int length) {
    for (int i = 0; i < length - 1; i++) {
        reg[i] = reg[i + 1];
    }
    reg[length - 1] = feedback;
}

// Function prototype for shift
//void shift(int *reg, int feedback, int length);

// Function to generate NavIC CA Code Pilot Overlay
int* genNavicCaCode_Pilot_Overlay(int sv) {
    // Check bounds
    if (sv < 1 || sv > 64) {
        printf("Error: PRN ID out of bounds!\n");
        return NULL;
    }


    // Initialize r0_pl and r1_pl arrays
    int r0_pl[10], r1_pl[10];
    for (int i = 0; i < 10; i++) {
        r0_pl[i] = SV_L1_Pilot_Overlay_r0[sv][i] - '0';  // Convert char to int (0 or 1)
        r1_pl[i] = SV_L1_Pilot_Overlay_r1[sv][i] - '0';  // Convert char to int (0 or 1)
    }

int* ca = (int*)malloc(1800 * sizeof(int));
    //int ca[1800];  // PRN sequence
    //int overlaycodeLength = 1800;

    for (int idx = 0; idx < overlaycodeLength; idx++) {
        // Calculate r_pl
        int r_pl = (r0_pl[5] ^ r0_pl[2]) ^ (r0_pl[1] ^ r0_pl[0]);
        printf("r_pl: %d\n", r_pl);
        
        // Calculate sigma2A
        int sigma2A = (r0_pl[5] ^ r0_pl[2]) & (r0_pl[1] ^ r0_pl[0]);
        printf("sigma2A: %d\n", sigma2A);
        
        // Calculate sigma2B
        int sigma2B = (r0_pl[5] & r0_pl[2]) ^ (r0_pl[1] & r0_pl[0]);
        printf("sigma2B: %d\n", sigma2B);

        // Calculate sigma2
        int sigma2 = sigma2A ^ sigma2B;
        printf("sigma2: %d\n", sigma2);
        
        // Calculate R1A
        int temp = r0_pl[6] ^ r0_pl[3] ^ r0_pl[2] ^ r0_pl[0];
        int R1A = sigma2 ^ temp;
        printf("R1A: %d\n", R1A);
        
        // Calculate R1B
        int R1B = r1_pl[5] ^ r1_pl[2] ^ r1_pl[1] ^ r1_pl[0];
        printf("R1B: %d\n", R1B);
        
        // Calculate r3_pl
        int r3_pl = R1A ^ R1B;
        printf("r3_pl: %d\n", r3_pl);
        
        // Update registers
        int z2 = r1_pl[0];
        printf("z2: %d\n", z2);
        
        shift(r0_pl, r_pl, 10);
        shift(r1_pl, r3_pl, 10);
        
        // Append to PRN sequence
        ca[idx] = z2;
    }

    // Print or use PRN sequence as needed
    // For example, print first and last 24 bits:
    printf("First 24 bits of PRN sequence for Satellite ID %d:\n", sv + 1);
    for (int i = 0; i < 24; i++) {
        printf("%d ", ca[i]);
    }
    printf("\nLast 24 bits of PRN sequence for Satellite ID %d:\n", sv + 1);
    for (int i = overlaycodeLength - 24; i < overlaycodeLength; i++) {
        printf("%d ", ca[i]);
    }
    printf("\n");
    
    
    printf("ca=%d\n",*ca);
    return ca;
}

// Function to shift the register

int main() {
    int sv_id = 1;  // Replace with the satellite ID you want to generate PRN sequence for
    genNavicCaCode_Pilot_Overlay(sv_id);
    return 0;
}
