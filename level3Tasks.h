

void addToCacheMemory(char *query)
{
	char **queryTokens, temp = '/';
	int numberOfTokens = 0;
	queryTokens = stringTokeniser(query, ' ', &numberOfTokens);
	struct details comment;
	comment.type = 2;
	comment.size = strlen(query) - strlen("cachecoomentonpost ");
	FILE *fp = fopen("cache.bin", "ab");
	fwrite(&comment, sizeof(comment), 1, fp);
	char *buff = append(queryTokens, 1, 2);
	fwrite(buff, strlen(buff), 1, fp);
	fwrite(&temp, 1, 1, fp);
	for (int i = 3; i < numberOfTokens; i++){
		temp = ' ';
		fwrite(queryTokens[i], strlen(queryTokens[i]), 1, fp);
		if (i != numberOfTokens - 1)
			fwrite(&temp, 1, 1, fp);
	}

	fclose(fp);
	commentOnPost(query);

}

char *cacheViewComments(char *query,int *flag)
{
	int numberOfTokens = 0;
	char **commentTokens, **queryTokens = stringTokeniser(query, ' ', &numberOfTokens);
	char *res = malloc(1); res[0] = '\0';
	FILE *fp = fopen("cache.bin", "rb");
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
				strcat(res, commentTokens[2]);
				strcat(res, "/");
				*flag = 1;
			}

		}
	}
	return res;
}