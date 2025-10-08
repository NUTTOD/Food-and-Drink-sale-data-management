#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <assert.h>

#define MAXFILE 50

void print_symbol(int length, char symbol);
int getLastorderID(char *filepath);
void findFilelist(char *filelist[], int *filecount);
int SelectCSVFile(char *selected_path, int buffer_size);
void DisplayCSV(const char *filepath);
int ReadCSV();
int SaveCSV();
int AddCSV();
int SearchCSV();
int UpdateCSV();
int DeleteCSV();

void print_symbol(int length, char symbol){
    for (int i = 0; i < length; i++){
        putchar(symbol);
    }
    putchar('\n');
}

int getLastorderID(char *filepath){
    FILE *CSVFile = fopen(filepath, "r");
    char line[1024];
    int maxOrderID = 0;
    int currentID = 0;

    if (CSVFile == NULL){
        printf("เปิดไฟล์ไม่สำเร็จหรือไม่พบไฟล์: %s\n", filepath);
        return 0;
    }

    if (fgets(line, sizeof(line), CSVFile) == NULL) {
        fclose(CSVFile);
        return 0;
    }

    while (fgets(line, sizeof(line), CSVFile) != NULL){
        if (sscanf(line, "%d,", &currentID) == 1){
            if (currentID > maxOrderID){
                maxOrderID = currentID;
            }
        }
    }

    fclose(CSVFile);
    
    return maxOrderID;
}

void findFilelist(char *filelist[], int *filecount){
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile("csvfile\\*.*", &findData);

    if (hFind == INVALID_HANDLE_VALUE){
        printf("ไม่เจอ path หรือ error");
        return;
    }

    do{
        if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0){
            if (*filecount < MAXFILE){
                size_t len = strlen(findData.cFileName) + 1;
                filelist[*filecount] = (char *)malloc(len);

                if (filelist[*filecount] != NULL){
                    strcpy(filelist[*filecount], findData.cFileName);
                    (*filecount)++;
                } else {
                    printf("Memory allocation failed\n");
                }
            }
        }
    } while(FindNextFile(hFind, &findData) != 0);
    FindClose(hFind);
}

int SelectCSVFile(char *selectedPath, int buffer_size){
    char *filelist[MAXFILE];
    int filecount = 0;
    findFilelist(filelist, &filecount);

    if (filecount == 0){
        printf("ไม่พบไฟล์ CSV ในโฟลเดอร์ csvfile\n");
        return 0;
    }

    printf("โปรดเลือกไฟล์ที่ต้องการ\n");
    for (int i = 0; i < filecount; i++){
        printf("%d.%s\n", i + 1, filelist[i]);
    }
    print_symbol(44,'-');

    int filechoice = 0;
    printf("เลือกโดยใช้ตัวเลขตามหัวข้อ : ");
    int scancheck = scanf("%d", &filechoice);
    getchar();

    int success = 0;
    if (scancheck != 1 || filechoice < 1 || filechoice > filecount){
        printf("ตัวเลือกไม่ถูกต้อง\n");
        while(getchar() != '\n');
        success = 0;
    }
    else{
        snprintf(selectedPath, buffer_size, "csvfile\\%s", filelist[filechoice - 1]);
        success = 1;
    }

    for (int i = 0; i < filecount; i++){
        free(filelist[i]);
    }
    return success;
}

void DisplayCSV(const char *filepath){
    FILE *CSVFile = fopen(filepath, "r");
    if (CSVFile == NULL){
        printf("เปิดไฟล์ %s ไม่สำเร็จ\n", filepath);
        return;
    }

    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), CSVFile) != NULL) {
        char *header_token = strtok(buffer, ",\n");
        while (header_token != NULL) {
            printf("%-31s", header_token);
            header_token = strtok(NULL, ",\n");
        }
        printf("\n");
        print_symbol(155, '=');
    }

    while (fgets(buffer, sizeof(buffer), CSVFile) != NULL) {
        char *token = strtok(buffer, ",\n");
        printf("|");
        while (token != NULL) {
            printf("%-30s|", token);
            token = strtok(NULL, ",\n");
        }
        printf("\n");
    }
    print_symbol(155, '=');
    fclose(CSVFile);
}

int process_delete_record(const char* input_path, const char* output_path, int id_to_delete) {
    FILE *originalFile = fopen(input_path, "r");
    FILE *tempFile = fopen(output_path, "w");

    if (originalFile == NULL || tempFile == NULL) {
        if (originalFile) fclose(originalFile);
        if (tempFile) fclose(tempFile);
        return -1;
    }

    char line[1024];
    int found = 0;
    if (fgets(line, sizeof(line), originalFile) != NULL) {
        fputs(line, tempFile);
    }

    while (fgets(line, sizeof(line), originalFile) != NULL) {
        int currentID;
        if (sscanf(line, "%d,", &currentID) == 1 && currentID == id_to_delete) {
            found = 1;
            continue;
        } else {
            fputs(line, tempFile);
        }
    }

    fclose(originalFile);
    fclose(tempFile);
    return found;
}

int process_update_record(const char* input_path, const char* output_path, int id_to_update, const char* new_data_line) {
    FILE *originalFile = fopen(input_path, "r");
    FILE *tempFile = fopen(output_path, "w");

    if (originalFile == NULL || tempFile == NULL) {
        if (originalFile) fclose(originalFile);
        if (tempFile) fclose(tempFile);
        return -1;
    }

    char line[1024];
    int found = 0;
    if (fgets(line, sizeof(line), originalFile) != NULL) {
        fputs(line, tempFile);
    }

    while (fgets(line, sizeof(line), originalFile) != NULL) {
        int currentID;
        if (sscanf(line, "%d,", &currentID) == 1 && currentID == id_to_update) {
            found = 1;
            fprintf(tempFile, "%s\n", new_data_line);
        } else {
            fputs(line, tempFile);
        }
    }

    fclose(originalFile);
    fclose(tempFile);
    return found;
}

void test_delete_record_found() {
    printf("Unit Test: Deleting an existing record (ID 2)... ");
    const char* input = "test_in.csv";
    const char* output = "test_out.csv";
    FILE* f = fopen(input, "w");
    fprintf(f, "OrderID,Name\n0001,A\n0002,B\n0003,C\n");
    fclose(f);

    int result = process_delete_record(input, output, 2);
    assert(result == 1);

    FILE* out_f = fopen(output, "r");
    assert(out_f != NULL);
    char line[1024];
    int id2_found = 0;
    while(fgets(line, sizeof(line), out_f) != NULL) {
        if (strstr(line, "0002,B") != NULL) {
            id2_found = 1;
        }
    }
    fclose(out_f);
    assert(id2_found == 0);

    remove(input);
    remove(output);
    printf("[PASS]\n");
}

void test_delete_record_not_found() {
    printf("Unit Test: Deleting a non-existent record (ID 99)... ");
    const char* input = "test_in.csv";
    const char* output = "test_out.csv";
    FILE* f = fopen(input, "w");
    fprintf(f, "OrderID,Name\n0001,A\n0002,B\n");
    fclose(f);

    int result = process_delete_record(input, output, 99);
    assert(result == 0);

    remove(input);
    remove(output);
    printf("[PASS]\n");
}

void test_update_record_found() {
    printf("Unit Test: Updating an existing record (ID 1)... ");
    const char* input = "test_update_in.csv";
    const char* output = "test_update_out.csv";
    FILE* f = fopen(input, "w");
    fprintf(f, "OrderID,ProductName,Price\n0001,OldCoke,20\n0002,Pepsi,22\n");
    fclose(f);
    char new_line[] = "0001,NewCoke,25";

    int result = process_update_record(input, output, 1, new_line);
    assert(result == 1);

    FILE* out_f = fopen(output, "r");
    assert(out_f != NULL);
    char line[1024];
    int updated_line_found = 0;
    while(fgets(line, sizeof(line), out_f) != NULL) {
        if (strstr(line, "0001,NewCoke,25") != NULL) {
            updated_line_found = 1;
        }
    }
    fclose(out_f);
    assert(updated_line_found == 1);

    remove(input);
    remove(output);
    printf("[PASS]\n");
}

void run_all_unit_tests() {
    print_symbol(44, '=');
    printf("             Running All Unit Tests (with assert)\n");
    print_symbol(44, '=');

    test_delete_record_found();
    test_delete_record_not_found();
    test_update_record_found();

    print_symbol(44, '=');
    printf("All Unit Tests Passed Successfully!\n");
    print_symbol(44, '=');
}

int DeleteCSV() {
    char originalPath[1024];
    if (SelectCSVFile(originalPath, sizeof(originalPath))) {
        DisplayCSV(originalPath);

        int orderIDchoice = 0;
        printf("โปรดระบุ ID ของรายการที่ต้องการลบ: ");
        int scancheck = scanf("%d", &orderIDchoice);
        if (scancheck != 1) {
            printf("ตัวเลือกไม่ถูกต้อง\n");
            while (getchar() != '\n');
            print_symbol(44, '-');
            return 0;
        }
        getchar();

        char tempPath[] = "tempfile\\temp_delete.tmp";
        int result = process_delete_record(originalPath, tempPath, orderIDchoice);

        print_symbol(44, '-');
        if (result == 1) {
            remove(originalPath);
            rename(tempPath, originalPath);
            printf("ลบข้อมูลของ ID : %d เรียบร้อย\n", orderIDchoice);
        } else if (result == 0) {
            remove(tempPath);
            printf("ไม่พบรายการที่มี ID : %d\n", orderIDchoice);
        } else {
            printf("เกิดข้อผิดพลาดในการเปิดไฟล์เพื่อลบข้อมูล!\n");
        }
        print_symbol(44, '-');
    }
    return 1;
}

int UpdateCSV() {
    char originalPath[1024];
    if (SelectCSVFile(originalPath, sizeof(originalPath))) {
        DisplayCSV(originalPath);

        int orderIDchoice = 0;
        printf("โปรดระบุ ID ของรายการที่ต้องการแก้ไข: ");
        int scancheck = scanf("%d", &orderIDchoice);
        if (scancheck != 1) {
            printf("ตัวเลือกไม่ถูกต้อง\n"); while (getchar() != '\n'); return 0;
        }
        getchar();

        printf("\n------------ กำลังแก้ไข้ ID : %d ------------\n", orderIDchoice);
        char productName[1024], productType[10], new_data_line[2048];
        int quantity, price, productTypechoice;

        printf("ใส่ชื่อรายการใหม่ : ");
        fgets(productName, sizeof(productName), stdin);
        productName[strcspn(productName, "\n")] = 0;

        printf("ใส่จำนวนใหม่ : ");
        if (scanf("%d", &quantity) != 1) { printf("ตัวเลือกไม่ถูกต้อง\n"); while (getchar() != '\n'); return 0; }

        printf("ใส่ราคาใหม่ : ");
        if (scanf("%d", &price) != 1) { printf("ตัวเลือกไม่ถูกต้อง\n"); while (getchar() != '\n'); return 0; }

        printf("ประเภท\n1.Food\n2.Drink\nโปรดเลือกด้วตามหัวข้อ : ");
        if (scanf("%d", &productTypechoice) != 1 || productTypechoice < 1 || productTypechoice > 2) { printf("ตัวเลือกไม่ถูกต้อง\n"); while (getchar() != '\n'); return 0; }
        getchar();
        if (productTypechoice == 1) strcpy(productType, "Food");
        else strcpy(productType, "Drink");

        snprintf(new_data_line, sizeof(new_data_line), "%04d,%s,%d,%d,%s", orderIDchoice, productName, quantity, price, productType);

        char tempPath[] = "tempfile\\temp_update.tmp";
        int result = process_update_record(originalPath, tempPath, orderIDchoice, new_data_line);

        print_symbol(44, '-');
        if (result == 1) {
            remove(originalPath);
            rename(tempPath, originalPath);
            printf("อัปเดตข้อมูล ID : %d เรียบร้อย\n", orderIDchoice);
        } else if (result == 0) {
            remove(tempPath);
            printf("ไม่พบรายการที่มี ID : %d\n", orderIDchoice);
        } else {
            printf("เกิดข้อผิดพลาดในการเปิดไฟล์เพื่ออัปเดตข้อมูล!\n");
        }
        print_symbol(44, '-');
    }
    return 1;
}

int SearchCSV() {
    char selectedPath[1024];
    if (SelectCSVFile(selectedPath, sizeof(selectedPath))) {
        int searchType = 0;
        printf("โปรดเลือกประเภทการค้นหา\n1.ค้นหาด้วย ID\n2.ค้นหาด้วยชื่อ\n");
        printf("โปรดเลือกตัวเลือกตามหัวข้อ : ");
        if (scanf("%d", &searchType) != 1 || (searchType != 1 && searchType != 2)) {
            printf("ตัวเลือกไม่ถูกต้อง\n"); while(getchar() != '\n'); return 0;
        }
        getchar();

        FILE *CSVFile = fopen(selectedPath, "r");
        if (CSVFile == NULL) { printf("เปิดไฟล์ไม่สำเร็จ\n"); return 0; }

        switch (searchType) {
            case 1: {
                printf("โปรดใส่หมายเลข ID ที่ต้องการค้นหา : ");
                int searchID;
                if (scanf("%d", &searchID) != 1) {
                    printf("ตัวเลือกไม่ถูกต้อง\n"); while(getchar() != '\n'); return 0;
                }
                getchar();

                char line[1024]; int found = 0;
                fgets(line, sizeof(line), CSVFile);
                while (fgets(line, sizeof(line), CSVFile) != NULL) {
                    int currentID;
                    if (sscanf(line, "%d,", &currentID) == 1 && currentID == searchID) {
                        print_symbol(44, '-');
                        printf("พบสินค้ารหัส : %d\n", currentID);
                        print_symbol(155, '=');
                        printf("%-31s%-31s%-31s%-31s%-31s\n", "OrderID", "ProductName", "Quantity", "Price", "Type");
                        char *token = strtok(line, ",\n");
                        while (token != NULL) {
                            printf("%-30s ", token);
                            token = strtok(NULL, ",\n");
                        }
                        printf("\n");
                        print_symbol(155, '=');
                        found = 1;
                        break;
                    }
                }
                if (!found) { printf("ไม่พบรหัสสินค้าที่ค้นหา\n"); }
                break;
            }
            case 2: {
                char searchName[1024];
                printf("โปรดใส่ \"ชื่อ\" หรือ \"ประเภท\" ที่ต้องการค้นหา : ");
                fgets(searchName, sizeof(searchName), stdin);
                searchName[strcspn(searchName, "\n")] = 0;

                char line[1024]; int productCount = 0;
                print_symbol(44, '-');
                printf("\n%-31s%-31s%-31s%-31s%-31s\n", "OrderID", "ProductName", "Quantity", "Price", "Type");
                print_symbol(155, '=');
                fgets(line, sizeof(line), CSVFile); // Skip header
                while (fgets(line, sizeof(line), CSVFile) != NULL) {
                    if (strstr(line, searchName) != NULL) {
                        char *token = strtok(line, ",\n");
                        printf("|");
                        while (token != NULL) {
                            printf("%-30s|", token);
                            token = strtok(NULL, ",\n");
                        }
                        printf("\n");
                        productCount++;
                    }
                }
                print_symbol(155, '=');
                printf("เจอสินค้า %d อย่างจากคำค้นหา %s\n", productCount, searchName);
                break;
            }
        }
        fclose(CSVFile);
    }
    return 1;
}

int AddCSV() {
    char selectedPath[1024];
    if (SelectCSVFile(selectedPath, sizeof(selectedPath))) {
        FILE *CSVFile = fopen(selectedPath, "a");
        if (CSVFile == NULL) { printf("เปิดไฟล์ไม่สำเร็จ\n"); return 0; }

        int choice = 1;
        int orderID = getLastorderID(selectedPath) + 1;
        do {
            char name[100], productType[10];
            int quantity, price, productTypechoice;

            printf("ใส่ชื่อรายการที่ %d : ", orderID);
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;

            printf("ใส่จำนวน: ");
            if (scanf("%d", &quantity) != 1) { printf("ตัวเลือกไม่ถูกต้อง\n"); while(getchar() != '\n'); return 0; }

            printf("ใส่ราคา: ");
            if (scanf("%d", &price) != 1) { printf("ตัวเลือกไม่ถูกต้อง\n"); while(getchar() != '\n'); return 0; }

            printf("ประเภท\n1.Food\n2.Drink\nโปรดเลือกด้วตามหัวข้อ : ");
            if (scanf("%d", &productTypechoice) != 1 || productTypechoice < 1 || productTypechoice > 2) {
                printf("ตัวเลือกไม่ถูกต้อง\n"); while(getchar() != '\n'); return 0;
            }
            getchar();
            if (productTypechoice == 1) strcpy(productType, "Food");
            else strcpy(productType, "Drink");

            fprintf(CSVFile, "%04d,%s,%d,%d,%s\n", orderID, name, quantity, price, productType);

            printf("\nต้องการเพิ่มรายการต่อไปหรือไม่ (1 = ใช่, 0 = ไม่ใช่): ");
            scanf("%d", &choice);
            getchar();
            orderID++;
            print_symbol(44, '-');
        } while (choice != 0);
        fclose(CSVFile);
    }
    return 1;
}

int SaveCSV() {
    char filename[1024];
    printf("โปรดระบุชื่อไฟล์ที่ต้องการสร้าง (ไม่ต้องใส่นามสกุล .csv) : ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    if (strlen(filename) == 0) { printf("ชื่อไฟล์ห้ามว่างเปล่า\n"); return 0; }

    char path[1024];
    snprintf(path, sizeof(path), "csvfile\\%s.csv", filename);

    FILE *checkFile = fopen(path, "r");
    if (checkFile != NULL) {
        printf("มีไฟล์ชื่อนี้อยู่แล้ว\n"); fclose(checkFile); return 0;
    }

    FILE *CSVFile = fopen(path, "w");
    if (CSVFile == NULL) { printf("ไม่สามารถสร้างไฟล์ได้\n"); return 0; }

    printf("สร้างไฟล์ %s.csv สำเร็จ\n", filename);
    fprintf(CSVFile, "OrderID,ProductName,Quantity,Price,Type\n");
    char selectedPath[1024];
    int choice = 1;
    int orderID = 1;
    do {
        char name[100], productType[10];
        int quantity, price, productTypechoice;

        printf("ใส่ชื่อรายการที่ %d : ", orderID);
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;

        printf("ใส่จำนวน: ");
        if (scanf("%d", &quantity) != 1) { printf("ตัวเลือกไม่ถูกต้อง\n"); while(getchar() != '\n'); return 0; }

        printf("ใส่ราคา: ");
        if (scanf("%d", &price) != 1) { printf("ตัวเลือกไม่ถูกต้อง\n"); while(getchar() != '\n'); return 0; }

        printf("ประเภท\n1.Food\n2.Drink\nโปรดเลือกด้วตามหัวข้อ : ");
        if (scanf("%d", &productTypechoice) != 1 || productTypechoice < 1 || productTypechoice > 2) {
            printf("ตัวเลือกไม่ถูกต้อง\n"); while(getchar() != '\n'); return 0;
        }

        getchar();

        if (productTypechoice == 1) strcpy(productType, "Food");
        else strcpy(productType, "Drink");

        fprintf(CSVFile, "%04d,%s,%d,%d,%s\n", orderID, name, quantity, price, productType);

        printf("\nต้องการเพิ่มรายการต่อไปหรือไม่ (1 = ใช่, 0 = ไม่ใช่): ");
        if (scanf("%d", &choice) != 1 || choice < 0 || choice > 1) {
            printf("ตัวเลือกไม่ถูกต้อง\n"); while(getchar() != '\n'); return 0;
        }
        
        getchar();
        orderID++;
        print_symbol(44, '-');
    } while (choice != 0);
    fclose(CSVFile);
    return 1;
}

int ReadCSV() {
    char selectedPath[1024];
    if (SelectCSVFile(selectedPath, sizeof(selectedPath))) {
        DisplayCSV(selectedPath);
    }
    return 1;
}

int main() {
    int menu = 0;
    while(menu != 7) {
        CreateDirectory("csvfile", NULL);
        CreateDirectory("tempfile", NULL);

        printf("     ระบบจัดการข้อมูลการซื้ออาหารและเครื่องดื่ม\n");
        printf("------------ โปรดเลือกฟังก์ชันที่ต้องการ ------------\n");
        printf("1.อ่านข้อมูลการซื้อ\n");
        printf("2.บันทึกข้อมูลการซื้อ (สร้างไฟล์ใหม่)\n");
        printf("3.เพิ่มข้อมูลการซื้อ (ในไฟล์ที่มีอยู่)\n");
        printf("4.ค้นหาข้อมูลการซื้อ\n");
        printf("5.อัพเดตข้อมูลการซื้อ\n");
        printf("6.ลบข้อมูลการซื้อ\n");
        printf("7.ออกจากโปรแกรม\n");
        printf("8.Unit Test\n");
        printf("9.End-to-End Test\n");
        printf("เลือกฟังก์ชันโดยตัวเลข 1-9 : ");
        
        if (scanf("%d", &menu) != 1) {
            menu = 0;
        }
        getchar();

        print_symbol(44, '-');
        switch (menu) {
            case 1: ReadCSV(); break;
            case 2: SaveCSV(); break;
            case 3: AddCSV(); break;
            case 4: SearchCSV(); break;
            case 5: UpdateCSV(); break;
            case 6: DeleteCSV(); break;
            case 7: printf("จบการทำงานของโปรแกรม\n"); break;
            case 8: run_all_unit_tests(); break;
            case 9:
                printf("         End-to-End Automated Test\n");
                print_symbol(44, '=');
                printf("คำแนะนำ:\n");
                printf("1. ตรวจสอบว่ามีไฟล์ 3 ไฟล์อยู่ในโฟลเดอร์เดียวกัน:\n");
                printf("   - %s.exe\n", "Food-and-Drink-sale-data-management");
                printf("   - commands_e2e.txt\n");
                printf("   - run_e2e_test.bat\n");
                printf("2. ปิดโปรแกรมนี้ และดับเบิ้ลคลิก 'run_e2e_test.bat'\n");
                break;
            default:
                printf("โปรดเลือกตัวเลือกที่ 1-9\n");
                break;
        }
        print_symbol(44, '-');

        if (menu != 7) {
            printf("\nกด Enter เพื่อกลับไปที่เมนูหลัก...");
            //getchar();
        }
        printf("\n\n");
    }
    return 0;
}