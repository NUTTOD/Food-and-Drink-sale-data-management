@echo off
rem ตั้งค่า Code Page ของหน้าต่างนี้เป็น UTF-8 เพื่อให้แสดงภาษาไทยได้ถูกต้อง
chcp 65001 > nul
setlocal enabledelayedexpansion

rem --- ตั้งชื่อโปรแกรมของคุณตรงนี้ ---
set PROGRAM_NAME=Food-and-Drink-sale-data-management.exe

echo.
echo =======================================================
echo          STARTING END-TO-END AUTOMATED TEST
echo =======================================================
echo.

rem 1. SETUP: เตรียมสภาพแวดล้อมและลบไฟล์เก่า
echo [SETUP] Cleaning up old test files...
if not exist csvfile mkdir csvfile
if exist csvfile\e2e_test.csv del csvfile\e2e_test.csv
if exist output.log del output.log
echo [SETUP] Environment is clean.
echo.

rem 2. EXECUTE: รันโปรแกรมโดยใช้ "บทพูด" จากไฟล์ commands_e2e.txt
echo [EXECUTE] Running the program with predefined commands...
if not exist %PROGRAM_NAME% (
    echo [ERROR] ไม่พบไฟล์โปรแกรม '%PROGRAM_NAME%' กรุณาคอมไพล์โค้ดภาษา C ของคุณก่อน
    goto :end
)
%PROGRAM_NAME% < commands_e2e.txt > output.log
echo [EXECUTE] Program finished. ผลลัพธ์ถูกบันทึกใน output.log.
echo.

rem 3. VERIFY: ตรวจสอบผลลัพธ์ที่อยู่ในไฟล์ output.log และไฟล์ csv
echo [VERIFY] Checking the results...
set ALL_PASSED=1
set "FAIL_REASON="

rem ตรวจสอบทีละขั้นตอน
call :check_step "สร้างไฟล์ e2e_test.csv สำเร็จ" "ขั้นตอนที่ 1: การสร้างไฟล์"
call :check_step "อัปเดตข้อมูล ID : 1 เรียบร้อย" "ขั้นตอนที่ 2: การอัปเดตข้อมูล"
call :check_step "เจอสินค้า 1 อย่างจากคำค้นหา Espresso" "ขั้นตอนที่ 3: การค้นหาข้อมูล"
call :check_step "ลบข้อมูลของ ID : 2 เรียบร้อย" "ขั้นตอนที่ 4: การลบข้อมูล"

echo.
echo =======================================================
if %ALL_PASSED%==1 (
    echo         RESULT: END-TO-END TEST PASSED!
) else (
    echo         RESULT: END-TO-END TEST FAILED!
    echo         REASON: !FAIL_REASON!
    echo         กรุณาตรวจสอบไฟล์ 'output.log' สำหรับรายละเอียดเพิ่มเติม
)
echo =======================================================
echo.
goto :end

rem --- ฟังก์ชันสำหรับช่วยตรวจสอบ ---
:check_step
set "search_string=%~1"
set "step_name=%~2"
echo   - Verifying %step_name%...
findstr /C:"%search_string%" output.log > nul
if %errorlevel% neq 0 (
    echo     [FAIL]
    set ALL_PASSED=0
    if "!FAIL_REASON!"=="" set "FAIL_REASON=การตรวจสอบ '%step_name%' ล้มเหลว"
) else (
    echo     [PASS]
)
goto :eof

:end
pause