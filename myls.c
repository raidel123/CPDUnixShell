#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

void print_ls(struct stat buf, char* name);
int get_month(char* mmm);
//char** split_line(char *buff, char DELIM[2]);


int main(int argc, char *argv[])
{
    //int i;
    struct stat buf;
    
    char dirname[100];
    
    if (argc > 2) {
        printf("Usage: a.out [dirname]\n");
        exit(0);
    }
    
    if (argc == 1){
        strcpy(dirname, ".");
    }else{
        strcpy(dirname, argv[1]);
    }
    
    if (stat(dirname, &buf) <0) {
        printf("stat %s failed\n", dirname);
        exit(0);
    }
    
    if (S_ISDIR(buf.st_mode)){
        //printf("is directory\n");
        DIR *d;
        struct dirent *w;
        struct stat dir_buf;
        
        char filename[100];
        
        if ((d = opendir(dirname)) == NULL) {
            printf("Cannot access directory: %s\n", dirname);
            exit(0);
        }
        
        printf("total %d\n", 0);
        
        while((w = readdir(d)) != NULL){
            sprintf(filename, "%s/%s", dirname, w->d_name);
            
            char currdir[] = ".";
            char prevdir[] = "..";
            
            if ((strcmp(w->d_name, currdir) == 0) ||
                (strcmp(w->d_name, prevdir)==0)) continue;
            
            if (stat(filename, &dir_buf) <0) {
                printf("stat %s failed\n", filename);
                exit(0);
            }

            print_ls(dir_buf, w->d_name);
        }
        
        closedir(d);
    } else {
        print_ls(buf, dirname);
    }
    
    return 1;
}

void print_ls(struct stat buf, char* name){
    
    //char t_buf[100];
    //char *usr;
    
    // print filetype
    if (S_ISREG(buf.st_mode)) printf("-");
    else if (S_ISDIR(buf.st_mode)) printf("d");
    else if (S_ISCHR(buf.st_mode)) printf("c");  //, argv[1]);
    else if (S_ISBLK(buf.st_mode)) printf("b");
    else if (S_ISFIFO(buf.st_mode)) printf("f");
    else if (S_ISLNK(buf.st_mode)) printf("l");
    else if (S_ISSOCK(buf.st_mode)) printf("s");
    else printf("u");
    
    // print file mode
    if (S_IRUSR & buf.st_mode) printf("r");
    else printf("-");
    if (S_IWUSR & buf.st_mode) printf("w");
    else printf("-");
    if (S_IXUSR & buf.st_mode) printf("x");
    else printf("-");
    
    if (S_IRGRP & buf.st_mode) printf("r");
    else printf("-");
    if (S_IWGRP & buf.st_mode) printf("w");
    else printf("-");
    if (S_IXGRP & buf.st_mode) printf("x");
    else printf("-");
    
    if (S_IROTH & buf.st_mode) printf("r");
    else printf("-");
    if (S_IWOTH & buf.st_mode) printf("w");
    else printf("-");
    if (S_IXOTH & buf.st_mode) printf("x");
    else printf("-");
    printf("  ");

    // print number of links
    printf("%d", buf.st_nlink);
    printf(" ");
    
    // print owner name
    struct passwd *pw = getpwuid(buf.st_uid);
    printf("%s", pw->pw_name);
    printf(" ");
    
    // print group name
    struct group  *gr = getgrgid(buf.st_gid);
    printf("%s", gr->gr_name);
    printf(" ");
    
    //print bytes in file
    printf("%10d ", buf.st_size);
    
    // print time format: Mmm dd [mm:ss || YYYY]
    //time(&buf.st_mtime);
    //strftime (t_buf,sizeof(t_buf),"%b %e %Y ", localtime (&buf.st_mtime));
    
    char y_or_t[10];
    
    time_t rawtime;
    time ( &rawtime );
    
    char mmm1[10];
    char dd1[10];
    char yyyy1[10];
    strftime (mmm1,sizeof(mmm1),"%b", localtime (&rawtime));
    strftime (dd1,sizeof(dd1),"%e", localtime (&rawtime));
    strftime (yyyy1,sizeof(y_or_t),"%Y", localtime (&rawtime));
    
    int year1Val;
    year1Val = atoi(yyyy1);
    
    char mmm2[10];
    char dd2[10];
    char yyyy2[10];
    strftime (mmm2,sizeof(mmm2),"%b", localtime (&buf.st_mtime));
    strftime (dd2,sizeof(dd2),"%e", localtime (&buf.st_mtime));
    strftime (yyyy2,sizeof(y_or_t),"%Y", localtime (&buf.st_mtime));
    
    int year2Val;
    year2Val = atoi(yyyy2);
    
    if(abs(year1Val - year2Val) <= 1){
        
            if(abs(get_month(mmm1) - get_month(mmm2)) <= 7)
                strftime (y_or_t,sizeof(y_or_t),"%R", localtime (&buf.st_mtime));
            else
                strftime (y_or_t,sizeof(y_or_t),"%Y", localtime (&buf.st_mtime));
    
    }else{
        strftime (y_or_t,sizeof(y_or_t),"%Y", localtime (&buf.st_mtime));
    }
    
    printf("%s %s %s", mmm2, dd2, y_or_t);
    //printf("\n%s", ctime(&buf.st_mtime)+4);
    
    // print pathname
    printf(" %s\n", name);
    
}

int get_month(char* mmm){
 
    int num;
    
    if(strcmp(mmm, "Jan") == 0)
        num = 1;
    else if(strcmp(mmm, "Feb") == 0)
        num = 2;
    else if(strcmp(mmm, "Mar") == 0)
        num = 3;
    else if(strcmp(mmm, "Apr") == 0)
        num = 4;
    else if(strcmp(mmm, "May") == 0)
        num = 5;
    else if(strcmp(mmm, "Jun") == 0)
        num = 6;
    else if(strcmp(mmm, "Jul") == 0)
        num = 7;
    else if(strcmp(mmm, "Aug") == 0)
        num = 8;
    else if(strcmp(mmm, "Set") == 0)
        num = 9;
    else if(strcmp(mmm, "Oct") == 0)
        num = 10;
    else if(strcmp(mmm, "Nov") == 0)
        num = 11;
    else if(strcmp(mmm, "Dec") == 0)
        num = 12;
    else
        num = -1;
    
    return num;
}

