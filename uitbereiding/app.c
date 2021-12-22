#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
 
#include "query_ioctl.h"
 
void get_vars(int fd)
{
    query_arg_t q;
 
    if (ioctl(fd, QUERY_GET_VARIABLES, &q) == -1)
    {
        perror("get level of gpio");
    }
    else
    {
        printf("output : %d\n", q.output);
        printf("level: %d\n", q.level);
        printf("togglespeed    : %d\n", q.togglespeed);
    }
}
void clr_vars(int fd)
{
    if (ioctl(fd, QUERY_CLR_VARIABLES) == -1)
    {
        perror("clr gpio");
    }
}
void set_vars(int fd)
{
    int v;
    query_arg_t q;
 
    printf("Enter output: ");
    scanf("%d", &v);
    getchar();
    q.output = v;
    printf("Enter level: ");
    scanf("%d", &v);
    getchar();
    q.level = v;
    printf("Enter togglespeed: ");
    scanf("%d", &v);
    getchar();
    q.togglespeed = v;
 
    if (ioctl(fd, QUERY_SET_VARIABLES, &q) == -1)
    {
        perror("toggle a gpio output");
    }
}
 
int main(int argc, char *argv[])
{
    char *file_name = "/dev/query";
    int fd;
    enum
    {
        e_get,
        e_clr,
        e_set
    } option;
 
    if (argc == 1)
    {
        option = e_get;
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "-g") == 0)
        {
            option = e_get;
        }
        else if (strcmp(argv[1], "-c") == 0)
        {
            option = e_clr;
        }
        else if (strcmp(argv[1], "-s") == 0)
        {
            option = e_set;
        }
        else
        {
            fprintf(stderr, "Usage: %s [-g | -c | -s]\n", argv[0]);
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Usage: %s [-g | -c | -s]\n", argv[0]);
        return 1;
    }
    fd = open(file_name, O_RDWR);
    if (fd == -1)
    {
        perror("query_apps open");
        return 2;
    }
 
    switch (option)
    {
        case e_get:
            get_vars(fd);
            break;
        case e_clr:
            clr_vars(fd);
            break;
        case e_set:
            set_vars(fd);
            break;
        default:
            break;
    }
 
    close (fd);
 
    return 0;
}
