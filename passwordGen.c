/*
			
	Password Generator
	Version: 0.2.0		
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

/*MANUAL - prints out program synopsis and gives examples of how
  to run the program
  escape codes: 
  "\e[1mbold\e[0m"
  "\e[4munderline\e[0m"*/
void man() {
	clear(); 
	printf("\e[1m\e[4mMAN\e[0m\e[0m\n\n");

	printf("Password Generator generates random passwords using libsodium.\n");

	printf("\n\e[1mSYNOPSIS\e[0m:\n\n"
			"[ \e[1m-l\e[0m ] [ \e[1m-c\e[0m ] [ \e[1m-f\e[0m ] [ \e[1mx y z\e[0m ]\n\n");

	printf("\e[1mEXAMPLES\e[0m:\n\n"
			"Example [0]\n\n"

			"-l 20 -c 3\n\n"

			"this would generate a password of length 20\n"
			"consisting of a combination of letters, numbers\n"
			"and special characters\n\n\n"

			"Example [1]\n\n"

			"--length 30 --c 2\n\n"

			"this would generate a password of length 30\n"
			"consisting of a combination of letters and\n"
			"numbers\n\n\n");

	printf("Press any key to continue");
	getchar();
	printf("\033[1A%c[2K", 27); //move cursor up one line and clear that line
	printf("Example [2]\n\n");

	printf("--length 50 -c 1\n\n"

			"this would generate a password of length 50\n"
			"consisting of only letters\n\n\n"

			"Example [3]\n\n"

			"-l 100 -c 3 -f 3 4 6\n\n"

			"this would generate a password of length 100\n"
			"consisting letters, numbers and symbols;\n"
			"the frequency of the characters is determined\n"
			"by the numbers x, y and z; where the frequency\n"
			"of the letters/numbers/special characters is\n"
			"determined by checking if first y is factor of\n"
			"the randomly generated number, then if z, else\n"
			"a letter is used as the character.\n\n\n"			
			);
	printf("\e[1mADDITIONAL INFO\e[0m:\n\nYou can mix short and long argument options\n"
			"Length ranges from 1-100 (inclusive)\n"
			"Complexity ranges from 1-3 (inclusive)\n\n"

			"-c 1 means only letters\n"
			"-c 2 means letters and numbers\n"
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
  return 1 else -1
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
						return 1;
			}
		}
	}
	return -1;
}

/*checks if user entered man || --man || MAN || -man
  and calls man if true else does nothing*/
void getMan(char *argv[]) {
	if(strcmp(argv[1],"man")==0 || strcmp(argv[1],"--man")==0 || 
	   strcmp(argv[1],"MAN")==0 || strcmp(argv[1],"-man")==0) {
		man();
	}
}


/*checks the arg input and
  successful calls inputHandler
  otherwise calls man() */
int argHandler(int argc, char *argv[] ) {
	switch(argc) {
		case 1:
			return 0;
		case 2:
			getMan(argv);
			return 1;
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
	if (check == 1) {
		if (argc == 5) {
			pref = prefCheck(argv, pref);
		} else if (argc == 9) {
			pref.lttrModulo = atoi(argv[6]);
			pref.nmbrModulo = atoi(argv[7]);
			pref.spclModulo = atoi(argv[8]);
		} else {
			getMan(argv);
		}
	}else {
			help();
			exit(0);
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