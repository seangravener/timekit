#include <stdio.h>
#include <time.h>

int main() {
    time_t now;
    struct tm *timeinfo;

    // Get current time
    time(&now);

    // Convert to local time format
    timeinfo = localtime(&now);

    // Display formatted time
    printf("Current local time: %s", asctime(timeinfo));

    return 0;
}

