struct requestObject
{
	char *query;
	struct requestObject *next;
};
struct requestObject *head = NULL;

struct threadObject
{
	int free;
	pthread_t thread;
};
struct threadPool
{
	struct threadObject threadobj[5];
	int writeLock;
};
struct details
{
	int type;
	long size;
	int likes;
};
struct checkForFree
{
	int *thr;
	struct threadPool *tp;
};
void prints(char *a)
{
	int i;
	for (i = 0; a[i] != '\0'; i++)
	{
		if (a[i] == '/')
			printf("\n");
		else
			printf("%c", a[i]);
	}
}
void writeContents(char *filename)
{
	FILE *fp = fopen("insta.bin", "ab");
	FILE *fp1 = fopen(filename, "rb");
	char b;
	while (fread(&b, 1, 1, fp1))
		fwrite(&b, 1, 1, fp);
	fclose(fp1);
	fclose(fp); 
	


}
char *append(char **tokens, int start, int end)
{
	char *res = malloc(strlen(tokens[start]));;
	int size = 0,i,j;
	for (i = start; i <= end; i++)
	{
		if (i == start)
			strcpy(res, tokens[i]);
		else
		{
			strcat(res, "/");
			strcat(res, tokens[i]);
		}
	}
	return res;
}
void displaceContents(long offset1, long offset2)
{
	FILE *fp = fopen("insta.bin", "rb");
	FILE *fp1 = fopen("temp.bin", "wb");
	fseek(fp, 0, SEEK_END);
	long offset = ftell(fp);
	char *a;
	fseek(fp, 0, SEEK_SET);
	a= (char *)malloc(offset1);
	fread(a, offset1, 1, fp);
	fwrite(a, offset1, 1, fp1);
	a = (char *)malloc(offset - offset2);
	fseek(fp,offset2, SEEK_SET);
	fread(a, offset - offset2, 1, fp);
	fwrite(a, offset - offset2, 1, fp1); fclose(fp); fclose(fp1);
	//free((void *)a);
	fp = fopen("insta.bin", "wb");
	fp1 = fopen("temp.bin", "rb");
	fseek(fp1, 0, SEEK_END);
	offset = ftell(fp1);
	a = (char *)malloc(offset);
	fseek(fp1, 0, SEEK_SET);
	fread(a, offset, 1, fp1);
	fwrite(a, offset, 1, fp);
	free((void *)a);

	fclose(fp);
	fclose(fp1);


}



char **stringTokeniser(char *string, char delimiter,int *numberOfTokens)
{
	char **res = NULL;
	int i,count=0,j;
	for (i = 0; i < strlen(string); i++)
	{
		if (string[i] != delimiter)
		{
			count++;
			res = (char **)realloc(res, sizeof(char *)*count);
			res[count - 1] = NULL;
			res = (char **)realloc(res, sizeof(char *)*count);
			for (j = 0; string[i + j] != delimiter&&string[i + j] != '\0'; j++)
			{
				res[count-1] = (char *)realloc(res[count-1], sizeof(char)*(j + 2));
				res[count-1][j] = string[i + j];
			}
			res[count-1][j] = '\0';
		}
		i+= j;
	}
	*numberOfTokens = count;
	return res;
}
int index(char *mainString, char *subString)
{
	int i, j;
	for (i = 0; mainString[i] != '\0'; i++)
	{
		if (mainString[i] == subString[0])
		{
			for (j = 0; subString[j]!='\0'&&subString[j] == mainString[i + j];j++)
			{ }
			if (subString[j] == '\0')
				return i;
		}
	}
	return -1;
}
