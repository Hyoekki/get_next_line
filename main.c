#include <stdio.h>
#include "get_next_line.h"

int main(){
	char *line;

	return (get_next_line(1)==NULL);
	
	while ((line = get_next_line(STDIN_FILENO)) != NULL){
		printf("%s", line);
		free(line);
	}
}
