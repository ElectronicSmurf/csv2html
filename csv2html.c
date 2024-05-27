#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

void print_usage(const char *progname) {
    fprintf(stderr, "Usage: %s <input.csv> <output.html> <delimiter>\n", progname);
    fprintf(stderr, "Converts a CSV file to an HTML table.\n");
    fprintf(stderr, "Parameters:\n");
    fprintf(stderr, "  input.csv   - The input CSV file.\n");
    fprintf(stderr, "  output.html - The output HTML file.\n");
    fprintf(stderr, "  delimiter   - The delimiter used in the CSV file.\n");
}

void csv_to_html(FILE *csv, FILE *html, const char *title, const char *delimiter) {
    char line[MAX_LINE_LENGTH];
    fprintf(html, "<html>\n<head>\n<title>%s</title>\n<style>\n", title);
    fprintf(html, "body { background-color: #333; color: #fff; font-family: Arial, sans-serif; }\n");
    fprintf(html, "h1 { text-align: center; }\n");
    fprintf(html, "table { border-collapse: collapse; width: 100%%; }\n");
    fprintf(html, "td { border: 1px solid #fff; padding: 8px; }\n");
    fprintf(html, "</style>\n</head>\n<body>\n<h1>%s</h1>\n<table>\n", title);
    while (fgets(line, sizeof(line), csv)) {
        char *field = strtok(line, delimiter);
        fprintf(html, "<tr>\n");
        while (field) {
            char *start = strchr(field, '\"');
            char *end = strrchr(field, '\"');
            if (start && end && start != end) {
                *start = *end = ' ';
            }
            fprintf(html, "<td>%s</td>\n", field);
            field = strtok(NULL, delimiter);
        }
        fprintf(html, "</tr>\n");
    }
    fprintf(html, "</table>\n</body>\n</html>\n");
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }

    char *title = strdup(argv[1]);
    char *dot = strrchr(title, '.');
    if (dot) *dot = '\0';

    FILE *csv = fopen(argv[1], "r");
    if (!csv) {
        perror("Failed to open CSV file");
        return 1;
    }

    FILE *html = fopen(argv[2], "w");
    if (!html) {
        perror("Failed to open HTML file");
        return 1;
    }

    csv_to_html(csv, html, title, argv[3]);

    fclose(csv);
    fclose(html);
    free(title);

    return 0;
}