#include <stdio.h>

int main()
{
  int a = 1;
  int b = 2;
  int c = a + b;
  a = 12;
  b = a - c;//[COMMENT-AUDIO-test.cpp-2023_02_28_12_03_27][COMMENT-PIC-test.cpp-2023_03_01_15_56_23]
  printf("Please input a num:");//[COMMENT-AUDIO-test.cpp-2023_02_27_21_47_22]
  scanf("%d", &c);//[COMMENT-AUDIO-test.cpp-2023_02_27_21_31_48][COMMENT-AUDIO-test.cpp-2023_02_27_21_36_43]
  return 0; //[COMMENT-PIC-test.cpp-2023_02_27_20_01_58][COMMENT-PIC-test.cpp-2023_02_27_22_21_53][COMMENT-PIC-test.cpp-2023_02_27_22_22_17][COMMENT-PIC-test.cpp-2023_02_27_22_27_56]
}