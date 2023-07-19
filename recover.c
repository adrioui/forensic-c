// Implement a program that recovers JPEGs from a forensic image.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover card.raw\n");
        return 1;
    }

    // Open input file and initialize output file to be written to
    FILE *file = fopen(argv[1], "r");
    FILE *img = NULL;

    // Variable to store file that has read
    BYTE buffer[BLOCK_SIZE];

    // Variable to keep track of the uncovered images and name it
    int counter = 0;
    char filename[counter + 1];

    // Repeat until the end of the file
    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        // If JPEG founded
        if (buffer[0] == 0xff && buffer[1] == 0xd8 &&
            buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Start of the first JPEG
            if (counter == 0)
            {
                // Open and write to a file
                sprintf(filename, "%03i.jpg", counter++);
                img = fopen(filename, "w");
                if (img == NULL)
                {
                    printf("Could not create.\n");
                    return 1;
                }
                fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, img);
            }

            // If not the first JPEG
            else
            {
                // Close the previous JPEG, make the new one and write it
                fclose(img);
                sprintf(filename, "%03i.jpg", counter++);
                img = fopen(filename, "w");
                fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, img);
            }
        }

        // If not a new JPEG
        else if (counter != 0)
        {
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, img);
        }
    }
    // Close the files
    fclose(file);
    fclose(img);
    return 0;
}