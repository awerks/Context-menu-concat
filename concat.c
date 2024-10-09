#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>

#define MAX_FILES 100


char* lowerString(char* s) {
  for(char *p=s; *p; p++)
     *p=tolower(*p);
  return s;
}

// Function to check if a file has a valid video extension
int is_video_file(const char *filename) {
    char *extension = strrchr(filename, '.');
    if (!extension) return 0; // No extension

    const char *valid_extensions[] = { ".mp4", ".mov", ".mkv", ".avi", ".flv", ".wmv", ".webm" };
    int num_extensions = sizeof(valid_extensions) / sizeof(valid_extensions[0]);

    for (int i = 0; i < num_extensions; i++) {
        if (strcmp(lowerString(extension), valid_extensions[i]) == 0) {
            return 1; // Valid video file
        }
    }
    return 0; // Not a valid video file
}

// Function to compare filenames numerically (for sorting)
int numeric_filename_compare(const void *a, const void *b) {
   
    const char *filename_a = strrchr(*(const char **)a, '\\');
    if (filename_a) {
        filename_a++;
    } else {
        filename_a = *(const char **)a;
    }
    const char *filename_b = strrchr(*(const char **)b, '\\');
    if (filename_b) {
        filename_b++;
    } else {
        filename_b = *(const char **)b;
    }

    char *endptr_a, *endptr_b;

    long num_a = strtol(filename_a, &endptr_a, 10);
    if (*endptr_a != '.') {

        fprintf(stderr, "Incorrect file format! It should be [number.extension, e.g 1.mp4 2.mp4 ..]\n");
        sleep(5);
        exit(EXIT_FAILURE);
    }

    long num_b = strtol(filename_b, &endptr_b, 10);
    if  (*endptr_b != '.') {
        fprintf(stderr, "Incorrect file format! It should be [number.extension, e.g 1.mp4 2.mp4 ..]\n");
        sleep(5);  
        exit(EXIT_FAILURE);
    }
    if (num_a < num_b) return -1;
    if (num_a > num_b) return 1;

    return strcmp(strrchr(filename_a, '.'), strrchr(filename_b, '.'));
}

// Function to run the ffmpeg command
void run_ffmpeg_command(const char *output_filename, const char *ext) {
    char command[512];
    snprintf(command, sizeof(command), 
             "ffmpeg -f concat -loglevel error -safe 0 -i concatenate_list_temp.txt -c copy -y %s.%s", 
             output_filename, ext);

    int ret = system(command);
    if (ret == -1) {
        fprintf(stderr, "Error: system() failed\n");
        sleep(5);
        exit(EXIT_FAILURE);
    }
}

void get_current_time_str(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y%m%d_%H%M%S", t);
}

void process_files(char *video_files[], int video_count, const char *output_filename, const char *temp_list, const char *ext, const char *path) {
    FILE *fptr = fopen(temp_list, "w");
    if (!fptr) {
        fprintf(stderr, "Error opening file\n");
        sleep(5);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < video_count; i++) {
        if (path) {
            fprintf(fptr, "file '%s\\%s'\n", path, video_files[i]);
        } else {
            fprintf(fptr, "file '%s'\n", video_files[i]);
        }
        fprintf(stdout, "Parsed file: %s\n", video_files[i]);
        free(video_files[i]);
    }

    fclose(fptr);
    run_ffmpeg_command(output_filename, ext);
}

int main(int argc, char **argv) {
    if (system("ffmpeg -version > nul") != 0) {
        fprintf(stderr, "Error: ffmpeg not found\n");
        sleep(5);
        return EXIT_FAILURE;
    }

    char *video_files[MAX_FILES];
    int video_count = 0;
    char output_filename[256] = "output_";
    char *temp_list = "concatenate_list_temp.txt";
    char time_str[32];
    get_current_time_str(time_str, sizeof(time_str));
    strcat(output_filename, time_str);  // Append time to output filename

    if (argc == 2) {
        // Directory mode
        struct stat statbuf;
        if (stat(argv[1], &statbuf) != 0 || !S_ISDIR(statbuf.st_mode)) {
            fprintf(stderr, "Provided argument is not a valid directory\n");
            sleep(5);
            return EXIT_FAILURE;
        }

        DIR *dir = opendir(argv[1]);
        if (!dir) {
            perror("opendir");
            sleep(5);
            return EXIT_FAILURE;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (is_video_file(entry->d_name)) {
                video_files[video_count++] = strdup(entry->d_name);
                if (video_count >= MAX_FILES) break;
            }
        }
        closedir(dir);

        qsort(video_files, video_count, sizeof(char *), numeric_filename_compare);
        char *ext = strdup(strrchr(video_files[0], '.') + 1);
        process_files(video_files, video_count, output_filename, temp_list, ext, argv[1]);
        free(ext);
    } else if (argc > 2) {
        // Multiple file mode
        for (int i = 1; i < argc; i++) {
            if (is_video_file(argv[i])) {
                video_files[video_count++] = strdup(argv[i]);
                if (video_count >= MAX_FILES) break;
            }
        }
        qsort(video_files, video_count, sizeof(char *), numeric_filename_compare);

        char *ext = strdup(strrchr(video_files[0], '.') + 1);
        process_files(video_files, video_count, output_filename, temp_list, ext, NULL);
        free(ext);
    } else {
        fprintf(stderr, "Usage: .\\concat.exe <directory> or <file1> <file2> ...\n");
        sleep(5);
        return EXIT_FAILURE;
    }
    
    return remove(temp_list);
}
