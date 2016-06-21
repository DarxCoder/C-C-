#include <stdio.h>
#include <string.h>

int main() {

  int i;
  char chaine[1000];
  char p = 37;
  printf("Sasir la chaine a encoder : ");
  scanf("%s", chaine);
  for (i=0;i<strlen(chaine);i++) {
    printf("%c%x",p,chaine[i]);
  }
  printf("\n\n");
  return 0;
}
