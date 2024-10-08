
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void run_ffmpeg_command(const char *output_filename, const char *ext) {
    size_t needed_size = snprintf(NULL, 0, "ffmpeg -f concat -safe 0 -i concatenate_list_temp.txt -c copy -y %s.%s", 
    output_filename, ext) + 1;

    char *command = (char *)malloc(needed_size);
    if (command == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    snprintf(command, needed_size, "ffmpeg -f concat -safe 0 -i concatenate_list_temp.txt -c copy -y %s.%s", output_filename, ext);

    int ret = system(command);

    free(command);

    if (ret == -1) {
        fprintf(stderr, "system() error");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char ** argv)
{
    
    FILE *fptr;
    char *filename = "concatenate_list_temp.txt";
    char *output_filename = "output";

   
    if (argc < 2) {
        fprintf(stderr, "No input files provided\n");
        exit(EXIT_FAILURE);
    }
    if (system("ffmpeg -version") != 0)
    {
        fprintf(stderr, "ffmpeg not found\n");
        exit(EXIT_FAILURE);
    }
    char *ext = strrchr(argv[1], '.');
     if (ext == NULL){
        fprintf(stderr, "No extension found\n");
        exit(EXIT_FAILURE);
    }

    fptr = fopen(filename, "w");
    if (fptr == NULL) {
        fprintf(stderr,"Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++){
        fprintf(fptr, "file '%s'\n", argv[i]);
    }

    fclose(fptr);

    run_ffmpeg_command(output_filename, ext + 1);

    return remove(filename);

}