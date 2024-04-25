#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<string.h>
void gotoxy(int x,int y)
{
    COORD c;
    c.X=x;
    c.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}
struct event
{
    char date[15];
    char title[50];
    char description[1000];
};
void viewcalendar()
{
    system("cls");
    int year,month;
    printf("Enter any year(like 2023):");
    scanf("%d",&year);
    printf("Enter any month(1-12):");
    scanf("%d",&month);
    while(month<1 || month>12) {printf("Enter a valid month:");scanf("%d",&month);}
    calendar(month,year);
}
void calendar(int month,int year)
{
    system("cls");
    const char* monthname[]={"","January","February","March","April","May","June","July","August","September","October","November","December"};
    gotoxy(40,2);
    printf("**********CALENDAR**********");
    gotoxy(40,4);
    printf("      %s-%d    ",monthname[month],year);
    gotoxy(40,5);
    printf("----------------------------");
    gotoxy(40,6);
    printf("sun mon tue wed thu fri sat");
    gotoxy(40,7);
    printf("----------------------------");
    gotoxy(40,8);
    int daysinmonth[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    if((year%4==0 && year%100!=0)|| (year%400==0)) daysinmonth[2]=29;
    int firstday=dayofweek(1,month,year);
    for(int i=0;i<firstday;i++) printf("    ");
    int day,i=9;
    for(day=1;day<=daysinmonth[month];day++)
    {
        printf("%-4d",day);
        if((firstday+day)%7==0 || day==daysinmonth[month]) gotoxy(40,i++);
    }
    char character;
    printf("\nTo view next month calendar enter 'n',for previous month calendar enter 'p'. ");
    printf("\nTo exit enter any key except n and p.");
    fflush(stdin);
    scanf("%c",&character);
    if(character=='n')
    {
        (month<12)? calendar(month+1,year):calendar(1,year+1);
    }
    else if(character=='p')
    {
        (month>2)? calendar(month-1,year):calendar(12,year-1);
    }
    else menu();
}
int dayofweek(int d,int m,int y)
{
    if(m<3) {m+=12;y--;}
    int k=y%100;int j=y/100;
    int day=(d+(13*(m+1))/5+k+k/4+j/4+5*j)%7;
    return (day+6)%7;
}
void add()
{
    FILE *fp;
    struct event evt;
    char another='y';
    system("cls");
    fp=fopen("event.txt","ab+");
    if(fp==NULL)
    {
        gotoxy(10,5);
        printf("File is empty");
        exit(1);
    }
    fflush(stdin);
    while(another=='y')
    {
        gotoxy(10,2);
        printf("<----ADD AN EVENT---->");
        gotoxy(10,5);
        printf("Enter the details of the event:");
        gotoxy(10,7);
        printf("Enter the date:");
        gets(evt.date);
        gotoxy(10,8);
        printf("Enter the title:");
        gets(evt.title);
        gotoxy(10,9);
        printf("Write any note about event:");
        gets(evt.description);
        fwrite(&evt,sizeof(evt),1,fp);
        gotoxy(10,15);
        printf("Want to add another event? Then press 'y' else 'n'.");
        fflush(stdin);
        another=getch();
        system("cls");
        fflush(stdin);
    }
    fclose(fp);
    gotoxy(10,18);
    printf("Press any key to continue.");
    getch();
    menu();
}
void edit()
{
    char eventname[50];
    FILE *fp;
    struct event evt;
    system("cls");
    gotoxy(10,2);
    printf("<-------EDITING OF AN EVENT-------->");
    gotoxy(10,5);
    printf("Enter name of event to edit:");
    fflush(stdin);
    gets(eventname);
    fp=fopen("event.txt","rb+");
    if(fp==NULL)
    {
        gotoxy(10,6);
        printf("File is empty");
        exit(1);
    }
    rewind(fp);
    fflush(stdin);
    while(fread(&evt,sizeof(evt),1,fp)==1)
    {
        if(strcmp(eventname,evt.title)==0)
        {
            gotoxy(10,7);
            printf("Enter the details of the event:");
            gotoxy(10,9);
            printf("Enter the date of event:");
            gets(evt.date);
            gotoxy(10,10);
            printf("Enter the title of the event:");
            fflush(stdin);
            gets(evt.title);
            gotoxy(10,11);
            printf("Write any note about event:");
            fflush(stdin);
            gets(evt.description);
            fseek(fp,-sizeof(evt),SEEK_CUR);
            fwrite(&evt,sizeof(evt),1,fp);
            break;
        }
    }
    fclose(fp);
    gotoxy(10,16);
    printf("Press any key to continue.");
    getch();
    menu();
}
void view()
{
    FILE *fp;
    int i=1,j;
    struct event evt;
    system("cls");
    gotoxy(10,3);
    printf("<----VIEW EVENTS---->");
    gotoxy(10,5);
    printf("S.No   Date         Name of event                Description");
    gotoxy(10,6);
    printf("------------------------------------------------------------");
    fp=fopen("event.txt","rb+");
    if(fp==NULL)
    {
        gotoxy(10,8);
        printf("File is empty.");
        exit(1);
    }
    j=8;
    while(fread(&evt,sizeof(evt),1,fp)==1)
    {
        gotoxy(10,j++);
        printf("%-7d%-12s%-30s%-48s",i++,evt.date,evt.title,evt.description);
    }
    fclose(fp);
    gotoxy(10,j+3);
    printf("Press any key to continue.");
    getch();
    menu();
}
void del()
{
    char eventname[50];
    FILE *fp,*ft;
    struct event evt;
    system("cls");
    gotoxy(10,2);
    printf("<--------DELETING AN EVENT-------->");
    gotoxy(10,5);
    printf("Enter name of event to delete: ");
    fflush(stdin);
    gets(eventname);
    fp=fopen("event.txt","rb+");
    if(fp==NULL)
    {
        gotoxy(10,6);
        printf("File is empty");
        exit(1);
    }
    ft=fopen("temp.txt","wb+");
    if(ft==NULL)
    {
        gotoxy(10,6);
        printf("File is empty");
        exit(1);
    }
    while(fread(&evt,sizeof(evt),1,fp)==1)
    {
        if(strcmp(eventname,evt.title)!=0)
            fwrite(&evt,sizeof(evt),1,ft);
    }
    fclose(fp);
    fclose(ft);
    remove("event.txt");
    rename("temp.txt","event.txt");
    gotoxy(10,10);
    printf("Press any key to continue.");
    getch();
    menu();
}
void deleteall()
{
    remove("event.txt");
    system("cls");
    gotoxy(10,2);
    printf("All events are deleted");
    gotoxy(10,5);
    printf("Press any key to continue.");
    getch();
    menu();
}
void menu()
{
    system("cls");
    gotoxy(10,2);
    printf("<------MENU------>\n\n\n");
    int choice;
    printf("1.View calendar\n2.Add event\n3.Edit event\n4.View events\n5.Delete event\n6.Delete all the events\n7.Exit\n");
    printf("Enter a number according to your choice:");
    scanf("%d",&choice);
    switch(choice)
    {
        case 1:viewcalendar();break;
        case 2:add();break;
        case 3:edit();break;
        case 4:view();break;
        case 5:del();break;
        case 6:deleteall();break;
        case 7:return;
        default:printf("Invalid Choice");
    }
}
int main(void)
{
    gotoxy(45,8);
    printf("*******CALENDAR APPLICATION*******");
    gotoxy(10,15);
    printf("Enter any key to Start:");
    getch();
    menu();
    return 0;
}
