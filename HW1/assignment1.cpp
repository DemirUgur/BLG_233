    #include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>

using namespace std;
//Structure for holding the instructions
struct instructions {
	string direction;
	int rowX;
	int colY;
	int moveBy;
};
//Structure for holding the operator bofore and while placing them on grid
struct operators {
	char type;
	int x;
	int y;
	int size;
};
//Structure to hold operators after being placed on grid
struct placedOperators {
	int size;
	int existent;
	int Centerx;
	int Centery;
	char type;

	void place(placedOperators&, int, int);//Function to place operators on a placedOperators structure 1D; 2D interpreted array.
	void initialize(placedOperators*, int);//Function to innitialize structure grid to 0 values.
	void emptyout(placedOperators&);
};

void getOperant(FILE*, int*, int , int, placedOperators*);
void placeOnGrid(operators, int*, int , int, placedOperators*);
void placePlus(operators, int*, int , int, placedOperators*);
void placeMinus(operators , int *, int , int, placedOperators*);
void placeX(operators , int *, int , int, placedOperators*);
void placeSlash(operators , int *, int , int, placedOperators*);
void moveOperatorRight(placedOperators*,instructions, int, int);
void moveOperatorUp(placedOperators*, instructions,int, int);
void moveOperatorDown(placedOperators*, instructions, int, int);
void moveOperatorLeft(placedOperators*, instructions, int, int);
void initialize(FILE*, int&, int&);
void getInstructions(placedOperators*, instructions, int,int, char*);

int main(int argc, char *argv[]) {
	instructions instruct;
	placedOperators *GridPtr;
	FILE *file1 = fopen(argv[1], "r");//grid and operant spot file
	if (!file1) {
		cout << "An error has occured.";
		return EXIT_FAILURE;
	}
	int x, y;

	initialize(file1,x,y); // Reads the size of the Grid
	//Grid is innitialized as a 1D array but interpreted as a 2D array.
	int size = x * y;
	int *ptr = new int[size];
	for (int i = 0; i < x*y; i++) {
		*(ptr + i) = 0;
	}
	GridPtr = new placedOperators[size]; 
	GridPtr->initialize(GridPtr, size); //Structure array is innitilialized.
	cout << "A grid is created: " << x << " " << y << endl;

	getOperant(file1, ptr, x, y, GridPtr); 

	getInstructions(GridPtr, instruct, x, y, argv[2]);

	return 0;
}
void placedOperators::emptyout(placedOperators& a){
	//deletes all the values on a given structure in structure grid
    a.Centerx = 0;
    a.Centery = 0;
    a.existent = 0;
    a.size = 0;
    a.type = 0;
   return;
}


void conflictErrorMassage(int size, int row, int col,char type) {
	printf("CONFLICT ERROR: Operator %c with size %d can not be placed on (%d,%d).\n",type,size,row,col);
	//cout << "CONFLICT ERROR: Operator " << type << " with size " << size << " can not be placed on (" << row << "," << col << ")." << endl;
}
void MoveErrorMassage(char type, int row, int col, int moveby, int direction, int ErrorType){
		if(ErrorType==1){//border error
			
			if(direction == 0){ //Move right
				printf("BORDER ERROR: %c can not be moved from (%d,%d) to (%d,%d).\n\r",type,row,col,row,col+moveby);
			}
			else if (direction == 1) { // move left
				printf("BORDER ERROR: %c can not be moved from (%d,%d) to (%d,%d).\n\r",type,row,col,row,col-moveby);
			}
			else if (direction == 2) {//move up
				printf("BORDER ERROR: %c can not be moved from (%d,%d) to (%d,%d).\n\r",type,row,col,row-moveby,col);
			}
			else { //move down
				printf("BORDER ERROR: %c can not be moved from (%d,%d) to (%d,%d).\n\r",type,row,col,row+moveby,col);
			}
		}else{ //Conflict Error
			if(direction == 0){ //Move right
				printf("CONFLICT ERROR: %c can not be moved from (%d,%d) to (%d,%d).\n\r",type,row,col,row,col+moveby);
			}
			else if (direction == 1) { // move left
				printf("CONFLICT ERROR: %c can not be moved from (%d,%d) to (%d,%d).\n\r",type,row,col,row,col-moveby);
			}
			else if (direction == 2) {//move up
				printf("CONFLICT ERROR: %c can not be moved from (%d,%d) to (%d,%d).\n\r",type,row,col,row-moveby,col);
			}
			else { //move down
				printf("CONFLICT ERROR: %c can not be moved from (%d,%d) to (%d,%d).\n\r",type,row,col,row+moveby,col);
			}
	}
}
void MoveSuccessMessage(char type, int row, int col, int moveby, int direction) {
	
	if(direction == 0){ //Move right
		printf("SUCCESS: %c moved from (%d,%d) to (%d,%d).\n\r",type,row,col,row,col+moveby);
	}
	else if (direction == 1) { // move left
		printf("SUCCESS: %c moved from (%d,%d) to (%d,%d).\n\r",type,row,col,row,col-moveby);
	}
	else if (direction == 2) {//move up
		printf("SUCCESS: %c moved from (%d,%d) to (%d,%d).\n\r",type,row,col,row-moveby,col);
	}
	else { //move down
		printf("SUCCESS: %c moved from (%d,%d) to (%d,%d).\n\r",type,row,col,row+moveby,col);
	}
}
void PlaceSuccessMessage(char type,int size ,int row, int col){
	printf("SUCCESS: Operator %c with size %d is placed on (%d,%d).\n",type,size,row,col); 
	}


void placedOperators::initialize(placedOperators* ptr,int size) {
	// innitializes the whole grid
	int i;
	for (i = 0; i < size; i++) {
		(ptr + i)->Centerx = 0;
		(ptr + i)->Centery = 0;
		(ptr + i)->existent = 0;
	}

	return;
}
void place(placedOperators& boi, operators placed) {
	boi.Centerx = placed.x;
	boi.Centery = placed.y;
	boi.existent = 1;
	boi.size = placed.size;
	boi.type = placed.type;
	return;
}
void initialize(FILE* fptr, int &xSize, int &ySize) { //reads size for grid
	fscanf(fptr, "%d %d\n", &xSize, &ySize);
	return;
};

void getOperant(FILE*fptr, int *ptr, int x, int y, placedOperators* GridPtr){//reads the operants to be placed
	operators operant;
	while (fscanf(fptr, "%c%d%d%d\n", &operant.type,&operant.x,&operant.y,&operant.size ) != EOF) {
		placeOnGrid(operant, ptr, x,  y, GridPtr);
	}
	fclose(fptr);
	return;
};

void placeOnGrid(operators operant,int *ptr,int x, int y, placedOperators* GridPtr) { //checks for type of operant
	switch (operant.type) {
	case '+':
		placePlus(operant,ptr , x, y, GridPtr);
		break;
	case '-':
		placeMinus(operant, ptr, x, y, GridPtr);
		break;
	case 'x':
		placeX(operant, ptr, x, y, GridPtr);
		break;
	case '/':
		placeSlash(operant, ptr, x, y, GridPtr);
		break;
	};
	return;
};

void placePlus(operators operant, int *ptr, int x, int y, placedOperators* GridPtr) {
	bool border = false;
	if (operant.x + operant.size > x || operant.y + operant.size > y || operant.y - operant.size < 1 || operant.x - operant.size < 1) {//checks for border collision
		cout << "BORDER ERROR:Operator + with size " << operant.size << " can not be placed on (" << operant.x << "," << operant.y << ")."<< endl;
		border = true ;
	}

	int i = 1;
	//checks for conflict collision
	if (*(ptr + (operant.x - 1) * y + operant.y - 1) == 1) { conflictErrorMassage(operant.size,operant.x,operant.y,operant.type); return ; }
	while (i <= operant.size){
		if (*(ptr + (operant.x - 1) * y + operant.y - 1 - i) == 1) { conflictErrorMassage(operant.size, operant.x, operant.y, operant.type); return ; }
		if (*(ptr + (operant.x - 1) * y + operant.y - 1 + i) == 1) { conflictErrorMassage(operant.size, operant.x, operant.y, operant.type); return ; }
		if (*(ptr + (operant.x - 1 - i) * y + operant.y - 1) == 1) { conflictErrorMassage(operant.size, operant.x, operant.y, operant.type); return ; }
		if (*(ptr + (operant.x - 1 + i) * y + operant.y - 1) == 1) { conflictErrorMassage(operant.size, operant.x, operant.y, operant.type); return ; }
		i++;
	}
	if (border) { return; }
	//marking the wanted spots on the grid
	i = 1;
	*(ptr + (operant.x - 1) * y + operant.y - 1) = 1;//center
	place(GridPtr[(operant.x - 1) * y + operant.y - 1], operant);
	while (i <= operant.size) {
		*(ptr + (operant.x - 1) * y + operant.y - 1 - i) = 1;
		*(ptr + (operant.x - 1) * y + operant.y - 1 + i) = 1;
		*(ptr + (operant.x - 1 - i) * y + operant.y - 1) = 1;
		*(ptr + (operant.x - 1 + i) * y + operant.y - 1) = 1;
		place(GridPtr[(operant.x - 1) * y + operant.y - 1 - i], operant);
		place(GridPtr[(operant.x - 1) * y + operant.y - 1 + i], operant);
		place(GridPtr[(operant.x - 1 - i) * y + operant.y - 1], operant);
		place(GridPtr[(operant.x - 1 + i) * y + operant.y - 1], operant);
		i++;
	}
	PlaceSuccessMessage(operant.type,operant.size,operant.x,operant.y);
	//cout << "SUCCESS: Operator + with size " << operant.size <<" is placed on(" << operant.x << "," << operant.y << ")." << endl;
	return ;
};

void placeMinus(operators operant, int *ptr, int x, int y, placedOperators* GridPtr) {
	bool border = false;
	if(operant.y + operant.size > y || operant.y - operant.size < 1){
		cout << "BORDER ERROR: Operator - with size " << operant.size << " can not be placed on (" << operant.x << "," << operant.y << ")." << endl;
		border = true;
	}

	int i = 0;
	if (*(ptr + (operant.x - 1) * y + operant.y - 1) == 1) { conflictErrorMassage(operant.size, operant.x, operant.y, operant.type); return ; }
	while (i <= operant.size) {
		if (*(ptr + (operant.x - 1) * y + operant.y - 1 - i) == 1) { conflictErrorMassage(operant.size, operant.x, operant.y, operant.type); return ; }
		if (*(ptr + (operant.x - 1) * y + operant.y - 1 + i) == 1) { conflictErrorMassage(operant.size,operant.x,operant.y,operant.type); return ; }
		i++;
	}
	if (border) { return; }
	i = 0;
	*(ptr + (operant.x - 1) * y + operant.y - 1) = 1;//center
	place(GridPtr[(operant.x - 1) * y + operant.y - 1], operant);
	while (i <= operant.size) {
		*(ptr + (operant.x - 1) * y + operant.y - 1 - i) = 1;
		*(ptr + (operant.x - 1) * y + operant.y - 1 + i) = 1;
		place(GridPtr[(operant.x - 1) * y + operant.y - 1 - i], operant);
		place(GridPtr[(operant.x - 1) * y + operant.y - 1 + i], operant);
		i++;
	}
	PlaceSuccessMessage(operant.type,operant.size,operant.x,operant.y);
	//cout << "SUCCESS: Operator - with size " << operant.size << " is placed on(" << operant.x << "," << operant.y << ")." << endl;
	return;
};

void placeX(operators operant, int *ptr, int x, int y, placedOperators* GridPtr) {
	bool border = false;
	if (operant.y + operant.size > y || operant.y - operant.size < 1 || operant.x + operant.size > x || operant.x - operant.size < 1) {
		cout << "BORDER ERROR: Operator x with size "<< operant.size <<" can not be placed on (" << operant.x << "," << operant.y << ")." << endl;
		border = true ;
	}

	int i = 1;
	if (*(ptr + (operant.x - 1) * y + operant.y - 1) == 1) { conflictErrorMassage(operant.size,operant.x,operant.y,operant.type); return ; }//center
	while (i <= operant.size) {
		if (*(ptr + (operant.x - 1 + i) * y + operant.y - 1 + i) == 1) { conflictErrorMassage(operant.size,operant.x,operant.y,operant.type); return ; }
		if (*(ptr + (operant.x - 1 + i) * y + operant.y - 1 - i) == 1) { conflictErrorMassage(operant.size,operant.x,operant.y,operant.type); return ; }
		if (*(ptr + (operant.x - 1 - i) * y + operant.y - 1 + i) == 1) { conflictErrorMassage(operant.size,operant.x,operant.y,operant.type); return ; }
		if (*(ptr + (operant.x - 1 - i) * y + operant.y - 1 - i) == 1) { conflictErrorMassage(operant.size,operant.x,operant.y,operant.type); return ; }
		i++;
	}
	if (border) { return; }
	i = 1;
	*(ptr + (operant.x - 1) * y + operant.y - 1) = 1;//center
	place(GridPtr[(operant.x - 1) * y + operant.y - 1], operant);
	while (i <= operant.size) {
		*(ptr + (operant.x - 1 + i) * y + operant.y - 1 + i) = 1;
		*(ptr + (operant.x - 1 + i) * y + operant.y - 1 - i) = 1;
		*(ptr + (operant.x - 1 - i) * y + operant.y - 1 + i) = 1;
		*(ptr + (operant.x - 1 - i) * y + operant.y - 1 - i) = 1;
		place(GridPtr[(operant.x - 1 + i) * y + operant.y - 1 + i], operant);
		place(GridPtr[(operant.x - 1 + i) * y + operant.y - 1 - i], operant);
		place(GridPtr[(operant.x - 1 - i) * y + operant.y - 1 + i], operant);
		place(GridPtr[(operant.x - 1 - i) * y + operant.y - 1 - i], operant);
		i++;
	}
	PlaceSuccessMessage(operant.type,operant.size,operant.x,operant.y);
	//cout << "SUCCESS: Operator x with size " << operant.size << " is placed on(" << operant.x << "," << operant.y << ")." << endl;
	return ;
};

void placeSlash(operators operant, int *ptr, int x, int y, placedOperators* GridPtr) {
	bool border = false;
	if (operant.x + operant.size > x || operant.x - operant.size < 1 || operant.y + operant.size > y || operant.y + operant.size < 1) {
		cout << "BORDER ERROR: Operator / with size " << operant.size << " can not be placed on (" << operant.x << "," << operant.y << ")." << endl;
		border = true ;
	}
	int i = 1;
	if (*(ptr + (operant.x - 1) * y + operant.y - 1) == 1) { conflictErrorMassage(operant.size,operant.x,operant.y,operant.type); return ; }//center
	while (i <= operant.size) {
		if (*(ptr + (operant.x - 1 + i) * y + operant.y - 1 - i) == 1) { conflictErrorMassage(operant.size,operant.x,operant.y,operant.type); return ; }
		if (*(ptr + (operant.x - 1 - i) * y + operant.y - 1 + i) == 1) { conflictErrorMassage(operant.size,operant.x,operant.y,operant.type); return ; }
		i++;
	}
	if (border) { return; }
	i = 1;
	*(ptr + (operant.x - 1) * y + operant.y - 1) = 1;//center
	place(GridPtr[(operant.x - 1) * y + operant.y - 1], operant);
	while (i <= operant.size) {
		*(ptr + (operant.x - 1 + i) * y + operant.y - 1 - i) = 1;
		*(ptr + (operant.x - 1 - i) * y + operant.y - 1 + i) = 1;
		place(GridPtr[(operant.x - 1 + i) * y + operant.y - 1 - i], operant);
		place(GridPtr[(operant.x - 1 - i) * y + operant.y - 1 + i], operant);
		i++;
	}
	PlaceSuccessMessage(operant.type,operant.size,operant.x,operant.y);
	//cout << "SUCCESS: Operator / with size " << operant.size << " is placed on(" << operant.x << "," << operant.y << ")." << endl;
	return ;
};

void getInstructions(placedOperators* ptr,instructions instruct,int x ,int y, char* filename) {//reads instructions
	ifstream fptr;
	fptr.open(filename);

	while(fptr >> instruct.direction >> instruct.rowX >> instruct.colY >> instruct.moveBy){

		if (!(instruct.direction.compare("MVR"))) {
			moveOperatorRight(ptr, instruct, x, y);
		}
		else if (!(instruct.direction.compare("MVU"))) {
			moveOperatorUp(ptr, instruct, x, y);
		}
		else if (!(instruct.direction.compare("MVD"))) {
			moveOperatorDown(ptr, instruct, x, y);
		}
		else if (!(instruct.direction.compare("MVL"))) {
			moveOperatorLeft(ptr, instruct, x, y);
		}
	}
	fptr.close();
}

void moveOperatorRight(placedOperators* ptr, instructions instruct, int GridSizeX, int GridSizeY) {

	int x = ptr[(instruct.rowX - 1)*GridSizeY + instruct.colY - 1].Centerx;
	int y = ptr[(instruct.rowX - 1)*GridSizeY + instruct.colY - 1].Centery;
	//find the center address for pointer arithmetic//
	int centerAddress = (x - 1)*GridSizeY + y - 1;
	
	//Assign neccecary values for error checking
	bool border = false;
	bool conflict = false;
	//int size = ptr[centerAddress].size;
	char type = ptr[centerAddress].type;
	int row = ptr[centerAddress].Centerx;
	int col = ptr[centerAddress].Centery;
	//Create a copy of former grid without the said operator//
	placedOperators* newGrid = new placedOperators[GridSizeX * GridSizeY];
	for (int i = 0; i < GridSizeX * GridSizeY; i++) {
		if (ptr[i].Centerx == x && ptr[i].Centery == y) {
            newGrid[i].emptyout(newGrid[i]);
		}
		else { newGrid[i] = ptr[i]; }
	}
	//check for errors
	for (int k = 0; k < GridSizeX; k++) {
		for (int l = 0; l < GridSizeY; l++) {
			if (ptr[k*GridSizeY + l].Centerx == x && ptr[k*GridSizeY + l].Centery == y) {
				if (l + 1 + instruct.moveBy > GridSizeY) { border = true; }
				if (newGrid[k*GridSizeY + l + instruct.moveBy].existent == 1) { conflict = true; }
			}
		}
	}
	if(border && conflict){
		MoveErrorMassage(type,row,col,instruct.moveBy,0,1);
		MoveErrorMassage(type,row,col,instruct.moveBy,0,0);
		return;
	}else if(border){
		MoveErrorMassage(type,row,col,instruct.moveBy,0,1);
		return;
	}else if(conflict){
			MoveErrorMassage(type,row,col,instruct.moveBy,0,0);
			return;
	}

	//if no errors move the operator to said value
	for (int k = 0; k < GridSizeX; k++) {
		for (int l = 0; l < GridSizeY; l++) {
			if (ptr[k*GridSizeY + l].Centerx == x && ptr[k*GridSizeY + l].Centery == y) {
				newGrid[k*GridSizeY + l + instruct.moveBy] = ptr[centerAddress];
				newGrid[k*GridSizeY + l + instruct.moveBy].Centery = ptr[centerAddress].Centery + instruct.moveBy;
			}
		}
	}

	MoveSuccessMessage(type, row, col, instruct.moveBy,0);
	//change the original grid to anew.
	for (int i = 0; i < GridSizeX * GridSizeY; i++) {
		ptr[i] = newGrid[i];
	}

	delete[] newGrid;//cleanup
}
void moveOperatorLeft(placedOperators* ptr, instructions instruct, int GridSizeX, int GridSizeY){

	int x = ptr[(instruct.rowX - 1)*GridSizeY + instruct.colY - 1].Centerx;
	int y = ptr[(instruct.rowX - 1)*GridSizeY + instruct.colY - 1].Centery;
	//find the center address for pointer arithmetic//
	int centerAddress = (x - 1)*GridSizeY + y - 1;
	//Assign neccecary values for error checking
	bool border = false;
	bool conflict = false;
	//int size = ptr[centerAddress].size;
	char type = ptr[centerAddress].type;
	int row = ptr[centerAddress].Centerx;
	int col = ptr[centerAddress].Centery;
	//Create a copy of former grid without the said operator//
	placedOperators* newGrid = new placedOperators[GridSizeX * GridSizeY];
	for (int i = 0; i < GridSizeX * GridSizeY; i++) {
		if (ptr[i].Centerx == x && ptr[i].Centery == y) {
            newGrid[i].emptyout(newGrid[i]);
		}
		else { newGrid[i] = ptr[i]; }
	}

	for (int k = 0; k < GridSizeX; k++) {
		for (int l = 0; l < GridSizeY; l++) {
			if (ptr[k*GridSizeY + l].Centerx == x && ptr[k*GridSizeY + l].Centery == y) {
				if (l + 1 - instruct.moveBy < 1) { border = true; }
				if (newGrid[k*GridSizeY + l - instruct.moveBy].existent == 1) { conflict = true; }
			}
		}
	}
	if(border && conflict){
		MoveErrorMassage(type,row,col,instruct.moveBy,1,1);
		MoveErrorMassage(type,row,col,instruct.moveBy,1,0);
		return;
	}else if(border){
		MoveErrorMassage(type,row,col,instruct.moveBy,1,1);
		return;
	}else if(conflict){
			MoveErrorMassage(type,row,col,instruct.moveBy,1,0);
			return;
	}
	for (int k = 0; k < GridSizeX; k++) {
		for (int l = 0; l < GridSizeY; l++) {
			if (ptr[k*GridSizeY + l].Centerx == x && ptr[k*GridSizeY + l].Centery == y) {
				newGrid[k * GridSizeY + l - instruct.moveBy] = ptr[centerAddress];
				newGrid[k * GridSizeY + l - instruct.moveBy].Centery = ptr[centerAddress].Centery - instruct.moveBy;

			}
		}
	}

    MoveSuccessMessage(type, row, col, instruct.moveBy,1);

	for (int i = 0; i < GridSizeX * GridSizeY; i++) {
		ptr[i] = newGrid[i];
	}
	delete[] newGrid;
}
void moveOperatorUp(placedOperators* ptr, instructions instruct, int GridSizeX, int GridSizeY) {
	int x = ptr[(instruct.rowX - 1)*GridSizeY + instruct.colY - 1].Centerx;
	int y = ptr[(instruct.rowX - 1)*GridSizeY + instruct.colY - 1].Centery;

	//find the center address for pointer arithmetic//
	int centerAddress = (x - 1)*GridSizeY + y - 1;
	//Assign neccecary values for error checking
	bool border = false;
	bool conflict = false;
    //int size = ptr[centerAddress].size;
	char type = ptr[centerAddress].type;
	int row = ptr[centerAddress].Centerx;
	int col = ptr[centerAddress].Centery;
	//Create a copy of former grid without the said operator//
	placedOperators* newGrid = new placedOperators[GridSizeX * GridSizeY];
	for (int i = 0; i < GridSizeX * GridSizeY; i++) {
		if (ptr[i].Centerx == x && ptr[i].Centery == y) {
            newGrid[i].emptyout(newGrid[i]);
		}
		else { newGrid[i] = ptr[i]; }
	}

	for (int k = 0; k < GridSizeX; k++) {
		for (int l = 0; l < GridSizeY; l++) {
			if (ptr[k*GridSizeY + l].Centerx == x && ptr[k*GridSizeY + l].Centery == y) {
				if (k + 1 - instruct.moveBy < 1) { border = true; }
				if (newGrid[(k - instruct.moveBy)*GridSizeY + l].existent == 1) { conflict = true; }
			}
		}
	}
	if(border && conflict){
		MoveErrorMassage(type,row,col,instruct.moveBy,2,1);
		MoveErrorMassage(type,row,col,instruct.moveBy,2,0);
		return;
	}else if(border){
		MoveErrorMassage(type,row,col,instruct.moveBy,2,1);
		return;
	}else if(conflict){
			MoveErrorMassage(type,row,col,instruct.moveBy,2,0);
			return;
	}
	

	for (int k = 0; k < GridSizeX; k++) {
		for (int l = 0; l < GridSizeY; l++) {
			if (ptr[k*GridSizeY + l].Centerx == x && ptr[k*GridSizeY + l].Centery == y) {
				newGrid[(k - instruct.moveBy)*GridSizeY + l] = ptr[centerAddress];
				newGrid[(k - instruct.moveBy)*GridSizeY + l].Centery = ptr[centerAddress].Centerx - instruct.moveBy;
			}
		}
	}
	
	MoveSuccessMessage(type, row, col, instruct.moveBy,2);
	for (int i = 0; i < GridSizeX * GridSizeY; i++) {
		ptr[i] = newGrid[i];
	}
	delete[] newGrid;
}
void moveOperatorDown(placedOperators* ptr, instructions instruct, int GridSizeX, int GridSizeY){
	int x = ptr[(instruct.rowX - 1)*GridSizeY + instruct.colY - 1].Centerx;
	int y = ptr[(instruct.rowX - 1)*GridSizeY + instruct.colY - 1].Centery;
	//find the center address for pointer arithmetic//
	int centerAddress = (x - 1)*GridSizeY + y - 1;
	//int size = ptr[centerAddress].size;
	//Assign neccecary values for error checking
	bool border = false;
	bool conflict = false;
	char type = ptr[centerAddress].type;
	int row = ptr[centerAddress].Centerx;
	int col = ptr[centerAddress].Centery;
	//Create a copy of former grid without the said operator//
	placedOperators* newGrid = new placedOperators[GridSizeX * GridSizeY];
	for (int i = 0; i < GridSizeX * GridSizeY; i++) {
		if (ptr[i].Centerx == x && ptr[i].Centery == y) {
            newGrid[i].emptyout(newGrid[i]);
		}
		else { newGrid[i] = ptr[i]; }
	}

	for (int k = 0; k < GridSizeX; k++) {
		for (int l = 0; l < GridSizeY; l++) {
			if (ptr[k*GridSizeY + l].Centerx == x && ptr[k*GridSizeY + l].Centery == y) {
				if (k + 1 + instruct.moveBy > GridSizeX) { border = true; }
				if (newGrid[(k + instruct.moveBy)*GridSizeY + l].existent == 1) { conflict = true; }
			}
		}
	}
	if(border && conflict){
		MoveErrorMassage(type,row,col,instruct.moveBy,3,1);
		MoveErrorMassage(type,row,col,instruct.moveBy,3,0);
		return;
	}else if(border){
		MoveErrorMassage(type,row,col,instruct.moveBy,3,1);
		return;
	}else if(conflict){
			MoveErrorMassage(type,row,col,instruct.moveBy,3,0);
			return;
	}
	

	for (int k = 0; k < GridSizeX; k++) {
		for (int l = 0; l < GridSizeY; l++) {
			if (ptr[k*GridSizeY + l].Centerx == x && ptr[k*GridSizeY + l].Centery == y) {
				newGrid[(k + instruct.moveBy)*GridSizeY + l] = ptr[centerAddress];
				newGrid[(k + instruct.moveBy)*GridSizeY + l].Centery = ptr[centerAddress].Centerx + instruct.moveBy;
			}
		}
	}
	
	MoveSuccessMessage(type, row, col, instruct.moveBy,3);
	for (int i = 0; i < GridSizeX * GridSizeY; i++) {
		ptr[i] = newGrid[i];
	}

	delete [] newGrid;
}
