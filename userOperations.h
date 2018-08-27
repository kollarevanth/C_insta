void post(char *query)
{
	char **queryTokens,temp1='/';
	int numberOfTokens = 0;
	queryTokens = stringTokeniser(query, ' ', &numberOfTokens);
	FILE *fp1 = fopen(queryTokens[3], "rb");
	FILE *fp = fopen("insta.bin", "ab");
	struct details postDetails;
	postDetails.type = 1;
	postDetails.likes = 0;
	fseek(fp1, 0, SEEK_END);
	postDetails.size = strlen(queryTokens[1]) + strlen(queryTokens[2]) + ftell(fp1)+2;
	fseek(fp1, 0, SEEK_SET);
	fwrite(&postDetails, sizeof(postDetails), 1, fp);
	char *buff = append(queryTokens, 1, 2);
	fwrite(buff, strlen(buff), 1, fp);
	fwrite(&temp1, 1, 1, fp);
	char temp;
	while (fread(&temp, 1, 1, fp1))
		fwrite(&temp, 1, 1, fp);
	fclose(fp1);
	fclose(fp);
}
void commentOnPost(char *query)
{
	char **queryTokens,temp='/';
	int numberOfTokens = 0;
	queryTokens = stringTokeniser(query, ' ', &numberOfTokens);
	struct details comment;
	comment.type = 2;
	comment.size = strlen(query) - strlen("coomentonpost ");
	FILE *fp = fopen("insta.bin", "ab");
	fwrite(&comment, sizeof(comment), 1, fp);
	char *buff = append(queryTokens, 1, 2);
	fwrite(buff, strlen(buff), 1, fp);
	fwrite(&temp, 1, 1, fp);
	for (int i = 3; i < numberOfTokens; i++){
		temp = ' ';
		fwrite(queryTokens[i], strlen(queryTokens[i]), 1, fp);
		if (i!=numberOfTokens-1)
			fwrite(&temp, 1, 1, fp);
	}

	fclose(fp);

}
void removePost(char *query)
{
	FILE *fp = fopen("insta.bin", "rb");
	fseek(fp, 0, SEEK_SET);
	int numberOfFields = 0,numberOfTokens=0;
	char **queryTokens = stringTokeniser(query, ' ', &numberOfTokens);
	struct details post;
	while (1)
	{
		fread(&post, sizeof(post), 1, fp);
		if (post.type != 1)
			fseek(fp, post.size, SEEK_CUR);
		else
		{
			char *buffer = (char *)malloc(sizeof(char)*post.size);
			fread(buffer, post.size, 1, fp); buffer[post.size] = '\0';
			char **bufferTokens = stringTokeniser(buffer, '/', &numberOfFields);
			if (strcmp(bufferTokens[1], queryTokens[1]) == 0){
				long g = ftell(fp);
				displaceContents(ftell(fp) -post.size-sizeof(post), ftell(fp));
				fclose(fp);
				return;
			}
			free((void *)buffer);

		}
	}
}
void updatePost(char *query)
{
	removePost(query);
	post(query + strlen("updatepost "));
}
void likePost(char *query)
{
	FILE *fp = fopen("insta.bin", "rb");
	int numberOfFields = 0, numberOfTokens = 0;
	char **queryTokens = stringTokeniser(query, ' ', &numberOfTokens);
	struct details post;
	while (1)
	{
		fread(&post, sizeof(post), 1, fp);
		if (post.type != 1)
			fseek(fp, post.size, SEEK_CUR);
		else
		{
			char *buffer = (char *)malloc(sizeof(char)*post.size);
			fread(buffer, post.size, 1, fp); buffer[post.size] = '\0';
			char **bufferTokens = stringTokeniser(buffer, '/', &numberOfFields);
			if (strcmp(bufferTokens[1], queryTokens[1]) == 0)
			{
				post.likes++;
				fseek(fp, -1*(post.size + (int)sizeof(post)), SEEK_CUR);
				fwrite(&post, sizeof(post), 1,fp);
				fclose(fp); return;
			}
			free((void *)buffer);
		}
	}

}

void savePost(char *query)
{
	FILE *fp = fopen("insta.bin", "rb");
	fseek(fp, 0, SEEK_SET);
	int numberOfFields = 0, numberOfTokens = 0;
	char **queryTokens = stringTokeniser(query, ' ', &numberOfTokens);
	struct details post;
	while (1)
	{
		fread(&post, sizeof(post), 1, fp);
		if (post.type != 1)
			fseek(fp, post.size, SEEK_CUR);
		else
		{
			char *buffer = (char *)malloc(sizeof(char)*post.size);
			fread(buffer, post.size, 1, fp); buffer[post.size] = '\0';
			char **bufferTokens = stringTokeniser(buffer, '/', &numberOfFields);
			if (strcmp(bufferTokens[1], queryTokens[1]) == 0)
			{
				FILE *fp1 = fopen(queryTokens[2], "wb");
				fwrite(buffer, post.size, 1, fp1);
				fclose(fp1);
				goto out;
			}
			free((void *)buffer);
		}
	}
	out:
	fclose(fp);
}