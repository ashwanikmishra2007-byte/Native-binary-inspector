#include <stdio.h>
#include <stdlib.h>

int main()
{ /*Command-line arguments in C allow you to pass inputs or parameters directly to a program
when it is executed from the terminal. This eliminates the need to prompt
the user for input at runtime using functions like scanf().*/
    const char *filename = "sample.bin";

    FILE *fileptr = fopen(filename, "rb"); /*Because argv[0] represents the program's name or invocation path, whereas argv[1]
      represents the first argument we pass to the program — in our case, the file we want to read.*/
    if (fileptr == NULL)
    {
        perror("Could not open file");
        return 1;
    }

    unsigned char *data = (unsigned char *)malloc(4096);

    if (data == NULL)
    {
        perror("Memory allocation failed");
        fclose(fileptr);
        return 1;
    }

    size_t bytesread = fread(data, 1, 4096, fileptr);
    /*data	Destination address in memory
    1	Size of each element, in C bytes
    4096	Maximum number of elements requested
    file	Source file stream*/

    if (ferror(fileptr))
    {
        perror("File read failed");
        free(data);
        fclose(fileptr);
        return 1;
    }

    if (fseek(fileptr, 0, SEEK_END) != 0)
    {
        perror("fseek failed");
        fclose(fileptr);
        return 1;
        /*This moves the file position to the end of the stream.
        The three arguments are:
        Argument	Meaning
        file	The file stream we opened
        0	Move zero bytes relative to the reference point
        SEEK_END	Use the end of the file as the reference point*/
    }

    size_t filesize = ftell(fileptr);

    if (filesize < 0)
    {
        perror("ftell failed");
        fclose(fileptr);
        return 1;
    }

    printf("File size: %zu bytes\n", filesize);

    printf("Bytes read: %zu\n", bytesread);

    for (size_t i = 0; i < bytesread; i++)
    {
        printf("%02X", data[i]);
        /*In C, %02X is a format specifier used in functions like printf to print an
        integer as a hexadecimal number in uppercase, ensuring it is always at least
        2 digits wide and padded with a leading zero if necessary*/
        if ((i + 1) % 16 == 0)
        {
            printf("\n");
        }
        printf("\n");
    }
    free(data);
    fclose(fileptr);

    return 0;
}