#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void converting(char *size)
{
   int temp;
   temp = size[0];
   size[0] = size[3];  // convert littile endian to bigendian
   size[3] = temp;
   temp = size[1];
   size[1] = size[2];
   size[2] = temp;
}
    void print(char str[],int size)
    {
        int i=0;
        while(i<size-1)
        {
            printf("%c",str[i]);       
            i++;
        }
    }
    void tagg(char tag[], int size, char cont[])
    {
       char *str1[6] = {"TITLE", "ARTIST", "ALBUM", "YEAR", "GENRE", "COMPOSER"};       //intializing the pointer array to store all the addresss of  respective strings
       char *str2[6] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};
       for(int i=0;i<6;i++)
       {
         if(!(strcmp(tag,str2[i])))
         {
            printf("%s:  ",str1[i]);    //print tag tittles
            print(cont,size);
            printf("\n");
         }
       }
    }

    void  view(FILE *ptr)
  {
    fseek(ptr,10,SEEK_SET);    //skip 10 bytes
    for(int i=0;i<6;i++)
    {
        char tag[5];          
        fread(tag,1,4,ptr);    //read the tags from file
        tag[4]='\0';           
        int size;
        fread(&size,4,1,ptr);     //read the size from file
        converting((char *)&size);        // convert big endian to little endian
        fseek(ptr,3,SEEK_CUR);     // skip the 3 bytes.  2for flag,  1 for NULL
        char cont[size];           
        fread(cont,size-1,1,ptr);    //read the content from file
        tagg(tag,size,cont);       //first use for loop fior view print
    }
     fclose(ptr);
  }
  
void edit(FILE *str, char  *ncont, char ch)
{
    
    char *tags[6] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "TCOM"};    //intializing the pointer array to store all the addresss of  respective strings
   char flag[6] = {'t', 'a', 'A', 'y', 'g', 'c'};          //
    FILE *ptr1 = fopen("temp.mp3","w");
        if(ptr1==NULL)
        {
            printf("file not open\n");
            return ;
        }
        
    char skip[10];
    fread(skip,1,10,str);           
    fwrite(skip,1,10,ptr1);     //copy 10 bytes from old file to new file
    for(int i=0;i<6;i++)
    {
        char tag[5]={0};
        fread(tag,1,4,str);
        tag[4]='\0';
        int size;
        fread(&size,4,1,str);      //read the size
        converting((char *)&size);
        char flag1[3]; 
        fread(flag1,1,3,str);        //read 3 bytres 2flag and 1 null
        if(ch==flag[i])
        {
            int new_size;
            new_size= strlen(ncont)+1 ;   //+1 for null
            fwrite(tag,1,4,ptr1); 
            int *ptr= &new_size;       //copyint ptr data to ptr1
            converting((char *)ptr);
            fwrite(&new_size,4,1,ptr1);
            fwrite(flag,1,3,ptr1);
            fwrite(ncont,1,strlen(ncont),ptr1);
            fseek(str,size-1,SEEK_CUR);
        }
        else
        {
            fwrite(tag,1,4,ptr1);
            int Nsize=size;
            converting((char *)&Nsize);
            fwrite(&Nsize,4,1,ptr1);
            fwrite(tag,1,3,ptr1);
            char new_content[size+1];
            fread(new_content,size-1,1,str);
            fwrite(new_content,1,size-1,ptr1);

        }
    }
    char new;
    while((fread(&new,1,1,str))>0)
    {
        fwrite(&new,1,1,ptr1);
    }
    fclose(ptr1);
    fclose(str);
    remove("sample.mp3");
    rename("temp.mp3","sample.mp3");
    printf("edited successfully\n");
    FILE *ptr= fopen("sample.mp3","r");
    if(ptr==NULL)
    {
        printf("file not open");
        return ;
    }
    view(ptr);
}


