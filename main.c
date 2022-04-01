#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define clear system("clear||@cls");
#define COMMAND_LEN 128
#define bool char

typedef struct fac_struct {
    char* name;
    struct fac_struct* next;
    struct fac_struct* prev;
} f_node;
typedef struct fac_head {
    struct fac_struct* first;
    struct fac_struct* last;
    int cnt;
} f_head;
typedef struct student_struct {
    char* name;
    f_node* faculty;
    int no;
    int age;
    int id;
    float avg_score;
    float completion_rate;
    int gia_results[3];
    struct student_struct *next;
    struct student_struct *prev;
} node;
typedef struct student_head {
    struct student_struct* first;
    struct student_struct* last;
    int cnt;
} head;

void csv_line_parser(head *hd, f_head* f_hd, char* line);
char** split(char *line, char sep);
char* bgets(char *st, int len, FILE *fp);
int ibgets(char *st, FILE *fp);
float fbgets(char *st, FILE *fp);
f_node * foreign_key(f_head *f_hd, char* fac_name);
void make_new_f(f_head * f_hd, char * node_name);
void create(head * hd, f_head * f_hd);
void UI(head * hd, f_head * f_hd);
void quick_look(head * hd);
bool cmd_check(char * cmd, head * hd, f_head * f_hd);
void enter(head * hd, f_head * f_hd, char* cmd);
void import(head * hd, f_head * f_hd, char * cmd);
void export(head * hd, char * cmd);
bool func_cmp(char * cmd, char * compare);
void help(char * cmd);
char* just_copy(const char* st);
void delete_all(head * hd, f_head * f_hd);
int strcount(char* in, char* substring);
int main() {
    f_head * f_hd;
    head * hd;
    printf("Welcome to Kartoteka v1.0\n"
           " ___   ___   ________   ______   _________  ______   _________  ______   ___   ___   ________      \n"
           "/___/\\/__/\\ /_______/\\ /_____/\\ /________/\\/_____/\\ /________/\\/_____/\\ /___/\\/__/\\ /_______/\\     \n"
           "\\::.\\ \\\\ \\ \\\\::: _  \\ \\\\:::_ \\ \\\\__.::.__\\/\\:::_ \\ \\\\__.::.__\\/\\::::_\\/_\\::.\\ \\\\ \\ \\\\::: _  \\ \\    \n"
           " \\:: \\/_) \\ \\\\::(_)  \\ \\\\:(_) ) )_ \\::\\ \\   \\:\\ \\ \\ \\  \\::\\ \\   \\:\\/___/\\\\:: \\/_) \\ \\\\::(_)  \\ \\   \n"
           "  \\:. __  ( ( \\:: __  \\ \\\\: __ `\\ \\ \\::\\ \\   \\:\\ \\ \\ \\  \\::\\ \\   \\::___\\/_\\:. __  ( ( \\:: __  \\ \\  \n"
           "   \\: \\ )  \\ \\ \\:.\\ \\  \\ \\\\ \\ `\\ \\ \\ \\::\\ \\   \\:\\_\\ \\ \\  \\::\\ \\   \\:\\____/\\\\: \\ )  \\ \\ \\:.\\ \\  \\ \\ \n"
           "    \\__\\/\\__\\/  \\__\\/\\__\\/ \\_\\/ \\_\\/  \\__\\/    \\_____\\/   \\__\\/    \\_____\\/ \\__\\/\\__\\/  \\__\\/\\__\\/ "
           "\nPlease, resize your window so that Kartoteka logo is readable and press Enter\n");
    getchar();
    clear

    /* Allocating Memory */
    f_hd = (f_head*)malloc(sizeof(f_head));
    hd = (head*)malloc(sizeof(head));
    create(hd, f_hd);
    /* Launching User Interface */
    UI(hd, f_hd);
    return 0;
}

void UI(head * hd, f_head * f_hd) {
    char * cmd;
    bool quit;
    quit = 0;
    cmd = malloc(COMMAND_LEN);
    quick_look(hd);
    while (!quit) {
        printf(">>> ");
        bgets(cmd, COMMAND_LEN - 1, stdin);
        quit = cmd_check(cmd, hd, f_hd);
    }
}

bool cmd_check(char * cmd, head * hd, f_head * f_hd) {
    bool q;
    q = 0;
    if(func_cmp(cmd, "Quit")) {
        printf("Thank You for using Kartoteka\n");
        q = 1;
    }
    else if(func_cmp(cmd, "Enter")) {
        clear
        enter(hd, f_hd, cmd);
        clear
    }
    else if(func_cmp(cmd, "Import")) {
        import(hd, f_hd, cmd);
    }
    else if(func_cmp(cmd, "Export")) {
        export(hd, cmd);
    }
    else if(func_cmp(cmd, "Delete All")) {
        //delete_all(hd, f_hd);
    }
    else if(func_cmp(cmd, "Quick")) {
        quick_look(hd);
    } else if(func_cmp(cmd, "Help")) {
        help(cmd);
    } else if(func_cmp(cmd, "Clear")) clear
    else {
        printf("typo error: Command not found: %s\n", cmd);
        if(cmd[0] > 'Z' || cmd[0] < 'A') printf("Note that all commands start with capital letter\n");
        else printf("Type 'Help' to see commands available\n\n");
    }
    return q;
}

void enter(head * hd, f_head * f_hd, char * cmd) {
    char * mode, * temp, * yn;
    node * student;
    f_node * fac;
    int i;
    mode = malloc(2);
    if (*(cmd+5) != '\0') {
        cmd += 5;
        for(;*cmd == ' '; cmd++);
        if(!strcmp(cmd, "csv")) *mode = 'c';
        else if(!strcmp(cmd, "lbl")) *mode = 'l';
        else { printf("typo error: Mode %s not found\n", cmd); *mode = 0; }
        *(mode+1) = 0;
    }
    while (*mode == 0) {
        printf("How would you like to print data?\n1 - Line by line\n2 - CSV Format\n0 - Cancel\n\n");
        bgets(mode, 2, stdin);
        if (*mode == '1') *mode = 'l';
        else if (*mode == '2') *mode = 'c';
        else if (*mode == '0') *mode = 'a';
        else *mode = 0;
        if (*mode == 0) printf("Write only '1', '2' or '0'.\n");
        fflush(stdin);
    }

    yn = malloc(3); *yn = 'Y'; *(yn+1) = 0;
    if (*mode == 'l') {
        while(*yn == 'Y' || *yn == 'y') {
            temp = malloc(41);
            student = (node*)malloc(sizeof(node));
            if(student == NULL) {printf("fatal error: Unknown Memory Error while allocating memory in Enter. (student)\n"); exit(1);}
            printf("Enter the name: ");
            bgets(temp, 32 - 1, stdin);
            student->name = just_copy(temp);
            printf("Enter the faculty: ");
            bgets(temp, 32 - 1, stdin);
            student->faculty = foreign_key(f_hd, temp);
            printf("Enter the Age: ");
            student->age = ibgets(temp, stdin);
            printf("Enter the ID: ");
            student->id = ibgets(temp, stdin);
            printf("Enter the Average Score: ");
            student->avg_score = fbgets(temp, stdin);
            printf("Enter the Completion Rate: ");
            student->completion_rate = fbgets(temp, stdin);
            for (i = 0; i < 3; i++) {
                printf("Enter the GIA Result #%d: ", i + 1);
                student->gia_results[i] = ibgets(temp, stdin);
            }
            student->no = ++hd->cnt;
            student->next = NULL;
            if (hd->cnt < 1) {
                printf("fatal error: Unknown Error Negatives in counter (Enter: hd->cnt < 0)");
                exit(1);
            }
            if (hd->cnt == 1) {
                hd->first = student;
                hd->last = student;
                student->prev = NULL;
            } else {
                student->prev = hd->last;
                hd->last->next = student;
                hd->last = student;
            }
            printf("Would you like to Enter Line by line once more? (Y/N): ");
            bgets(yn, 3, stdin);
        }

    }
    else if (*mode == 'c') {
        temp = malloc(128);
        printf("Enter CSV Kartoteka line. Type Stop when finished:\n");
        bgets(temp, 127, stdin);
        while(strcmp(temp, "Stop") != 0) {
            if(strcount(temp, ";") != 8) {
                printf("typo error: Wrong Amount of Members in CSV\n"
                       "Input Should something like 'one;two;3;4;5;6;7;8;9'\n"
                       "Type Stop to finish\n");
            } else csv_line_parser(hd, f_hd, temp);

            bgets(temp, 127, stdin);
        }
    }
}
void import(head * hd, f_head * f_hd, char * cmd) {
    printf("error: Import function is not done yet.\n\n");
}
void export(head * hd, char * cmd) {
    printf("error: Export function is not done yet.\n\n");
}

void quick_look(head * hd) {
    unsigned long count;
    int i;
    node * temp;
    count = 0;
    if (!hd->cnt) printf("Kartoteka is empty for now.\n"
                         "Use Enter or Import to fill it.\n");
    else {
        printf("| %-2s | %-2s | %-23s | %-7s | %-3s | %3s | %2s | %15s |\n", "No", "ID", "Name", "Faculty", "Age", "Avg.Scr", "CR","GIA Results");
        for (temp = hd->first; temp != NULL && count < 5; temp = temp->next, count++) {
            printf("| %-2d | %-2d | %-23s | %-7s | %-3d | %.3f | %.2f | %-3d | %-3d | %-3d |\n",
                   temp->no, temp->id, temp->name,
                   temp->faculty->name, temp->age,
                   temp->avg_score, temp->completion_rate,
                   temp->gia_results[0], temp->gia_results[1], temp->gia_results[2]);
        }
        if(hd->cnt - 5 > 0) { for(i = 0; i <= 85; i++) printf("-"); printf("\n| Hidden : %-5d %68s|\n", hd->cnt - 5, " "); }
    }
    printf("Type Help to see what Kartoteka can do\n"
           "Type Quit to quit\n\n");
}


void csv_line_parser(head *hd, f_head* f_hd, char* line) {
    int j;
    node *db, *temp;
    char **splitLine;
    db = (node*)malloc(sizeof(node));
    if(db == NULL) {
        printf("fatal error: Unknown Memory Error while parsing CSV line. (db)\n");
        exit(1);
    }
    splitLine = split(line, ';');
    if(splitLine == NULL) {
        printf("fatal error: Unknown Memory Error while parsing CSV line. (splitLine)\n");
        exit(1);
    }

    db->name = splitLine[0];
    db->faculty = foreign_key(f_hd, splitLine[1]);
    db->age = (int)strtol(splitLine[2], NULL, 10);
    db->id = (int)strtol(splitLine[3], NULL, 10);
    db->avg_score = (float)atof(splitLine[4]);
    db->completion_rate = (float)atof(splitLine[5]);
    for(j = 0; j < 3; j++) db->gia_results[j] = (int)strtol(splitLine[6+j], NULL, 10);
    db->next = NULL;
    db->no = ++hd->cnt;
    if (hd->cnt < 1) {printf("fatal error: Unknown Error Negatives in counter (Parser: hd->cnt < 0)"); exit(1);}
    if(hd->cnt == 1) {
        hd->first = db;
        db->prev = NULL;
    } else {
        temp = hd->last;
        temp->next = db;
        db->prev = temp;
    }
    hd->last = db;
    free(splitLine);
}

f_node * foreign_key(f_head *f_hd, char* fac_name) {
    f_node * node, * res;
    bool found;
    found = 0;
    for(node = f_hd->first; node != NULL && found == 0; node = node->next) {
        if(!strcmp(fac_name, node->name)) {
            found = 1;
            res = node;
        }
    }
    if(!found) {
        make_new_f(f_hd, fac_name);
        res = f_hd->last;
    }
    return res;
}

void make_new_f(f_head * f_hd, char * node_name) {
    f_node * node;
    node = (f_node*)malloc(sizeof(f_node));
    node->name = just_copy(node_name);
    node->next = NULL;
    if(!f_hd->cnt) {
        node->prev = NULL;
        f_hd->first = node;
        f_hd->last = node;
    } else {
        node->prev = f_hd->last;
        f_hd->last = node;
    }
    f_hd->cnt++;
}

char** split(char *line, const char sep) {
    int separators_in_line, i, k, m;
    char** result_array = NULL;

    for (i = 0, separators_in_line = 0; i < strlen(line); i++)
        if(line[i] == sep) separators_in_line++;

    result_array = (char**)malloc((separators_in_line + 1) * sizeof (char*));

    for(i = 0; i < separators_in_line + 1; i++) {
        if(result_array != NULL) {
            result_array[i] = (char*)malloc(strlen(line) * sizeof(char));
            if (result_array[i] == NULL) result_array = NULL;
        } else i = separators_in_line + 1;
    }

    if (result_array != NULL) {
        k = m = 0;
        for (i = 0; i < strlen(line); i++) {
            if (line[i] != sep) result_array[m][i - k] = line[i];
            else {
                result_array[m][i - k] = '\0';
                k = i + 1;
                m++;
            }
        }
    }

    return result_array;
}

char* bgets(char *st, int const len, FILE *fp) {
    unsigned long str_len;
    char* err;
    err = fgets(st, len, fp);
    str_len = strlen(st);
    if (st[str_len - 1] == '\n') {
        if (st[str_len - 2] == '\r')
            st[str_len - 2] = '\0';
        else
            st[str_len - 1] = '\0';
    }
    return err;
}

int ibgets(char *st, FILE *fp) {
    return (int)strtol(bgets(st, 11, fp), NULL, 10);
}
float fbgets(char *st, FILE *fp) {
    return atof(bgets(st, 11, fp));
}

bool func_cmp(char * cmd, char * compare) {
    char * temp1, * temp2;
    bool res;
    res = 1;
    for(temp1 = cmd, temp2 = compare; *temp1 != '\0' && *temp2 != '\0' && res == 1; temp1++, temp2++) {
        if (*temp1 != *temp2) res = 0;
    }
    return res;
}

int strcount(char* in, char* substring) {
    int count;
    unsigned long substring_len;
    char* st0;
    count = 0;
    substring_len = strlen(substring);
    st0 = strstr(in, substring);
    while(st0 != NULL) {
        count++;
        st0 += substring_len;
        st0 = strstr(st0, substring);
    }
    return count;
}

void create(head * hd, f_head * f_hd) {
    hd->cnt = 0;
    hd->first = NULL;
    hd->last = NULL;
    f_hd->cnt = 0;
    f_hd->first = NULL;
    f_hd->last = NULL;
}

char* just_copy(const char* st) {
    char * res;
    unsigned long len, i;
    len = strlen(st);
    res = malloc(len);
    for(i = 0; i < len; *(res + i) = *(st + i), i++);
    return res;
}

void help(char * cmd) {
    if (!strcmp(cmd, "Help"))
        printf("Available Commands:\n"
           "Quit                           - to quit\n"
           "Enter <csv/lbl>                - to fill Kartoteka from keyboard\n" // Unfinished
           "Import <file name>             - to fill Kartoteka from file\n" // Not Done
           "Export <file name>             - to make file from Kartoteka data\n" // Not Done
           "Show <max amount>              - show up to positive max amount of lines\n" // Not Done
           "Change <N>                     - Change line #N\n" // Not Done
           "Sort <column> <a/d>            - Sort column ascending/descending\n" // Not Done
           "Filter <column> < (how)value>  - Get All lines with necessary value\n"
           "Delete All                     - to delete Kartoteka database and start from the beginning\n" // Not Done
           "Quick                          - to quick look data in Kartoteka\n"
           "Help <Command>                 - for documentation\n\n" // Unfinished
           "Use Help <Command> for specific Function documentation\n\n");
    else {
        cmd += 4;
        for(; *cmd == ' '; cmd++);
        if (!strcmp(cmd, "Quit"))
            printf("Quit function is used to terminate Kartoteka\n"
                   "Just type 'Quit' if you had enough.\n\n");
    }
}
