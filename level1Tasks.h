char *cacheViewComments(char *query, int *flag);
void uploadFile(char *query)
{
	char **queryTokens = NULL,temp='/';
	char *appe;
	int numberOfTokens = 0;
	queryTokens = stringTokeniser(query, ' ', &numberOfTokens);
	FILE *fp = fopen(queryTokens[2], "rb"); fseek(fp, 0, SEEK_END);
	struct details file;
	file.type = 3; file.size = ftell(fp)+2+strlen(queryTokens[1])+strlen(queryTokens[2]);
	FILE *fp1 = fopen("insta.bin", "ab");
	fwrite(&file, sizeof(file), 1, fp1);
	appe = append(queryTokens, 1, 2);
	fwrite(appe, strlen(appe), 1, fp1);
	fwrite(&temp, 1, 1, fp1);
	long ofset = ftell(fp1);
	fclose(fp1);
	fclose(fp);
	fp = fopen("insta.bin", "ab");
	fp1 = fopen(queryTokens[2], "rb");
	char b;
	while (fread(&b, 1, 1, fp1))
		fwrite(&b, sizeof(char), 1, fp);
	fclose(fp1);
	ofset = ftell(fp);
	fclose(fp);
}
char *viewFiles(char *query)
{
	int numberOfTokens = 0;
	char **bufferTokens,**queryTokens = stringTokeniser(query, ' ', &numberOfTokens);
	char *res = malloc(1); res[0] = '\0';
	FILE *fp = fopen("insta.bin", "rb");
	struct details temp;
	while (fread(&temp, sizeof(temp), 1, fp))
	{		
		if (temp.type != 3)
			fseek(fp, temp.size, SEEK_CUR);
		else
		{
			char *buff = (char *)malloc(temp.size);
			fread(buff, temp.size, 1, fp); buff[temp.size] = '\0';
			bufferTokens = stringTokeniser(buff, '/', &numberOfTokens);
			if (strcmp(bufferTokens[0], queryTokens[1]) == 0)
			{
				strcat(res, bufferTokens[1]);
				strcat(res, "/");
			}
		}
	}
	return res;
}
void downloadFile(char *query)
{
	int numberOfTokens;
	char **contentTokens,**queryTokens = stringTokeniser(query, ' ', &numberOfTokens);
	FILE *fp = fopen("insta.bin", "rb");
	struct details file;
	while (fread(&file, sizeof(file), 1, fp))
	{
		if (file.type != 3)
			fseek(fp, file.size, SEEK_CUR);
		else
		{
			char *buffer = (char *)malloc(sizeof(char)*file.size);
			fread(buffer, file.size, 1, fp); buffer[file.size] = '\0';
			contentTokens = stringTokeniser(buffer, '/', &numberOfTokens);
			if (strcmp(contentTokens[1], queryTokens[1]) == 0)
			{
				FILE *fp1 = fopen(queryTokens[2], "wb");
				fwrite(buffer + strlen(append(contentTokens, 0, 1))+1, file.size - 1 - strlen(append(contentTokens, 0, 1)), 1, fp1);
				fclose(fp1); fclose(fp);
				free((void *)buffer);
				return;
			}
		}
	}

}
void deleteFile(char *query)
{
	int numberOfTokens;
	char **contentTokens,**queryTokens=stringTokeniser(query,' ',&numberOfTokens);
	FILE *fp = fopen("insta.bin", "ab");
	fseek(fp, 0, SEEK_SET);
	struct details file;
	while (fread(&file, sizeof(file), 1, fp))
	{
		if (file.type != 3)
			fseek(fp, file.size, SEEK_CUR);
		else
		{
			char *buffer = (char *)malloc(file.size);
			fread(buffer, file.size, 1, fp); buffer[file.size] = '\0';
			contentTokens = stringTokeniser(buffer, '/', &numberOfTokens);
			if (strcmp(contentTokens[1], queryTokens[1]) == 0)
			{
				displaceContents(ftell(fp) - file.size - sizeof(file), ftell(fp));
			}
			free((void *)buffer);

		}
	}
}
char *viewComments(char *query)
{
	int numberOfTokens = 0;
	char **commentTokens,**queryTokens = stringTokeniser(query, ' ', &numberOfTokens);
	char *res = malloc(1); res[0] = '\0';
	FILE *fp = fopen("insta.bin", "rb");
	int flag = 0;
	char *res1;
	res1=cacheViewComments(query, &flag);
	if (flag == 1)
	{
		return res1;
	}
	struct details comment;
	while (fread(&comment, sizeof(comment), 1, fp))
	{
		if (comment.type != 2)
			fseek(fp, comment.size, SEEK_CUR);
		else
		{
			char *buffer = (char *)malloc(sizeof(char)*comment.size);
			fread(buffer, comment.size, 1, fp); buffer[comment.size] = '\0';
			commentTokens = stringTokeniser(buffer, '/', &numberOfTokens);
			if (strcmp(commentTokens[1], queryTokens[1]) == 0)
			{
				commentTokens[2][comment.size - strlen(append(commentTokens, 0, 1))] = '\0';
				strcat(res,commentTokens[2]);
				strcat(res, "/");
			}
			
		}
	}
	return res;
}
void deleteComment(char *query)
{
	int numberOfTokens = 0;
	char **commentTokens,**queryTokens = stringTokeniser(query, ' ', &numberOfTokens);
	FILE *fp = fopen("insta.bin", "rb+");
	struct details comment;
	while (fread(&comment, sizeof(comment), 1, fp))
	{
		if (comment.type != 2)
			fseek(fp, comment.size, SEEK_CUR);
		else
		{
			char *buffer = (char *)malloc(sizeof(char)*comment.size);
			fread(buffer, comment.size, 1, fp); buffer[comment.size] = '\0';
			commentTokens = stringTokeniser(buffer, '/', &numberOfTokens);
			if (strcmp(commentTokens[1], queryTokens[1]) == 0)
			{
				displaceContents(ftell(fp) - comment.size - sizeof(comment), ftell(fp));
				fseek(fp, -1 * (ftell(fp) - comment.size - sizeof(comment)), SEEK_CUR);
			}
			free((void *)buffer);
		}
	}
	fclose(fp);
}