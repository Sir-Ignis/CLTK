/*
  Password Generator
	Version: 0.1.0
	Author: Sir-Ignis
*/

#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include "sodium.h"

/*modulos determine frequencies of
  the respective chars: letters,
  numbers and special characters.*/
struct password {
	int length;
	int complexity;
	int lttrModulo;
	int nmbrModulo;
	int spclModulo;
};

/*initialize the password with default settings*/
struct password initPwd(struct password pref) {
	pref.length = 15;
	pref.complexity = 3;
	pref.lttrModulo= 2;
	pref.nmbrModulo = 4;
	pref.spclModulo = 6;
	return pref;
}

/*clears the screen using regex*/
void clear() {
	for(int i = 0; i < 30; i++) {
		printf("\e[1;1H\e[2J");
	}
}

/*returns a random character*/
char getChar(int start, int end) {
    uint32_t myInt;

    myInt = start+randombytes_uniform(end-start);
    char c = myInt;
    return c;
}

/*returns a random lower/uppercase character*/
char *letters(char * pwd, int i) {
	if(randombytes_uniform(100)%2==0){
		pwd[i] = getChar(97,122);
	}else {
		pwd[i] = getChar(65,90);
	}
	return pwd;
}

/*returns a random special character*/
char * special(char * pwd, int i, int spclModulo, int nmbrModulo, int lttrModulo) {
	char sc[27] = {'!','$','%','^','&','*','(',')','-','_','=','+','{','}','[',']',';',':','\"','\\',',','<','>','/','\?','\"', '\0'};
	int r = randombytes_uniform(100);
	if (r%nmbrModulo==0) {
		pwd[i] = getChar(48,57);
	}else if (r%spclModulo==0) {
		r = randombytes_uniform(strlen(sc)-1); //-2 to stay in array boundaries
		pwd[i] = sc[r];
	}else {
		pwd = letters(pwd, i);
	}
	return pwd;
}

/*generates a password depending on preferences*/
char *genPassword(struct password pref) {
	if (sodium_init() < 0) {
				printf("FATAL ERROR! sodium.h couldn't be initialized!");
        return NULL;
    }
	char *pwd;
 	// Dynamically allocate memory using malloc()
    pwd = (char*)malloc(pref.length * sizeof(char));
	for(int i = 0; i < pref.length; i++) {
		switch(pref.complexity) {
			case 1: //letters only
				pwd = letters(pwd, i);
				break;
			case 2: //numbers+letters
				if(randombytes_uniform(100)%pref.nmbrModulo==0) {
					pwd[i] = getChar(48,57);
				}else {
					pwd = letters(pwd, i);
				}
				break;
			case 3: //numbers+letters+special characters
				special(pwd, i, pref.spclModulo, pref.nmbrModulo, pref.lttrModulo);
				break;

		default:
			pwd = letters(pwd,i);
			break;
		}
	}
	return pwd;
}

void man() {
	clear(); //clears the console using regex
	printf("\nMAN\n\n");
	printf("arg format: -l || --length <n> -c || --complexity <n1>\nvalid examples:\n"
			"[1]\n\n"

			"-l 20 -c 3\n\n"

			"this would generate a password of length 20\n"
			"consisting of a combination of letters, numbers\n"
			"and special characters\n\n"

			"[2]\n\n"

			"--length 30 --c 2\n\n"

			"this would generate a password of length 30\n"
			"consisting of a combination of letters and\n"
			"numbers\n"

			"[3]\n\n"

			"--length 50 -c 1\n\n"

			"this would generate a password of length 50\n"
			"conisting of only letters\n\n"
			);
	printf("You can mix short and long argument options\n"
			"Length ranges from 1-100 (inclusive)\n"
			"Complexity ranges from 1-3 (inclusive)\n\n"

			"-c 1 means only letters"
			"-c 2 means letters and numbers"
			"-c 3 means letters, numbers and special characters\n\n");
}

/* helps the user by giving them some advice*/
void help() {
	printf("Incorrect arguments! Try typing \"man\" for help\n");
	printf("Press any key to exit...");
	getchar();
}

/*checks if the five arguments
  entered match criteria if so
	return 1 else -1
*/
int fiveArgs(char *argv[] ) {
	if(strcmp(argv[1],"-l")==0 || strcmp(argv[1],"--length") == 0) {
		int arg2 = atoi(argv[2]);
		if(arg2 > 0 && arg2 < 101) {
			if(strcmp(argv[3],"-c") ==0 || strcmp(argv[3],"--complexity") == 0) {
				int arg4 = atoi(argv[4]);
				if(arg4 > 0 && arg4 < 4) {
					return 1;
				}
			}
		}
	}
	return -1;
}

/*checks if the nine arguments
  entered match criteria if so
	return 2 else -1
*/
int nineArgs(char *argv[] ) {
	if(fiveArgs(argv)) {
		if(strcmp(argv[5],"-f")==0 || strcmp(argv[5],"--freq")==0 ) {
			int ltrMod = atoi(argv[6]);
			int nmbrMod = atoi(argv[7]);
			int spclMod = atoi(argv[8]);
			if( (ltrMod >= 0 && ltrMod <= 100) &&
					(nmbrMod >= 0 && nmbrMod <= 100) &&
					(spclMod >= 0 && spclMod <= 100) ) {
						return 2;
			}
		}
	}
	return -1;
}

/*checks the arg input and
  successful calls inputHandler
  otherwise calls man() */
int argHandler(int argc, char *argv[] ) {
	switch(argc) {
		case 1:
			return 0;
		case 5:
			return fiveArgs(argv);
		case 9:
			return nineArgs(argv);
	}
	return -1;
}


/*checks user input in main returning
  number if successful else -1*/
int inputChecker() {
	char line[20];
	if (fgets(line, 20, stdin) != NULL) {
   		int number;
   		if ( sscanf(line, "%d", &number) == 1) {
   			if(number > 100) {
   				printf("The number you entered is too large!\nlength: ");
   				inputChecker();
   			}
     		return number;
   		}else {
      		return -1;
   		}
	}
	return -1;
}

/*used when 5 args entered*/
struct password prefCheck(char *argv[], struct password pref) {
	pref.length = atoi(argv[2]);
	pref.complexity = atoi(argv[4]);
	return pref;
}

/*assigns values to the preferences and returns them
  or calls exits(0) depending on the user's input*/
struct password inputHandler(int argc, char *argv[] ) {
	struct password pref = initPwd(pref);
	int check = argHandler(argc, argv);
	if (check == 1 || check == 2) {
		pref = prefCheck(argv, pref);
		if (check == 2){
			pref.lttrModulo = atoi(argv[6]);
			pref.nmbrModulo = atoi(argv[7]);
			pref.spclModulo = atoi(argv[8]);
		}
	}
	else {
		if(check != 0) {
			help();
			exit(0);
		}
	}
	return pref;
}

/*calls the input handler (IH) function
  and generates & prints a password based
	on the pref returned from IH*/
void mainHandler(int argc, char *argv[]) {
	password pref = inputHandler(argc, argv);
	char *pwd = (char*)malloc(pref.length+1);
	pwd = genPassword(pref);
	printf("Password: %s\n", pwd);
	free(pwd);
}

int main(int argc, char *argv[] ) {
	mainHandler(argc, argv);
	return 0;
}
