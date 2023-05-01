<!--
 * @Author: Cao Ning, Zeng XiangYu
 * @Date: 2023-02-02 01:13:37
 * @FilePath: README.md
 * @Assignment: HW4 Multiprocessing in C
 * @Course: CS5600 Spring 2023
-->
make clean
  description: clean the environment
make
  description: complie the code

Usage:
  ./main <input_file>
    example: ./main text.txt
    description: encode the text with multi threads and write to a file
  ./main <input_file> -d
    example: ./main text.txt -d
    description: for detailed output
  ./main -h
    example: ./main -h
    description: for help

Test case:
  text.txt: 32 sentences with ?,!,. ending
  blank.txt: no sentences in it

Project:
  main.c: Read sentences from a text file, use multi threads to encode and save in the file
  polybius.c: Cipher from HW2
  polybius.h: Cipher from HW2
  text.txt: text file
  blank.txt: text file
  Makefile
  README
