#include <math.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>



//** Colors
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



//** cls Command
int run_cls = 1;
#define RUN_CLS if (run_cls) system("cls")



//TODO:
//// Sign_In_Function: ctrl + c back to menu
//// User_Input_String: added Mode National Code
//// get string function 
//// print all doctor
// tarikh vorod dar barname
// off color
// file Error Management
// Hash PassWord
// fix duplicate National Code



//** Define Const Num
#define NAME_SIZE 31
#define EMAIL_SIZE 51
#define NATIONAL_CODE_SIZE 11
#define PASSWORD_SIZE 31



//** Data:

//* Doctor
typedef struct doctor {
    int id;
    int wallet;
    char name[NAME_SIZE];
    char email[EMAIL_SIZE];
    char code_n[NATIONAL_CODE_SIZE];
    char password[PASSWORD_SIZE];

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



//* Date Time
typedef struct date_in {
    int y;
    int m;
    int d;
}date_in;

char date_in_path[] = "Date.bin";

typedef enum week {Sat, Sun, Mon, Tue, Wed, Thu, Fri} week;

typedef struct date_start_m {
    int Start;
    week Day;
}date_start_m;

char date_start_m_path[] = "Date-Start.bin";



//** Functions
void Sign_In_Function();
void Forgot_Password_Function();
void Exit_Function(int bar_status_code, int exit_code);

void Bar_Status(int login);

void Admin_Panel();
void AP_Add_Doctor();
void AP_Doctors_List();
void AP_Add_Patient();
void AP_Patients_List();

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

    printf("Welcome Back.\n\n");
    
    while (1) {

        RUN_CLS;

        Bar_Status(0);
        printf("Home Page:\n\n");
        printf("    %s1 %s> %sSign In\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s2 %s> %sForgot PassWord\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s3 %s> %sExit\n", Color_Yellow, Color_Aqua, Color_Reset);

        Sleep(500);

        Bar_Status(0);
        printf("Select one More: ");

        int in_code = User_Input_Number_Range(1, 3);

        Sleep(500);

        switch ( in_code ) {
            
            // ctrl + c exit code 
            case -2:
                printf("Run Exit Function.\n");
                Exit_Function(0, 0);
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
                Exit_Function(0, 0);
                return 0;
                break;
            

        } // switch end


    } // while end


    return 0;
}



void Sign_In_Function() {

    while(1) {
        
        RUN_CLS;

        Bar_Status(0);
        printf("Sign In (Ctrl+C ~ Back)\n");

        Bar_Status(0);
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

        Bar_Status(0);
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
            
            Bar_Status(1);
            printf("Login was Successful ~ %sAdmin%s\n", Color_Red, Color_Reset);
            
            Sleep(1500);
            Admin_Panel();
        
        }

        else if(strcmp(PassWordInput, "admin") == 0 && strcmp(UserNameInput, "admin") == 0) {
            
            Bar_Status(0);
            printf("Do You Mean %sAdmin%s ? Try Again.\n", Color_Red, Color_Reset);
            Sleep(3000);
            continue;
        
        }

        else {
            
            // Doctor Panel
            for (int i=0; i<doctor_count; i++) {

                if (strcmp(UserNameInput, Doctors[i].code_n) == 0 && strcmp(PassWordInput, Doctors[i].password) == 0 ) {

                    printf("doctoc panel %d", i);
                    Sleep(3000);
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


            Bar_Status(0);
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

        Bar_Status(0);
        printf("Forgot PassWord (Ctrl+C ~ Back)\n");

        Bar_Status(0);
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

        Bar_Status(0);
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
            
            Bar_Status(0);
            printf("Admin PassWord is: '%sAdmin%s' :/\n", Color_Red, Color_Reset);
            Sleep(3000);
        }

        else {

            // Doctors
            for(int i=0; i < doctor_count; i++) {

                if (strcmp(UserNameInput, Doctors[i].code_n) == 0 && strcmp(EmailInput, Doctors[i].email) == 0 ) {
                    
                    while(1) {

                        Bar_Status(0);
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

                        Bar_Status(0);
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

                        Bar_Status(0);
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

                        Bar_Status(0);
                        printf("The password was changed.\n");
                        Sleep(3000);
                        return;
                        break;          

                    }       
                }
            } // for pat end


            Bar_Status(0);
            printf("User With this Username and Email was Not Found!\n");
            Sleep(5000);
            continue;


        } // else end

        break;

    } // while end


}



void Bar_Status(int login) {
    
    if (login == 0) {
        printf("\n%s! Login ! %s>>> %s", Color_Yellow, Color_Aqua, Color_Reset);
    }

    // login as admin
    else if (login == 1) {
        printf("\n%s#A%s %s %s>>> %s", Color_Blue, Color_Red, "Admin", Color_Aqua, Color_Reset);
    }
    
    // //login as player
    // else if (login == 1) {
    //     printf("\n%s#%d%s %s %s>>> %s", Color_Blue, User->id, Color_Green, User->Username, Color_Aqua, Color_Reset);
    // }


}



void Admin_Panel() {

    while (1) {
        
        RUN_CLS;

        Bar_Status(1);
        printf("Admin Panel:\n\n");
        printf("    %s1 %s> %sAdd Doctor\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s2 %s> %sDoctors List\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s3 %s> %sAdd Patient\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s4 %s> %sPatients List\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s5 %s> %sMonthly Schedule\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s6 %s> %sVisits Schedule\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s7 %s> %sExit ~ Logout\n", Color_Yellow, Color_Aqua, Color_Reset);

        Sleep(500);

        Bar_Status(1);
        printf("Select one More: ");

        int AdminInput = User_Input_Number_Range(1, 7);

        Sleep(500);

        if (AdminInput == -1) continue;

        if (AdminInput == -2) {
            // ctrl + c ~ code -2
            printf("Run Exit Function.\n");
            Exit_Function(1, 0);
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
                /* code */
                break;
            
            case 6:
                /* code */
                break;
            
            case 7:
                Bar_Status(1);
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

        Bar_Status(1);
        printf("Add Doctor (Ctrl + C ~ Back to Admin Panel)\n");

        if (doctor_count == DOCTOR_MAX_COUNT) {

            Bar_Status(1);
            printf("There is no vacant office in the building :')\n");

            Sleep(3000);

            return;

        }

        // get name
        Bar_Status(1);
        printf("Enter Doctor Name: ");

        str_func_return_code = User_Input_String(doc.name, NAME_SIZE, 0);

        if (str_func_return_code == -1) continue;

        if (str_func_return_code == -2) {
            printf("Back to Admin Panel.\n");
            Sleep(3000);
            return;
        }

        // get email
        Bar_Status(1);
        printf("Enter Doctor Email: ");

        str_func_return_code = User_Input_String(doc.email, EMAIL_SIZE, 0);

        if (str_func_return_code == -1) continue;

        if (str_func_return_code == -2) {
            printf("Back to Admin Panel.\n");
            Sleep(3000);
            return;
        }


        // get National Code
        Bar_Status(1);
        printf("Enter Doctor National Code: ");

        str_func_return_code = User_Input_String(doc.code_n, NATIONAL_CODE_SIZE, 1);

        if (str_func_return_code == -1) continue;

        if (str_func_return_code == -2) {
            printf("Back to Admin Panel.\n");
            Sleep(3000);
            return;
        }


        // get password
        Bar_Status(1);
        printf("Enter Doctor Password: ");

        str_func_return_code = User_Input_PassWord(doc.password, PASSWORD_SIZE);

        if (str_func_return_code == -1) continue;

        if (str_func_return_code == -2) {
            printf("Back to Admin Panel.\n");
            Sleep(3000);
            return;
        }


        doc.wallet = 10;
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

        Bar_Status(1);
        printf("Add Patient (Ctrl + C ~ Back to Admin Panel)\n");

        if (patient_count == PATIENT_MAX_COUNT) {

            Bar_Status(1);
            printf("The capacity of patients is completed :')\n");

            Sleep(3000);

            return;

        }

        // get name
        Bar_Status(1);
        printf("Enter Patient Name: ");

        str_func_return_code = User_Input_String(pat.name, NAME_SIZE, 0);

        if (str_func_return_code == -1) continue;

        if (str_func_return_code == -2) {
            printf("Back to Admin Panel.\n");
            Sleep(3000);
            return;
        }

        // get email
        Bar_Status(1);
        printf("Enter Patient Email: ");

        str_func_return_code = User_Input_String(pat.email, EMAIL_SIZE, 0);

        if (str_func_return_code == -1) continue;

        if (str_func_return_code == -2) {
            printf("Back to Admin Panel.\n");
            Sleep(3000);
            return;
        }


        // get National Code
        Bar_Status(1);
        printf("Enter Patient National Code: ");

        str_func_return_code = User_Input_String(pat.code_n, NATIONAL_CODE_SIZE, 1);

        if (str_func_return_code == -1) continue;

        if (str_func_return_code == -2) {
            printf("Back to Admin Panel.\n");
            Sleep(3000);
            return;
        }


        // get password
        Bar_Status(1);
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



void Exit_Function(int bar_status_code, int exit_code) {
    
    Bar_Status(bar_status_code);
    printf("%sExit%s :) %sBye Bye.%s\n",Color_Red, Color_Yellow, Color_Gray, Color_Reset);
    Sleep(3000);

    RUN_CLS;
    
    exit(exit_code);

}



// END ¯\_(ツ)_/¯