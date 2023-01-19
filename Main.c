#include <io.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>



//* Colors
char Color_Reset[] = "\033[0m";
char Color_Red[] = "\033[0;31m";
char Color_Red_Dark[] = "\033[38;2;0;255;255m";
char Color_Green[] = "\033[0;32m";
char Color_Yellow[] = "\033[0;33m";
char Color_Yellow_Dark[] = "\033[38;2;200;200;0m";
char Color_Blue[] = "\033[0;34m";
char Color_Purple[] = "\033[0;35m";
char Color_Aqua[] = "\033[38;2;0;255;255m";
char Color_Orange[] = "\033[38;2;255;80;0m";
char Color_Gray[] = "\033[38;2;128;128;128m";
char Color_Deep_Pink[] = "\033[38;2;250;40;155m";
char Color_Green_Blue[] = "\033[38;2;0;200;80m";



//* cls Command
int run_cls = 0;
#define RUN_CLS if (run_cls) system("cls")



//TODO:
// tarikh vorod dar barname
// get string function 
// Sign_In_Function: ctrl + c back to menu
// off color



//* Functions
void Sign_In_Function();
void Exit_Function(int bar_status_code, int exit_code);

void Bar_Status(int login);

void Error_Management(int code);
int User_Input_PassWord(char* pass_list, int pass_size);
int User_Input_String(char* str_list, int str_size);
int User_Input_Number_Range(int start, int end);



int main() {

    RUN_CLS;

    printf("Welcome Back.\n\n");
    
    while (1) {
            
        Bar_Status(0);
        printf("Sign In Page:\n\n");
        printf("    %s1 %s> %sSign in\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s2 %s> %sForgot PassWord\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s3 %s> %sExit\n", Color_Yellow, Color_Aqua, Color_Reset);

        Sleep(500);

        Bar_Status(0);
        printf("Select one More: ");

        int in_code = User_Input_Number_Range(1, 3);

        switch ( in_code ) {
            
            // ctrl + c exit code 
            case -2:
                printf("Run Exit Function.\n");
                Exit_Function(0, 0);
                break;

            case -1: // Error input
                break;

            case 1:
                // sign in 
                Sign_In_Function();
                break;

            case 2:
                // forgot pass
                break;
            
            case 3:
                // Exit
                Exit_Function(0, 0);
                return 0;
                break;
            

        } // switch end


    } // while end


    return 0;
}



void Sign_In_Function() {

    while(1) {

        Bar_Status(0);
        printf("Enter Your UserName (Ctrl+C ~ Exit): ");

        char UserNameInput[31];
        int UserInt = User_Input_String(UserNameInput, 31);
        
        // ctrl+c ~ -2
        if (UserInt == -2) {
            printf("Back to Sign in Page.\n");
            return;
        }

        Bar_Status(0);
        printf("Enter Your PassWord: ");

        char PassWordInput[31];
        int PassInt = User_Input_PassWord(PassWordInput, 51);

        // ctrl+c ~ -2
        if (PassInt == -2) {
            printf("Back to Get User.\n");
            continue;
        }

        // Admin Login Panel
        if (strcmp(PassWordInput, "Admin") == 0 && strcmp(UserNameInput, "Admin") == 0) {
            
            printf("Admin Login \n");
            // admin panel
        
        }

        else if(strcmp(PassWordInput, "admin") == 0 && strcmp(UserNameInput, "admin") == 0) {
            
            Bar_Status(0);
            printf("Do You Mean %sAdmin%s ? Try Again.\n", Color_Red, Color_Reset);
            continue;
        
        }

        else {

            // login in doc
            // login in 
            // کاربرد تغریف نشده

        }


    } // while end


}



void Bar_Status(int login) {
// void Bar_Status(struct Player* User, int login) {
    
    if (login == 0) {
        printf("\n%s! Login ! %s>>> %s", Color_Yellow, Color_Aqua, Color_Reset);
    }

    // //login as player
    // else if (login == 1){
    //     printf("\n%s#%d%s %s %s>>> %s", Color_Blue, User->id, Color_Green, User->Username, Color_Aqua, Color_Reset);
    // }

    // // login as admin
    // else if (login == 2) {
    //     printf("\n%s#%d%s %s %s>>> %s", Color_Blue, User->id, Color_Red, User->Username, Color_Aqua, Color_Reset);
    // }

}



void Error_Management(int code) {

    printf("\n    %sError: %s", Color_Red, Color_Reset);

    //! Error 
    //? 10 - 19 Number
    //? 20 - 29 User Error
    //? 30 - 39 Admin Error

    switch (code) {
        case 10:
            printf("%s#Code 1-0%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in Receiving the Number.\n", Color_Gray);
            printf("      Please Enter a Number in the given Range.%s\n", Color_Reset);
            break;

        case 11:
            printf("%s#Code 1-1%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in Receiving the Number.\n", Color_Gray);
            printf("      Please Enter a NUMBER :/%s\n", Color_Reset);
            break;

        case 12: //very out of range
            printf("%s#Code 1-2%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in Receiving the Number. Keep Calm :)\n", Color_Gray);
            printf("      Everything is Under Control :)\n");
            printf("      Please Enter a Number in the given Range.%s\n", Color_Reset);
            break;

        case 20:
            printf("%s#Code 2-0%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in receiving UserName.\n", Color_Gray);
            printf("      Please do not Enter a duplicate UserName.%s\n", Color_Reset);
            break;

        case 21:
            printf("%s#Code 2-1%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in receiving UserName.\n", Color_Gray);
            printf("      Username length should not be more than 50.%s\n", Color_Reset);
            break;

        case 22:
            printf("%s#Code 2-2%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in receiving UserName.\n", Color_Gray);
            printf("      Username length must be more than 1 Character.%s\n", Color_Reset);
            break;
    
        case 30: // delete admin :/
            printf("%s#Code 3-0%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Deleting Admin.\n", Color_Gray);
            printf("      Why do you want to destroy yourself ? :/%s\n", Color_Reset);
            break;

        case 31: // edit admin :/
            printf("%s#Code 3-1%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in Editing Admin.\n", Color_Gray);
            printf("      Why do you want to edit the admin :/%s\n", Color_Reset);
            break;



        default:
            printf("%s¯\\_(ツ)_/¯%s\n", Color_Yellow, Color_Reset);
            break;
    }

    Sleep(3000);

    //* Spam Protection :))))))
    printf("    %sPress Ctrl+C to Continue. %s", Color_Green, Color_Reset);
    
    int x = -1;
    int c = 0, cc = 0;

    while (x != 3) {

        x = getch();
        c++;

        if (c>10) {
            printf("%s!%s", Color_Red, Color_Reset);
            c -= 10;
            cc++;
        }

        // Emergency EXIT :/
        if (cc >= 10) {
            printf("%s WTF :/%s", Color_Yellow, Color_Reset);
            Sleep(1000);
            exit(0);
        }

    }

    printf("\n");

}



int User_Input_Number_Range(int start, int end) {
    
    /*
        if ok return final
        -1 Error input
        -2 Ctrl+C Exit Code    
    */

    int final = 0, c = 0, tmp = 0, end_c = 0;

    tmp = end;
    while(tmp > 0) {
        tmp /= 10;
        end_c++;
    }
    
    int x = getch();

    // Ctrl+C Exit Code
    if (x == 3) {
        printf("%sCtrl+C %s", Color_Gray, Color_Reset);
        return -2;
    }

    if (x == 13 || x == 8) {
        printf("\n");
        Error_Management(11);
        return -1;
    }

    // enter code ~ 13
    while (x != 13) {
        
        // Ctrl+C Exit Code
        if (x == 3) {
            printf("%sCtrl+C %s", Color_Gray, Color_Reset);
            return -2;
        }

        printf("%c", (char)x);

        x -= 48;

        // enter a char or chert & pert :/
        if ( !(x >= 0 && x <= 9) ) {
            
            // backspace code ~ 8
            if (x == -40) {

                // back cursor
                printf(" ");
                printf("%c", (char)8);

                c-=2;
                final /= 100;
                x=0;

            }

            else {
                printf("\n");
                Error_Management(11);
                return -1;
            }
        }

        final += x;
        final *= 10;
        c++;

        if (end_c+2 < c) {
            printf("\n");
            Error_Management(12);
            return -1;
        }

        x = getch();
    }
    

    final /= 10;

    printf("\n");

    if ( !(final >= start && final <= end) ) {
        
        Error_Management(10);
        return -1;

    }

    return final;

}



int User_Input_String(char* str_list, int str_size) {

    /*
        0 ~ OK
        -1 Error input
        -2 Ctrl+C Exit Code 
        
    */

    int i = 0;

    while(i != str_size) {
        
        int x = getch();

        // ctrl+c exit code ~ 3
        if (x == 3) {
            printf("%sCtrl+C %s", Color_Gray, Color_Reset);
            return -2;
        }

        // exit enter code ~ 13
        if (x == 13) { printf("\n"); break; }


        // backspace code ~ 8
        if (x == 8 && i >= 1) {
            printf("\b \b");
            i-=1;
            str_list[i] = '\0';
            continue;
        }

        else if (x == 8 && i == 0) continue;
        // no backspace :)
        

        str_list[i] = (char)x;
        printf("%c", str_list[i]);

        i++;

        str_list[i] = '\0';

    }

    return 0;

}



int User_Input_PassWord(char* pass_list, int pass_size) {

    /*
        0 ~ OK
        -1 Error input
        -2 Ctrl+C Exit Code 
    */ 

    int i = 0;

    while (i != pass_size) {
        
        int x = getch();

        // ctrl+c exit code ~ 3
        if (x == 3) {
            printf("%sCtrl+C %s", Color_Gray, Color_Reset);
            return -2;
        }

        // exit enter code ~ 13
        if (x == 13) { printf("\n"); break; }

        // backspace code ~ 8
        if (x == 8 && i >= 1) {
            printf("\b\b  \b\b");
            i-=1;
            pass_list[i] = '\0';
            continue;
        }

        else if (x == 8 && i == 0) continue;
        // no backspace :)
        

        pass_list[i] = (char)x;
        printf("%s* %s", Color_Gray, Color_Reset);

        i++;

        pass_list[i] = '\0';

    }
    
    return 0;

}



void Exit_Function(int bar_status_code, int exit_code) {
    
    Bar_Status(bar_status_code);
    printf("%sExit%s :) %sBye Bye.%s\n",Color_Red, Color_Yellow, Color_Gray, Color_Reset);
    Sleep(3000);

    exit(exit_code);

}



// END ¯\_(ツ)_/¯