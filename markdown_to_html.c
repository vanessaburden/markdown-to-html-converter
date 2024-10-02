#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINES 1000
#define MAX_LINE_LENGTH 1024

void process_bold_italic(char *line, FILE *htmlFile); 
void convert_to_html(char markdown[][MAX_LINE_LENGTH], int num_lines, FILE *htmlFile);

int main()
{
    char markdown[MAX_INPUT_LINES][MAX_LINE_LENGTH];
    int num_lines = 0;

    printf("Enter Markdown: \n");

    while (num_lines < MAX_INPUT_LINES)
    {
        if (fgets(markdown[num_lines], MAX_LINE_LENGTH, stdin) == NULL)
        {
            break;
        }

        if (strcmp(markdown[num_lines], "\n") == 0)
        {
            break;
        }
        num_lines++;
    }

    FILE *htmlFile = fopen("output.html", "w");
    if (htmlFile == NULL)
    {
        printf("Error: Could not open files.\n");
        return 1;
    }

    convert_to_html(markdown, num_lines, htmlFile);

    fclose(htmlFile);

    printf("Markdown converted to HTML successfully.\n");
    return 0;

}

void process_bold_italic(char *line, FILE *htmlFile)
{
    char *start;
    while((start = strstr(line, "**")) != NULL)
    {
        *start = '\0';
        fprintf(htmlFile, "%s<strong>", line);
        line = start + 2;

        start = strstr(line, "**");
        if (start == NULL) 
        break;
        *start = '\0';
        fprintf(htmlFile, "%s</strong>", line);
        line = start + 2;
    }
    fprintf(htmlFile, "%s", line);
}

void convert_to_html(char markdown[][MAX_LINE_LENGTH], int num_lines, FILE *htmlFile)
{
    int in_list = 0;

    fprintf(htmlFile, "<!DOCTYPE html>\n<html>\n<head>\n<title>Markdown Output</title>\n</head>\n<body>\n");

    for (int i = 0; i < num_lines; i++)
    {
        char *line = markdown[i];

        if (strncmp(line, "# ", 2) == 0)
        {
            fprintf(htmlFile, "<h1>%s</h1>\n", line + 2);

        }
        else if (line[0] == '-' || line[0] == '*')
        {
            if(!in_list)
            {
                fprintf(htmlFile,"<ul>\n");
                in_list = 1;
            }
            fprintf(htmlFile, "<li>%s</li>\n", line + 2);
        }
        else
        {
            if (in_list)
            {
                fprintf(htmlFile, "</ul>\n");
                in_list = 0;
            }
            fprintf(htmlFile, "<p>");
            process_bold_italic(line, htmlFile);
            fprintf(htmlFile, "</p>\n");
        }
    }
    fprintf(htmlFile, "</body>\n</html>");
}