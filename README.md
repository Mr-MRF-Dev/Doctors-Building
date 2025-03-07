# 🏥 Doctors' Building

![GitHub repo size](https://img.shields.io/github/repo-size/mr-mrf-dev/Doctors-Building)
[![CMake on multiple platforms](https://github.com/Mr-MRF-Dev/Doctors-Building/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Mr-MRF-Dev/Doctors-Building/actions/workflows/cmake-multi-platform.yml)
[![GitHub License](https://img.shields.io/github/license/mr-mrf-dev/Doctors-Building)](/LICENSE)

a comprehensive healthcare management system with distinct user roles and functionalities for secretaries, doctors, and patients. This system aims to streamline healthcare management by providing tailored functionalities for secretaries, doctors, and patients, ensuring efficient and organized operations.

![Screenshot](/images/screenshot.png)

My Project in FOP (fundamental of programming) (BP (basic programming)) With C Programming Language.

> I recommend using [Windows Terminal](https://apps.microsoft.com/store/detail/windows-terminal/9N0DX20HK701)
> for the best experience.

## 📥 Getting Started

1. Clone the Repository

    If you have [Git](https://git-scm.com/) installed, run the following command in your terminal

    ```bash
    git clone https://github.com/Mr-MRF-Dev/Doctors-Building.git
    ```

    Alternatively, If You Have [Github CLI](https://cli.github.com/) installed:

    ```bash
    gh repo clone Mr-MRF-Dev/Doctors-Building
    ```

2. Navigate to the Project Directory

    ```bash
    cd ./Doctors-Building/src
    ```

3. Compile the Code

    Compile the program using a C compiler. For example, using GCC:

    ```bash
    gcc .\Main.c -o Doctors-Building
    ```

4. Run the Program

    ```bash
    ./Doctors-Building.exe
    ```

### Compile with CMake

after cloning the repository, you can compile the project using [CMake](https://cmake.org/download/):

1. Navigate to the Project Directory

    ```bash
    cd ./Doctors-Building
    ```

2. Create a build directory

   ```bash
   mkdir build
   cd build
   ```

3. Build the project using the following commands:

    ```bash
    cmake ..
    make
    ```

    Or use [MinGW MakeFiles](https://cmake.org/cmake/help/latest/generator/MinGW%20Makefiles.html) by running: (Windows)

    ```bash
    cmake .. -G "MinGW Makefiles"
    mingw32-make
    ```

4. Run Project!

    ```bash
    ./Doctors-Building.exe
    ```

## ✨️ Features

- Hash Password: Use a simple hash to handle the password hashing and verification.
- Login Page: Users can authenticate themselves on the platform.
- Admin Panel: Gain access to the admin panel to perform administrative tasks.
- Doctor Account: Doctors can view medical histories and prescribe treatments.
- Patient Account: Patients can view personal health information, appointment schedules, and test results.

## 📝 Request for Proposal ( [RFP](https://en.wikipedia.org/wiki/Request_for_proposal) )

A simple RFP for this project.

1. Login Page 🔒

   - Sign in
   - Forgot password
   - Exit

2. Secretary Account (Admin) 👾

    - Add Doctor
    - Add Patient
    - Monthly Schedule
    - Visits Schedule
    - Exit

3. Doctor's Account 🧑🏻‍⚕️

    - Determining the shifts
    - Reserved Visits
    - Patient Prescription
    - Rent Payment
    - Visirs Payment
    - Exit

4. Patient Account 😷

    - Book an Appointment
    - Cancel an Appointment
    - All Appointments
    - Prescriptions
    - Exit

## 🤝 Contributing

we welcome any contributions you may have. If you're interested in helping out, fork the repository
and create an [Issue](https://github.com/Mr-MRF-Dev/Doctors-Building/issues) and
[PR](https://github.com/Mr-MRF-Dev/Doctors-Building/pulls).

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](/LICENSE) file for details.
