char *deleteQueue()
{
	struct requestObject *temp = head;
	head = head->next;
	return temp->query;
}
int tempo;
void queryProcess(void *nargs)
{
	char *query = deleteQueue();
	int ft = *((int *)nargs);
	int a = tempo;
	struct threadPool *tp = (struct threadPool *)nargs;
	tp->threadobj[a].free = 0;
	if (index(query, "create user") >= 0)
		createUser(query + strlen("create") + 1);
	else if (index(query, "update user") >= 0)
		updateUser(query + strlen("update") + 1);
	else if (index(query, "post") == 0)
		post(query);
	else if (index(query, "commentonpost") == 0)
		addToCacheMemory(query);
	else if (index(query, "removepost") >= 0)
		removePost(query);
	else if (index(query, "updatepost") >= 0)
		updatePost(query);
	else if (index(query, "likepost") >= 0)
		likePost(query);
	else if (index(query, "savepost") >= 0)
		savePost(query);
	else if (index(query, "uploadfile") >= 0)
		uploadFile(query);
	else if (index(query, "viewfiles") >= 0)
		prints(viewFiles(query));
	else if (index(query, "downloadfiles") >= 0)
		downloadFile(query);
	else if (index(query, "viewcomment") >= 0)
		prints(viewComments(query));
	else if (index(query, "removeuser") >= 0)
		removeUser(query);
	else if (index(query, "cachecommentonpost") >= 0)
		addToCacheMemory(query);
	printf("Executed %s query\n", query);
	tp->writeLock = 0;
	tp->threadobj[a].free = 1;
}

void allocate(struct threadPool *threadpool)
{
	int i;
	(*threadpool).writeLock = 0;
	for (i = 0; i < 5; i++)
	{
		(*threadpool).threadobj[i].free = 1;
	}
}
void *freeThreadPoolObject(void *nargs)
{
	int *i, j = 0;
	struct checkForFree *temp;
	temp = (struct checkForFree *)nargs;
	struct threadPool *threadpool = temp->tp;
	i = temp->thr;
	while (1)
	{
		if (threadpool->writeLock == 0 && threadpool->threadobj[j].free == 1)
		{
			*i = j;
			goto out;
		}
		else
			j++;
		if (j == 5)
			j = 0;
	}
out:
	Sleep(5);
	*i = j;
	return NULL;
}


int request(struct threadPool *threadpool)
{
	pthread_t checkForFree;
	struct checkForFree f1;
	int freeThread;
	while ((*threadpool).writeLock == 1)
	{

	}
	f1.thr = &freeThread; f1.tp = threadpool;
	(pthread_create(&checkForFree, NULL, freeThreadPoolObject, &f1));
	pthread_join(checkForFree, NULL);
	return freeThread;
}
void freeThread(struct threadPool *threadpool,int id)
{
	threadpool->threadobj[id].free = 1;
}

void insertQueue(char *query)
{
	struct requestObject *req = (struct requestObject *)malloc(sizeof(struct requestObject));
	req->next = NULL;
	req->query = (char *)malloc(strlen(query)*sizeof(char));
	strcpy(req->query, query);
	if (head == NULL)
	{
		head = req;
		return;
	}
	struct requestObject *temp = head;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = req;

}





void processingUsingThreads(char *query, struct threadPool *threadpool)
{
	insertQueue(query);
	int freeThread=0;	
	freeThread = request(threadpool);
	pthread_t temp = (*threadpool).threadobj[freeThread].thread;
	(*threadpool).threadobj[freeThread].free = 0;
	if (index(query, "viewcomment") >= 0 || index(query, "viewfiles") >= 0 || index(query, "downloadfiles") >= 0)
		(*threadpool).writeLock = 0;
	else
		(*threadpool).writeLock = 1;
	//printf("%d", freeThread);
	tempo = freeThread;
	pthread_create(&temp, NULL, queryProcess,threadpool);
}

