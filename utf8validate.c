#include <stdio.h>

int main(){
	int ch , codepoint , countb1=0 , countb2=0, count=0 ;
	while((ch = getchar()) != EOF ){
		
		//Invalid header byte
		if ( ( (ch & 0xE0) != 0xC0 ) && ( (ch & 0xF0) != 0xE0 ) && ( (ch & 0xF8) != 0xF0 ) && ( (ch & 0x80) != 0) ){	
			printf("Invalid UTF-8 header byte: 0x%02X\n",ch );
			return 4;
		}

		
		//byte 1
		if ((ch & 0x80) == 0){			
			codepoint = ch;				
			countb1++;
		}
		else{
			

			// byte 2
			if((ch & 0xE0) == 0xC0){		// 0xE0 == einai mia maska opy kalhpth oloys toyw ariumoys mta apo aythn 
				codepoint = ch & 0x1F;		
				if((ch = getchar() ) == EOF){	
					printf("Unexpected EOF\n");
					return 1;
				}
				if ((ch & 0xC0)  != 0x80){		
					printf("Invalid tail byte 0x%02X\n",ch );
					return 2;
				}else{
					countb2++;
				}
				codepoint<<=6;			
				codepoint+=(ch & 0x3F);	
				if ( codepoint<=127){
					printf("oversize U+%04X\n",codepoint);
					return 5;
				}
			}
			
			
			//byte 3
			if((ch & 0xF0) == 0xE0){
				codepoint = ch & 0x0F;
				count=0;
				for(int i=1; i<=2; i++){
					if((ch = getchar() ) == EOF){	
						printf("Unexpected EOF\n");
						return 1;
					}
					if ((ch & 0xC0)  != 0x80){		
						printf("Invalid tail byte 0x%02X\n",ch );
						return 2;
					}else{
						count++;
					}	
					codepoint<<=6;	
					codepoint+=(ch & 0x3F);
				}	
				if( (count == 2) && ((codepoint<0xD800) || (codepoint>0xDFFF)) ) {
					countb2++;
				}
				else{
					printf("Invalid UTF-8 code point: U+%04X\n", codepoint);
					return 3;
				}
				if ( codepoint<=0x07ff){
					printf("oversize U+%04X\n",codepoint);
					return 5;
				}
			}
	
	
			// byte 4
			if((ch & 0xF8) == 0xF0){ 
				codepoint = ch & 0x07;
				count=0;
				for(int i=1; i<=3; i++){
					if((ch = getchar() ) == EOF){
						printf("Unexpected EOF\n");
						return 1;
					}
					if ((ch & 0xC0)  != 0x80){
						printf("Invalid tail byte 0x%02X\n",ch );
						return 2;
					}
					else{
						count++;
					}
					codepoint<<=6;			
					codepoint+=(ch & 0x3F);
				}
				if( (count==3) && (codepoint<0x10FFFF) ){
					countb2++;
				}
				else{
					printf("Invalid UTF-8 code point: U+%04X\n", codepoint);
					return 3;
				}
				if ( codepoint<=0xffff){
					printf("oversize U+%04X\n",codepoint);
					return 5;
				}
			}
		}
	}
	//printf("U+%04X\n",codepoint );
	printf("ASCII byte = %d multi byte = %d\n", countb1, countb2);
	return 0;
}