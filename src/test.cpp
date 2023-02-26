#include <stdio.h>

int main()
{
  int a = 1;
  int b = 2;
  int c = a + b;
  a = 12;
  b = a - c;
  printf("Please input a num:");
  scanf("%d", &c);
  return 0;
}