#include <stdio.h>
#include <stdlib.h>

void draw_interface();

int main(int argc, char* argv[])
{
	
	draw_interface();
	return 0;
}

void draw_interface()
{
	//Test values
	int moves = 3;
	int num_moves = 10;
	char* seed = "0b4aaf345e";


	printf("############### Rush Hour\n");
	for (int i = 5; i > -1; i--)
	{
		printf("# ");
		for (int j = 0; j < 6; j++)
		{
			//Check if there's a car here, and if not :
			printf(".");
			//end if
			printf(" ");
		}
		
		switch(i){
			//let's check if we have to display more informations on the right:
			case 4:	//Move num display
				printf("# Move %d / %d\n", moves, num_moves);
				break;
			case 1:	//Level's seed display (optional)
				printf("# Seed: %s\n", seed);
				break;
			case 3: //Exit
				printf(">\n");
				break;
			default:
				printf("#\n");
				break;
		}
	}
	printf("###############\n");
	printf("Enter the car's number you want to move or 'help' :\n");
}
