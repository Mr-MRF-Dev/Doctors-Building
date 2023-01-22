#include <math.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>



#pragma warning(disable:140)



//** Colors
char Color_Reset[] = "\033[0m";
char Color_Red[] = "\033[0;31m";
char Color_Red_Dark[] = "\033[38;2;160;0;0m";
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



//** cls Command
int run_cls = 1;
#define RUN_CLS if (run_cls) system("cls")



//TODO:
// off color
// file Error Management
// Hash PassWord
// 
// reset all 


//** Define Const Num
#define NAME_SIZE 31
#define EMAIL_SIZE 51
#define NATIONAL_CODE_SIZE 11
#define PASSWORD_SIZE 31



//** Data:



//* Date Time
typedef enum week {Sat, Sun, Mon, Tue, Wed, Thu, Fri} week;

typedef struct date {
    int y;
    int m;
    int d;
    week week_d;
}date;

char calendar_in_path[] = "calendar.bin";
int Active_Calendar = 0;
date Date_Start_Cal;
date Date_Start_Cal_Next = {0, 0, 0, 0};
date Date_Start_Cal_Last = {0, 0, 0, 0};
date Date_Login; 


typedef struct off_date {
    date Date;
    char Reason[101];
} off_date;

char off_date_path[] = "off-date.bin";
#define OFF_DATE_MAX_COUNT 31
int Cal_Off_Date_Count = 0;
off_date Cal_Off_Date[OFF_DATE_MAX_COUNT];



//* Srtuct
typedef struct doc_time_work {

    date date_time_work_arr[31];
    int start_time_work_arr[31];
    int end_time_work_arr[31];
    int count_time_work;
    
} doc_time_work;



//* Doctor
typedef struct doctor {

    int id;
    int wallet;
    char name[NAME_SIZE];
    char email[EMAIL_SIZE];
    char code_n[NATIONAL_CODE_SIZE];
    char password[PASSWORD_SIZE];

    doc_time_work time_work;

} doctor;

int doctor_count = 0;
#define DOCTOR_MAX_COUNT 50
doctor Doctors[DOCTOR_MAX_COUNT];
char doctor_file_path[] = "doctor.bin";



//* Patient
typedef struct patient {
    int id;
    char name[NAME_SIZE];
    char email[EMAIL_SIZE];
    char code_n[NATIONAL_CODE_SIZE];
    char password[PASSWORD_SIZE];

} patient;

// Patient visit

int patient_count = 0;
#define PATIENT_MAX_COUNT 800
patient Patients[PATIENT_MAX_COUNT];
char patient_file_path[] = "patient.bin";





//** Functions
void Sign_In_Function();
void Forgot_Password_Function();
void Exit_Function(int bar_status_code, int exit_code, int login_code);

void Bar_Status(int login, int id);
void Main_Func_Get_User_Date();

void Admin_Panel();
void AP_Add_Doctor();
void AP_Doctors_List();
void AP_Add_Patient();
void AP_Patients_List();
void AP_Monthly_Schedule();

void Doctor_Panel(int doc_login_id);
void DP_Determining_Shifts(int doc_id);
void DP_Print_Calendar(int doc_id, int y, int m, int d, int week_d);

void Print_Calendar(int y, int m, int d, int week_d);
void Print_WeekDay(int d);
void Print_Month(int m);
void Print_Off_Date();

void Get_Files();
void Update_Files();

void Error_Management(int code);
int User_Input_String(char* str_list, int str_size,int just_number);
int User_Input_PassWord(char* pass_list, int pass_size);
int User_Input_Number_Range(int start, int end);



int main() {

    RUN_CLS;

    Get_Files();

    RUN_CLS;

    if (Active_Calendar) {
        Main_Func_Get_User_Date();
    }

    else {
        Bar_Status(0, 0);
        printf("%sThe Program Calendar is Not Set%s\n", Color_Red, Color_Reset);
        Sleep(5000);
    }

    while (1) {

        RUN_CLS;

        Bar_Status(0, 0);
        printf("Home Page:\n\n");
        printf("    %s1 %s> %sSign In\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s2 %s> %sForgot PassWord\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s3 %s> %sExit\n", Color_Yellow, Color_Aqua, Color_Reset);

        Sleep(500);

        Bar_Status(0, 0);
        printf("Select one More: ");

        int in_code = User_Input_Number_Range(1, 3);

        Sleep(500);

        switch ( in_code ) {
            
            // ctrl + c exit code 
            case -2:
                printf("Run Exit Function.\n");
                Exit_Function(0, 0, 0);
                break;

            case -1: // Error input
                break;

            case 1:
                Sign_In_Function();
                break;

            case 2:
                Forgot_Password_Function();
                break;
            
            case 3:
                Exit_Function(0, 0, 0);
                return 0;
                break;
            

        } // switch end


    } // while end


    return 0;
}



void Sign_In_Function() {

    while(1) {
        
        RUN_CLS;

        Bar_Status(0, 0);
        printf("Sign In (Ctrl+C ~ Back)\n");

        Bar_Status(0, 0);
        printf("Enter Your UserName: ");

        // UserName is National Code or 'Admin'
        char UserNameInput[NATIONAL_CODE_SIZE];
        int UserInt = User_Input_String(UserNameInput, NATIONAL_CODE_SIZE, 0);
        
        // ctrl+c ~ -2
        if (UserInt == -2) {
            printf("Back to Home Page.\n");
            Sleep(3000);
            return;
        }

        if (UserInt == -1) continue;

        Bar_Status(0, 0);
        printf("Enter Your PassWord: ");

        char PassWordInput[PASSWORD_SIZE];
        int PassInt = User_Input_PassWord(PassWordInput, PASSWORD_SIZE);

        // ctrl+c ~ -2
        if (PassInt == -2) {
            printf("Back to Home Page.\n");
            Sleep(3000);
            return;
        }

        if (PassInt == -1) continue;

        // Admin Login Panel
        if (strcmp(PassWordInput, "Admin") == 0 && strcmp(UserNameInput, "Admin") == 0) {
            
            Bar_Status(1, 0);
            printf("Login was Successful ~ %sAdmin%s\n", Color_Red, Color_Reset);
            
            Sleep(1500);
            Admin_Panel();
        
        }

        else if(strcmp(PassWordInput, "admin") == 0 && strcmp(UserNameInput, "admin") == 0) {
            
            Bar_Status(0, 0);
            printf("Do You Mean %sAdmin%s ? Try Again.\n", Color_Red, Color_Reset);
            Sleep(3000);
            continue;
        
        }

        else {
            
            // Doctor Panel
            for (int i=0; i<doctor_count; i++) {

                if (strcmp(UserNameInput, Doctors[i].code_n) == 0 && strcmp(PassWordInput, Doctors[i].password) == 0 ) {

                    Bar_Status(2, i);
                    printf("Login was Successful ~ %s%s%s\n", Color_Green, Doctors[i].name, Color_Reset);
                    Sleep(1500);
                    Doctor_Panel(i);
                    return;

                }

            }


            // Patient Panel
            for (int i=0; i<patient_count; i++) {

                if (strcmp(UserNameInput, Patients[i].code_n) == 0 && strcmp(PassWordInput, Patients[i].password) == 0 ) {

                    printf("patient panel %d", i);
                    Sleep(3000);
                    return;

                }

            }


            Bar_Status(0, 0);
            printf("The Username or Password is incorrect!\n");
            Sleep(5000);
            continue;


        } // else end


        break;


    } // while end


}



void Forgot_Password_Function() {


    while(1) {
        
        RUN_CLS;

        Bar_Status(0, 0);
        printf("Forgot PassWord (Ctrl+C ~ Back)\n");

        Bar_Status(0, 0);
        printf("Enter Your UserName: ");

        // UserName is National Code or 'Admin'
        char UserNameInput[NATIONAL_CODE_SIZE];
        int UserInt = User_Input_String(UserNameInput, NATIONAL_CODE_SIZE, 0);
        
        if (UserInt == -1) continue;
        
        // ctrl+c ~ -2
        if (UserInt == -2) {
            printf("Back to Home Page.\n");
            Sleep(3000);
            return;
        }

        Bar_Status(0, 0);
        printf("Enter Your Email: ");

        char EmailInput[EMAIL_SIZE];
        int EmailInt = User_Input_String(EmailInput, EMAIL_SIZE, 0);

        if (EmailInt == -1) continue;
        
        // ctrl+c ~ -2
        if (EmailInt == -2) {
            printf("Back to Home Page.\n");
            Sleep(3000);
            return;
        }

        // Admin Login Panel
        if (strcmp(UserNameInput, "Admin") == 0) {
            
            Bar_Status(0, 0);
            printf("Admin PassWord is: '%sAdmin%s' :/\n", Color_Red, Color_Reset);
            Sleep(3000);
        }

        else {

            // Doctors
            for(int i=0; i < doctor_count; i++) {

                if (strcmp(UserNameInput, Doctors[i].code_n) == 0 && strcmp(EmailInput, Doctors[i].email) == 0 ) {
                    
                    while(1) {

                        Bar_Status(0, 0);
                        printf("Enter New PassWord (Ctrl+C ~ Cancel): ");

                        char PassWordInput[PASSWORD_SIZE];
                        int PassInt = User_Input_PassWord(PassWordInput, PASSWORD_SIZE);
                        
                        if (PassInt == -1) continue;

                        if (PassInt == -2) {
                            printf("%sCanceled%s\n", Color_Yellow, Color_Reset);
                            Sleep(3000);
                            return;
                        }

                        strcpy(Doctors[i].password, PassWordInput);
                        Update_Files();

                        Bar_Status(0, 0);
                        printf("The password was changed.\n");
                        Sleep(3000);
                        return;
                        break;          

                    }       
                }
            } // for doc end



            // Patients
            for(int i=0; i < patient_count; i++) {

                if (strcmp(UserNameInput, Patients[i].code_n) == 0 && strcmp(EmailInput, Patients[i].email) == 0 ) {
                    
                    while(1) {

                        Bar_Status(0, 0);
                        printf("Enter New PassWord (Ctrl+C ~ Cancel): ");

                        char PassWordInput[PASSWORD_SIZE];
                        int PassInt = User_Input_PassWord(PassWordInput, PASSWORD_SIZE);
                        
                        if (PassInt == -1) continue;

                        if (PassInt == -2) {
                            printf("%sCanceled%s\n", Color_Yellow, Color_Reset);
                            Sleep(3000);
                            return;
                        }

                        strcpy(Patients[i].password, PassWordInput);
                        Update_Files();

                        Bar_Status(0, 0);
                        printf("The password was changed.\n");
                        Sleep(3000);
                        return;
                        break;          

                    }       
                }
            } // for pat end


            Bar_Status(0, 0);
            printf("User With this Username and Email was Not Found!\n");
            Sleep(5000);
            continue;


        } // else end

        break;

    } // while end


}



void Bar_Status(int login, int id) {
    
    if (login == 0) {
        printf("\n%s! Login ! %s>>> %s", Color_Yellow, Color_Aqua, Color_Reset);
    }

    // login as admin
    else if (login == 1) {
        printf("\n%s#A%s %s %s>>> %s", Color_Blue, Color_Red, "Admin", Color_Aqua, Color_Reset);
    }
    
    //login as doctor
    else if (login == 2) {
        printf("\n%s#D%d%s %s %s>>> %s", Color_Blue, id, Color_Green, Doctors[id].name , Color_Aqua, Color_Reset);
    }


}



void Admin_Panel() {

    while (1) {
        
        RUN_CLS;

        Bar_Status(1, 0);
        printf("Admin Panel:\n\n");
        printf("    %s1 %s> %sAdd Doctor\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s2 %s> %sDoctors List\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s3 %s> %sAdd Patient\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s4 %s> %sPatients List\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s5 %s> %sMonthly Schedule\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s6 %s> %sVisits Schedule\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s7 %s> %sExit ~ Logout\n", Color_Yellow, Color_Aqua, Color_Reset);

        Sleep(500);

        Bar_Status(1, 0);
        printf("Select one More: ");

        int AdminInput = User_Input_Number_Range(1, 7);

        Sleep(500);

        if (AdminInput == -1) continue;

        if (AdminInput == -2) {
            // ctrl + c ~ code -2
            printf("Run Exit Function.\n");
            Exit_Function(1, 0, 0);
        }

        switch (AdminInput) {
        
            case 1:
                AP_Add_Doctor();
                break;
            
            case 2:
                AP_Doctors_List();
                break;
            
            case 3:
                AP_Add_Patient();
                break;
            
            case 4:
                AP_Patients_List();
                break;
            
            case 5:
                AP_Monthly_Schedule();
                break;
            
            case 6:
                /* code */
                break;
            
            case 7:
                Bar_Status(1, 0);
                printf("logout Successful.\n");
                Sleep(2000);
                return;
                break;
            

        } // switch end
    

    } // while end


}



void AP_Add_Doctor() {

    doctor doc;

    while(1) {

        RUN_CLS;

        int str_func_return_code = 0;

        Bar_Status(1, 0);
        printf("Add Doctor (Ctrl + C ~ Back to Admin Panel)\n");

        if (doctor_count == DOCTOR_MAX_COUNT) {

            Bar_Status(1, 0);
            printf("There is no vacant office in the building :')\n");

            Sleep(3000);

            return;

        }

        // get name
        Bar_Status(1, 0);
        printf("Enter Doctor Name: ");

        str_func_return_code = User_Input_String(doc.name, NAME_SIZE, 0);

        if (str_func_return_code == -1) continue;

        if (str_func_return_code == -2) {
            printf("Back to Admin Panel.\n");
            Sleep(3000);
            return;
        }

        // get email
        Bar_Status(1, 0);
        printf("Enter Doctor Email: ");

        str_func_return_code = User_Input_String(doc.email, EMAIL_SIZE, 0);

        if (str_func_return_code == -1) continue;

        if (str_func_return_code == -2) {
            printf("Back to Admin Panel.\n");
            Sleep(3000);
            return;
        }


        // get National Code
        Bar_Status(1, 0);
        printf("Enter Doctor National Code: ");

        str_func_return_code = User_Input_String(doc.code_n, NATIONAL_CODE_SIZE, 1);

        if (str_func_return_code == -1) continue;

        if (str_func_return_code == -2) {
            printf("Back to Admin Panel.\n");
            Sleep(3000);
            return;
        }


        // get password
        Bar_Status(1, 0);
        printf("Enter Doctor Password: ");

        str_func_return_code = User_Input_PassWord(doc.password, PASSWORD_SIZE);

        if (str_func_return_code == -1) continue;

        if (str_func_return_code == -2) {
            printf("Back to Admin Panel.\n");
            Sleep(3000);
            return;
        }


        doc.wallet = 10;
        doc.time_work.count_time_work = 0;
        doc.id = doctor_count;


        // Find a duplicate doctor
        int flag = 0;
        for (int i = 0; i < doctor_count; i++) {

            if (strcmp(doc.name, Doctors[i].name) == 0) {
                Error_Management(30);
                flag = 1;
                break;
            }

            if (strcmp(doc.email, Doctors[i].email) == 0) {
                Error_Management(31);
                flag = 1;
                break;
            }

            if (strcmp(doc.code_n, Doctors[i].code_n) == 0) {
                Error_Management(32);
                flag = 1;
                break;
            }

        }


        // Find a duplicate Patient (code_n or email)
        for (int i=0; i<patient_count; i++) {

            if(strcmp(doc.email, Patients[i].email) == 0) {
                Error_Management(33);
                flag = 1;
                break;
            }

            if(strcmp(doc.code_n, Patients[i].code_n) == 0) {
                Error_Management(34);
                flag = 1;
                break;
            }
        
        }


        if (flag) continue;


        // Added Doctor in Struct List
        Doctors[doctor_count] = doc;
        doctor_count++;

        Update_Files();

        printf("\n  %sDoctor Added Successfully.%s\n", Color_Gray, Color_Reset);
        printf("    %sDoctor Name:      %s%s\n", Color_Blue, Color_Reset, doc.name);
        printf("    %sNational Code:    %s%s\n", Color_Yellow, Color_Reset, doc.code_n);
        printf("    %sDoctor Email:     %s%s\n", Color_Yellow, Color_Reset, doc.email);
        printf("    %sDoctor ID:        %s%d\n", Color_Green, Color_Reset, doc.id);
        printf("    %sWallet:           %s%d\n", Color_Green, Color_Reset, doc.wallet);

        Sleep(3000);
        
        break;

    } // while end


}



void AP_Doctors_List() {

    RUN_CLS;

    printf("\n    %sList Of Doctors%s\n", Color_Purple, Color_Reset);
    printf("  ------------------------------\n");

    for (int i=0; i<doctor_count; i++) {
        
        doctor doc = Doctors[i];

        printf("    %sDoctor Name:      %s%s\n", Color_Blue, Color_Reset, doc.name);
        printf("    %sNational Code:    %s%s\n", Color_Yellow, Color_Reset, doc.code_n);
        printf("    %sDoctor Email:     %s%s\n", Color_Yellow, Color_Reset, doc.email);
        printf("    %sDoctor ID:        %s%d\n", Color_Green, Color_Reset, doc.id);
        printf("    %sWallet:           %s%d\n", Color_Green, Color_Reset, doc.wallet);

        printf("  ------------------------------\n");
    
    }

    printf("\n%sPress a Button to Continue...     %s", Color_Gray, Color_Reset);

    printf("%c\n", (char)getch() );

}



void AP_Add_Patient() {

    patient pat;

    while(1) {

        RUN_CLS;

        int str_func_return_code = 0;

        Bar_Status(1, 0);
        printf("Add Patient (Ctrl + C ~ Back to Admin Panel)\n");

        if (patient_count == PATIENT_MAX_COUNT) {

            Bar_Status(1, 0);
            printf("The capacity of patients is completed :')\n");

            Sleep(3000);

            return;

        }

        // get name
        Bar_Status(1, 0);
        printf("Enter Patient Name: ");

        str_func_return_code = User_Input_String(pat.name, NAME_SIZE, 0);

        if (str_func_return_code == -1) continue;

        if (str_func_return_code == -2) {
            printf("Back to Admin Panel.\n");
            Sleep(3000);
            return;
        }

        // get email
        Bar_Status(1, 0);
        printf("Enter Patient Email: ");

        str_func_return_code = User_Input_String(pat.email, EMAIL_SIZE, 0);

        if (str_func_return_code == -1) continue;

        if (str_func_return_code == -2) {
            printf("Back to Admin Panel.\n");
            Sleep(3000);
            return;
        }


        // get National Code
        Bar_Status(1, 0);
        printf("Enter Patient National Code: ");

        str_func_return_code = User_Input_String(pat.code_n, NATIONAL_CODE_SIZE, 1);

        if (str_func_return_code == -1) continue;

        if (str_func_return_code == -2) {
            printf("Back to Admin Panel.\n");
            Sleep(3000);
            return;
        }


        // get password
        Bar_Status(1, 0);
        printf("Enter Patient Password: ");

        str_func_return_code = User_Input_PassWord(pat.password, PASSWORD_SIZE);

        if (str_func_return_code == -1) continue;

        if (str_func_return_code == -2) {
            printf("Back to Admin Panel.\n");
            Sleep(3000);
            return;
        }


        pat.id = patient_count;


        // Find a duplicate Patient
        int flag = 0;
        for (int i = 0; i < patient_count; i++) {

            if (strcmp(pat.name, Patients[i].name) == 0) {
                Error_Management(35);
                flag = 1;
                break;
            }

            if (strcmp(pat.email, Patients[i].email) == 0) {
                Error_Management(36);
                flag = 1;
                break;
            }

            if (strcmp(pat.code_n, Patients[i].code_n) == 0) {
                Error_Management(37);
                flag = 1;
                break;
            }

        }

        // Find a duplicate Doctor (code_n or email)
        for (int i=0; i<doctor_count; i++) {

            if(strcmp(pat.email, Doctors[i].email) == 0) {
                Error_Management(38);
                flag = 1;
                break;
            }

            if(strcmp(pat.code_n, Doctors[i].code_n) == 0) {
                Error_Management(39);
                flag = 1;
                break;
            }
        
        }


        if (flag) continue;


        // Added Doctor in Struct List
        Patients[patient_count] = pat;
        patient_count++;

        Update_Files();

        printf("\n  %sPatient Added Successfully.%s\n", Color_Gray, Color_Reset);
        printf("    %sPatient Name:      %s%s\n", Color_Blue, Color_Reset, pat.name);
        printf("    %sNational Code:     %s%s\n", Color_Yellow, Color_Reset, pat.code_n);
        printf("    %sPatient Email:     %s%s\n", Color_Yellow, Color_Reset, pat.email);
        printf("    %sPatient ID:        %s%d\n", Color_Green, Color_Reset, pat.id);

        Sleep(3000);
        
        break;

    } // while end


}



void AP_Patients_List() {

    RUN_CLS;

    printf("\n    %sList Of Patients%s\n", Color_Purple, Color_Reset);
    printf("  ------------------------------\n");

    for (int i=0; i<patient_count; i++) {
        
        patient pat = Patients[i];

        printf("    %sPatient Name:      %s%s\n", Color_Blue, Color_Reset, pat.name);
        printf("    %sNational Code:     %s%s\n", Color_Yellow, Color_Reset, pat.code_n);
        printf("    %sPatient Email:     %s%s\n", Color_Yellow, Color_Reset, pat.email);
        printf("    %sPatient ID:        %s%d\n", Color_Green, Color_Reset, pat.id);

        printf("  ------------------------------\n");
    
    }

    printf("\n%sPress a Button to Continue...     %s", Color_Gray, Color_Reset);

    printf("%c\n", (char)getch() );

}



void AP_Monthly_Schedule() {

    RUN_CLS;

    if (Active_Calendar == 1) {

        if (Date_Login.y > Date_Start_Cal.y && !( Date_Login.y == Date_Start_Cal.y + 1 && Date_Start_Cal.m == 12 && Date_Login.m == 1 ) ) {
            Bar_Status(1, 0);
            printf("%sIn The Month, The Program Calendar is Not Set (Year)%s\n", Color_Red, Color_Reset);
            Sleep(5000);
            Active_Calendar = 0;
        }


        else if (Date_Login.m == Date_Start_Cal.m + 1 || ( Date_Login.y == Date_Start_Cal.y + 1 && Date_Start_Cal.m == 12 && Date_Login.m == 1) ) {
            
            if (Date_Start_Cal_Next.y == 0 && Date_Start_Cal_Next.m == 0 && Date_Start_Cal_Next.d == 0) {
                
                Bar_Status(1, 0);
                printf("%sIn The Month, The Program Calendar is Not Set (No Next)%s\n", Color_Red, Color_Reset);
                Sleep(5000);

                Date_Start_Cal_Last.y = Date_Start_Cal.y;
                Date_Start_Cal_Last.m = Date_Start_Cal.m;
                Date_Start_Cal_Last.d = Date_Start_Cal.d;
                Date_Start_Cal_Last.week_d = Date_Start_Cal.week_d;

                Active_Calendar = 0;

            }

            else {

                Bar_Status(1, 0);
                printf("%sSystem: Auto Go To Next Month%s\n", Color_Green, Color_Reset);

                Date_Start_Cal_Last.y = Date_Start_Cal.y;
                Date_Start_Cal_Last.m = Date_Start_Cal.m;
                Date_Start_Cal_Last.d = Date_Start_Cal.d;
                Date_Start_Cal_Last.week_d = Date_Start_Cal.week_d;
                
                Bar_Status(1, 0);
                printf("Last Month\n");
                Print_Calendar(Date_Start_Cal_Last.y, Date_Start_Cal_Last.m, Date_Start_Cal_Last.d, Date_Start_Cal_Last.week_d);

                Date_Start_Cal.y = Date_Start_Cal_Next.y;
                Date_Start_Cal.m = Date_Start_Cal_Next.m;
                Date_Start_Cal.d = Date_Start_Cal_Next.d;
                Date_Start_Cal.week_d = Date_Start_Cal_Next.week_d;

                Date_Start_Cal_Next.y = 0;
                Date_Start_Cal_Next.m = 0;
                Date_Start_Cal_Next.d = 0;
                Date_Start_Cal_Next.week_d = 0;

                Bar_Status(1, 0);
                printf("Next Month\n");
                Print_Calendar(Date_Start_Cal.y, Date_Start_Cal.m, Date_Start_Cal.d, Date_Start_Cal.week_d);

                // reset off day
                Cal_Off_Date_Count = 0;

                Update_Files();

                Sleep(5000);
            
            }

        }

        else if (Date_Login.m > Date_Start_Cal.m) {
            Bar_Status(1, 0);
            printf("%sIn The Month, The Program Calendar is Not Set (Month)%s\n", Color_Red, Color_Reset);
            Sleep(5000);
            Active_Calendar = 0;
        }


        if (Active_Calendar == 1 && Date_Start_Cal.d - Date_Login.d < 10) {
            Bar_Status(1, 0);
            printf("%sThe Next Month is Near, don't Forgot to Define it.%s\n", Color_Yellow, Color_Reset);
            Sleep(5000);
        }


    } // if end 



    if (Active_Calendar == 0) {

        while(1) {
            
            //* set calendar

            RUN_CLS;

            Bar_Status(1, 0);
            printf("Set Calendar (Ctrl+C ~ Back to Admin Panel)\n");

            Bar_Status(1, 0);
            printf("Year: ");
            
            int year_in = User_Input_Number_Range(1, 9999);

            if (year_in == -1) continue;

            if (year_in == -2) {
                printf("Back To Admin Panel\n");
                Sleep(3000);
                return;
            }


            Bar_Status(1, 0);
            printf("Month: ");
            
            int month_in = User_Input_Number_Range(1, 12);

            if (month_in == -1) continue;

            if (month_in == -2) {
                printf("Back To Admin Panel\n");
                Sleep(3000);
                return;
            }


            Bar_Status(1, 0);
            printf("Set Day Auto ? (y ~ Yes / To Cancel, Press Another Key): ");

            int x = getch();
            printf("%c\n", (char)x);


            int day_in;
            if (x == 'y' || x == 'Y') {

                if (1 <= month_in && month_in <= 6) day_in = 31;

                else if (7 <= month_in && month_in <= 11) day_in = 30;

                else if (month_in == 12) {
                    
                    if ((year_in - 1399) % 4 == 0) day_in = 30;

                    else day_in = 29;

                }

            }

            else {

                Bar_Status(1, 0);
                printf("Day Count: ");

                day_in = User_Input_Number_Range(1, 31);

                if (day_in == -1) continue;

                if (day_in == -2) {
                    printf("Back To Admin Panel\n");
                    Sleep(3000);
                    return;
                }

            }


            Bar_Status(1, 0);
            printf("Start WeekDay (0~Sat, 1~Sun, 2~Mon, 3~Tue, 4~Wed, 5~Thu, 6~Fri): ");

            int week_day_in = User_Input_Number_Range(0, 6);

            if (week_day_in == -1) continue;

            if (week_day_in == -2) {
                printf("Back To Admin Panel\n");
                Sleep(3000);
                return;
            }

            Date_Start_Cal.y = year_in;
            Date_Start_Cal.m = month_in;
            Date_Start_Cal.d = day_in;
            Date_Start_Cal.week_d = week_day_in;
            
            Date_Start_Cal_Next.y = 0;
            Date_Start_Cal_Next.m = 0;
            Date_Start_Cal_Next.d = 0;
            Date_Start_Cal_Next.week_d = 0;

            Cal_Off_Date_Count = 0;
            Active_Calendar = 1;
            Update_Files();


            Bar_Status(1, 0);
            printf("The App Calendar has been Set Successfully. ");
            printf("(Y~%d M~%d D-C~%d W-D~%d)\n", Date_Start_Cal.y, Date_Start_Cal.m, Date_Start_Cal.d, Date_Start_Cal.week_d);
            Sleep(5000);

            break;


        } // while end ~ set calendar

    } // if end 


    
    // menu
    while(1) {
        
        RUN_CLS;

        Print_Calendar(Date_Start_Cal.y, Date_Start_Cal.m, Date_Start_Cal.d, Date_Start_Cal.week_d);

        Print_Off_Date();

        Bar_Status(1, 0);
        printf("Monthly Schedule\n\n");
        printf("    %s1 %s> %sSet Off Date\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s2 %s> %sSet Next Month Calendar\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s3 %s> %sBack Admin Panel\n", Color_Yellow, Color_Aqua, Color_Reset);

        Sleep(500);

        Bar_Status(1, 0);
        printf("Select one More: ");

        int in_code = User_Input_Number_Range(1, 3);

        Sleep(500);

        switch (in_code) {
            
            case -1:
                continue;
                
            case -2:
                printf("Back\n");
                Sleep(1500);
                break;

            case 3:    
                Bar_Status(1, 0);
                printf("Back\n");
                Sleep(1500);
                break;


            case 1:

                Bar_Status(1, 0);
                printf("How Many Days do you Want to Declare a Holiday?: ");

                int get_off_date_record = User_Input_Number_Range(0, Date_Start_Cal.d);
                
                if (get_off_date_record == -1) {
                    Bar_Status(1, 0);
                    printf("Back\n");
                    Sleep(1500);
                    break;
                }

                if (get_off_date_record == -2) {
                    printf("Back\n");
                    Sleep(1500);
                    break;

                }

                if (get_off_date_record == 0) {
                    Bar_Status(1, 0);
                    printf("Ok :/ You Curse Me, Are you Making Fun of Me :/ ( 30 sec Sleep :) )\n ");
                    Sleep(30000);
                    break;
                }

                int i = 0;
                while(i != get_off_date_record) {

                    RUN_CLS;

                    Print_Calendar(Date_Start_Cal.y, Date_Start_Cal.m, Date_Start_Cal.d, Date_Start_Cal.week_d);

                    Bar_Status(1, 0);
                    printf("Enter the Data Related to the %s%d Record%s: (Ctrl + C ~ Back)\n", Color_Yellow, i+1, Color_Reset);


                    Bar_Status(1, 0);
                    printf("Enter Date (day): ");

                    int input_day = User_Input_Number_Range(0, Date_Start_Cal.d);

                    if (input_day == -1) continue;

                    if (input_day == -2) {
                        printf("Back ~ %d Record Saved\n", i);
                        Sleep(3000);
                        break;
                    }

                    // check fri
                    int tmp = Date_Start_Cal.week_d, flag_found = 0;
                    for (int i = 1; i <= Date_Start_Cal.d; i++) {
                        
                        if (tmp == 6) {
                            if (i == input_day) {
                                Bar_Status(1, 0);
                                printf("The Selected Day is Friday.\n");
                                flag_found = 1;
                                Sleep(3000);
                                break;
                            }
                        }
                        tmp++;
                        if (tmp > 6) tmp = 0;
                    }

                    if (flag_found) continue;

                    for (int i =0; i < Cal_Off_Date_Count; i++) {
                        if (Cal_Off_Date[i].Date.d == input_day) {

                            Bar_Status(1, 0);
                            printf("The Selected Day is Holiday.\n");
                            flag_found = 1;
                            Sleep(3000);
                            break;

                        }
                    }

                    if (flag_found) continue;


                    Bar_Status(1, 0);
                    printf("What is the Reason for the Closure? ~ ");
                    
                    char temp_char_r[101];
                    int temp_char_in = User_Input_String(temp_char_r, 101, 0);
                
                    if (temp_char_in == -1) continue;

                    if (temp_char_in == -2) {
                        printf("Back ~ %d Record Saved\n", i);
                        Sleep(3000);
                        break;
                    }

                    Cal_Off_Date[Cal_Off_Date_Count].Date.d = input_day;
                    Cal_Off_Date[Cal_Off_Date_Count].Date.m = Date_Start_Cal.m;
                    Cal_Off_Date[Cal_Off_Date_Count].Date.y = Date_Start_Cal.y;
                    strcpy(Cal_Off_Date[Cal_Off_Date_Count].Reason, temp_char_r);
                    Cal_Off_Date_Count++;

                    Update_Files();

                    Bar_Status(1, 0);
                    printf("Holiday Added Successfully (%s%d Record%s)\n", Color_Green, i+1, Color_Reset);
                    Sleep(3000);

                    i++;
                }

                break;

            case 2:

                if (Date_Start_Cal_Next.y != 0 && Date_Start_Cal_Next.m != 0 && Date_Start_Cal_Next.d != 0) {
                    
                    Bar_Status(1, 0);
                    printf("The next month is already defined. Would you like to redefine it(y/n)? (s ~ See): ");

                    int y = getch();
                    printf("%c\n", (char)y );

                    if (y == 's' || y == 'S') {
                        Print_Calendar(Date_Start_Cal_Next.y, Date_Start_Cal_Next.m, Date_Start_Cal_Next.d, Date_Start_Cal_Next.week_d);
                        Sleep(5000);
                        break;

                    }

                    else if (y != 'y' && y != 'Y') {
                        Bar_Status(1, 0);
                        printf("Back To Admin Panel\n");
                        Sleep(3000);
                        return;
                    }


                }


                while(1) {
                    
                    //* set calendar for next month

                    RUN_CLS;

                    Bar_Status(1, 0);
                    printf("Set Next Month Calendar (Ctrl+C ~ Back to Admin Panel)\n");

                    int year_in = (Date_Start_Cal.m == 12)?(Date_Start_Cal.y + 1):(Date_Start_Cal.y);
                    
                    int month_in = (Date_Start_Cal.m == 12)?(1):(Date_Start_Cal.m + 1);

                    Bar_Status(1, 0);
                    printf("Set Calendar For %d/%d\n", year_in, month_in);
                    
                    Bar_Status(1, 0);
                    printf("Set Day Auto? (y ~ Yes / To Cancel, Press Another Key): ");

                    int x = getch();
                    printf("%c\n", (char)x);

                    int day_in;
                    if (x == 'y' || x == 'Y') {

                        if (1 <= month_in && month_in <= 6) day_in = 31;

                        else if (7 <= month_in && month_in <= 11) day_in = 30;

                        else if (month_in == 12) {
                            
                            if ((year_in - 1399) % 4 == 0) day_in = 30;

                            else day_in = 29;

                        }

                    }

                    else {

                        Bar_Status(1, 0);
                        printf("Day Count: ");

                        day_in = User_Input_Number_Range(1, 31);

                        if (day_in == -1) continue;

                        if (day_in == -2) {
                            printf("Back To Admin Panel\n");
                            Sleep(3000);
                            return;
                        }

                    }

                    Bar_Status(1, 0);
                    printf("Set Start WeekDay Auto? (y ~ Yes / To Cancel, Press Another Key): ");

                    x = getch();
                    printf("%c\n", (char)x);

                    int week_day_in;
                    if (x == 'y' || x == 'Y') {
                        week_day_in = Date_Start_Cal.d % 7 + Date_Start_Cal.week_d;
                        
                        Bar_Status(1, 0);
                        printf("Next Month Start Weekday (SYS AUTO): ");
                        Print_WeekDay(week_day_in);
                        printf("\n");
                    }

                    else {

                        Bar_Status(1, 0);
                        printf("Start WeekDay (0~Sat, 1~Sun, 2~Mon, 3~Tue, 4~Wed, 5~Thu, 6~Fri): ");

                        week_day_in = User_Input_Number_Range(0, 6);

                        if (week_day_in == -1) continue;

                        if (week_day_in == -2) {
                            printf("Back To Admin Panel\n");
                            Sleep(3000);
                            return;
                        }

                    }

                    Date_Start_Cal_Next.y = year_in;
                    Date_Start_Cal_Next.m = month_in;
                    Date_Start_Cal_Next.d = day_in;
                    Date_Start_Cal_Next.week_d = week_day_in;

                    Update_Files();

                    Bar_Status(1, 0);
                    printf("The App Calendar has been Set Successfully. (N-M)");
                    printf("(Y~%d M~%d D-C~%d W-D~%d)\n", year_in, month_in, day_in, week_day_in);

                    Sleep(3000);
                    
                    RUN_CLS;

                    Print_Calendar(Date_Start_Cal.y, Date_Start_Cal.m, Date_Start_Cal.d, Date_Start_Cal.week_d);

                    Print_Calendar(Date_Start_Cal_Next.y, Date_Start_Cal_Next.m, Date_Start_Cal_Next.d, Date_Start_Cal_Next.week_d);

                    Sleep(5000);

                    break;


                } // while end ~ set calendar


                break;


        }

        break;

    }


}



void Doctor_Panel(int doc_login_id) {
    
    while (1) {
        
        RUN_CLS;

        Bar_Status(2, doc_login_id);
        printf("Doctor Panel:\n\n");
        printf("    %s1 %s> %sDetermining The Shifts\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s2 %s> %sReserved Visits\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s3 %s> %sPatient Prescription\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s4 %s> %sRent Payment\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s5 %s> %sVisirs Payment\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s6 %s> %sExit ~ Logout\n", Color_Yellow, Color_Aqua, Color_Reset);

        Sleep(500);

        Bar_Status(2, doc_login_id);
        printf("Select one More: ");

        int DocInput = User_Input_Number_Range(1, 6);

        Sleep(500);

        if (DocInput == -1) continue;

        if (DocInput == -2) {
            // ctrl + c ~ code -2
            printf("Run Exit Function.\n");
            Exit_Function(2, 0, doc_login_id);
        }

        switch (DocInput) {
        
            case 1:
                DP_Determining_Shifts(doc_login_id);
                break;
            
            case 2:
                // 
                break;
            
            case 3:
                // 
                break;
            
            case 4:
                // 
                break;
            
            case 5:
                //
                break;
            
            case 6:
                Bar_Status(2, doc_login_id);
                printf("logout Successful.\n");
                Sleep(2000);
                return;
                break;
            

        } // switch end
    

    } // while end

}



void DP_Determining_Shifts(int doc_id) {

    while(1) {

        RUN_CLS;

        Bar_Status(2, doc_id);
        printf("Determining The Shifts\n\n");
        printf("    %s1 %s> %sSet For a Day\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s2 %s> %sRecurring Days in The Month\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s2 %s> %sBack\n", Color_Yellow, Color_Aqua, Color_Reset);
    
        Sleep(500);

        Bar_Status(2, doc_id);
        printf("Select one More: ");

        int DocInput = User_Input_Number_Range(1, 2);

        Sleep(500);

        if (DocInput == -1) continue;

        if (DocInput == -2) {
            // ctrl + c ~ code -2
            printf("Back.\n");
            Sleep(2000);
            return;
        }


        switch (DocInput) {

            case 1:
                
                while(1) {

                    RUN_CLS;
                    
                    Bar_Status(2, doc_id);
                    printf("Set For a Day (Ctrl+C ~ Back)\n");

                    DP_Print_Calendar(doc_id, Date_Start_Cal.y, Date_Start_Cal.m, Date_Start_Cal.d, Date_Start_Cal.week_d);

                    Bar_Status(2, doc_id);
                    printf("Enter Your Desired Day: ");

                    int input_day = User_Input_Number_Range(1, Date_Start_Cal.d);

                    if (input_day == -1) continue;

                    if (input_day == -2) {
                        printf("Back.\n");
                        Sleep(2000);
                        return;
                    }

                    int try_again = 0;

                    // holiday
                    for (int i = 0; i<Cal_Off_Date_Count; i++) {
                        
                        if (Cal_Off_Date[i].Date.d == input_day) {
                            
                            Bar_Status(2, doc_id);
                            printf("This Day is a Holiday!\n");
                            Sleep(3000);
                            try_again = 1;
                            break;
                        }

                    }

                    if (try_again) continue;

                    // friday
                    int tmp = 1 + (6 - Date_Start_Cal.week_d);
                    
                    for (; tmp <= Date_Start_Cal.d; tmp += 7) {
                        
                        if (tmp == input_day) {
                            Bar_Status(2, doc_id);
                            printf("This Day is Friday!\n");
                            Sleep(3000);
                            try_again = 1;
                            break;
                        }

                    }

                    if (try_again) continue;

                    for (int i =0; i < Doctors[doc_id].time_work.count_time_work; i++) {
                    
                        if (input_day == Doctors[doc_id].time_work.date_time_work_arr[i].d) {

                            Bar_Status(2, doc_id);
                            printf("This day has already been defined. Redefine (y/n)? ");

                            int x = getch();
                            printf("%c\n", (char)x);

                            if (x == 'y' || x == 'Y') {
                                break;
                            }

                            else {
                                Bar_Status(2, doc_id);
                                printf("Back\n");
                                Sleep(2000);
                                return;
                            }

                        }
                    
                    }

                    Bar_Status(2, doc_id);
                    printf("Enter Start Time: ");

                    int start_time = User_Input_Number_Range(1, 24);

                    if (start_time == -1) continue;

                    if (start_time == -2) {
                        printf("Back.\n");
                        Sleep(2000);
                        return;
                    }


                    Bar_Status(2, doc_id);
                    printf("Enter End Time: ");

                    int end_time = User_Input_Number_Range(1, 24);

                    if (end_time == -1) continue;

                    if (end_time == -2) {
                        printf("Back.\n");
                        Sleep(2000);
                        return;
                    }


                    tmp = 1 + (5 - Date_Start_Cal.week_d);
                    
                    for (; tmp <= Date_Start_Cal.d; tmp += 7) {
                        
                        if (tmp == input_day) {
                            
                            if (start_time < 8 || end_time > 13) { 
                                
                                Bar_Status(2, doc_id);
                                printf("The Hour Entered is Outside the Working Time (Thu - 8 ~ 13)\n");
                                Sleep(5000);
                                try_again = 1;
                                break;

                            }
                            
                        }

                    }

                    if (try_again) continue;

                    if (start_time < 8 || end_time > 21) { 
                                
                        Bar_Status(2, doc_id);
                        printf("The Hour Entered is Outside the Working Time (Sat ~ Wed - 8 ~ 21)\n");
                        Sleep(5000);
                        continue;

                    }
                    
                    
                    if (start_time >= end_time) {
                        Bar_Status(2, doc_id);
                        printf("The Start and End Times Do Not Match\n");
                        Sleep(5000);
                        continue;
                    }


                    tmp = Doctors[doc_id].time_work.count_time_work;
                    Doctors[doc_id].time_work.date_time_work_arr[tmp].d = input_day;
                    Doctors[doc_id].time_work.date_time_work_arr[tmp].m = Date_Start_Cal.m;
                    Doctors[doc_id].time_work.date_time_work_arr[tmp].y = Date_Start_Cal.y;

                    Doctors[doc_id].time_work.start_time_work_arr[tmp] = start_time;
                    Doctors[doc_id].time_work.end_time_work_arr[tmp] = end_time;

                    Doctors[doc_id].time_work.count_time_work++;

                    Update_Files();

                    Bar_Status(2, doc_id);
                    printf("Business Hours have been Successfully Added\n");
                    Sleep(5000);

                    break;

                } // while end

                break;
            
            case 2:
                /* code */
                break;
            
            case 3:
                Bar_Status(2, doc_id);
                printf("Back.\n");
                Sleep(2000);
                return;
                break;
            
        
        } // switch end




        break;
    
    
    } // while end


}



void DP_Print_Calendar(int doc_id, int y, int m, int d, int week_d) {

    printf("\n    %s>>> %s # ", Color_Green, Doctors[doc_id].name);

    Print_Month(m);

    printf(" ~ %d %s\n", y, Color_Reset);
    
    printf("    %s--- --- --- --- --- --- ---%s\n", Color_Gray, Color_Reset);
    printf("    Sat Sun Mon Tue Wed Thu %sFri%s\n", Color_Red, Color_Reset);
    printf("    %s--- --- --- --- --- --- ---%s\n", Color_Gray, Color_Reset);

    printf("    ");

    for (int i =0; i<week_d; i++) {
        printf("    ");
    }

    for (int i = 1; i<=d; i++) {

        if (week_d == 6) printf("%s%3d%s", Color_Red, i, Color_Reset);

        int found_flag = 0;

        // off date
        for (int j=0; j<Cal_Off_Date_Count; j++) {
            
            if (Cal_Off_Date[j].Date.m == m && Cal_Off_Date[j].Date.y == y) {

                if (Cal_Off_Date[j].Date.d == i) {
                    printf("%s%3d%s", Color_Red_Dark, i, Color_Reset);
                    found_flag = 1;
                    break;
                }

            }

        }
        
        // doc shift
        for (int j=0; j < Doctors[doc_id].time_work.count_time_work; j++) {
        
            if (Doctors[doc_id].time_work.date_time_work_arr[j].d == i) {
                printf("%s%3d%s", Color_Green, i, Color_Reset);
                found_flag = 1;
                break;
            }

        }

        if (found_flag == 0 && week_d != 6) printf("%3d", i);

        week_d++;

        if (week_d > 6) {
            printf("\n    ");
            week_d = 0;
        }

        else printf(" ");
    }

    printf("\n");

}




void Print_Off_Date() {

    printf("\n\n");
    printf("    %sDate ~ Reason\n%s",Color_Gray, Color_Reset);

    for (int i=0; i<Cal_Off_Date_Count; i++) {

        printf("    %s%3d%s", Color_Green, Cal_Off_Date[i].Date.d, Color_Reset);

        printf("%s  ~ %s", Color_Gray, Color_Reset);

        printf("%s%s%s\n", Color_Blue, Cal_Off_Date[i].Reason, Color_Reset);

    }

    printf("\n");

}



void Print_Calendar(int y, int m, int d, int week_d) {

    printf("\n    %s>>> ", Color_Green);

    Print_Month(m);

    printf(" ~ %d %s\n", y, Color_Reset);
    
    printf("    %s--- --- --- --- --- --- ---%s\n", Color_Gray, Color_Reset);
    printf("    Sat Sun Mon Tue Wed Thu %sFri%s\n", Color_Red, Color_Reset);
    printf("    %s--- --- --- --- --- --- ---%s\n", Color_Gray, Color_Reset);

    printf("    ");

    for (int i =0; i<week_d; i++) {
        printf("    ");
    }

    for (int i = 1; i<=d; i++) {

        if (week_d == 6) printf("%s%3d%s", Color_Red, i, Color_Reset);

        int found_flag = 0;

        for (int j=0; j<Cal_Off_Date_Count; j++) {
            
            if (Cal_Off_Date[j].Date.m == m && Cal_Off_Date[j].Date.y == y) {

                if (Cal_Off_Date[j].Date.d == i) {
                    printf("%s%3d%s", Color_Red_Dark, i, Color_Reset);
                    found_flag = 1;
                    break;
                }

            }

        }

        if (found_flag == 0 && week_d != 6) printf("%3d", i);

        week_d++;

        if (week_d > 6) {
            printf("\n    ");
            week_d = 0;
        }

        else printf(" ");
    }

    printf("\n");

}



void Get_Files() {

    printf("%s# GET FILES #%s\n\n", Color_Red, Color_Reset);



    // Get Doctors 
    FILE *fp_Doctor = fopen(doctor_file_path, "rb");

    if (fp_Doctor != NULL) {
        
        while (fread(&Doctors[doctor_count], sizeof(doctor), 1, fp_Doctor)) {
            doctor_count++;
        }

        printf("The file information was read successfully. %d Doctors.\n", doctor_count);

    }

    else printf("The file does not exist or could not be opened ~ (%s).\n", doctor_file_path);

    fclose(fp_Doctor);


    printf("------------------------------\n");


    // Get Patients
    FILE *fp_Patient = fopen(patient_file_path, "rb");

    if (fp_Patient != NULL) {
        
        while (fread(&Patients[patient_count], sizeof(patient), 1, fp_Patient)) {
            patient_count++;
        }

        printf("The file information was read successfully. %d Patients.\n", patient_count);

    }

    else printf("The file does not exist or could not be opened ~ (%s).\n", patient_file_path);

    fclose(fp_Patient);


    printf("------------------------------\n");


    // Get Date
    FILE *fp_Date = fopen(calendar_in_path, "rb");

    if (fp_Date != NULL) {
        
        Active_Calendar = fread(&Date_Start_Cal_Last, sizeof(date), 1, fp_Date);
        Active_Calendar = fread(&Date_Start_Cal, sizeof(date), 1, fp_Date);
        Active_Calendar = fread(&Date_Start_Cal_Next, sizeof(date), 1, fp_Date);

        printf("The file information was read successfully, Date Start Cal\n");
        printf("Date_Start_Cal_Last: %d ~ %d ~ %d\n", Date_Start_Cal_Last.y, Date_Start_Cal_Last.m, Date_Start_Cal_Last.d);
        printf("Date_Start_Cal: %d ~ %d ~ %d\n", Date_Start_Cal.y, Date_Start_Cal.m, Date_Start_Cal.d);
        printf("Date_Start_Cal_Next: %d ~ %d ~ %d\n", Date_Start_Cal_Next.y, Date_Start_Cal_Next.m, Date_Start_Cal_Next.d);

    }

    else printf("The file does not exist or could not be opened ~ (%s).\n", calendar_in_path);

    fclose(fp_Date);


    printf("------------------------------\n");


    // Get Off Date
    FILE *fp_Off_Date = fopen(off_date_path, "rb");

    if (fp_Off_Date != NULL) {
        
        while (fread(&Cal_Off_Date[Cal_Off_Date_Count], sizeof(off_date), 1, fp_Off_Date)) {
            Cal_Off_Date_Count++;
        }

        printf("The file information was read successfully. %d Off Date.\n", Cal_Off_Date_Count);

    }

    else printf("The file does not exist or could not be opened ~ (%s).\n", off_date_path);

    fclose(fp_Off_Date);


    printf("------------------------------\n");




    Sleep(5000);

}



void Update_Files() {


    // Doctors Update
    FILE *fp_Doctor = fopen(doctor_file_path, "wb");

    if (fp_Doctor != NULL) {
        
        for(int i =0; i < doctor_count; i++) {

            fwrite(&Doctors[i], sizeof(doctor), 1, fp_Doctor);
        
        }

    }

    fclose(fp_Doctor);
    

    
    // Patients Update
    FILE *fp_Patient = fopen(patient_file_path, "wb");

    if (fp_Patient != NULL) {
        
        for(int i =0; i < patient_count; i++) {

            fwrite(&Patients[i], sizeof(patient), 1, fp_Patient);
        
        }

    }

    fclose(fp_Patient);



    // Date Update
    if (Active_Calendar) {

        FILE *fp_Date = fopen(calendar_in_path, "wb");

        if (fp_Date != NULL) {
            
            fwrite(&Date_Start_Cal_Last, sizeof(date), 1, fp_Date);
            fwrite(&Date_Start_Cal, sizeof(date), 1, fp_Date);
            fwrite(&Date_Start_Cal_Next, sizeof(date), 1, fp_Date);

        }

        fclose(fp_Date);

    }



    // Off Date Update
    FILE *fp_off_date = fopen(off_date_path, "wb");

    if (fp_off_date != NULL) {
        
        for(int i =0; i < Cal_Off_Date_Count; i++) {

            fwrite(&Cal_Off_Date[i], sizeof(off_date), 1, fp_off_date);
        
        }

    }

    fclose(fp_off_date);



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
            printf("      %sError in receiving String (Number Mode).\n", Color_Gray);
            printf("      Please use Numeric Characters.%s\n", Color_Reset);
            break;

        case 22:
            printf("%s#Code 2-2%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in receiving String.\n", Color_Gray);
            printf("      The Length of the String should not Exceed the limit.%s\n", Color_Reset);
            break;
        
        case 23:
            printf("%s#Code 2-3%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in receiving PassWord.\n", Color_Gray);
            printf("      The Length of the PassWord should not Exceed the limit.%s\n", Color_Reset);
            break;
    
        case 30:
            printf("%s#Code 3-0%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Doctor.\n", Color_Gray);
            printf("      Another doctor with this name has already been registered.%s\n", Color_Reset);
            break;

        case 31:
            printf("%s#Code 3-1%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Doctor.\n", Color_Gray);
            printf("      Another doctor with this email has already been registered.%s\n", Color_Reset);
            break;
        
        case 32:
            printf("%s#Code 3-2%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Doctor.\n", Color_Gray);
            printf("      Another doctor with this national code has already been registered.%s\n", Color_Reset);
            break;
       
        case 33:
            printf("%s#Code 3-3%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Doctor.\n", Color_Gray);
            printf("      Another patient with this email has already been registered.%s\n", Color_Reset);
            break;
        
        case 34:
            printf("%s#Code 3-4%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Doctor.\n", Color_Gray);
            printf("      Another patient with this national code has already been registered.%s\n", Color_Reset);
            break;
        
        case 35:
            printf("%s#Code 3-5%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding void Patient.\n", Color_Gray);
            printf("      Another patient with this name has already been registered.%s\n", Color_Reset);
            break;

        case 36:
            printf("%s#Code 3-6%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Patient.\n", Color_Gray);
            printf("      Another patient with this email has already been registered.%s\n", Color_Reset);
            break;
        
        case 37:
            printf("%s#Code 3-7%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Patient.\n", Color_Gray);
            printf("      Another patient with this national code has already been registered.%s\n", Color_Reset);
            break;
       
        case 38:
            printf("%s#Code 3-8%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Patient.\n", Color_Gray);
            printf("      Another doctor with this email has already been registered.%s\n", Color_Reset);
            break;
        
        case 39:
            printf("%s#Code 3-9%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Patient.\n", Color_Gray);
            printf("      Another doctor with this national code has already been registered.%s\n", Color_Reset);
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

        // backspace code ~ 8
        if (x != 8) printf("%c", (char)x);

        x -= 48;

        // enter a char or chert & pert :/
        if ( !(x >= 0 && x <= 9) ) {
            
            // backspace code ~ 8
            if (x == -40 && c != 0) {

                // back cursor
                printf("\b \b");

                c-=2;
                final /= 100;
                x=0;

            }

            else if (x == -40 && c == 0) {
                // skip backspace
                x = 0;
                final /= 10;
                c--;
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



int User_Input_String(char* str_list, int str_size, int just_number) {

    /*
        0 ~ OK
        -1 Error input
        -2 Ctrl+C Exit Code 
        
    */

    int i = 0;

    while(1) {
        
        int x = getch();

        // ctrl+c exit code ~ 3
        if (x == 3) {
            printf("%s Ctrl+C %s", Color_Gray, Color_Reset);
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
        
        if (i >= str_size - 1) {
            printf("\n");
            Error_Management(22);
            return -1;
        }

        if (just_number && (x < '0' || x > '9') ) {
            printf("%c\n", (char)x);
            Error_Management(21);
            return -1;
        }

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

    while (1) {
        
        int x = getch();

        // ctrl+c exit code ~ 3
        if (x == 3) {
            printf("%s Ctrl+C %s", Color_Gray, Color_Reset);
            return -2;
        }

        // exit enter code ~ 13
        if (x == 13) { printf("\n"); break; }

        // backspace code ~ 8
        if (x == 8 && i >= 1) {
            printf("\b \b");
            i-=1;
            pass_list[i] = '\0';
            continue;
        }

        else if (x == 8 && i == 0) continue;
        // no backspace :)

        if(i >= pass_size - 1) {
            printf("\n");
            Error_Management(23);
            return -1;
        }

        pass_list[i] = (char)x;
        printf("%s*%s", Color_Gray, Color_Reset);

        i++;

        pass_list[i] = '\0';

    }
    
    return 0;

}



void Exit_Function(int bar_status_code, int exit_code, int login_code) {
    
    Bar_Status(bar_status_code, login_code);
    printf("%sExit%s :) %sBye Bye.%s\n",Color_Red, Color_Yellow, Color_Gray, Color_Reset);
    Sleep(3000);

    RUN_CLS;
    
    exit(exit_code);

}



void Main_Func_Get_User_Date() {
    
    while(1) {

        RUN_CLS;
        
        Bar_Status(0, 0);
        printf("Enter Today's Date (Ctrl+C ~ Exit)\n");


        Bar_Status(0, 0);
        printf("Year: ");
        
        int year_in = User_Input_Number_Range(1, 9999);

        if (year_in == -1) continue;

        if (year_in == -2) {
            printf("\n");
            Exit_Function(0, 0, 0);
            break;
        }


        Bar_Status(0, 0);
        printf("Month: ");
        
        int month_in = User_Input_Number_Range(1, 12);

        if (month_in == -1) continue;

        if (month_in == -2) {
            printf("\n");
            Exit_Function(0, 0, 0);
            break;
        }


        Bar_Status(0, 0);
        printf("Day: ");

        int day_in = User_Input_Number_Range(1, 31);

        if (day_in == -1) continue;

        if (day_in == -2) {
            printf("\n");
            Exit_Function(0, 0, 0);
            break;
        }


        int flag_bad_date = 0;

        if (year_in < Date_Start_Cal.y) flag_bad_date = 1;

        else if (year_in == Date_Start_Cal.y && month_in < Date_Start_Cal.m ) flag_bad_date = 1;


        // Bad Date
        if (flag_bad_date) {
            Bar_Status(0, 0);
            printf("%sError:%s The Entered Date is before the Calendar Date.\n", Color_Red, Color_Reset);
            Sleep(5000);
            continue;
        }


        Date_Login.y = year_in;
        Date_Login.m = month_in;
        Date_Login.d = day_in;


        Sleep(3000);
        break;
    

    } // while end


}



void Print_WeekDay(int d) {
    
    switch (d) {
           
        case 0:
            printf("Saturday");
            break;
        case 1:
            printf("Sunday");
            break;
        case 2:
            printf("Monday");
            break;
        case 3:
            printf("Tuesday");
            break;
        case 4:
            printf("Wednesday");
            break;
        case 5:
            printf("Thursday");
            break;
        case 6:
            printf("Friday");
            break;

    }

}



void Print_Month(int m) {

    switch (m) {

        case 1:
            printf("Farvardin");
            break;
        case 2:
            printf("Ordibehesht");
            break;
        case 3:
            printf("Khordad");
            break;
        case 4:
            printf("Tir");
            break;
        case 5:
            printf("Mordad");
            break;
        case 6:
            printf("Shahrivar");
            break;
        case 7:
            printf("Mehr");
            break;
        case 8:
            printf("Aban");
            break;
        case 9:
            printf("Azar");
            break;
        case 10:
            printf("Dei");
            break;
        case 11:
            printf("Bahman");
            break;
        case 12:
            printf("Esfand");
            break;
    }

}



// END ¯\_(ツ)_/¯