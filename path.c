#include <malloc.h>
#include <string.h>


// Composes a Target File Path.
const char *getFilePath(
        const char *folderName,
        int frameNumber,
        const char *fileExtFull
) {
    const char pathSeparator = '/';

    // While the 'itoa' Function is absent in ANSI C, we are inventing a Wheel.
    char frameNumberStr[12];
    sprintf(frameNumberStr, "%d", frameNumber);

    size_t filePathLen = strlen(folderName) + 1 + strlen(frameNumberStr) + strlen(fileExtFull);
    char *filePathName = calloc(filePathLen + 1, sizeof(char)); // With Null Terminator at the End.

    sprintf(filePathName, "%s%c%s%s", folderName, pathSeparator, frameNumberStr, fileExtFull);
    return filePathName;
}
