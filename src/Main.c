#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #define CLEAR_SCREEN "cls"
#else
    #include <termios.h>
    #include <unistd.h>
    #define CLEAR_SCREEN "clear"

int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

void Sleep(int milliseconds) { usleep(milliseconds * 1000); }

#endif

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
    #define RUN_CLS \
        if (run_cls) system("cls")

// TODO:
//  off color
//  file Error Management

//** Define Const Num
    #define NAME_SIZE 31
    #define EMAIL_SIZE 51
    #define NATIONAL_CODE_SIZE 11
    #define PASSWORD_SIZE 31
    #define HASH_PART 3           // Hash Partition
    #define HASH_CAPACITY 995533  // Size of the Hash
    #define PRESCRIPTION_SIZE 201

//** Data:

//* Date Time
typedef enum week { Sat, Sun, Mon, Tue, Wed, Thu, Fri } week;

typedef struct date {
        int y;
        int m;
        int d;
        week week_d;
} date;

char calendar_in_path[] = "calendar.bin";
int Active_Calendar = 0;
date Date_Start_Cal;
date Date_Start_Cal_Next = { 0, 0, 0, 0 };
date Date_Start_Cal_Last = { 0, 0, 0, 0 };
date Date_Login = { 0, 0, 0, 0 };

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

        int visit_pay;

        int pay_rent_ext;
        int pay_not_month;
        int pay_total;
        date pay_deadline;

        doc_time_work time_work;

} doctor;

int doctor_count = 0;
    #define DOCTOR_MAX_COUNT 50
doctor Doctors[DOCTOR_MAX_COUNT];
char doctor_file_path[] = "doctor.bin";

//* Patient
typedef struct patient {
        int id;
        int wallet;
        char name[NAME_SIZE];
        char email[EMAIL_SIZE];
        char code_n[NATIONAL_CODE_SIZE];
        char password[PASSWORD_SIZE];

} patient;

int patient_count = 0;
    #define PATIENT_MAX_COUNT 800
patient Patients[PATIENT_MAX_COUNT];
char patient_file_path[] = "patient.bin";

//* Visit
typedef struct visit {

        int id;
        char doc_code_n[NATIONAL_CODE_SIZE];
        char pat_code_n[NATIONAL_CODE_SIZE];

        date Date;

        int start_time;

        int see_visit;
        int doc_pay_visit;

        char Prescription[PRESCRIPTION_SIZE];

} visit;

int visit_count = 0;
    #define VISIT_MAX_COUNT 1000
visit Visits[VISIT_MAX_COUNT];
char visit_file_path[] = "visit.bin";

//** Functions
void Sign_In_Function();
void Forgot_Password_Function();
void Exit_Function(int bar_status_code, int exit_code, int login_code);
void Hash_Function(char *str);

void Bar_Status(int login, int id);
void Main_Func_Get_User_Date();
void Main_Check_Active_Calendar();
void Main_Reset_Doctors();

void Admin_Panel();
void AP_Add_Doctor();
void AP_Doctors_List();
void AP_Add_Patient();
void AP_Patients_List();
void AP_Monthly_Schedule();
void AP_Visits_Schedule();

void Doctor_Panel(int doc_login_id);
void DP_Determining_Shifts(int doc_id);
void DP_Print_Calendar(int doc_id, int y, int m, int d, int week_d);
void DP_Print_Work_Time(int doc_id);
void DP_Visirs_Payment(int doc_id);
void DP_Rent_Payment(int doc_id);
void DP_Reserved_Visits(int doc_id);
void DP_Patient_Prescription(int doc_id);

void Patient_Panel(int pat_login_id);
void PP_Book_An_Appointment(int pat_id);
int PP_Find_Doctor_By_N_Code(char n_code[NATIONAL_CODE_SIZE]);
int PP_Find_Patient_By_N_Code(char n_code[NATIONAL_CODE_SIZE]);
void PP_Cancel_An_Appointment(int pat_id);
void PP_All_Appointments(int pat_id);
void PP_Prescriptions(int pat_id);

void Print_Calendar(int y, int m, int d, int week_d);
void Print_WeekDay(int d);
void Print_Month(int m);
void Print_Off_Date();

void Get_Files();
void Update_Files();

void Error_Management(int code);
int User_Input_String(char *str_list, int str_size, int just_number);
int User_Input_PassWord(char *pass_list, int pass_size);
int User_Input_Number_Range(int start, int end);

int main() {

    RUN_CLS;

    Get_Files();

    RUN_CLS;

    Main_Func_Get_User_Date();

    Main_Check_Active_Calendar();

    while (1) {

        RUN_CLS;

        Bar_Status(0, 0);
        printf("Home Page:\n\n");
        printf("    %s1 %s> %sSign In\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s2 %s> %sForgot PassWord\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s3 %s> %sExit\n", Color_Yellow, Color_Aqua, Color_Reset);

        Sleep(500);

        Bar_Status(0, 0);
        printf("Select one More: ");

        int in_code = User_Input_Number_Range(1, 3);

        Sleep(500);

        switch (in_code) {

            // ctrl + c exit code
            case -2:
                printf("Run Exit Function.\n");
                Exit_Function(0, 0, 0);
                break;

            case -1:  // Error input
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

        }  // switch end

    }  // while end

    return 0;
}

void Sign_In_Function() {

    while (1) {

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
        if (strcmp(PassWordInput, "Admin") == 0 &&
            strcmp(UserNameInput, "Admin") == 0) {

            Bar_Status(1, 0);
            printf("Login was Successful ~ %sAdmin%s\n", Color_Red,
                   Color_Reset);

            Sleep(1500);
            Admin_Panel();

        }

        else if (strcmp(PassWordInput, "admin") == 0 &&
                 strcmp(UserNameInput, "admin") == 0) {

            Bar_Status(0, 0);
            printf("Do You Mean %sAdmin%s ? Try Again.\n", Color_Red,
                   Color_Reset);
            Sleep(3000);
            continue;

        }

        else if (Active_Calendar == 0) {
            Bar_Status(0, 0);
            printf("%sSet the Calendar First! (Admin)%s\n", Color_Red,
                   Color_Reset);
            Sleep(3000);
            continue;
        }

        else {

            Hash_Function(PassWordInput);

            // Doctor Panel
            for (int i = 0; i < doctor_count; i++) {

                if (strcmp(UserNameInput, Doctors[i].code_n) == 0 &&
                    strcmp(PassWordInput, Doctors[i].password) == 0) {

                    Bar_Status(2, i);
                    printf("Login was Successful ~ %s%s%s\n", Color_Green,
                           Doctors[i].name, Color_Reset);
                    Sleep(1500);
                    Doctor_Panel(i);
                    return;
                }
            }

            // Patient Panel
            for (int i = 0; i < patient_count; i++) {

                if (strcmp(UserNameInput, Patients[i].code_n) == 0 &&
                    strcmp(PassWordInput, Patients[i].password) == 0) {

                    Bar_Status(3, i);
                    printf("Login was Successful ~ %s%s%s\n", Color_Deep_Pink,
                           Patients[i].name, Color_Reset);
                    Sleep(1500);
                    Patient_Panel(i);
                    return;
                }
            }

            Bar_Status(0, 0);
            printf("The Username or Password is incorrect!\n");
            Sleep(5000);
            continue;

        }  // else end

        break;

    }  // while end
}

void Forgot_Password_Function() {

    while (1) {

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
            printf("Admin PassWord is: '%sAdmin%s' :/\n", Color_Red,
                   Color_Reset);
            Sleep(3000);
        }

        else {

            // Doctors
            for (int i = 0; i < doctor_count; i++) {

                if (strcmp(UserNameInput, Doctors[i].code_n) == 0 &&
                    strcmp(EmailInput, Doctors[i].email) == 0) {

                    while (1) {

                        Bar_Status(0, 0);
                        printf("Enter New PassWord (Ctrl+C ~ Cancel): ");

                        char PassWordInput[PASSWORD_SIZE];
                        int PassInt =
                            User_Input_PassWord(PassWordInput, PASSWORD_SIZE);

                        if (PassInt == -1) continue;

                        if (PassInt == -2) {
                            printf("%sCanceled%s\n", Color_Yellow, Color_Reset);
                            Sleep(3000);
                            return;
                        }

                        Hash_Function(PassWordInput);

                        strcpy(Doctors[i].password, PassWordInput);
                        Update_Files();

                        Bar_Status(0, 0);
                        printf("The password was changed.\n");
                        Sleep(3000);
                        return;
                        break;
                    }
                }
            }  // for doc end

            // Patients
            for (int i = 0; i < patient_count; i++) {

                if (strcmp(UserNameInput, Patients[i].code_n) == 0 &&
                    strcmp(EmailInput, Patients[i].email) == 0) {

                    while (1) {

                        Bar_Status(0, 0);
                        printf("Enter New PassWord (Ctrl+C ~ Cancel): ");

                        char PassWordInput[PASSWORD_SIZE];
                        int PassInt =
                            User_Input_PassWord(PassWordInput, PASSWORD_SIZE);

                        if (PassInt == -1) continue;

                        if (PassInt == -2) {
                            printf("%sCanceled%s\n", Color_Yellow, Color_Reset);
                            Sleep(3000);
                            return;
                        }

                        Hash_Function(PassWordInput);

                        strcpy(Patients[i].password, PassWordInput);
                        Update_Files();

                        Bar_Status(0, 0);
                        printf("The password was changed.\n");
                        Sleep(3000);
                        return;
                        break;
                    }
                }
            }  // for pat end

            Bar_Status(0, 0);
            printf("User With this Username and Email was Not Found!\n");
            Sleep(5000);
            continue;

        }  // else end

        break;

    }  // while end
}

void Bar_Status(int login, int id) {

    if (login == 0) {
        printf("\n%s! Login ! %s>>> %s", Color_Yellow, Color_Aqua, Color_Reset);
    }

    // login as admin
    else if (login == 1) {
        printf("\n%s#A%s %s %s>>> %s", Color_Blue, Color_Red, "Admin",
               Color_Aqua, Color_Reset);
    }

    // login as doctor
    else if (login == 2) {
        printf("\n%s#D%d%s %s %s>>> %s", Color_Blue, id, Color_Green,
               Doctors[id].name, Color_Aqua, Color_Reset);
    }

    // login as patient
    else if (login == 3) {
        printf("\n%s#P%d%s %s %s>>> %s", Color_Blue, id, Color_Deep_Pink,
               Patients[id].name, Color_Aqua, Color_Reset);
    }
}

void Admin_Panel() {

    while (1) {

        RUN_CLS;

        Bar_Status(1, 0);
        printf("Admin Panel:\n\n");
        printf("    %s1 %s> %sAdd Doctor\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s2 %s> %sDoctors List\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s3 %s> %sAdd Patient\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s4 %s> %sPatients List\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s5 %s> %sMonthly Schedule\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s6 %s> %sSee Calendar\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s7 %s> %sVisits Schedule\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s8 %s> %sExit ~ Logout\n", Color_Yellow, Color_Aqua,
               Color_Reset);

        Sleep(500);

        Bar_Status(1, 0);
        printf("Select one More: ");

        int AdminInput = User_Input_Number_Range(1, 8);

        Sleep(500);

        if (AdminInput == -1) continue;

        if (AdminInput == -2) {
            // ctrl + c ~ code -2
            printf("Run Exit Function.\n");
            Exit_Function(1, 0, 0);
        }

        if (!Active_Calendar && AdminInput != 5) {
            Bar_Status(1, 0);
            printf("%sSet the Calendar First!%s\n", Color_Red, Color_Reset);
            Sleep(3000);
            continue;
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
                RUN_CLS;
                Print_Calendar(Date_Start_Cal.y, Date_Start_Cal.m,
                               Date_Start_Cal.d, Date_Start_Cal.week_d);
                Print_Off_Date();

                printf("\n%sPress a Button to Continue...     %s", Color_Gray,
                       Color_Reset);
                printf("%c\n", (char)getch());
                Sleep(500);
                break;

            case 7:
                AP_Visits_Schedule();
                break;

            case 8:
                Bar_Status(1, 0);
                printf("logout Successful.\n");
                Sleep(2000);
                return;
                break;

        }  // switch end

    }  // while end
}

void AP_Add_Doctor() {

    doctor doc;

    while (1) {

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

        str_func_return_code =
            User_Input_String(doc.code_n, NATIONAL_CODE_SIZE, 1);

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
        doc.visit_pay = 0;

        doc.pay_rent_ext = 0;
        doc.pay_not_month = 1;
        doc.pay_total = Date_Start_Cal.d * 10;

        int d_l_m = Date_Start_Cal.m;
        int d_l_y = Date_Start_Cal.y;

        if (d_l_m > 12) {
            d_l_m -= 12;
            d_l_y += 1;
        }

        doc.pay_deadline.m = d_l_m;
        doc.pay_deadline.y = d_l_y;

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
        for (int i = 0; i < patient_count; i++) {

            if (strcmp(doc.email, Patients[i].email) == 0) {
                Error_Management(33);
                flag = 1;
                break;
            }

            if (strcmp(doc.code_n, Patients[i].code_n) == 0) {
                Error_Management(34);
                flag = 1;
                break;
            }
        }

        if (flag) continue;

        Hash_Function(doc.password);

        // Added Doctor in Struct List
        Doctors[doctor_count] = doc;
        doctor_count++;

        Update_Files();

        printf("\n  %sDoctor Added Successfully.%s\n", Color_Gray, Color_Reset);
        printf("    %sDoctor Name:      %s%s\n", Color_Blue, Color_Reset,
               doc.name);
        printf("    %sNational Code:    %s%s\n", Color_Yellow, Color_Reset,
               doc.code_n);
        printf("    %sDoctor Email:     %s%s\n", Color_Yellow, Color_Reset,
               doc.email);
        printf("    %sDoctor ID:        %s%d\n", Color_Green, Color_Reset,
               doc.id);
        printf("    %sWallet:           %s%d\n", Color_Green, Color_Reset,
               doc.wallet);

        Sleep(3000);

        break;

    }  // while end
}

void AP_Doctors_List() {

    RUN_CLS;

    printf("\n    %sList Of Doctors%s\n", Color_Purple, Color_Reset);
    printf("  ------------------------------\n");

    for (int i = 0; i < doctor_count; i++) {

        doctor doc = Doctors[i];

        printf("    %sDoctor Name:             %s%s\n", Color_Blue, Color_Reset,
               doc.name);
        printf("    %sNational Code:           %s%s\n", Color_Yellow,
               Color_Reset, doc.code_n);
        printf("    %sDoctor Email:            %s%s\n", Color_Yellow,
               Color_Reset, doc.email);
        printf("    %sDoctor Password(Hash):   %s%s\n", Color_Red_Dark,
               Color_Reset, doc.password);
        printf("    %sDoctor ID:               %s%d\n", Color_Green,
               Color_Reset, doc.id);
        printf("    %sWallet:                  %s%d$\n", Color_Green,
               Color_Reset, doc.wallet);
        printf("    %sCost Visit:              %s%d$\n", Color_Green,
               Color_Reset, doc.visit_pay);
        printf("    %sRent:                    %s%d$\n", Color_Green,
               Color_Reset, doc.pay_total);
        printf("    %sIs Extension:            %s%d\n", Color_Green,
               Color_Reset, doc.pay_rent_ext);
        printf("    %sDeadLine:                %s%d/%d\n", Color_Green,
               Color_Reset, doc.pay_deadline.y, doc.pay_deadline.m);

        printf("  ------------------------------\n");
    }

    printf("\n%sPress a Button to Continue...     %s", Color_Gray, Color_Reset);
    printf("%c\n", (char)getch());
    Sleep(500);
}

void AP_Add_Patient() {

    patient pat;

    while (1) {

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

        str_func_return_code =
            User_Input_String(pat.code_n, NATIONAL_CODE_SIZE, 1);

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
        pat.wallet = 1000;

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
        for (int i = 0; i < doctor_count; i++) {

            if (strcmp(pat.email, Doctors[i].email) == 0) {
                Error_Management(38);
                flag = 1;
                break;
            }

            if (strcmp(pat.code_n, Doctors[i].code_n) == 0) {
                Error_Management(39);
                flag = 1;
                break;
            }
        }

        if (flag) continue;

        Hash_Function(pat.password);

        // Added Doctor in Struct List
        Patients[patient_count] = pat;
        patient_count++;

        Update_Files();

        printf("\n  %sPatient Added Successfully.%s\n", Color_Gray,
               Color_Reset);
        printf("    %sPatient Name:      %s%s\n", Color_Blue, Color_Reset,
               pat.name);
        printf("    %sNational Code:     %s%s\n", Color_Yellow, Color_Reset,
               pat.code_n);
        printf("    %sPatient Email:     %s%s\n", Color_Yellow, Color_Reset,
               pat.email);
        printf("    %sPatient ID:        %s%d\n", Color_Green, Color_Reset,
               pat.id);

        Sleep(3000);

        break;

    }  // while end
}

void AP_Patients_List() {

    RUN_CLS;

    printf("\n    %sList Of Patients%s\n", Color_Purple, Color_Reset);
    printf("  ------------------------------\n");

    for (int i = 0; i < patient_count; i++) {

        patient pat = Patients[i];

        printf("    %sPatient Name:             %s%s\n", Color_Blue,
               Color_Reset, pat.name);
        printf("    %sNational Code:            %s%s\n", Color_Yellow,
               Color_Reset, pat.code_n);
        printf("    %sPatient Email:            %s%s\n", Color_Yellow,
               Color_Reset, pat.email);
        printf("    %sPatient Password(Hash):   %s%s\n", Color_Red_Dark,
               Color_Reset, pat.password);
        printf("    %sPatient ID:               %s%d\n", Color_Green,
               Color_Reset, pat.id);
        printf("    %sWallet:                   %s%d$\n", Color_Green,
               Color_Reset, pat.wallet);

        printf("  ------------------------------\n");
    }

    printf("\n%sPress a Button to Continue...     %s", Color_Gray, Color_Reset);
    printf("%c\n", (char)getch());
    Sleep(500);
}

void AP_Monthly_Schedule() {

    RUN_CLS;

    // Main_Check_Active_Calendar();

    if (Active_Calendar == 0) {

        while (1) {

            //* set calendar

            RUN_CLS;

            int year_in;
            int month_in;
            int day_in;
            int week_day_in;

            Bar_Status(1, 0);
            printf("Set Calendar (Ctrl+C ~ Back to Admin Panel)\n");

            Bar_Status(1, 0);
            printf("Year: ");

            year_in = User_Input_Number_Range(1, 9999);

            if (year_in == -1) continue;

            if (year_in == -2) {
                printf("Back To Admin Panel\n");
                Sleep(3000);
                return;
            }

            Bar_Status(1, 0);
            printf("Month: ");

            month_in = User_Input_Number_Range(1, 12);

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

            if (x == 'y' || x == 'Y') {

                if (1 <= month_in && month_in <= 6)
                    day_in = 31;

                else if (7 <= month_in && month_in <= 11)
                    day_in = 30;

                else if (month_in == 12) {

                    if ((year_in - 1399) % 4 == 0)
                        day_in = 30;

                    else
                        day_in = 29;
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
            printf(
                "Start WeekDay (0~Sat, 1~Sun, 2~Mon, 3~Tue, 4~Wed, 5~Thu, "
                "6~Fri): ");

            week_day_in = User_Input_Number_Range(0, 6);

            if (week_day_in == -1) continue;

            if (week_day_in == -2) {
                printf("Back To Admin Panel\n");
                Sleep(3000);
                return;
            }

            if (year_in != Date_Login.y ||
                (year_in == Date_Login.y && month_in != Date_Login.m) ||
                (year_in == Date_Login.y && month_in == Date_Login.m &&
                 day_in < Date_Login.d)) {

                if (Date_Login.y == 0 && Date_Login.m == 0 &&
                    Date_Login.d == 0) {

                    Bar_Status(1, 0);
                    printf("Your Login Date Auto Set: %d/%d/%d\n", year_in,
                           month_in, 1);

                    Date_Login.y = year_in;
                    Date_Login.m = month_in;
                    Date_Login.d = 1;

                }

                else {
                    Bar_Status(1, 0);
                    printf(
                        "The Date you Entered with is not Defined in the "
                        "Calendar, Try Again\n");
                    Sleep(3000);
                    continue;
                }
            }

            Date_Start_Cal.y = year_in;
            Date_Start_Cal.m = month_in;
            Date_Start_Cal.d = day_in;
            Date_Start_Cal.week_d = week_day_in;

            Date_Start_Cal_Next.y = 0;
            Date_Start_Cal_Next.m = 0;
            Date_Start_Cal_Next.d = 0;
            Date_Start_Cal_Next.week_d = 0;

            // Main_Reset_Doctors();

            Cal_Off_Date_Count = 0;
            visit_count = 0;
            Active_Calendar = 1;
            Update_Files();

            Bar_Status(1, 0);
            printf("The App Calendar has been Set Successfully. ");
            printf("(Y~%d M~%d D-C~%d W-D~%d)\n", Date_Start_Cal.y,
                   Date_Start_Cal.m, Date_Start_Cal.d, Date_Start_Cal.week_d);
            Sleep(5000);

            break;

        }  // while end ~ set calendar

    }  // if end

    // menu
    while (1) {

        RUN_CLS;

        Print_Calendar(Date_Start_Cal.y, Date_Start_Cal.m, Date_Start_Cal.d,
                       Date_Start_Cal.week_d);

        Print_Off_Date();

        Bar_Status(1, 0);
        printf("Monthly Schedule\n\n");
        printf("    %s1 %s> %sSet Off Date\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s2 %s> %sSet Next Month Calendar\n", Color_Yellow,
               Color_Aqua, Color_Reset);
        printf("    %s3 %s> %sBack Admin Panel\n", Color_Yellow, Color_Aqua,
               Color_Reset);

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

                int get_off_date_record =
                    User_Input_Number_Range(0, Date_Start_Cal.d);

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
                    printf(
                        "Ok :/ You Curse Me, Are you Making Fun of Me :/ ( 30 "
                        "sec Sleep :) )\n ");
                    Sleep(30000);
                    break;
                }

                int i = 0;
                while (i != get_off_date_record) {

                    RUN_CLS;

                    Print_Calendar(Date_Start_Cal.y, Date_Start_Cal.m,
                                   Date_Start_Cal.d, Date_Start_Cal.week_d);

                    Bar_Status(1, 0);
                    printf(
                        "Enter the Data Related to the %s%d Record%s: (Ctrl + "
                        "C ~ Back)\n",
                        Color_Yellow, i + 1, Color_Reset);

                    Bar_Status(1, 0);
                    printf("Enter Date (day): ");

                    int input_day =
                        User_Input_Number_Range(0, Date_Start_Cal.d);

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

                    for (int i = 0; i < Cal_Off_Date_Count; i++) {
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
                    strcpy(Cal_Off_Date[Cal_Off_Date_Count].Reason,
                           temp_char_r);
                    Cal_Off_Date_Count++;

                    Update_Files();

                    Bar_Status(1, 0);
                    printf("Holiday Added Successfully (%s%d Record%s)\n",
                           Color_Green, i + 1, Color_Reset);
                    Sleep(3000);

                    i++;
                }

                break;

            case 2:

                if (Date_Start_Cal_Next.y != 0 && Date_Start_Cal_Next.m != 0 &&
                    Date_Start_Cal_Next.d != 0) {

                    Bar_Status(1, 0);
                    printf(
                        "The next month is already defined. Would you like to "
                        "redefine it(y/n)? (s ~ See): ");

                    int y = getch();
                    printf("%c\n", (char)y);

                    if (y == 's' || y == 'S') {
                        Print_Calendar(
                            Date_Start_Cal_Next.y, Date_Start_Cal_Next.m,
                            Date_Start_Cal_Next.d, Date_Start_Cal_Next.week_d);
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

                while (1) {

                    //* set calendar for next month

                    RUN_CLS;

                    Bar_Status(1, 0);
                    printf(
                        "Set Next Month Calendar (Ctrl+C ~ Back to Admin "
                        "Panel)\n");

                    int year_in = (Date_Start_Cal.m == 12)
                                      ? (Date_Start_Cal.y + 1)
                                      : (Date_Start_Cal.y);

                    int month_in =
                        (Date_Start_Cal.m == 12) ? (1) : (Date_Start_Cal.m + 1);

                    Bar_Status(1, 0);
                    printf("Set Calendar For %d/%d\n", year_in, month_in);

                    Bar_Status(1, 0);
                    printf(
                        "Set Day Auto? (y ~ Yes / To Cancel, Press Another "
                        "Key): ");

                    int x = getch();
                    printf("%c\n", (char)x);

                    int day_in;
                    if (x == 'y' || x == 'Y') {

                        if (1 <= month_in && month_in <= 6)
                            day_in = 31;

                        else if (7 <= month_in && month_in <= 11)
                            day_in = 30;

                        else if (month_in == 12) {

                            if ((year_in - 1399) % 4 == 0)
                                day_in = 30;

                            else
                                day_in = 29;
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
                    printf(
                        "Set Start WeekDay Auto? (y ~ Yes / To Cancel, Press "
                        "Another Key): ");

                    x = getch();
                    printf("%c\n", (char)x);

                    int week_day_in;
                    if (x == 'y' || x == 'Y') {
                        week_day_in =
                            Date_Start_Cal.d % 7 + Date_Start_Cal.week_d;

                        week_day_in = (week_day_in > 6) ? (week_day_in - 7)
                                                        : (week_day_in);

                        Bar_Status(1, 0);
                        printf("Next Month Start Weekday (SYS AUTO): ");
                        Print_WeekDay(week_day_in);
                        printf("\n");
                    }

                    else {

                        Bar_Status(1, 0);
                        printf(
                            "Start WeekDay (0~Sat, 1~Sun, 2~Mon, 3~Tue, 4~Wed, "
                            "5~Thu, 6~Fri): ");

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
                    printf("(Y~%d M~%d D-C~%d W-D~%d)\n", year_in, month_in,
                           day_in, week_day_in);

                    Sleep(3000);

                    RUN_CLS;

                    Print_Calendar(Date_Start_Cal.y, Date_Start_Cal.m,
                                   Date_Start_Cal.d, Date_Start_Cal.week_d);

                    Print_Calendar(Date_Start_Cal_Next.y, Date_Start_Cal_Next.m,
                                   Date_Start_Cal_Next.d,
                                   Date_Start_Cal_Next.week_d);

                    Sleep(5000);

                    break;

                }  // while end ~ set calendar

                break;
        }

        break;
    }
}

void AP_Visits_Schedule() {

    while (1) {

        RUN_CLS;

        Bar_Status(1, 0);
        printf("Visits Schedule:\n\n");
        printf("    %s1 %s> %sAll\n", Color_Yellow, Color_Aqua, Color_Reset);
        printf("    %s2 %s> %sChoose Doctor\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s3 %s> %sBack\n", Color_Yellow, Color_Aqua, Color_Reset);

        Sleep(500);

        Bar_Status(1, 0);
        printf("Select one More: ");

        int AdminInput = User_Input_Number_Range(1, 3);

        Sleep(500);

        if (AdminInput == -1) continue;

        if (AdminInput == -2) {
            // ctrl + c ~ code -2
            printf("Back\n");
            Sleep(3000);
            return;
        }

        if (visit_count == 0 && AdminInput != 3) {
            Bar_Status(1, 0);
            printf("%sThere Are No Visits This Month%s\n", Color_Red_Dark,
                   Color_Reset);
            Sleep(3000);
            return;
        }

        switch (AdminInput) {

            case 1:

                RUN_CLS;

                printf("\n    %sAll Visits%s\n", Color_Purple, Color_Reset);
                printf("  ------------------------------\n");

                for (int i = 0; i < visit_count; i++) {

                    visit vis = Visits[i];

                    printf("    %sVisit ID: %d, Cost: %d$%s\n", Color_Yellow,
                           vis.id, vis.doc_pay_visit, Color_Reset);
                    printf("    %sDate: %d/%d/%d, ", Color_Blue, vis.Date.y,
                           vis.Date.m, vis.Date.d);
                    printf("Time: %d ~ %d%s\n\n", vis.start_time,
                           vis.start_time + 1, Color_Reset);

                    printf(
                        "    %sDoctor Name: %s%s%s\n", Color_Green, Color_Gray,
                        Doctors[PP_Find_Doctor_By_N_Code(vis.doc_code_n)].name,
                        Color_Reset);
                    printf("    %sPatients Name: %s%s%s\n\n", Color_Green,
                           Color_Gray,
                           Patients[PP_Find_Patient_By_N_Code(vis.pat_code_n)]
                               .name,
                           Color_Reset);

                    if (vis.see_visit) {
                        printf("    %sPrescription: %s%s\n", Color_Aqua,
                               Color_Reset, vis.Prescription);
                    }

                    else {
                        printf("    The Doctor did not Check this Visit!\n");
                    }

                    printf("  ------------------------------\n");
                }

                printf("\n%sPress a Button to Continue...     %s", Color_Gray,
                       Color_Reset);
                printf("%c\n", (char)getch());
                Sleep(500);
                break;

            case 2:

                RUN_CLS;

                printf("\n    %s ID | Name\n%s", Color_Gray, Color_Reset);
                printf("    %s---------------------------%s\n", Color_Gray,
                       Color_Reset);

                for (int i = 0; i < doctor_count; i++) {
                    printf("    %3d %s|%s %s\n", i + 1, Color_Gray, Color_Reset,
                           Doctors[i].name);
                }

                printf("    %s---------------------------%s\n\n", Color_Gray,
                       Color_Reset);

                Sleep(500);

                Bar_Status(1, 0);
                printf("Select one More: ");

                int DocSelect = User_Input_Number_Range(1, doctor_count);

                Sleep(500);

                if (DocSelect == -1) continue;

                if (DocSelect == -2) {
                    // ctrl + c ~ code -2
                    printf("Back\n");
                    Sleep(3000);
                    return;
                }

                DocSelect--;

                RUN_CLS;

                printf("\n    %sAll Visits ~ Doctor: %s %s\n", Color_Purple,
                       Doctors[DocSelect].name, Color_Reset);
                printf("  ------------------------------\n");

                int not_found = 1;

                for (int i = 0; i < visit_count; i++) {

                    visit vis = Visits[i];

                    if (strcmp(vis.doc_code_n, Doctors[DocSelect].code_n) ==
                        0) {

                        not_found = 0;

                        printf("    %sVisit ID: %d, Cost: %d$%s\n",
                               Color_Yellow, vis.id, vis.doc_pay_visit,
                               Color_Reset);
                        printf("    %sDate: %d/%d/%d, ", Color_Blue, vis.Date.y,
                               vis.Date.m, vis.Date.d);
                        printf("Time: %d ~ %d%s\n\n", vis.start_time,
                               vis.start_time + 1, Color_Reset);

                        printf(
                            "    %sPatients Name: %s%s%s\n\n", Color_Green,
                            Color_Gray,
                            Patients[PP_Find_Patient_By_N_Code(vis.pat_code_n)]
                                .name,
                            Color_Reset);

                        if (vis.see_visit) {
                            printf("    %sPrescription: %s%s\n", Color_Aqua,
                                   Color_Reset, vis.Prescription);
                        }

                        else {
                            printf(
                                "    The Doctor did not Check this Visit!\n");
                        }

                        printf("  ------------------------------\n");
                    }
                }

                if (not_found) {

                    printf(
                        "    %sThere Are no Visits for Doctor %s This "
                        "Month%s\n",
                        Color_Red_Dark, Doctors[DocSelect].name, Color_Reset);
                }

                printf("\n%sPress a Button to Continue...     %s", Color_Gray,
                       Color_Reset);
                printf("%c\n", (char)getch());
                Sleep(500);
                break;

            case 3:
                Bar_Status(1, 0);
                printf("Back\n");
                Sleep(3000);
                return;
                break;

        }  // switch end

        break;

    }  // while end
}

void Doctor_Panel(int doc_login_id) {

    while (1) {

        RUN_CLS;

        Bar_Status(2, doc_login_id);
        printf("Doctor Panel (Wallet: %d$)\n\n", Doctors[doc_login_id].wallet);
        printf("    %s1 %s> %sDetermining The Shifts\n", Color_Yellow,
               Color_Aqua, Color_Reset);
        printf("    %s2 %s> %sSee Calendar\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s3 %s> %sReserved Visits\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s4 %s> %sPatient Prescription\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s5 %s> %sRent Payment\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s6 %s> %sVisirs Payment\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s7 %s> %sExit ~ Logout\n", Color_Yellow, Color_Aqua,
               Color_Reset);

        Sleep(500);

        Bar_Status(2, doc_login_id);
        printf("Select one More: ");

        int DocInput = User_Input_Number_Range(1, 7);

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
                RUN_CLS;
                DP_Print_Calendar(doc_login_id, Date_Start_Cal.y,
                                  Date_Start_Cal.m, Date_Start_Cal.d,
                                  Date_Start_Cal.week_d);
                DP_Print_Work_Time(doc_login_id);

                printf("\n%sPress a Button to Continue...     %s", Color_Gray,
                       Color_Reset);
                printf("%c\n", (char)getch());

                Sleep(500);
                break;

            case 3:
                DP_Reserved_Visits(doc_login_id);
                break;

            case 4:
                DP_Patient_Prescription(doc_login_id);
                break;

            case 5:
                DP_Rent_Payment(doc_login_id);
                break;

            case 6:
                DP_Visirs_Payment(doc_login_id);
                break;

            case 7:
                Bar_Status(2, doc_login_id);
                printf("logout Successful.\n");
                Sleep(2000);
                return;
                break;

        }  // switch end

    }  // while end
}

void DP_Determining_Shifts(int doc_id) {

    while (1) {

        RUN_CLS;

        DP_Print_Calendar(doc_id, Date_Start_Cal.y, Date_Start_Cal.m,
                          Date_Start_Cal.d, Date_Start_Cal.week_d);

        DP_Print_Work_Time(doc_id);

        Bar_Status(2, doc_id);
        printf("Determining The Shifts\n\n");
        printf("    %s1 %s> %sSet For a Day\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s2 %s> %sRecurring Days in The Month\n", Color_Yellow,
               Color_Aqua, Color_Reset);
        printf("    %s3 %s> %sBack\n", Color_Yellow, Color_Aqua, Color_Reset);

        Sleep(500);

        Bar_Status(2, doc_id);
        printf("Select one More: ");

        int DocInput = User_Input_Number_Range(1, 3);

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

                while (1) {

                    RUN_CLS;

                    Bar_Status(2, doc_id);
                    printf("Set For a Day (Ctrl+C ~ Back)\n");

                    DP_Print_Calendar(doc_id, Date_Start_Cal.y,
                                      Date_Start_Cal.m, Date_Start_Cal.d,
                                      Date_Start_Cal.week_d);

                    DP_Print_Work_Time(doc_id);

                    Bar_Status(2, doc_id);
                    printf("Enter Your Desired Day: ");

                    int input_day =
                        User_Input_Number_Range(1, Date_Start_Cal.d);

                    Sleep(500);

                    if (input_day == -1) continue;

                    if (input_day == -2) {
                        printf("Back.\n");
                        Sleep(2000);
                        return;
                    }

                    int try_again = 0;

                    // holiday
                    for (int i = 0; i < Cal_Off_Date_Count; i++) {

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

                    int dub = -1;

                    for (int i = 0;
                         i < Doctors[doc_id].time_work.count_time_work; i++) {

                        if (input_day ==
                            Doctors[doc_id].time_work.date_time_work_arr[i].d) {

                            Bar_Status(2, doc_id);
                            printf(
                                "This day has already been defined. Redefine "
                                "(y/n)? ");

                            int x = getch();
                            printf("%c\n", (char)x);

                            if (x == 'y' || x == 'Y') {
                                dub = i;
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
                                printf(
                                    "The Hour Entered is Outside the Working "
                                    "Time (Thu - 8 ~ 13)\n");
                                Sleep(5000);
                                try_again = 1;
                                break;
                            }
                        }
                    }

                    if (try_again) continue;

                    if (start_time < 8 || end_time > 21) {

                        Bar_Status(2, doc_id);
                        printf(
                            "The Hour Entered is Outside the Working Time (Sat "
                            "~ Wed - 8 ~ 21)\n");
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
                    if (dub != -1) tmp = dub;

                    Doctors[doc_id].time_work.date_time_work_arr[tmp].d =
                        input_day;
                    Doctors[doc_id].time_work.date_time_work_arr[tmp].m =
                        Date_Start_Cal.m;
                    Doctors[doc_id].time_work.date_time_work_arr[tmp].y =
                        Date_Start_Cal.y;

                    Doctors[doc_id].time_work.start_time_work_arr[tmp] =
                        start_time;
                    Doctors[doc_id].time_work.end_time_work_arr[tmp] = end_time;

                    if (dub == -1) Doctors[doc_id].time_work.count_time_work++;

                    Update_Files();

                    Bar_Status(2, doc_id);
                    printf("Business Hours have been Successfully Added\n");
                    Sleep(5000);

                    break;

                }  // while end

                break;

            case 2:

                while (1) {

                    RUN_CLS;

                    Bar_Status(2, doc_id);
                    printf("Recurring Days in The Month (Ctrl+C ~ Back)\n");

                    DP_Print_Calendar(doc_id, Date_Start_Cal.y,
                                      Date_Start_Cal.m, Date_Start_Cal.d,
                                      Date_Start_Cal.week_d);

                    DP_Print_Work_Time(doc_id);

                    Bar_Status(2, doc_id);
                    printf(
                        "Select WeekDay (0~Sat, 1~Sun, 2~Mon, 3~Tue, 4~Wed, "
                        "5~Thu): ");

                    int input_week = User_Input_Number_Range(0, 5);

                    Sleep(500);

                    if (input_week == -1) continue;

                    if (input_week == -2) {
                        printf("Back.\n");
                        Sleep(2000);
                        return;
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

                    if (input_week == 5) {

                        if (start_time < 8 || end_time > 13) {

                            Bar_Status(2, doc_id);
                            printf(
                                "The Hour Entered is Outside the Working Time "
                                "(Thu - 8 ~ 13)\n");
                            Sleep(5000);
                            continue;
                        }

                    }

                    else {

                        if (start_time < 8 || end_time > 21) {

                            Bar_Status(2, doc_id);
                            printf(
                                "The Hour Entered is Outside the Working Time "
                                "(Sat ~ Wed - 8 ~ 21)\n");
                            Sleep(5000);
                            continue;
                        }
                    }

                    if (start_time >= end_time) {
                        Bar_Status(2, doc_id);
                        printf("The Start and End Times Do Not Match\n");
                        Sleep(5000);
                        continue;
                    }

                    int tmp_day = 1 + (input_week - Date_Start_Cal.week_d);

                    for (; tmp_day <= Date_Start_Cal.d; tmp_day += 7) {

                        if (tmp_day >= 1) {

                            int try_again = 0;

                            // holiday
                            for (int i = 0; i < Cal_Off_Date_Count; i++) {

                                if (Cal_Off_Date[i].Date.d == tmp_day) {

                                    Bar_Status(2, doc_id);
                                    printf(
                                        "This Day is a Holiday! (%d) Skip This "
                                        "Day.\n",
                                        tmp_day);
                                    Sleep(1500);

                                    try_again = 1;
                                    break;
                                }
                            }

                            if (try_again) continue;

                            int dub = -1;

                            for (int i = 0;
                                 i < Doctors[doc_id].time_work.count_time_work;
                                 i++) {

                                if (tmp_day ==
                                    Doctors[doc_id]
                                        .time_work.date_time_work_arr[i]
                                        .d) {

                                    Bar_Status(2, doc_id);
                                    printf(
                                        "This day has already been defined. "
                                        "SYSTEM: Redefine It (%d)\n",
                                        tmp_day);
                                    Sleep(1500);
                                    dub = i;
                                    break;
                                }
                            }

                            int tmp = Doctors[doc_id].time_work.count_time_work;
                            if (dub != -1) tmp = dub;

                            Doctors[doc_id]
                                .time_work.date_time_work_arr[tmp]
                                .d = tmp_day;
                            Doctors[doc_id]
                                .time_work.date_time_work_arr[tmp]
                                .m = Date_Start_Cal.m;
                            Doctors[doc_id]
                                .time_work.date_time_work_arr[tmp]
                                .y = Date_Start_Cal.y;

                            Doctors[doc_id].time_work.start_time_work_arr[tmp] =
                                start_time;
                            Doctors[doc_id].time_work.end_time_work_arr[tmp] =
                                end_time;

                            if (dub == -1)
                                Doctors[doc_id].time_work.count_time_work++;

                        }  // if end

                    }  // for day end

                    Update_Files();

                    Bar_Status(2, doc_id);
                    printf("Business Hours have been Successfully Added\n");
                    Sleep(5000);

                    break;

                }  // while end

                break;

            case 3:
                Bar_Status(2, doc_id);
                printf("Back.\n");
                Sleep(2000);
                return;
                break;

        }  // switch end

        break;

    }  // while end
}

void DP_Print_Work_Time(int doc_id) {

    if (Doctors[doc_id].time_work.count_time_work == 0) return;

    printf("\n");
    printf("    %sDate ~ Start <=> End  ||  Date ~ Start <=> End\n%s",
           Color_Gray, Color_Reset);

    int flag = 1;

    for (int i = 0; i < Doctors[doc_id].time_work.count_time_work; i++) {

        if (flag) {
            printf("    ");
        }

        printf("%s%3d%s", Color_Green,
               Doctors[doc_id].time_work.date_time_work_arr[i].d, Color_Reset);

        printf("%s  ~ %s", Color_Gray, Color_Reset);

        printf("%s%3d   --- ", Color_Blue,
               Doctors[doc_id].time_work.start_time_work_arr[i]);

        printf("%3d%s", Doctors[doc_id].time_work.end_time_work_arr[i],
               Color_Reset);

        if (flag) {
            printf("%s  ||  %s", Color_Gray, Color_Reset);
            flag = 0;
        }

        else {
            printf("\n");
            flag = 1;
        }
    }

    printf("\n");
}

void DP_Print_Calendar(int doc_id, int y, int m, int d, int week_d) {

    printf("\n    %s>>> %s # ", Color_Green, Doctors[doc_id].name);

    Print_Month(m);

    printf(" ~ %d %s\n", y, Color_Reset);

    printf("    %s--- --- --- --- --- --- ---%s\n", Color_Gray, Color_Reset);
    printf("    Sat Sun Mon Tue Wed Thu %sFri%s\n", Color_Red, Color_Reset);
    printf("    %s--- --- --- --- --- --- ---%s\n", Color_Gray, Color_Reset);

    printf("    ");

    int today = Date_Login.d;
    int month = Date_Login.m;

    for (int i = 0; i < week_d; i++) {
        printf("    ");
    }

    for (int i = 1; i <= d; i++) {

        if (week_d == 6 && i == today && m == month)
            printf("%s*%2d%s", Color_Red, i, Color_Reset);

        else if (week_d == 6)
            printf("%s%3d%s", Color_Red, i, Color_Reset);

        int found_flag = 0;

        // off date
        for (int j = 0; j < Cal_Off_Date_Count; j++) {

            if (Cal_Off_Date[j].Date.m == m && Cal_Off_Date[j].Date.y == y) {

                if (Cal_Off_Date[j].Date.d == i) {

                    if (i == today && m == month)
                        printf("%s*%2d%s", Color_Red_Dark, i, Color_Reset);

                    else
                        printf("%s%3d%s", Color_Red_Dark, i, Color_Reset);

                    found_flag = 1;
                    break;
                }
            }
        }

        // doc shift
        for (int j = 0; j < Doctors[doc_id].time_work.count_time_work; j++) {

            if (Doctors[doc_id].time_work.date_time_work_arr[j].d == i) {

                if (i == today && m == month)
                    printf("%s*%2d%s", Color_Green, i, Color_Reset);

                else
                    printf("%s%3d%s", Color_Green, i, Color_Reset);

                found_flag = 1;
                break;
            }
        }

        if (found_flag == 0 && week_d != 6 && i == today && m == month)
            printf("*%2d", i);

        else if (found_flag == 0 && week_d != 6)
            printf("%3d", i);

        week_d++;

        if (week_d > 6) {
            printf("\n    ");
            week_d = 0;
        }

        else
            printf(" ");
    }

    printf("\n");
}

void DP_Visirs_Payment(int doc_id) {

    while (1) {

        RUN_CLS;

        Bar_Status(2, doc_id);

        if (Doctors[doc_id].visit_pay == 0) {
            printf("You Have Not Set Up Paying Visitors\n");
            Sleep(1000);
        }

        else {

            printf(
                "Paying Visitors Are Already Set Up, Reset It? (%d$) (y/n): ",
                Doctors[doc_id].visit_pay);

            int x = getch();
            printf("%c\n", (char)x);

            if (x == 'y' || x == 'Y') {
                Sleep(1000);
            }

            else {
                Bar_Status(2, doc_id);
                printf("Back.\n");
                Sleep(2000);
                return;
            }
        }

        Sleep(500);

        Bar_Status(2, doc_id);
        printf("Set Up Paying Visitors (10 ~ 50): ");

        int PayInput = User_Input_Number_Range(10, 50);

        Sleep(500);

        if (PayInput == -1) continue;

        if (PayInput == -2) {
            // ctrl + c ~ code -2
            printf("Back.\n");
            Sleep(2000);
            return;
        }

        Doctors[doc_id].visit_pay = PayInput;
        Update_Files();

        Bar_Status(2, doc_id);
        printf("Changes Applied Successfully\n");
        Sleep(3000);

        break;

    }  // while end
}

void DP_Rent_Payment(int doc_id) {

    while (1) {

        RUN_CLS;

        Bar_Status(2, doc_id);
        printf("Rent Payment ~ Wallet: %d$, Ext: %s\n\n",
               Doctors[doc_id].wallet,
               (Doctors[doc_id].pay_rent_ext) ? ("True") : ("False"));
        printf("    %s1 %s> %sExtension\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s2 %s> %sPayment\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s3 %s> %sBack\n", Color_Yellow, Color_Aqua, Color_Reset);

        Sleep(500);

        Bar_Status(2, doc_id);
        printf("Select one More: ");

        int RentInput = User_Input_Number_Range(1, 3);

        Sleep(500);

        if (RentInput == -1) continue;

        if (RentInput == -2) {
            // ctrl + c ~ code -2
            printf("Back\n");
            Sleep(3000);
            return;
        }

        switch (RentInput) {

            case 1:

                if (Doctors[doc_id].pay_rent_ext == 0) {

                    if (Doctors[doc_id].pay_not_month != 0) {

                        Bar_Status(2, doc_id);
                        printf(
                            "This Month's Payment was Transferred to the Next "
                            "Month\n");
                        Doctors[doc_id].pay_rent_ext = 1;

                        int d_l_m = Date_Start_Cal.m + 1;
                        int d_l_y = Date_Start_Cal.y;

                        if (d_l_m > 12) {
                            d_l_m -= 12;
                            d_l_y += 1;
                        }

                        Doctors[doc_id].pay_deadline.m = d_l_m;
                        Doctors[doc_id].pay_deadline.y = d_l_y;

                        Update_Files();

                    }

                    else {

                        Bar_Status(2, doc_id);
                        printf("This Month's Money has Already Been Paid\n");
                    }

                }

                else {

                    if (Doctors[doc_id].pay_not_month == 1) {
                        Bar_Status(2, doc_id);
                        printf("This Option Was Used This Month\n");
                    }

                    else {
                        Bar_Status(2, doc_id);
                        printf("This Option Was Used Last Month\n");
                    }
                }

                Sleep(5000);
                break;

            case 2:

                if (Doctors[doc_id].pay_rent_ext == 1 &&
                    Doctors[doc_id].pay_not_month == 1) {
                    Bar_Status(2, doc_id);
                    printf(
                        "This Month's Payment is transferred to the Next "
                        "Month. Come Back Next Month\n");
                }

                else {
                    Bar_Status(2, doc_id);
                    printf("Your debt is %d$, Do you Want to Pay? (y/n) ",
                           Doctors[doc_id].pay_total);

                    int x = getch();
                    printf("%c\n", x);

                    if (x == 'y' || x == 'Y') {

                        if (Doctors[doc_id].wallet <
                            Doctors[doc_id].pay_total) {

                            Bar_Status(2, doc_id);
                            printf("You Have no Money. ¯\\_(ツ)_/¯\n");

                        }

                        else {

                            Doctors[doc_id].wallet -= Doctors[doc_id].pay_total;
                            Doctors[doc_id].pay_not_month = 0;
                            Doctors[doc_id].pay_rent_ext = 0;
                            Doctors[doc_id].pay_total = 0;

                            int d_l_m = Date_Start_Cal.m + 1;
                            int d_l_y = Date_Start_Cal.y;

                            if (d_l_m > 12) {
                                d_l_m -= 12;
                                d_l_y += 1;
                            }

                            Doctors[doc_id].pay_deadline.m = d_l_m;
                            Doctors[doc_id].pay_deadline.y = d_l_y;

                            Update_Files();

                            Bar_Status(2, doc_id);
                            printf("%sPayment was Successful%s ~ Wallet: %d$\n",
                                   Color_Green, Color_Reset,
                                   Doctors[doc_id].wallet);
                        }

                    }

                    else {
                        Bar_Status(2, doc_id);
                        printf("The Payment was Cancelled\n");
                    }
                }

                Sleep(5000);
                break;

            case 3:
                Bar_Status(2, doc_id);
                printf("Back\n");
                Sleep(3000);
                return;
                break;

        }  // switch end

        break;

    }  // while end
}

void DP_Reserved_Visits(int doc_id) {

    while (1) {

        if (visit_count == 0) {
            Bar_Status(2, doc_id);
            printf("%sThere Are No Visits This Month%s\n", Color_Red_Dark,
                   Color_Reset);
            Sleep(3000);
            return;
        }

        RUN_CLS;

        int tmp_day = 1;
        int tmp_day2 = 1 + (6 - Date_Start_Cal.week_d);

        while (Date_Login.d > tmp_day2) {

            tmp_day = tmp_day2 + 1;

            if (tmp_day2 + 7 > Date_Start_Cal.d) {
                tmp_day2 = Date_Start_Cal.d;
            }

            else {
                tmp_day2 += 7;
            }
        }

        printf("\n    %sReserved Visits ~ %d <=> %d %s\n", Color_Purple,
               tmp_day, tmp_day2, Color_Reset);
        printf("  ------------------------------\n");

        int not_found = 1;
        int all_visit_week[7];
        int count_visit_week = 0;

        for (; tmp_day <= tmp_day2; tmp_day++) {

            for (int i = 0; i < visit_count; i++) {

                visit vis = Visits[i];

                if (strcmp(vis.doc_code_n, Doctors[doc_id].code_n) == 0) {

                    if (tmp_day == vis.Date.d &&
                        vis.Date.m == Date_Start_Cal.m &&
                        vis.Date.y == Date_Start_Cal.y) {

                        not_found = 0;

                        printf("    %sVisit ID: %d, Cost: %d$%s\n",
                               Color_Yellow, vis.id, vis.doc_pay_visit,
                               Color_Reset);
                        printf("    %sDate: %d/%d/%d, ", Color_Blue, vis.Date.y,
                               vis.Date.m, vis.Date.d);
                        printf("Time: %d ~ %d%s\n\n", vis.start_time,
                               vis.start_time + 1, Color_Reset);

                        printf(
                            "    %sPatients Name: %s%s%s\n\n", Color_Green,
                            Color_Gray,
                            Patients[PP_Find_Patient_By_N_Code(vis.pat_code_n)]
                                .name,
                            Color_Reset);

                        if (vis.see_visit) {
                            printf("    %sPrescription: %s%s\n", Color_Aqua,
                                   Color_Reset, vis.Prescription);
                        }

                        else {
                            printf(
                                "    The Doctor did not Check this Visit!\n");
                        }

                        printf("  ------------------------------\n");

                        all_visit_week[count_visit_week] = vis.id;
                        count_visit_week++;
                    }
                }
            }
        }

        if (not_found) {
            Bar_Status(2, doc_id);
            printf("There Are no Visits for Doctor %s This Week\n",
                   Doctors[doc_id].name);
            Sleep(3000);
            return;
        }

        Sleep(500);

        Bar_Status(2, doc_id);
        printf(
            "To See Patient Info, Select one More (ID Visit)(Ctrl+C ~ Back): ");

        int ID_Visit = User_Input_Number_Range(0, visit_count - 1);

        Sleep(500);

        if (ID_Visit == -1) continue;

        if (ID_Visit == -2) {
            // ctrl + c ~ code -2
            printf("Back\n");
            Sleep(3000);
            return;
        }

        not_found = 1;
        for (int i = 0; i < count_visit_week; i++) {

            if (ID_Visit == all_visit_week[i]) {
                not_found = 0;
                break;
            }
        }

        if (not_found) {
            Bar_Status(2, doc_id);
            printf("Enter The Number of One of the Above Visits\n");
            Sleep(3000);
            continue;
        }

        RUN_CLS;

        int pat_id = PP_Find_Patient_By_N_Code(Visits[ID_Visit].pat_code_n);

        patient pat = Patients[pat_id];

        printf("\n    %sPatient Info ~ %s%s\n", Color_Purple,
               Patients[pat_id].name, Color_Reset);
        printf("  ------------------------------\n");

        printf("    %sPatient Name:      %s%s\n", Color_Blue, Color_Reset,
               pat.name);
        printf("    %sNational Code:     %s%s\n", Color_Yellow, Color_Reset,
               pat.code_n);
        printf("    %sPatient Email:     %s%s\n", Color_Yellow, Color_Reset,
               pat.email);
        printf("    %sPatient ID:        %s%d\n", Color_Green, Color_Reset,
               pat.id);

        printf("  ------------------------------\n");

        printf("\n%sPress a Button to Continue...     %s", Color_Gray,
               Color_Reset);
        printf("%c\n", (char)getch());
        Sleep(500);

        break;

    }  // while end
}

void DP_Patient_Prescription(int doc_id) {

    while (1) {

        if (visit_count == 0) {
            Bar_Status(2, doc_id);
            printf("%sThere Are No Visits This Month%s\n", Color_Red_Dark,
                   Color_Reset);
            Sleep(3000);
            return;
        }

        RUN_CLS;

        printf("\n    %sPatient Prescription%s\n", Color_Purple, Color_Reset);
        printf("  ------------------------------\n");

        int not_found = 1;

        for (int i = 0; i < visit_count; i++) {

            if (strcmp(Visits[i].doc_code_n, Doctors[doc_id].code_n) != 0)
                continue;

            not_found = 0;

            visit vis = Visits[i];

            char pat_name[NAME_SIZE];

            strcpy(pat_name,
                   Patients[PP_Find_Patient_By_N_Code(vis.pat_code_n)].name);

            if (vis.see_visit == 0) {
                printf(
                    "    %sID: %d, Name: %s, Date: %d/%d/%d, Time: %d ~ %d%s\n",
                    Color_Green, vis.id, pat_name, vis.Date.y, vis.Date.m,
                    vis.Date.d, vis.start_time, vis.start_time + 1,
                    Color_Reset);
            }

            else {
                printf(
                    "    %sID: %d, Name: %s, Date: %d/%d/%d, Time: %d ~ %d --- "
                    "Done%s\n",
                    Color_Gray, vis.id, pat_name, vis.Date.y, vis.Date.m,
                    vis.Date.d, vis.start_time, vis.start_time + 1,
                    Color_Reset);
            }

        }  // end for

        if (not_found) {
            Bar_Status(2, doc_id);
            printf("There Are no Visits for Doctor %s This Month\n",
                   Doctors[doc_id].name);
            Sleep(3000);
            return;
        }

        Sleep(500);

        Bar_Status(2, doc_id);
        printf("Select one More (ID): ");

        int IDSelect = User_Input_Number_Range(0, visit_count - 1);

        Sleep(500);

        if (IDSelect == -1) continue;

        if (IDSelect == -2) {
            // ctrl + c ~ code -2
            printf("Back\n");
            Sleep(3000);
            return;
        }

        if (strcmp(Visits[IDSelect].doc_code_n, Doctors[doc_id].code_n) != 0) {

            Bar_Status(2, doc_id);
            printf("Enter The Number of One of the Above Visits\n");
            Sleep(3000);
            continue;
        }

        if (Visits[IDSelect].see_visit == 1) {

            Bar_Status(2, doc_id);
            printf("The Prescription of this visit has Already been Written\n");
            Sleep(3000);
            continue;
        }

        Bar_Status(2, doc_id);
        printf("What is Your Prescription: ");

        char InPrescription[PRESCRIPTION_SIZE];

        int IntPre = User_Input_String(InPrescription, PRESCRIPTION_SIZE, 0);

        if (IntPre == -1) continue;

        if (IntPre == -2) {
            printf("Back\n");
            Sleep(3000);
            return;
        }

        strcpy(Visits[IDSelect].Prescription, InPrescription);
        Visits[IDSelect].see_visit = 1;

        Doctors[doc_id].wallet += Visits[IDSelect].doc_pay_visit;

        Update_Files();

        Bar_Status(2, doc_id);
        printf("Your Prescription has been Successfully Registered\n");
        Sleep(5000);

        break;

    }  // while end
}

void Patient_Panel(int pat_login_id) {

    while (1) {

        RUN_CLS;

        Bar_Status(3, pat_login_id);
        printf("Patient Panel: (Wallet: %d$)\n\n",
               Patients[pat_login_id].wallet);
        printf("    %s1 %s> %sBook an Appointment\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s2 %s> %sCancel an Appointment\n", Color_Yellow,
               Color_Aqua, Color_Reset);
        printf("    %s3 %s> %sAll Appointments\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s4 %s> %sPrescriptions\n", Color_Yellow, Color_Aqua,
               Color_Reset);
        printf("    %s5 %s> %sExit ~ Logout\n", Color_Yellow, Color_Aqua,
               Color_Reset);

        Sleep(500);

        Bar_Status(3, pat_login_id);
        printf("Select one More: ");

        int PatInput = User_Input_Number_Range(1, 5);

        Sleep(500);

        if (PatInput == -1) continue;

        if (PatInput == -2) {
            // ctrl + c ~ code -2
            printf("Run Exit Function.\n");
            Exit_Function(3, 0, pat_login_id);
        }

        switch (PatInput) {

            case 1:
                PP_Book_An_Appointment(pat_login_id);
                break;

            case 2:
                PP_Cancel_An_Appointment(pat_login_id);
                break;

            case 3:
                PP_All_Appointments(pat_login_id);
                break;

            case 4:
                PP_Prescriptions(pat_login_id);
                break;

            case 5:
                Bar_Status(3, pat_login_id);
                printf("logout Successful.\n");
                Sleep(2000);
                return;
                break;

        }  // switch end

    }  // while end
}

void PP_Book_An_Appointment(int pat_id) {

    while (1) {

        RUN_CLS;

        Bar_Status(3, pat_id);
        printf("Book an Appointment ~ Wallet: %d$\n\n",
               Patients[pat_id].wallet);

        printf("    %s ID | Name\n%s", Color_Gray, Color_Reset);
        printf("    %s---------------------------%s\n", Color_Gray,
               Color_Reset);

        for (int i = 0; i < doctor_count; i++) {
            printf("    %3d %s|%s %s\n", i + 1, Color_Gray, Color_Reset,
                   Doctors[i].name);
        }

        printf("    %s---------------------------%s\n\n", Color_Gray,
               Color_Reset);

        Sleep(500);

        Bar_Status(3, pat_id);
        printf("Select one More: ");

        int DocSelect = User_Input_Number_Range(1, doctor_count);

        Sleep(500);

        if (DocSelect == -1) continue;

        if (DocSelect == -2) {
            // ctrl + c ~ code -2
            printf("Back\n");
            Sleep(3000);
            return;
        }

        DocSelect--;

        if (Doctors[DocSelect].time_work.count_time_work == 0) {
            Bar_Status(3, pat_id);
            printf("The Doctor dont Set Shift\n");
            Sleep(3000);
            continue;
        }

        if (Doctors[DocSelect].visit_pay == 0) {
            Bar_Status(3, pat_id);
            printf("The Doctor dont Set Visit Pay\n");
            Sleep(3000);
            continue;
        }

        while (1) {

            RUN_CLS;

            printf("    %sDoctor Shifts ~%s %s\n", Color_Gray, Color_Reset,
                   Doctors[DocSelect].name);

            DP_Print_Calendar(DocSelect, Date_Start_Cal.y, Date_Start_Cal.m,
                              Date_Start_Cal.d, Date_Start_Cal.week_d);

            Sleep(500);

            Bar_Status(3, pat_id);
            printf("Select one Green Day: ");

            int DaySelect = User_Input_Number_Range(1, 31);

            if (DaySelect == -1) continue;

            if (DaySelect == -2) {
                printf("Back\n");
                Sleep(3000);
                return;
            }

            int find_work_i = -1;
            for (int i = 0; i < Doctors[DocSelect].time_work.count_time_work;
                 i++) {

                if (DaySelect ==
                    Doctors[DocSelect].time_work.date_time_work_arr[i].d) {

                    find_work_i = i;
                    break;
                }
            }

            if (find_work_i == -1) {

                Bar_Status(3, pat_id);
                printf("This Day the Doctor is Not in the Office\n");
                Sleep(3000);

                continue;
            }

            int start_time_work =
                Doctors[DocSelect].time_work.start_time_work_arr[find_work_i];
            int end_time_work =
                Doctors[DocSelect].time_work.end_time_work_arr[find_work_i];

            RUN_CLS;

            Bar_Status(3, pat_id);
            printf("Doctor: %s, Day: %d, Cost: %d$\n\n",
                   Doctors[DocSelect].name, DaySelect,
                   Doctors[DocSelect].visit_pay);

            for (int tmp = start_time_work; tmp < end_time_work; tmp++) {

                int find_flag = 0;

                for (int i = 0; i < visit_count; i++) {

                    if (PP_Find_Doctor_By_N_Code(Visits[i].doc_code_n) ==
                        DocSelect) {

                        if (Visits[i].Date.d == DaySelect &&
                            Visits[i].Date.m == Date_Start_Cal.m &&
                            Visits[i].Date.y == Date_Start_Cal.y) {

                            if (Visits[i].start_time == tmp) {

                                find_flag = 1;

                                break;
                            }
                        }
                    }
                }

                if (find_flag)
                    printf("    %s %2d --- %2d ~ Reserved%s\n", Color_Red, tmp,
                           tmp + 1, Color_Reset);

                else
                    printf("    %s %2d --- %2d %s\n", Color_Green, tmp, tmp + 1,
                           Color_Reset);

            }  // for end

            Sleep(500);

            Bar_Status(3, pat_id);
            printf("Choose Your Desired Time: (Hour): ");

            int TimeSelect = User_Input_Number_Range(1, 24);

            if (TimeSelect == -1) continue;

            if (TimeSelect == -2) {
                printf("Back\n");
                Sleep(3000);
                return;
            }

            int find_flag = 0;

            for (int i = 0; i < visit_count; i++) {

                if (PP_Find_Doctor_By_N_Code(Visits[i].doc_code_n) ==
                    DocSelect) {

                    if (Visits[i].Date.d == DaySelect &&
                        Visits[i].Date.m == Date_Start_Cal.m &&
                        Visits[i].Date.y == Date_Start_Cal.y) {

                        if (Visits[i].start_time == TimeSelect) {

                            Bar_Status(3, pat_id);
                            printf("This Time is Reserved\n");
                            Sleep(3000);
                            find_flag = 1;
                            break;
                        }
                    }
                }
            }

            if (find_flag) continue;

            visit vis;

            strcpy(vis.doc_code_n, Doctors[DocSelect].code_n);
            strcpy(vis.pat_code_n, Patients[pat_id].code_n);

            vis.id = visit_count;
            vis.see_visit = 0;
            vis.start_time = TimeSelect;
            vis.Date.d = DaySelect;
            vis.Date.y = Date_Start_Cal.y;
            vis.Date.m = Date_Start_Cal.m;
            vis.doc_pay_visit = Doctors[DocSelect].visit_pay;

            if (Patients[pat_id].wallet < vis.doc_pay_visit * 1.1) {

                Bar_Status(3, pat_id);
                printf("Your Money is not Enough\n");
                Sleep(3000);

            }

            else {

                Patients[pat_id].wallet -= vis.doc_pay_visit * 1.1;

                Visits[visit_count] = vis;
                visit_count++;

                Update_Files();

                Bar_Status(3, pat_id);
                printf(
                    "Your Appointment has been Successfully Booked (%.0lf$ Was "
                    "Paid)\n",
                    Doctors[DocSelect].visit_pay * 1.1);
                Sleep(3000);
            }

            break;

        }  // while end

        break;

    }  // while main end
}

int PP_Find_Doctor_By_N_Code(char n_code[NATIONAL_CODE_SIZE]) {

    for (int i = 0; i < doctor_count; i++) {

        if (strcmp(Doctors[i].code_n, n_code) == 0) {

            return i;
        }
    }

    return -1;
}

int PP_Find_Patient_By_N_Code(char n_code[NATIONAL_CODE_SIZE]) {

    for (int i = 0; i < patient_count; i++) {

        if (strcmp(Patients[i].code_n, n_code) == 0) {

            return i;
        }
    }

    return -1;
}

void PP_Cancel_An_Appointment(int pat_id) {

    while (1) {

        if (visit_count == 0) {
            Bar_Status(3, pat_id);
            printf("%sThere Are No Visits This Month%s\n", Color_Red_Dark,
                   Color_Reset);
            Sleep(3000);
            return;
        }

        RUN_CLS;

        printf("\n    %sCancel An Appointment%s\n", Color_Purple, Color_Reset);
        printf("  ------------------------------\n");

        int not_found = 1;

        for (int i = 0; i < visit_count; i++) {

            visit vis = Visits[i];

            if (strcmp(vis.pat_code_n, Patients[pat_id].code_n) != 0) continue;

            not_found = 0;

            char doc_name[NAME_SIZE];

            strcpy(doc_name,
                   Doctors[PP_Find_Doctor_By_N_Code(vis.doc_code_n)].name);

            if (vis.see_visit == 0) {
                printf(
                    "    %sID: %d, Dr.Name: %s, Date: %d/%d/%d, Time: %d ~ "
                    "%d%s\n",
                    Color_Green, vis.id, doc_name, vis.Date.y, vis.Date.m,
                    vis.Date.d, vis.start_time, vis.start_time + 1,
                    Color_Reset);
            }

            else {
                printf(
                    "    %sID: %d, Dr.Name: %s, Date: %d/%d/%d, Time: %d ~ %d "
                    "--- This Visit is Done%s\n",
                    Color_Gray, vis.id, doc_name, vis.Date.y, vis.Date.m,
                    vis.Date.d, vis.start_time, vis.start_time + 1,
                    Color_Reset);
            }

        }  // for end

        if (not_found) {
            Bar_Status(3, pat_id);
            printf("There Are no Visits for Patient %s This Month\n",
                   Patients[pat_id].name);
            Sleep(3000);
            return;
        }

        Bar_Status(3, pat_id);
        printf("Select one More to Cancel (ID) (Ctrl+C ~ Back): ");

        int IDSelect = User_Input_Number_Range(0, visit_count - 1);

        Sleep(500);

        if (IDSelect == -1) continue;

        if (IDSelect == -2) {
            // ctrl + c ~ code -2
            printf("Back\n");
            Sleep(3000);
            return;
        }

        if (strcmp(Visits[IDSelect].pat_code_n, Patients[pat_id].code_n) != 0) {

            Bar_Status(3, pat_id);
            printf("Enter The Number of One of the Above Visits\n");
            Sleep(3000);
            continue;
        }

        if (Visits[IDSelect].see_visit == 1) {

            Bar_Status(3, pat_id);
            printf("The Prescription of this Visit has Already been Written\n");
            Sleep(3000);
            continue;
        }

        Bar_Status(3, pat_id);
        printf("Your Appointment has been Successfully Cancelled (+%d$)\n",
               Visits[IDSelect].doc_pay_visit / 2);

        Patients[pat_id].wallet += Visits[IDSelect].doc_pay_visit / 2;

        for (int i = IDSelect; i < visit_count - 1; i++) {

            Visits[i] = Visits[i + 1];
            Visits[i].id--;
        }

        visit_count--;

        Update_Files();

        Sleep(3000);
        break;

    }  // while end
}

void PP_All_Appointments(int pat_id) {

    while (1) {

        if (visit_count == 0) {
            Bar_Status(3, pat_id);
            printf("%sThere Are No Visits This Month%s\n", Color_Red_Dark,
                   Color_Reset);
            Sleep(3000);
            return;
        }

        RUN_CLS;

        printf("\n    %sAll Appointments%s\n", Color_Purple, Color_Reset);
        printf("  ------------------------------\n");

        int not_found = 1;

        for (int i = 0; i < visit_count; i++) {

            visit vis = Visits[i];

            if (strcmp(vis.pat_code_n, Patients[pat_id].code_n) == 0) {

                not_found = 0;

                printf("    %sVisit ID: %d, Cost: %d$%s\n", Color_Yellow,
                       vis.id, vis.doc_pay_visit, Color_Reset);
                printf("    %sDate: %d/%d/%d, ", Color_Blue, vis.Date.y,
                       vis.Date.m, vis.Date.d);
                printf("Time: %d ~ %d%s\n\n", vis.start_time,
                       vis.start_time + 1, Color_Reset);

                printf("    %sDoctor Name: %s%s%s\n\n", Color_Green, Color_Gray,
                       Doctors[PP_Find_Doctor_By_N_Code(vis.doc_code_n)].name,
                       Color_Reset);

                if (vis.see_visit) {
                    printf("    %sPrescription: %s%s\n", Color_Aqua,
                           Color_Reset, vis.Prescription);
                }

                else {
                    printf("    The Doctor did not Check this Visit!\n");
                }

                printf("  ------------------------------\n");
            }
        }

        if (not_found) {
            Bar_Status(3, pat_id);
            printf("There Are no Visits for Patients %s This Month\n",
                   Patients[pat_id].name);
            Sleep(3000);
            return;
        }

        Sleep(500);

        Bar_Status(3, pat_id);
        printf(
            "To See Doctor Info, Select one More (ID Visit)(Ctrl+C ~ Back): ");

        int ID_Visit = User_Input_Number_Range(0, visit_count - 1);

        Sleep(500);

        if (ID_Visit == -1) continue;

        if (ID_Visit == -2) {
            // ctrl + c ~ code -2
            printf("Back\n");
            Sleep(3000);
            return;
        }

        if (strcmp(Visits[ID_Visit].pat_code_n, Patients[pat_id].code_n) != 0) {

            Bar_Status(3, pat_id);
            printf("Enter The Number of One of the Above Visits\n");
            Sleep(3000);
            continue;
        }

        RUN_CLS;

        int doc_id = PP_Find_Doctor_By_N_Code(Visits[ID_Visit].doc_code_n);

        doctor doc = Doctors[doc_id];

        printf("\n    %sDoctor Info %s\n", Color_Purple, Color_Reset);
        printf("  ------------------------------\n");

        printf("    %sDoctor Name:      %s%s\n", Color_Blue, Color_Reset,
               doc.name);
        printf("    %sDoctor Email:     %s%s\n", Color_Yellow, Color_Reset,
               doc.email);
        printf("    %sDoctor ID:        %s%d\n", Color_Green, Color_Reset,
               doc.id);
        printf("    %sCost Visit:       %s%d$\n", Color_Green, Color_Reset,
               doc.visit_pay);

        printf("  ------------------------------\n");

        printf("\n\n    %sDoctor Calendar %s\n", Color_Purple, Color_Reset);

        DP_Print_Calendar(doc_id, Date_Start_Cal.y, Date_Start_Cal.m,
                          Date_Start_Cal.d, Date_Start_Cal.week_d);

        printf("\n\n%sPress a Button to Continue...     %s", Color_Gray,
               Color_Reset);
        printf("%c\n", (char)getch());
        Sleep(500);

        break;

    }  // while end
}

void PP_Prescriptions(int pat_id) {

    while (1) {

        if (visit_count == 0) {
            Bar_Status(3, pat_id);
            printf("%sThere Are No Visits This Month%s\n", Color_Red_Dark,
                   Color_Reset);
            Sleep(3000);
            return;
        }

        RUN_CLS;

        printf("\n    %sPrescriptions%s\n", Color_Purple, Color_Reset);
        printf("  ------------------------------\n");

        int not_found = 1;

        for (int i = 0; i < visit_count; i++) {

            visit vis = Visits[i];

            if (strcmp(vis.pat_code_n, Patients[pat_id].code_n) == 0) {

                if (vis.see_visit) {
                    not_found = 0;
                    printf("    %sVisit ID: %d, Cost: %d$%s\n", Color_Yellow,
                           vis.id, vis.doc_pay_visit, Color_Reset);
                    printf("    %sDate: %d/%d/%d, ", Color_Blue, vis.Date.y,
                           vis.Date.m, vis.Date.d);
                    printf("Time: %d ~ %d%s\n\n", vis.start_time,
                           vis.start_time + 1, Color_Reset);
                    printf(
                        "    %sDoctor Name: %s%s%s\n\n", Color_Green,
                        Color_Gray,
                        Doctors[PP_Find_Doctor_By_N_Code(vis.doc_code_n)].name,
                        Color_Reset);
                    printf("    %sPrescription: %s%s\n", Color_Aqua,
                           Color_Reset, vis.Prescription);
                }

                // else {
                //     printf("    %sVisit ID: %d, Cost: %d$%s # ",
                //     Color_Yellow, vis.id, vis.doc_pay_visit, Color_Reset);
                //     printf("%sDate: %d/%d/%d, ", Color_Blue, vis.Date.y,
                //     vis.Date.m, vis.Date.d); printf("Time: %d ~ %d%s\n",
                //     vis.start_time, vis.start_time+1, Color_Reset); printf("
                //     %sDoctor Name: %s%s%s # ", Color_Green, Color_Gray,
                //     Doctors[PP_Find_Doctor_By_N_Code(vis.doc_code_n)].name,
                //     Color_Reset); printf("%sThe Doctor did not Check this
                //     Visit!%s\n", Color_Red_Dark, Color_Reset);
                // }

                printf("  ------------------------------\n");
            }
        }

        if (not_found) {
            Bar_Status(3, pat_id);
            printf("There Are no Prescription for Patients %s This Month\n",
                   Patients[pat_id].name);
            Sleep(3000);
            return;
        }

        printf("\n\n%sPress a Button to Continue...     %s", Color_Gray,
               Color_Reset);
        printf("%c\n", (char)getch());
        Sleep(500);

        break;

    }  // while end
}

void Print_Off_Date() {

    if (Cal_Off_Date_Count == 0) return;

    printf("\n");
    printf("    %sDate ~ Reason\n%s", Color_Gray, Color_Reset);

    for (int i = 0; i < Cal_Off_Date_Count; i++) {

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

    int today = Date_Login.d;
    int month = Date_Login.m;

    for (int i = 0; i < week_d; i++) {
        printf("    ");
    }

    for (int i = 1; i <= d; i++) {

        if (week_d == 6 && i == today && m == month)
            printf("%s*%2d%s", Color_Red, i, Color_Reset);

        else if (week_d == 6)
            printf("%s%3d%s", Color_Red, i, Color_Reset);

        int found_flag = 0;

        for (int j = 0; j < Cal_Off_Date_Count; j++) {

            if (Cal_Off_Date[j].Date.m == m && Cal_Off_Date[j].Date.y == y) {

                if (Cal_Off_Date[j].Date.d == i) {

                    if (i == today && m == month)
                        printf("%s*%2d%s", Color_Red_Dark, i, Color_Reset);

                    else
                        printf("%s%3d%s", Color_Red_Dark, i, Color_Reset);

                    found_flag = 1;
                    break;
                }
            }
        }

        if (found_flag == 0 && week_d != 6 && i == today && m == month)
            printf("*%2d", i);

        else if (found_flag == 0 && week_d != 6)
            printf("%3d", i);

        week_d++;

        if (week_d > 6) {
            printf("\n    ");
            week_d = 0;
        }

        else
            printf(" ");
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

        printf("The file information was read successfully. %d Doctors.\n",
               doctor_count);

    }

    else
        printf("The file does not exist or could not be opened ~ (%s).\n",
               doctor_file_path);

    fclose(fp_Doctor);

    printf("------------------------------\n");

    // Get Patients
    FILE *fp_Patient = fopen(patient_file_path, "rb");

    if (fp_Patient != NULL) {

        while (
            fread(&Patients[patient_count], sizeof(patient), 1, fp_Patient)) {
            patient_count++;
        }

        printf("The file information was read successfully. %d Patients.\n",
               patient_count);

    }

    else
        printf("The file does not exist or could not be opened ~ (%s).\n",
               patient_file_path);

    fclose(fp_Patient);

    printf("------------------------------\n");

    // Get Date
    FILE *fp_Date = fopen(calendar_in_path, "rb");

    if (fp_Date != NULL) {

        Active_Calendar = fread(&Date_Start_Cal_Last, sizeof(date), 1, fp_Date);
        Active_Calendar = fread(&Date_Start_Cal, sizeof(date), 1, fp_Date);
        Active_Calendar = fread(&Date_Start_Cal_Next, sizeof(date), 1, fp_Date);

        printf("The file information was read successfully, Date Start Cal\n");
        printf("Date_Start_Cal_Last: %d ~ %d ~ %d\n", Date_Start_Cal_Last.y,
               Date_Start_Cal_Last.m, Date_Start_Cal_Last.d);
        printf("Date_Start_Cal: %d ~ %d ~ %d\n", Date_Start_Cal.y,
               Date_Start_Cal.m, Date_Start_Cal.d);
        printf("Date_Start_Cal_Next: %d ~ %d ~ %d\n", Date_Start_Cal_Next.y,
               Date_Start_Cal_Next.m, Date_Start_Cal_Next.d);

    }

    else
        printf("The file does not exist or could not be opened ~ (%s).\n",
               calendar_in_path);

    fclose(fp_Date);

    printf("------------------------------\n");

    // Get Off Date
    FILE *fp_Off_Date = fopen(off_date_path, "rb");

    if (fp_Off_Date != NULL) {

        while (fread(&Cal_Off_Date[Cal_Off_Date_Count], sizeof(off_date), 1,
                     fp_Off_Date)) {
            Cal_Off_Date_Count++;
        }

        printf("The file information was read successfully. %d Off Date.\n",
               Cal_Off_Date_Count);

    }

    else
        printf("The file does not exist or could not be opened ~ (%s).\n",
               off_date_path);

    fclose(fp_Off_Date);

    printf("------------------------------\n");

    // Get Visit
    FILE *fp_Visit = fopen(visit_file_path, "rb");

    if (fp_Visit != NULL) {

        while (fread(&Visits[visit_count], sizeof(visit), 1, fp_Visit)) {
            visit_count++;
        }

        printf("The file information was read successfully. %d Visit.\n",
               visit_count);

    }

    else
        printf("The file does not exist or could not be opened ~ (%s).\n",
               visit_file_path);

    fclose(fp_Visit);

    printf("------------------------------\n");

    printf("\n%sPress a Button to Continue...     %s", Color_Gray, Color_Reset);
    printf("%c\n", (char)getch());
    Sleep(500);
}

void Update_Files() {

    // Doctors Update
    FILE *fp_Doctor = fopen(doctor_file_path, "wb");

    if (fp_Doctor != NULL) {

        for (int i = 0; i < doctor_count; i++) {

            fwrite(&Doctors[i], sizeof(doctor), 1, fp_Doctor);
        }
    }

    fclose(fp_Doctor);

    // Patients Update
    FILE *fp_Patient = fopen(patient_file_path, "wb");

    if (fp_Patient != NULL) {

        for (int i = 0; i < patient_count; i++) {

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

        for (int i = 0; i < Cal_Off_Date_Count; i++) {

            fwrite(&Cal_Off_Date[i], sizeof(off_date), 1, fp_off_date);
        }
    }

    fclose(fp_off_date);

    // Visit Update
    FILE *fp_visit = fopen(visit_file_path, "wb");

    if (fp_visit != NULL) {

        for (int i = 0; i < visit_count; i++) {

            fwrite(&Visits[i], sizeof(visit), 1, fp_visit);
        }
    }

    fclose(fp_visit);
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
            printf("      Please Enter a Number in the given Range.%s\n",
                   Color_Reset);
            break;

        case 11:
            printf("%s#Code 1-1%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in Receiving the Number.\n", Color_Gray);
            printf("      Please Enter a NUMBER :/%s\n", Color_Reset);
            break;

        case 12:  // very out of range
            printf("%s#Code 1-2%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in Receiving the Number. Keep Calm :)\n",
                   Color_Gray);
            printf("      Everything is Under Control :)\n");
            printf("      Please Enter a Number in the given Range.%s\n",
                   Color_Reset);
            break;

        case 20:
            printf("%s#Code 2-0%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in receiving UserName.\n", Color_Gray);
            printf("      Please do not Enter a duplicate UserName.%s\n",
                   Color_Reset);
            break;

        case 21:
            printf("%s#Code 2-1%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in receiving String (Number Mode).\n",
                   Color_Gray);
            printf("      Please use Numeric Characters.%s\n", Color_Reset);
            break;

        case 22:
            printf("%s#Code 2-2%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in receiving String.\n", Color_Gray);
            printf(
                "      The Length of the String should not Exceed the "
                "limit.%s\n",
                Color_Reset);
            break;

        case 23:
            printf("%s#Code 2-3%s\n", Color_Yellow, Color_Reset);
            printf("      %sError in receiving PassWord.\n", Color_Gray);
            printf(
                "      The Length of the PassWord should not Exceed the "
                "limit.%s\n",
                Color_Reset);
            break;

        case 30:
            printf("%s#Code 3-0%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Doctor.\n", Color_Gray);
            printf(
                "      Another doctor with this name has already been "
                "registered.%s\n",
                Color_Reset);
            break;

        case 31:
            printf("%s#Code 3-1%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Doctor.\n", Color_Gray);
            printf(
                "      Another doctor with this email has already been "
                "registered.%s\n",
                Color_Reset);
            break;

        case 32:
            printf("%s#Code 3-2%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Doctor.\n", Color_Gray);
            printf(
                "      Another doctor with this national code has already been "
                "registered.%s\n",
                Color_Reset);
            break;

        case 33:
            printf("%s#Code 3-3%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Doctor.\n", Color_Gray);
            printf(
                "      Another patient with this email has already been "
                "registered.%s\n",
                Color_Reset);
            break;

        case 34:
            printf("%s#Code 3-4%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Doctor.\n", Color_Gray);
            printf(
                "      Another patient with this national code has already "
                "been registered.%s\n",
                Color_Reset);
            break;

        case 35:
            printf("%s#Code 3-5%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding void Patient.\n", Color_Gray);
            printf(
                "      Another patient with this name has already been "
                "registered.%s\n",
                Color_Reset);
            break;

        case 36:
            printf("%s#Code 3-6%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Patient.\n", Color_Gray);
            printf(
                "      Another patient with this email has already been "
                "registered.%s\n",
                Color_Reset);
            break;

        case 37:
            printf("%s#Code 3-7%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Patient.\n", Color_Gray);
            printf(
                "      Another patient with this national code has already "
                "been registered.%s\n",
                Color_Reset);
            break;

        case 38:
            printf("%s#Code 3-8%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Patient.\n", Color_Gray);
            printf(
                "      Another doctor with this email has already been "
                "registered.%s\n",
                Color_Reset);
            break;

        case 39:
            printf("%s#Code 3-9%s\n", Color_Yellow, Color_Reset);
            printf("      %sError Adding Patient.\n", Color_Gray);
            printf(
                "      Another doctor with this national code has already been "
                "registered.%s\n",
                Color_Reset);
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

        if (c > 10) {
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
    while (tmp > 0) {
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
        if (!(x >= 0 && x <= 9)) {

            // backspace code ~ 8
            if (x == -40 && c != 0) {

                // back cursor
                printf("\b \b");

                c -= 2;
                final /= 100;
                x = 0;

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

        if (end_c + 2 < c) {
            printf("\n");
            Error_Management(12);
            return -1;
        }

        x = getch();
    }

    final /= 10;

    printf("\n");

    if (!(final >= start && final <= end)) {

        Error_Management(10);
        return -1;
    }

    return final;
}

int User_Input_String(char *str_list, int str_size, int just_number) {

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
        if (x == 13) {
            printf("\n");
            break;
        }

        // backspace code ~ 8
        if (x == 8 && i >= 1) {
            printf("\b \b");
            i -= 1;
            str_list[i] = '\0';
            continue;
        }

        else if (x == 8 && i == 0)
            continue;
        // no backspace :)

        if (i >= str_size - 1) {
            printf("\n");
            Error_Management(22);
            return -1;
        }

        if (just_number && (x < '0' || x > '9')) {
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

int User_Input_PassWord(char *pass_list, int pass_size) {

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
        if (x == 13) {
            printf("\n");
            break;
        }

        // backspace code ~ 8
        if (x == 8 && i >= 1) {
            printf("\b \b");
            i -= 1;
            pass_list[i] = '\0';
            continue;
        }

        else if (x == 8 && i == 0)
            continue;
        // no backspace :)

        if (i >= pass_size - 1) {
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
    printf("%sExit%s :) %sBye Bye.%s\n", Color_Red, Color_Yellow, Color_Gray,
           Color_Reset);
    Sleep(3000);

    RUN_CLS;

    exit(exit_code);
}

void Main_Func_Get_User_Date() {

    while (1) {

        RUN_CLS;

        Bar_Status(0, 0);
        printf("Enter Today's Date (Ctrl+C ~ Reset)\n");

        Bar_Status(0, 0);
        printf("Year: ");

        int year_in = User_Input_Number_Range(1, 9999);

        if (year_in == -1) continue;

        if (year_in == -2) continue;

        Bar_Status(0, 0);
        printf("Month: ");

        int month_in = User_Input_Number_Range(1, 12);

        if (month_in == -1) continue;

        if (month_in == -2) continue;

        Bar_Status(0, 0);
        printf("Day: ");

        int day_in = User_Input_Number_Range(1, 31);

        if (day_in == -1) continue;

        if (day_in == -2) continue;

        int flag_bad_date = 0;

        if (year_in < Date_Start_Cal.y)
            flag_bad_date = 1;

        else if (year_in == Date_Start_Cal.y && month_in < Date_Start_Cal.m)
            flag_bad_date = 1;

        // Bad Date
        if (flag_bad_date) {
            Bar_Status(0, 0);
            printf("%sError:%s The Entered Date is before the Calendar Date.\n",
                   Color_Red, Color_Reset);
            Sleep(5000);
            continue;
        }

        Date_Login.y = year_in;
        Date_Login.m = month_in;
        Date_Login.d = day_in;

        Sleep(1500);
        break;

    }  // while end
}

void Main_Check_Active_Calendar() {

    if (Active_Calendar == 1) {

        if (Date_Login.y > Date_Start_Cal.y &&
            !(Date_Login.y == Date_Start_Cal.y + 1 && Date_Start_Cal.m == 12 &&
              Date_Login.m == 1)) {
            Bar_Status(0, 0);
            printf("%sIn The Month, The Program Calendar is Not Set (Year)%s\n",
                   Color_Red, Color_Reset);
            Sleep(5000);
            Active_Calendar = 0;
        }

        else if (Date_Login.m == Date_Start_Cal.m + 1 ||
                 (Date_Login.y == Date_Start_Cal.y + 1 &&
                  Date_Start_Cal.m == 12 && Date_Login.m == 1)) {

            if (Date_Start_Cal_Next.y == 0 && Date_Start_Cal_Next.m == 0 &&
                Date_Start_Cal_Next.d == 0) {

                Bar_Status(0, 0);
                printf(
                    "%sIn The Month, The Program Calendar is Not Set (No "
                    "Next)%s\n",
                    Color_Red, Color_Reset);
                Sleep(5000);

                Date_Start_Cal_Last.y = Date_Start_Cal.y;
                Date_Start_Cal_Last.m = Date_Start_Cal.m;
                Date_Start_Cal_Last.d = Date_Start_Cal.d;
                Date_Start_Cal_Last.week_d = Date_Start_Cal.week_d;

                Active_Calendar = 0;

            }

            else {

                RUN_CLS;

                Bar_Status(0, 0);
                printf("%sSystem: Auto Go To Next Month%s\n", Color_Green,
                       Color_Reset);

                Date_Start_Cal_Last.y = Date_Start_Cal.y;
                Date_Start_Cal_Last.m = Date_Start_Cal.m;
                Date_Start_Cal_Last.d = Date_Start_Cal.d;
                Date_Start_Cal_Last.week_d = Date_Start_Cal.week_d;

                Bar_Status(0, 0);
                printf("Last Month\n");
                Print_Calendar(Date_Start_Cal_Last.y, Date_Start_Cal_Last.m,
                               Date_Start_Cal_Last.d,
                               Date_Start_Cal_Last.week_d);

                Date_Start_Cal.y = Date_Start_Cal_Next.y;
                Date_Start_Cal.m = Date_Start_Cal_Next.m;
                Date_Start_Cal.d = Date_Start_Cal_Next.d;
                Date_Start_Cal.week_d = Date_Start_Cal_Next.week_d;

                Date_Start_Cal_Next.y = 0;
                Date_Start_Cal_Next.m = 0;
                Date_Start_Cal_Next.d = 0;
                Date_Start_Cal_Next.week_d = 0;

                Bar_Status(0, 0);
                printf("Next Month\n");
                Print_Calendar(Date_Start_Cal.y, Date_Start_Cal.m,
                               Date_Start_Cal.d, Date_Start_Cal.week_d);

                // reset off day
                Cal_Off_Date_Count = 0;
                Active_Calendar = 1;
                visit_count = 0;

                Update_Files();

                Main_Reset_Doctors();

                printf("\n%sPress a Button to Continue...     %s", Color_Gray,
                       Color_Reset);
                printf("%c\n", (char)getch());
                Sleep(500);
            }

        }

        else if (Date_Login.m > Date_Start_Cal.m) {
            Bar_Status(0, 0);
            printf(
                "%sIn The Month, The Program Calendar is Not Set (Month)%s\n",
                Color_Red, Color_Reset);
            Sleep(5000);
            Active_Calendar = 0;
        }

        if (Active_Calendar == 1 && Date_Start_Cal.d - Date_Login.d < 10) {
            Bar_Status(0, 0);
            printf("%sThe Next Month is Near, don't Forgot to Define it.%s\n",
                   Color_Yellow, Color_Reset);
            Sleep(5000);
        }

    }  // if end

    if (Active_Calendar == 0) {

        Main_Reset_Doctors();

        printf("\n%sPress a Button to Continue...     %s", Color_Gray,
               Color_Reset);
        printf("%c\n", (char)getch());
        Sleep(500);
    }
}

void Main_Reset_Doctors() {

    // reset all doctor info
    for (int dc = 0; dc < doctor_count; dc++) {

        Doctors[dc].time_work.count_time_work = 0;

        // Doctors[dc].visit_pay = 0;

        if (Doctors[dc].pay_deadline.y == Date_Login.y &&
            Doctors[dc].pay_deadline.m < Date_Login.m) {

            Doctors[dc].pay_not_month = -1;

        }

        else if (Doctors[dc].pay_deadline.y < Date_Login.y) {

            Doctors[dc].pay_not_month = -1;
        }

        Doctors[dc].pay_not_month++;

        if (Doctors[dc].pay_not_month == 1) {

            Doctors[dc].pay_total = Date_Start_Cal.d * 10;

        }

        else if (Doctors[dc].pay_not_month == 2 &&
                 Doctors[dc].pay_rent_ext == 1) {

            Doctors[dc].pay_total += Date_Start_Cal.d * 10;

        }

        // kick doctor
        else {

            Bar_Status(0, 0);
            printf("Doctor %s was Fired from the Building\n", Doctors[dc].name);

            for (int j = dc; j < doctor_count - 1; j++) {

                Doctors[j] = Doctors[j + 1];
                Doctors[j].id--;
            }

            doctor_count--;
            dc--;
        }
    }

    Update_Files();
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

void Hash_Function(char *str) {

    int str_len = 0;
    for (str_len = 0; str[str_len]; str_len++);

    srand(HASH_CAPACITY + (HASH_CAPACITY / (str_len + 1)));
    char str_tmp[PASSWORD_SIZE];
    strcpy(str_tmp, str);

    unsigned long long hash_code = rand();

    /*
    32 in ancii code is space
    126 in ancii code is ~

    Ex: 126 - 32 / 3 = 31 ~ ancii step

    part 1 = 32 ~ 63
    part 2 = 63 ~ 94
    part 3 = 94 ~ 125

    */

    int ancii_step = (126 - 32) / HASH_PART;

    /*

    Ex: str_step = 30 / 3 = 10
    Ex: last_char_str_step = 30 % 3 + 1 = 1

    +1 ---> char[30] no set

    Ex: str_step = 30 / 7 = 4
    Ex: last_char_str_step = 30 % 7 + 1 = 3

    */

    int str_step = (PASSWORD_SIZE - 1) / HASH_PART;
    int last_char_str_step = (PASSWORD_SIZE - 1) % HASH_PART + 1;

    for (int count = 0, j = 0; count < str_step;
         count++, j++, hash_code %= HASH_CAPACITY) {

        if (j >= str_len) j = 0;
        hash_code += str_tmp[rand() % str_len];
        hash_code *= 3;
        hash_code += (hash_code % 10 == 0)
                         ? (++hash_code * 4)
                         : ((int)(str_tmp[rand() % str_len]) * count * 3);
        hash_code += count;
        hash_code += (hash_code % 7 == 0)
                         ? ((int)(str_tmp[rand() % str_len]) * count * 2)
                         : (hash_code-- * 8);
        hash_code *= 9;
        hash_code += str_tmp[rand() % str_len];

        for (int i = 0; i < HASH_PART; i++) {

            char tmp_char =
                (char)(hash_code % ancii_step + 32 + (i * ancii_step));
            str[count + (str_step * i)] = tmp_char;

            (i % 2 == 0) ? (hash_code += rand()) : (hash_code -= rand());
        }
    }

    for (int i = 0; i < last_char_str_step; i++) {

        str[(PASSWORD_SIZE - 1) - i] = (char)(hash_code % 95 + 32);
        (i % 2 == 0) ? (hash_code *= (i + 2), hash_code -= rand())
                     : (hash_code /= (i + 2), hash_code += rand());
    }

    str[PASSWORD_SIZE] = '\0';
}

// END ¯\_(ツ)_/¯