#include "disk_img_manager.h"
#include "disk_folder_manager.h"
#include "../errno.h"

int main(int argc, char **argv)
{
    
    // load_image_file("imgs/winb98se.IMA");
    
    if (argv[1] == NULL) 
    {
        printf("\nError: Please enter the image filepath.\n\n");
        return 0;
    }
    
    
    if(load_image_file(argv[1]) == E_FILE_NF)
    {
        printf("\nError: Could not find file %s.\n\n", argv[1]);
        return 0;
    }
    
    if (argv[2])
    {
        printf("\nTODO: THUNDERKRAKENS - Implement me!\n\n");
    }
    
    folder_manager_init();
    
    print_curr_dir_entry_list();
    list_curr_file_and_dir();
    char * command_str;
    size_t command_str_len = 0;
    int is_run = 1;
    while(is_run)
    {
        print_curr_path();
        printf("> ");
        getline(&command_str, &command_str_len, stdin);
        int i = 0;
        for(i = 0; i < command_str_len; i++)
        {
            if(isspace(command_str[i]) && command_str[i] != ' ')
            {
                command_str[i] = '\0';
            }
        }
        
        if(!strcmp(command_str, "exit"))
        {
            is_run = 0;
        }
        
    }
    
    free(command_str);
    command_str = NULL;
    clean_buffers();
    return 0;
}

#if 0
------------------------------
cs450_2.img 
~ ls
1984.TXT SUBDIR   WVU.JPG

~ ls -a
.          .Trashes   .fseventsd SUBDIR
..         ._.Trashes 1984.TXT   WVU.JPG

~ cd SUBDIR
~ ls
ANDREW.TXT WOW.JPG

~ ls -a
.          ..         ANDREW.TXT WOW.JPG

-------------------------------
450_3.img
~ ls
CS111     EX14.C    EX17.C    EX7.C     FELEEX6.C WREATHS

~ ls -a
.          .Trashes   .fseventsd EX14.C     EX7.C      WREATHS
..         ._.Trashes CS111      EX17.C     FELEEX6.C

~ cd CS111
~ ls 
BON1.DOC     CRUISES.TXT  HMK3.DOC     MAZE2.TXT    READMAZE.TXT TESTFILE.DAT
BTREES.HTM   EMPLOYEE.DAT HOURLY.DAT   POLY.RTF     REV1.DOC     TMAZE1.TXT
CODE.RTF     EXAMPLE1.TXT INSTDES.TXT  PRO2SUM2.TXT SLINLST.JAV  TMAZE2.TXT
CODING.DOC   GCD.TXT      INTNOD~1.JAV PROJ5.DAT    STKSPEC.DOC
COMM.DAT     HMK1.DOC     MAZE1.TXT    QUESPEC.DOC  STUDENT.TXT

~ ls -a
.            CODING.DOC   GCD.TXT      INTNOD~1.JAV PROJ5.DAT    STKSPEC.DOC
..           COMM.DAT     HMK1.DOC     MAZE1.TXT    QUESPEC.DOC  STUDENT.TXT
BON1.DOC     CRUISES.TXT  HMK3.DOC     MAZE2.TXT    READMAZE.TXT TESTFILE.DAT
BTREES.HTM   EMPLOYEE.DAT HOURLY.DAT   POLY.RTF     REV1.DOC     TMAZE1.TXT
CODE.RTF     EXAMPLE1.TXT INSTDES.TXT  PRO2SUM2.TXT SLINLST.JAV  TMAZE2.TXT

~ cd ../WREATHS
~ ls
EMPLOYEE.JAV POSTER1.PDF  POSTER2.PDF

~ ls -a
.            ..           EMPLOYEE.JAV POSTER1.PDF  POSTER2.PDF

//things we still need:
//      print boot sec info
//      print root dir
//      change directory
//      list dir
//      Type
//      Rename
//Extra Credit:
//      Move
//      Adding file

#endif 