#include <math.h>

//Function declaration
void freeMat(double **matrix, int rows);
double calculateAngleA(int x1, int y1, int x2, int y2, int x3, int y3);
//End Function declaration



double calculateAngleA(int x1, int y1, int x2, int y2, int x3, int y3) {
    double angleA;

    // Calculate vectors BA and CA
    double BAx = x1 - x2;
    double BAy = y1 - y2;
    double CAx = x3 - x2;
    double CAy = y3 - y2;

    // Calculate dot product of BA and CA
    double dotProduct = BAx * CAx + BAy * CAy;

    // Calculate magnitudes of BA and CA
    double magnitudeBA = sqrt(BAx * BAx + BAy * BAy);
    double magnitudeCA = sqrt(CAx * CAx + CAy * CAy);

    // Calculate the cosine of angle A
    double cosAngleA = dotProduct / (magnitudeBA * magnitudeCA);

    // Ensure the value is within the valid range [-1, 1]
    if (cosAngleA >= -1.0 && cosAngleA <= 1.0) {
        // Calculate angle A in radians
        angleA = acos(cosAngleA);
    } else {
        // Handle the case when the value is outside the valid range
        angleA = 0.0;  // You can set a default value or handle it as needed
    }

    // Convert angleA to degrees
    //angleA = rad2deg(angleA);

    return angleA;
}


void freeMat(double **matrix, int rows) {
    for (int i = 0; i < rows; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

