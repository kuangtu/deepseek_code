#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#pragma pack(push, 1)

typedef struct {
	uint32_t u32key;
}Array3;


typedef struct {
	uint16_t u16Num;
	Array3 a3[];
}Array2;


typedef struct {
	uint32_t BodyLength;
	uint32_t MsgType;
	Array2 a2[];
}Array1;

#pragma pack(pop)

int main()
{
	
	printf("the sizeof array3 is:%zu\n", sizeof(Array3));
	printf("the sizeof array2 is:%zu\n", sizeof(Array2));
	printf("the sizeof array1 is:%zu\n", sizeof(Array1));
	printf("offsetof(Array2, a3)=%zu\n", offsetof(Array2, a3)); 
	printf("offsetof(Array2, a3)=%zu\n", offsetof(Array2, u16Num)); 
	
	return 0;
	
}

