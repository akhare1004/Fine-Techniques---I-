#include <stdio.h>
#include <stdlib.h>
#include "server.h"

int main()
{
	FILE *fp = init_tree("bst.dat");
	int opt, key;
	node_t n;
	printf("0.exit\n1.insert\n2.delete\n3.display\n");
	scanf("%d", &opt);
	while(opt)
	{
		switch(opt)
		{
			case 1: printf("Enter key to insert : ");
				scanf("%d", &key);
				insert_key(key, fp);
				break;
			case 2: printf("Enter key to delete : ");
				scanf("%d", &key);
				delete_key(key, fp);
				break;
			case 3: display_preorder(fp);
				display_inorder(fp);
				break;
			case 0: exit(0);
		}
		printf("0.exit\n1.insert\n2.delete\n3.display\n");
		scanf("%d", &opt);
	}
	close_tree(fp);
	return 0;
}
