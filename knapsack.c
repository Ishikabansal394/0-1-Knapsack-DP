#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 100
#define MAX_LINE 256

void solveKnapsack(int n, int W, int weights[],
                   int values[], FILE *out, int testNo) {

    int dp[MAX_ITEMS + 1][W + 1];

    printf("\n====================================\n");
    printf("       KNAPSACK TEST CASE %d\n", testNo);
    printf("====================================\n");
    printf("Items: %d | Capacity: %d\n", n, W);

    // Initialize the first row
    for (int w = 0; w <= W; w++)
        dp[0][w] = 0;

    printf("\nInitial DP Table:\n");
    printf("Item/Cap ");
    for (int w = 0; w <= W; w++)
        printf("%4d", w);
    printf("\n");

    for (int w = 0; w <= W; w++)
        printf("%4d", dp[0][w]);
    printf("\n");

    // Build the DP table one item at a time
    for (int i = 1; i <= n; i++) {

        for (int w = 0; w <= W; w++) {

            if (weights[i - 1] <= w) {

                int include =
                    values[i - 1] +
                    dp[i - 1][w - weights[i - 1]];

                int exclude = dp[i - 1][w];

                dp[i][w] = (include > exclude)
                           ? include : exclude;

            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }

        // Display the table after processing each item
        printf("\nAfter processing Item %d "
               "(Weight=%d, Value=%d):\n",
               i, weights[i - 1], values[i - 1]);

        printf("Item/Cap ");
        for (int w = 0; w <= W; w++)
            printf("%4d", w);
        printf("\n");

        for (int row = 0; row <= i; row++) {
            printf("%4d    ", row);

            for (int w = 0; w <= W; w++)
                printf("%4d", dp[row][w]);

            printf("\n");
        }
    }

    int result = dp[n][W];

    printf("\nMaximum Value = %d\n", result);

    fprintf(out, "#Test Case %d (n=%d, W=%d) Result: %d\n",
            testNo, n, W, result);
}

int main(void) {

    FILE *input = fopen("input.txt", "r");

    if (input == NULL) {
        printf("Error: Cannot open input.txt\n");
        printf("Keep input.txt in the same folder as knapsack.c.\n");
        return 1;
    }

    FILE *out = fopen("result.txt", "w");

    if (out == NULL) {
        printf("Error: Cannot create result.txt\n");
        fclose(input);
        return 1;
    }

    char line[MAX_LINE];
    int testNo = 0;
    int n, W;

    while (fgets(line, sizeof(line), input) != NULL) {

        // Find the start of a test case
        if (strncmp(line, "#Test", 5) != 0)
            continue;

        // Read n and W from the next non-empty line
        int found = 0;

        while (fgets(line, sizeof(line), input) != NULL) {

            if (sscanf(line, "%d %d", &n, &W) == 2) {
                found = 1;
                break;
            }
        }

        if (!found)
            break;

        if (n < 1 || n > MAX_ITEMS || W < 0) {
            printf("Invalid test case input.\n");
            break;
        }

        int weights[MAX_ITEMS];
        int values[MAX_ITEMS];
        int valid = 1;

        // Read weight and value of every item
        for (int i = 0; i < n; i++) {

            if (fgets(line, sizeof(line), input) == NULL ||
                sscanf(line, "%d %d",
                       &weights[i], &values[i]) != 2) {
                valid = 0;
                break;
            }

            if (weights[i] < 0 || values[i] < 0) {
                valid = 0;
                break;
            }
        }

        if (!valid) {
            printf("Invalid or incomplete item data.\n");
            break;
        }

        testNo++;

        solveKnapsack(n, W, weights, values, out, testNo);
    }

    fclose(input);
    fclose(out);

    printf("\n====================================\n");
    printf("All test cases processed.\n");
    printf("Results saved in result.txt\n");
    printf("====================================\n");

    return 0;
}
