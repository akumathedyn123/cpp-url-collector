// Owner: Md. N e H Jack
// Github profile: https://github.com/akumathedynd
// Copyright ©2020 Md. N e H Jack. All right reseved.
// Disclaimer: This code is provided for educational purposes only. The author is not responsible for any damages or hazards caused by its use.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <sys/stat.h>

#define MAX_URL_LEN 2048
#define MAX_FILENAME_LEN 4096

void read_urls_from_file(const char *filename, char urls[][MAX_URL_LEN], int *num_urls);
void download_html(const char *url, char **html_content, long *content_length);
void extract_links(const char *html_content, char **links);
void save_links_to_file(const char *links, const char *url, const char *output_dir);
void delete_url(const char *url, const char *input_file);

int main()
{
    char input_file[] = "urls.txt";
    char output_dir[] = "/path/to/text";
    char urls[100][MAX_URL_LEN];
    int num_urls = 0;
    read_urls_from_file(input_file, urls, &num_urls);
    int num_copied = 0;
    for (int i = 0; i < num_urls; i++)
    {
        char *html_content = NULL;
        long content_length = 0;
        download_html(urls[i], &html_content, &content_length);
        char *links = NULL;
        extract_links(html_content, &links);
        save_links_to_file(links, urls[i], output_dir);
        delete_url(urls[i], input_file);
        num_copied++;
        printf("Links from %s have been copied\n", urls[i]);
        free(html_content);
        free(links);
    }
    printf("Total %d links copied and removed from input file\n", num_copied);
    return 0;
}

void read_urls_from_file(const char *filename, char urls[][MAX_URL_LEN], int *num_urls)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    char line[MAX_URL_LEN];
    while (fgets(line, MAX_URL_LEN, fp) != NULL)
    {
        line[strcspn(line, "\n")] = '\0'; // remove newline character
        strncpy(urls[*num_urls], line, MAX_URL_LEN);
        (*num_urls)++;
    }
    fclose(fp);
}

void download_html(const char *url, char **html_content, long *content_length)
{
    CURL *curl = curl_easy_init();
    if (curl == NULL)
    {
        printf("Error initializing curl\n");
        exit(EXIT_FAILURE);
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, html_content);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        printf("Error downloading HTML content: %s\n", curl_easy_strerror(res));
        exit(EXIT_FAILURE);
    }
    curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_LONG, content_length);
    curl_easy_cleanup(curl);
}

void extract_links(const char *html_content, char **links)
{
    htmlDocPtr doc = htmlReadMemory(html_content, strlen(html_content), NULL, NULL, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
    if (doc == NULL)
    {
        printf("Error parsing HTML content\n");
        exit(EXIT_FAILURE);
    }
    xmlNodePtr root = xmlDocGetRootElement(doc);
    xmlNodePtr node = root;
    *links = calloc(1, 1);
    while (node != NULL)
    {
        if (node->type == XML_ELEMENT_NODE && strcmp((char *)node->name, "a") == 0)
        {
            xmlChar *href = xmlGetProp(node, (const xmlChar *)"href");
            if (href != NULL)
            {
                size_t len = strlen(*links);
                *links = realloc(*links, len + strlen((char *)href) + 2);
                strcat(*links, (char *)href);
                strcat(*links, "\n");
                xmlFree(href);
            }
        }
        node = node->next;
    }
    xmlFreeDoc(doc);
    xmlCleanupParser();
}

void save_links_to_file(const char *links, const char *url, const char *output_dir)
{
    char page_name[MAX_URL_LEN];
    const char *last_slash = strrchr(url, '/');
    if (last_slash == NULL)
    {
        strcpy(page_name, url);
    }
    else
    {
        strcpy(page_name, last_slash + 1);
    }
    char filename[MAX_FILENAME_LEN];
    snprintf(filename, MAX_FILENAME_LEN, "%s/URLs_%s_%s.txt", output_dir, page_name, url);
    for (int i = 0; i < strlen(filename); i++)
    {
        if (filename[i] == '/' || filename[i] == ':')
        {
            filename[i] = '_';
        }
    }
    mkdir(output_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%s", links);
    fclose(fp);
}

void delete_url(const char *url, const char *input_file)
{
    char urls[100][MAX_URL_LEN];
    int num_urls = 0;
    FILE *fp = fopen(input_file, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    char line[MAX_URL_LEN];
    while (fgets(line, MAX_URL_LEN, fp) != NULL)
    {
        line[strcspn(line, "\n")] = '\0'; // remove newline character
        if (strcmp(line, url) != 0)
        {
            strncpy(urls[num_urls], line, MAX_URL_LEN);
            num_urls++;
        }
    }
    fclose(fp);
    fp = fopen(input_file, "w");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_urls; i++)
    {
        fprintf(fp, "%s\n", urls[i]);
    }
    fclose(fp);
}