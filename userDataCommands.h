void removeUser(char *query)
{
	struct details userDetails;
	int numberOfQueries;
	char **queryTokens = stringTokeniser(query,' ',&numberOfQueries);
	FILE *fp1 = fopen("insta.bin", "rb");
	FILE *fp = fopen("insta.bin", "ab");
	while (1)
	{
		fread(&userDetails, sizeof(userDetails), 1, fp1);
		if (userDetails.type != 0)
			fseek(fp1, userDetails.size, SEEK_CUR);
		else{
			char *buffer;
			buffer = (char *)malloc(sizeof(char)*(userDetails.size));
			fread(buffer, userDetails.size, 1, fp1); buffer[userDetails.size] = '\0';
			if (strcmp(buffer, queryTokens[1]) == 0)
			{
				displaceContents(ftell(fp1) - sizeof(userDetails) - strlen(queryTokens[1]), ftell(fp1));
				return;
			}
		}
	}
}





void createUser(char *query)
{
	char **queryToekns;
	char *userName;
	FILE *fp = fopen("insta.bin", "ab");
	long ofset=ftell(fp);
	struct details userDetais;
	int numberOfTokens = 0;
	queryToekns = stringTokeniser(query, ' ', &numberOfTokens);
	userName = queryToekns[1];
	userDetais.type = 0;
	userDetais.size = (long)strlen(userName);
	fwrite(&userDetais, sizeof(userDetais), 1, fp);
	fwrite(userName, strlen(userName), 1, fp);
	fclose(fp);
}

void updateUser(char *query)
{
	int numberOfTokens = 0;
	char **queryTokens = stringTokeniser(query,' ',&numberOfTokens);
	removeUser(query);
	FILE *fp = fopen("insta.bin", "rb");
	struct details user;
	while (fread(&user, sizeof(user), 1, fp))
	{

	}
	createUser(query+strlen(queryTokens[0])+1);
}
