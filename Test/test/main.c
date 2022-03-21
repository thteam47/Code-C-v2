#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAXN 100
#define MAXCHOICE '5'
#define FILENAME "database.txt"

char *ltrim(char *str)
{
	int i, len = strlen(str);
	for (i = 0; i < len && (str[i] == ' ' || str[i] == '\n'); ++i)
		;
	strcpy(str, str + i);
	str[len - i] = NULL;
	return str;
}

char *rtrim(char *str)
{
	int i, len = strlen(str);
	for (i = len - 1; i >= 0 && (str[i] == ' ' || str[i] == '\n'); --i)
		;
	str[i + 1] = 0;
	return str;
}
char *trim(char *str)
{
	rtrim(ltrim(str));
	int lstSpc = -1, len = strlen(str), c = 0;
	int i;
	for (i = 0; i < len; ++i)
		if (str[i] == ' ')
		{
			if (lstSpc < 0)
				lstSpc = i;
			else
				c += 1;
		}
		else
		{
			char t = str[i - c];
			str[i - c] = str[i];
			str[i] = t;
			lstSpc = -1;
		}
	str[len - c] = 0;
	return str;
}

char *nameStr(char s[])
{
	trim(s);
	strlwr(s);
	int i = 0;
	for (i = 0; i < strlen(s); i++)
		if (i == 0 || s[i - 1] == ' ')
			s[i] = toupper(s[i]);
	return s;
}
char getUserChoice()
{
	int c;
	printf("1-Adding a new student\n");
	printf("2-Find data about a student using a name inputted\n");
	printf("3-Remove a student based on a ID inputted\n");
	printf("4-Print the list in descending order based on grade + height\n");
	printf("5-Quit\n");
	printf("Choice = ");
	fflush(stdin);
	scanf("%c", &c);
	return c;
}

void add(char ID[][8], char name[][21], double grade[], double height[], int *pn)
{
	char ts[21];
	double g;
	int i, existed;
	do
	{
		printf("New ID (8 character) : ");
		fflush(stdin);
		scanf("%8[^\n]", ts);
		existed = 1;
		for (i = 0; i < *pn; i++)
			if (strcmp(ts, ID[i]) == 0)
			{
				printf("ID existed!Retype!\n");
				existed = 0;
				i = *pn - 1;
			}
	} while (!existed);
	strcpy(ID[*pn], ts);
	do
	{
		printf("New Name(20 character) : ");
		fflush(stdin);
		scanf("%20[^\n]", ts);
		existed = 1;
		for (i = 0; i < *pn; i++)
			if (strcmp(ts, name[i]) == 0)
			{
				printf("Name existed!Retype!\n");
				existed = 0;
				i = *pn - 1;
			}
	} while (!existed);
	nameStr(ts);
	strcpy(name[*pn], ts);
	printf("grade : ");
	scanf("%lf", &g);
	grade[*pn] = g;
	printf("height : ");
	scanf("%lf", &g);
	height[*pn] = g;
	write_file_end(ID[*pn], name[*pn], grade[*pn], height[*pn]);
	(*pn)++;
	printf("Added!!\n\n");
	system("pause");
	system("cls");
}

void search(char ID[][8], char name[][21], double grade[], double height[], int *pn)
{
	int i;
	printf("Searching for : ");
	char nv[21];
	fflush(stdin);
	scanf("%20[^\n]", &nv);
	nameStr(nv);
	for (i = 0; i < *pn; i++)
	{
		int j;
		char *ptr = strstr(name[i], nv);
		if (ptr != '\0')
			printf("RESULT :[%d] := ID:%8s|Name:%20s|grade:%3.3lf|height:%3.3lf \n", i, ID[i], name[i], grade[i], height[i]);
	}
	system("pause");
	system("cls");
}

void deleted(char ID[][8], char name[][21], double grade[], double height[], int *pn)
{
	int i, j;
	for (i = 0; i < *pn; i++)
		printf("ID:%8s|Name:%20s|grade:%3.3lf|height:%3.3lf \n", ID[i], name[i], grade[i], height[i]);
	char del[8];
	fflush(stdin);
	printf("ID for deleted(Success only when ID exist) : ");
	scanf("%8[^\n]", &del);

	for (i = 0; i < *pn; i++)
		if (strcmp(ID[i], del) == 0)
		{
			for (j = i + 1; j < *pn; j++)
			{
				strcpy(name[i - 1], name[i]);
				strcpy(ID[i - 1], ID[i]);
				grade[i - 1] = grade[i];
				height[i - 1] = height[i];
			}
			(*pn)--;
			printf("Success!\n\n");
		}
	write_file(ID, name, grade, height, pn);
	system("pause");
	system("cls");
}

void print(char ID[][8], char name[][21], double grade[], double height[], int *pn)
{

	int i, j;
	for (i = 0; i < *pn - 1; i++)
		for (j = *pn - 1; j > i; j--)
			if ((grade[j] + height[j]) > (grade[j - 1] + height[j - 1]))
			{
				char t[21];
				strcpy(t, name[j]);
				strcpy(name[j], name[j - 1]);
				strcpy(name[j - 1], t);

				char tu[8];
				strcpy(tu, ID[j]);
				strcpy(ID[j], ID[j - 1]);
				strcpy(ID[j - 1], tu);

				double tg = grade[j];
				grade[j] = grade[j - 1];
				grade[j - 1] = tg;

				double tb = height[j];
				height[j] = height[j - 1];
				height[j - 1] = tb;
			}
	for (i = 0; i < *pn; i++)
		printf("ID:%-8s|Name:%-20s|grade:%-3.3lf|height:%-3.3lf \n", ID[i], name[i], grade[i], height[i]);
	if (*pn)
		printf("Success!\n\n");
	else
		printf("Nothing to print!\n\n");
	system("pause");
	system("cls");
}
void halt()
{
	printf("Thanks");
}

void read_file(char ID[][8], char name[][21], double grade[], double height[], int *pn)
{
	char **b = (char **)malloc(100 * sizeof(char));
	char *p;
	char *q;
	FILE *fp = fopen(FILENAME, "r");
	if (!fp)
	{
		printf("File does not exist\n");
        return;
	}

	char line[255];
	int i = 1;
	int index = 0;
	while (fgets(line, 255, fp))
	{
		int count = 0;
		p = strtok(line, "|");
		char *tmp;
		char tmp2[255];
		while (p != NULL)
		{
		    printf("%s\n",p);
		    strcpy(tmp2,p);
		    *tmp = tmp2;
			if (count == 0)
			{
				q = strtok(tmp, ":");
				printf("%s\n",q);
				q = strtok(NULL, ":");
				printf("%s\n",q);
				strcpy(ID[index], q);
			}
			else if (count == 1)
			{
				q = strtok(tmp, ":");
				q = strtok(NULL, ":");
				strcpy(name[index], nameStr(q));
			}
			else if (count == 2)
			{
				q = strtok(tmp, ":");
				q = strtok(NULL, ":");
				grade[index] = atof(q);
			}
			else if (count == 3)
			{
				q = strtok(tmp, ":");
				q = strtok(NULL, ":");
				height[index] = atof(q);
			}
			count++;
			p = strtok(p, "|");
			printf("okkk %s\n\n",p);
		}
		index++;
	}
	*pn = index;

	fclose(fp);
	if (*pn)
		printf("Read file success!\n\n");
	else
		printf("Nothing to read!\n\n");
	system("pause");
	system("cls");
}
void write_file(char ID[][8], char name[][21], double grade[], double height[], int *pn)
{
	FILE *fp = NULL;
	fopen_s(&fp, FILENAME, "w");
	for (int i = 0; i < *pn; i++)
		fprintf(fp, "ID:%-8s|Name:%-20s|grade:%-3.3lf|height:%-3.3lf \n", ID[i], name[i], grade[i], height[i]);
	fclose(fp);
	if (*pn)
		printf("Write file success!\n\n");
	else
		printf("Nothing to write!\n\n");
	system("pause");
	system("cls");
}
void write_file_end(char ID[8], char name[21], double grade, double height)
{
	FILE *fp = NULL;
	fopen_s(&fp, FILENAME, "a");
	fprintf(fp, "ID:%-8s|Name:%-20s|grade:%-3.3lf|height:%-3.3lf \n", ID, name, grade, height);
	fclose(fp);
	printf("Add data to file success!\n\n");
}

main()
{
	char userChoice;
	char ID[MAXN][8], name[MAXN][21];
	double grade[MAXN], height[MAXN];
	int n = 0;
	read_file(ID, name, grade, height, &n);
	do
	{
		userChoice = getUserChoice();
		switch (userChoice)
		{
		case '1':
			if (n == MAXN)
				printf("Khong the them!\n");
			else
				add(ID, name, grade, height, &n);
			break;
		case '2':
			if (n == 0)
				printf("Khong the tim kiem!\n");
			else
				search(ID, name, grade, height, &n);
			break;
		case '3':
			if (n == 0)
				printf("Khong co gi de xoa");
			else
				deleted(ID, name, grade, height, &n);
			break;
		case '4':
			if (n == 0)
				printf("Khong co gi de in");
			else
				print(ID, name, grade, height, &n);
			break;
		case '5':
			halt();
			break;
		}
		if (userChoice < '1' || userChoice > '5')
			printf("Chi nhap so tu 1 den 5!\n");
	} while (userChoice != MAXCHOICE);
}
