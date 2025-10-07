#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAXFILE 50

void print_symbol(int length, char symbol);
int getLastorderID(char *filepath);
char findFilelist(char *filelist[], int *filecount);
int SelectCSVFile(char *selected_path, int buffer_size);
void DisplayCSVContent(const char *filepath);

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
    int lastOrderID = 0;
    int currentID = 0;

    if (CSVFile == NULL){
        printf("เปิดไฟล์ไม่สำเร็จหรือไม่พบไฟล์\n");
        return 0;
    }
    else{
        while (fgets(line, sizeof(line), CSVFile) != NULL){
            if (sscanf(line, "%d,", &currentID) == 1){
                lastOrderID = currentID;
            }
        }
        fclose(CSVFile);
    }
    return lastOrderID;
}

char findFilelist(char *filelist[], int *filecount){
    WIN32_FIND_DATA findData;
    HANDLE hFind;
    hFind = FindFirstFile("csvfile\\*.*", &findData);

    char filename [1024];

    if (hFind == INVALID_HANDLE_VALUE){
        printf("ไม่เจอ path หรือ error");
        return 1;
    }

    do{
        if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0){
            if (*filecount < MAXFILE){
                size_t len = strlen(findData.cFileName) + 1;
                filelist[*filecount] = (char *)malloc(len);

                if (filelist[*filecount] != NULL){
                    strcpy(filelist[*filecount], findData.cFileName);
                    (*filecount) ++;
                }else{
                    printf("Memory allocation failded\n");
                }
            }
        }
    }while(FindNextFile(hFind, &findData) != 0);
    FindClose(hFind);
}

int SelectCSVFile(char *selectedPath, int buffer_size){
    char *filelist[MAXFILE];
    int filecount = 0;
    findFilelist(filelist, &filecount);

    if (filecount == 0){
        printf("ไม่พบไฟล์ CSV ในโฟลเดอร์ csvfile\n");
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
        printf("เปิดไฟล %s ไม่สำเร็จ\n", filepath);
        return ;
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

int UpdateCSV(){
    char originalPath[1024];
    if (SelectCSVFile(originalPath, sizeof(originalPath))){
        DisplayCSV(originalPath);

        int orderIDchoice = 0;
        printf("โปรดระบุ ID ของรายการที่ต้องการแก้ไข: ");
        int scancheck = scanf("%d", &orderIDchoice);
        if(scancheck != 1){
            printf("ตัวเลือกไม่ถูกต้อง\n");
            while (getchar() != '\n');
            print_symbol(44, '-');
            return 0;
        }
        getchar();
        
        char tempPath[1024] = "tempfile\\temp_update.tmp";
        FILE *originalFile = fopen(originalPath, "r");
        FILE *tempFile = fopen(tempPath, "w");

        if (originalFile == NULL || tempFile == NULL){
            printf("เกิดข้อพิดพลาดในการเปิดไฟล์เพื่ออัพเดต!\n");
            if (originalFile) fclose(originalFile);
            if (tempFile) fclose(tempFile);
            return 0;
        }

        char line[1024];
        int found = 0;

        if (fgets(line, sizeof(line), originalFile) != NULL){
            fputs(line, tempFile);
        }

        while(fgets(line, sizeof(line), originalFile) != NULL){
            int currentID;
            char lineCopy[1024];
            strcpy(lineCopy, line);

            if (sscanf(line, "%d,", &currentID) == 1 && currentID == orderIDchoice){
                found = 1;
                printf("\n------------ กำลังแก้ไข้ ID : %d ------------\n", currentID);

                char productName[1024];
                int quatity;
                int price;
                int productTypechoice;
                char productType[10];

                printf("ใส่ชื่อรายการใหม่ : ");
                fgets(productName, sizeof(productName), stdin);
                productName[strcspn(productName, "\n")] = 0;

                printf("ใส่จำนวนใหม่ : ");
                if (scanf("%d", &quatity) != 1){
                    print_symbol(44, '-');
                    printf("ตัวเลือกไม่ถูกต้อง\n");
                    print_symbol(44, '-');
                    while(getchar() != '\n');
                    return 0;
                }

                printf("ใส่ราคาใหม่ : ");
                if (scanf("%d", &price) != 1){
                    print_symbol(44, '-');
                    printf("ตัวเลือกไม่ถูกต้อง\n");
                    print_symbol(44, '-');
                    while(getchar() != '\n');
                    return 0;
                }

                printf("ประเภท\n1.Food\n2.Drink\nโปรดเลือกประเภทใหม่ : ");
                if (scanf("%d", &productTypechoice) != 1){
                    print_symbol(44, '-');
                    printf("ตัวเลือกไม่ถูกต้อง\n");
                    print_symbol(44, '-');
                    while(getchar() != '\n');
                    return 0;
                }
                getchar();

                if (productTypechoice == 1) strcpy(productType, "Food");
                else if (productTypechoice == 2) strcpy(productType, "Drink");
                else strcpy(productType, "None");

                fprintf(tempFile, "%04d,%s,%d,%d,%s\n", currentID, productName, quatity, price, productType);
            }
            else {
                fputs(lineCopy, tempFile);
            }
        }
        fclose(originalFile);
        fclose(tempFile);

        if (found) {
            remove(originalPath);
            rename(tempPath, originalPath);
            print_symbol(44, '-');
            printf("อัปเดตข้อมูล ID : %d เรียบร้อย\n", orderIDchoice);
            print_symbol(44, '-');
        }
        else {
            remove(tempPath);
            print_symbol(44, '-');
            printf("ไม่พบรายการที่มี ID : %d\n", orderIDchoice);
            print_symbol(44, '-');
        }
    }
    return 1;
}

int SearchCSV(){
    char selectedPath[1024];
    if (SelectCSVFile(selectedPath, sizeof(selectedPath))){
        int searchType = 0;
        printf("โปรดเลือกประเภทการค้นหา\n");
        printf("1.ค้นหาด้วย ID\n2.ค้นหาด้วยชื่อ\n");
        printf("โปรดเลือกตัวเลือกตามหัวข้อ : ");
        int scancheck = scanf("%d", &searchType);
        getchar();

        if (scancheck != 1 || (searchType != 1 && searchType != 2)){
            printf("ตัวเลือกไม่ถูกต้อง\n");
            while(getchar() != '\n');
            return 0;
        }

        FILE *CSVFile = fopen(selectedPath, "r");
        if (CSVFile == NULL){
            printf("เปิดไฟลไม่สำเร็จ\n");
            return 0;
        }

        switch (searchType){
            case 1 : {
                print_symbol(44, '-');
                printf("โปรดใส่หมายเลข ID ที่ต้องการค้นหา : ");
                int searchID;
                int scancheck = scanf("%d", &searchID);
                if (scancheck != 1){
                    print_symbol(44, '-');
                    printf("ตัวเลือกไม่ถูกต้อง\n");
                    while(getchar() != '\n');
                    return 0;
                }
                
                char line[1024];
                int found = 0;
                fgets(line, sizeof(line), CSVFile);

                while (fgets(line, sizeof(line), CSVFile) != NULL){
                    int cuurentID;
                    if (sscanf(line, "%d,", &cuurentID) == 1){
                        if (cuurentID == searchID) {
                            char *token = strtok(line, ",\n");
                            print_symbol(44, '-');
                            printf("พบสินค้ารหัส : %d\n", cuurentID);
                            print_symbol(155, '=');
                            printf("%-31s%-31s%-31s%-31s%-31s|\n", "OrderID", "ProductName", "Quantity", "Price", "Type");
                            while (token != NULL){
                                printf("%-30s ", token);
                                token = strtok(NULL, ",\n");
                            }
                            printf("|\n");
                            print_symbol(155, '=');
                            found = 1;
                            break;
                        }
                    }
                }
                if (found != 1){
                    printf("ไม่พบรหัสสินค้าที่ค้นหา\n");
                }
                break;
            }
            case 2: {
                char searchName[1024];
                printf("โปรดใส่ \"ชื่อ\" หรือ \"ประเภท\" ที่ต้องการค้นหา : ");
                fgets(searchName, sizeof(searchName), stdin);
                searchName[strcspn(searchName, "\n")] = 0;

                char line[1024];
                int found = 0;
                int productCount = 0;
                print_symbol(44, '-');
                printf("\n\n\n%-31s%-31s%-31s%-31s%-31s\n", "OrderID", "ProductName", "Quantity", "Price", "Type");
                print_symbol(155, '=');

                fgets(line, sizeof(line), CSVFile);
                while (fgets(line, sizeof(line), CSVFile) != NULL){
                    if (strstr(line, searchName) != NULL){
                        char *token = strtok(line, ",\n");
                        printf("|");
                        while (token != NULL){
                            printf("%-30s|", token);
                            token = strtok(NULL, ",\n");
                        }
                        printf("\n");
                        productCount++;
                        found = 1;
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

int AddCSV (){
    char filename [1024];
    char *filelist[MAXFILE];
    int filecount = 0;
    findFilelist(filelist, &filecount);

    int choice = 0;
    printf("โปรดเลือกไฟล์ที่ต้องการเพิ่มข้อมูล\n");
    for (int i = 0; i < filecount; i++){
        printf("%d.%s\n", i + 1, filelist[i]);
    }
    print_symbol(44, '-');
    printf("เลือกโดยใช้ตัวเลขตามหัวข้อ : ");
    scanf("%d", &choice);
    getchar();
    if (choice < 1 || choice > filecount){
        printf("ตัวเลือกไม่ถูกต้อง\n");
    }
    else{
        strcpy(filename, filelist[choice - 1]);
        char path[1024] = "csvfile\\";
        strcat(path, filename);
        FILE *CSVFile = fopen(path, "a");

        if (CSVFile == NULL){
            printf("เปิดไฟล์ไม่สำเร็จหรือไม่พบไฟล์\n");
        }
        else{
            int choice = 1;
            int orderID = getLastorderID(path) + 1;
            do{
                char name [100];
                int quatity;
                int price;
                int productTypechoice = 0;
                char productType [10];

                printf("ใส่ชื่อรายการที่ %d : ", orderID);
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                printf("ใส่จำนวน: ");
                scanf("%d", &quatity);

                printf("ใส่ราคา: ");
                scanf("%d", &price);

                printf("ประเภท\n1.Food\n2.Drink\nโปรดเลือกด้วตามหัวข้อ : ");
                scanf("%d", &productTypechoice);

                if (productTypechoice == 1) strcpy(productType, "Food");
                else if (productTypechoice == 2) strcpy(productType, "Drink");
                else strcpy(productType, "None");

                fprintf(CSVFile, "%04d,%s,%d,%d,%s\n", orderID, name, quatity, price, productType);

                printf("\nต้องการเพิ่มรายการต้อไปหรือไม่ (1 = ใช่, 0 = ไม่ใช่): ");
                scanf("%d", &choice);
                print_symbol(44, '-');

                getchar();

                orderID++;
                printf("\n");

            }while (choice != 0);
        }
    fclose(CSVFile);
    }

    for (int i = 0; i < filecount; i++){
        free(filelist[i]);
    }
    return 1;
}

int SaveCSV (char *filename){
    char path[1024] = "csvfile\\";
    strcat(path, filename);
    strcat(path, ".csv");

    FILE *checkFile = fopen(path, "r");
    if (checkFile != NULL){
        printf("มีไฟล์ชื่อนี้อยู่แล้ว");
        fclose(checkFile);
        return 0;
    }

    FILE *CSVFile = fopen(path, "a");
    if (CSVFile == NULL){
        printf("ไม่สามารถสร้างไฟล์ได้\n");
    }

    else{
        printf("สร้างไฟล์ %s.csv สำเร็จ\n", filename);
        fprintf(CSVFile, "OrderID,ProductName,Quantity,Price,Type\n");
        print_symbol(44, '-');
        int choice = 1;
        int orderID = 1;

        do{
            char name [100];
            int quatity;
            int price;
            int productTypechoice = 0;
            char productType [10];

            printf("ใส่ชื่อรายการที่ %d : ", orderID);
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;
            printf("ใส่จำนวน: ");
            scanf("%d", &quatity);
            printf("ใส่ราคา: ");
            scanf("%d", &price);
            print_symbol(44, '-');
            printf("ประเภท\n1.Food\n2.Drink\nโปรดเลือกด้วตามหัวข้อ : ");
            scanf("%d", &productTypechoice);
            print_symbol(44, '-');

            if (productTypechoice == 1){
                strcpy(productType, "Food");
            }
            else if (productTypechoice == 2){
                strcpy(productType, "Drink");
            }
            else{
                strcpy(productType, "None");
            }

            fprintf(CSVFile, "%04d,%s,%d,%d,%s\n", orderID, name, quatity, price, productType);
            printf("ต้องการเพิ่มรายการต้อไปหรือไม่ (1 = ใช่, 0 = ไม่ใช่): ");
            scanf("%d", &choice);
            getchar();

            orderID++;
        }while (choice != 0);
        print_symbol(44, '-');
        fclose(CSVFile);
    }
    return 1;
}

int ReadCSV (){
    char selectedPath[1024];
    if (SelectCSVFile(selectedPath, sizeof(selectedPath))){
        DisplayCSV(selectedPath);
    }
    return 1;
}

int main(){
    int menu = 0;
    int scan_check;
    while(menu != 7){
        CreateDirectory("csvfile", NULL);
        CreateDirectory("tempfile", NULL);
        
        printf("     ระบบจัดการข้อมูลการซื้ออาหารและเครื่องดื่ม\n");
        printf("------------ โปรดเลือกฟังก์ชันที่ต้องการ ------------\n");
        printf("1.อ่านข้อมูลการซื้อ\n");
        printf("2.บันทึกข้อมูลการซื้อ\n");
        printf("3.เพิ่มข้อมูลการซื้อ\n");
        printf("4.ค้นหาข้อมูลการซื้อ\n");
        printf("5.อัพเดตข้อมูลการซื้อ\n");
        printf("6.ลบข้อมูลการซื้อ\n");
        printf("7.ออกจากโปรแกรม\n");
        printf("8.Unit Test\n");
        printf("9.End-to-End Test\n");
        printf("เลือกฟังก์ชันโดยตัวเลข 1-9 : ");
        scan_check = scanf("%d", &menu);
        getchar();

        if (scan_check == 1){
            print_symbol(44, '-');
            switch (menu){
                case 1: 
                    ReadCSV();
                    break;
                case 2:{
                    char filename[1024];
                    printf("โปรดระบุชื่อไฟล์ที่ต้องการสร้าง : ");
                    scanf("%s", filename);
                    getchar();            
                    SaveCSV(filename);
                    break;}
                case 3:
                    AddCSV();
                    break;
                case 4:
                    SearchCSV();
                    break;
                case 5:
                    UpdateCSV();
                    break;
                case 6:
                    break;
                case 7:
                    break;
                case 8:
                    break;
                case 9:
                    break;
                default:
                    printf("โปรดเลือกตัวเลือกที่ 1-9\n");
                    menu = 0;
                    print_symbol(44, '-');
                    break;
            }
            print_symbol(3, '\n');
        }
        else{
            print_symbol(44, '-');
            printf("โปรดเลือกตัวเลือกที่ 1-9\n");
            while(getchar() != '\n');
            menu = 0;
            print_symbol(44, '-');
            print_symbol(3, '\n');
        }
    }
    return 1;
}